/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** hashtask.cpp
** Copyright (C) 2024 Ezekiel Oruven
**
** This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General
** Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any
** later version.
**
** This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
** details.
**
** The Qt library that this application depends on is itself a special exception to the Affero General Public License's
** requirements. The library may be separately distributed under the terms of the Library General Public License that
** Qt Software originally licensed the library under.
**
** You should have received a copy of the GNU Affero General Public License along with this program.  If not, see
** <http://www.gnu.org/licenses/>.
***********************************************************************************************************************/
#include "hashtask.hpp"

#include "md5hash.hpp"

#include <filesystem>
#include <fstream>

#include <atomic>

#include <QTimer>
#include <QtConcurrent/QtConcurrent>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/sha.h>
#include <cryptopp/sha3.h>
#include <cryptopp/blake2.h>
#include <cryptopp/tiger.h>
#include <cryptopp/whrlpool.h>
#include <cryptopp/ripemd.h>
#include <cryptopp/sm3.h>
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>
#include <cryptopp/md4.h>
#include <cryptopp/md2.h>

#include "md6/md6.hpp"

namespace KirHut::SFH
{
using KirHut::Md5Hash;
using KirHut::Md5Sum;

using std::string;
using std::bit_cast;
using std::ifstream;
namespace C = CryptoPP;
namespace FS = std::filesystem;

constexpr size_t PUMP_AMOUNT = 4096;

struct Hasher
{
	virtual ~Hasher() = default;
	virtual bool run(QPromise<QString> &promise, string &result, FS::path const &path) = 0;
};

struct MD5Hasher : public Hasher
{
	constexpr static char const hexAlphabet[] = "0123456789abcdef";

	inline void fillHexFromBytes(string &result, Md5Sum sum)
	{
		result.clear();
		result.reserve(sum.size() * 2);

		for (int i = 0; i < 16; ++i)
		{
			string::value_type c = bit_cast<string::value_type>(sum[i]);

			result.push_back(hexAlphabet[(c >> 4) & 0b1111]);
			result.push_back(hexAlphabet[(c >> 0) & 0b1111]);
		}
	}

	bool run(QPromise<QString> &promise, string &result, FS::path const &p) override
	{
		Md5Hash hashObj;
		uintmax_t completed = 0, fileSize = FS::file_size(p);
		int lastReport = 0;

		ifstream stream(p, std::ios::binary);
        char buffer_block[PUMP_AMOUNT];
        char *buffer = buffer_block;
		while (!promise.isCanceled() && completed != fileSize)
		{
			stream.read(buffer, PUMP_AMOUNT);
            completed += hashObj.provideInput(stream.gcount(), bit_cast<std::byte *>(buffer));
			// This ensures that "update(1000)" isn't called until complete is ready (IE the hash is truly
			// done).
			int permilliEst = std::min(int((double(completed) / double(fileSize)) * 1000.), 999);
			if (permilliEst > lastReport)
			{
				lastReport = permilliEst;
				promise.setProgressValue(permilliEst);
			}

			promise.suspendIfRequested();
		}

		if (!promise.isCanceled())
		{
			fillHexFromBytes(result, hashObj.getMd5());
			return true;
		}

		return false;
	}
};

struct MD6Hasher : public Hasher
{
	MD6Hasher(Algo algo)
	{
		switch (algo)
		{
		case Algo::MD6_224:
			md6Size = 28;
			break;

		case Algo::MD6_256:
			md6Size = 32;
			break;

		case Algo::MD6_384:
			md6Size = 48;
			break;

		case Algo::MD6_512:
			md6Size = 64;
			break;

		default:;
		}
	}

	bool run(QPromise<QString> &promise, string &result, FS::path const &p) override
	{
		md6_init(&state, md6Size * 8);
		uintmax_t completed = 0, fileSize = FS::file_size(p);
		int lastReport = 0;

		ifstream stream(p, std::ios::binary);
        unsigned char buffer_block[PUMP_AMOUNT];
        unsigned char *buffer = buffer_block;
		while (!promise.isCanceled() && completed != fileSize)
        {
            stream.read(bit_cast<char *>(buffer), PUMP_AMOUNT);
			if (md6_update(&state, buffer, stream.gcount() * 8) != MD6_SUCCESS)
			{
				// It failed for some reason, but given proper programming, this should never happen, so it is ignored
				// for now...
			}
			completed += stream.gcount();
			// This ensures that "update(1000)" isn't called until complete is ready (IE the hash is truly
			// done).
			int permilliEst = std::min(int((double(completed) / double(fileSize)) * 1000.), 999);
			if (permilliEst > lastReport)
			{
				lastReport = permilliEst;
				promise.setProgressValue(permilliEst);
			}

			promise.suspendIfRequested();
		}

		if (!promise.isCanceled())
		{
			// The MD6 reference implementation provides a lowercase hex string of the state, so good times.
			md6_final(&state, nullptr);
            unsigned char *stBuffer = state.hexhashval;
            result = bit_cast<char *>(stBuffer);
			return true;
		}

		return false;
	}

	int md6Size = 16;
	md6_state state;
};

struct CryptoPPHasher : public Hasher
{
	CryptoPPHasher(Algo algo) :
	    tPtr(getTransform(algo))
	{
		if (!tPtr)
		{
			// Throw an exception here.
		}
	}

	std::unique_ptr<C::HashTransformation> getTransform(Algo algo)
	{
		using std::make_unique;
		using enum Algo;

		switch (algo)
		{
		case MD5: return make_unique<C::Weak::MD5>();
		case SHA1: return make_unique<C::SHA1>();
		case SHA2_224: return make_unique<C::SHA224>();
		case SHA2_256: return make_unique<C::SHA256>();
		case SHA2_384: return make_unique<C::SHA384>();
		case SHA2_512: return make_unique<C::SHA512>();
		case SHA3_224: return make_unique<C::SHA3_224>();
		case SHA3_256: return make_unique<C::SHA3_256>();
		case SHA3_384: return make_unique<C::SHA3_384>();
		case SHA3_512: return make_unique<C::SHA3_512>();
		case BLAKE2b: return make_unique<C::BLAKE2b>();
		case BLAKE2s: return make_unique<C::BLAKE2s>();
		case Tiger: return make_unique<C::Tiger>();
		case Whirlpool: return make_unique<C::Whirlpool>();
		case MD4: return make_unique<C::Weak::MD4>();
		case MD2: return make_unique<C::Weak::MD2>();
		case RIPEMD160: return make_unique<C::RIPEMD160>();
		case RIPEMD256: return make_unique<C::RIPEMD256>();
		case SM3: return make_unique<C::SM3>();
		default: return {};
		}
	}

	bool run(QPromise<QString> &promise, string &result, FS::path const &p) override
	{
		C::HashFilter filter(*tPtr, new C::HexEncoder(new C::StringSink(result), false));

		int lastReport = 0;

		// This uses std::ifstream to avoid dependency on how Crypto++ opens files. This should work for all
        // localities on all OS's. Also, using char instead of std::byte because Crypto++ requires this.
		ifstream stream(p, std::ios::binary);
		C::FileSource src(stream, false, new C::Redirector(filter));

        // All the documentation for Crypto++ says that the Pump method returns the number of bytes that remain to
        // be processed, but this is a lie. It actually returns the number of bytes that were processed! Thus it is
        // correct to simply do this.
		for (uintmax_t completed = 0, fileSize = FS::file_size(p);
		     !promise.isCanceled() && completed != fileSize;
		     completed += src.Pump(PUMP_AMOUNT))
        {
			// This ensures that "update(1000)" isn't called until complete is ready (IE the hash is truly
			// done).
			int permilliEst = std::min(int((double(completed) / double(fileSize)) * 1000.), 999);
			if (permilliEst > lastReport)
			{
				lastReport = permilliEst;
				promise.setProgressValue(permilliEst);
			}

			promise.suspendIfRequested();
		}

		if (!promise.isCanceled())
		{
			filter.MessageEnd();
			return true;
		}

		return false;
	}

	unique_ptr<C::HashTransformation> tPtr;
};

struct HashTask::Impl
{
	Impl(HashTask *top, QString const &filepath, Algo algo, size_t index, bool startNow) :
	    top(top),
	    algorithm(algo),
	    ind(index),
	    fName(filepath)
	{
		connect(&watcher, SIGNAL(suspended()), top, SLOT(suspendOn()));
		connect(&watcher, SIGNAL(resumed()), top, SLOT(suspendOff()));
		connect(&watcher, SIGNAL(progressValueChanged(int)), top, SLOT(jobUpdate(int)));
		connect(&watcher, SIGNAL(resultReadyAt(int)), top, SLOT(hashComplete()));
		connect(&watcher, SIGNAL(finished()), top, SLOT(finished()));

		if (startNow)
		{
			start();
		}
	}

	static unique_ptr<Hasher> makeHasher(Algo algo)
	{
		using enum Algo;

		switch (algo)
		{
		case MD5:
			return make_unique<MD5Hasher>();

		case MD6_128:
		case MD6_224:
		case MD6_256:
		case MD6_384:
		case MD6_512:
			return make_unique<MD6Hasher>(algo);

		default:
			if (isImplemented(algo))
			{
				return make_unique<CryptoPPHasher>(algo);
			}
		}

		return {};
	}

	static void runHashNow(QPromise<QString> &promise, QString fName, Algo algorithm)
	{
		try
		{
			FS::path filePath(fName.toStdString());
			promise.setProgressRange(0, 1000);
			if (!FS::exists(filePath))
			{
				promise.addResult(HashTask::tr("The file path provided does not exist."));
			}
			else if (!FS::is_regular_file(filePath))
			{
				promise.addResult(HashTask::tr("The file provided is not a regular file."));
			}
			else if (auto hasher = makeHasher(algorithm))
			{
				string result;
				if (hasher->run(promise, result, filePath))
				{
					promise.setProgressValue(1000);
					promise.addResult(result.c_str());
				}
			}
			else
			{
				promise.addResult(HashTask::tr("Invalid Hash Function: ") + SFH::algoName(algorithm));
			}
		}
		catch (...)
		{
			// This is a convenient way to handle any exceptions thrown and propagate them to the main thread.
			promise.setException(std::current_exception());
		}
	}

	static void outputCryptoPPWarning(QString fName, C::FileSource::Err const &err, QString errType)
	{
		QDebug warn = qWarning();
        warn << QString("Received Crypto++ file %1 from attempt to read file: ").arg(errType) << fName << "\n";
		switch (err.GetErrorType())
		{
		case C::Exception::ErrorType::IO_ERROR:
            warn << "This is due to an underlying Input error.\n";
			break;
		case C::Exception::ErrorType::CANNOT_FLUSH:
            warn << "This is because a data buffer cannot be flushed correctly.\n";
			break;
		case C::Exception::ErrorType::DATA_INTEGRITY_CHECK_FAILED:
            warn << "There was a data integrity check that failed.\n";
			break;
		case C::Exception::ErrorType::INVALID_ARGUMENT:
            warn << "This is because an argument provided was invalid.\n";
			break;
		case C::Exception::ErrorType::INVALID_DATA_FORMAT:
            warn << "This is because the data was in an invalid format.\n";
			break;
		case C::Exception::ErrorType::NOT_IMPLEMENTED:
            warn << "This is reportedly because the functionality is not implemented.\n";
			break;
		case C::Exception::ErrorType::OTHER_ERROR:
            warn << "This is because of an unknown error within Crypto++.\n";
		}

        warn << "The error reports: " << err.GetWhat();
	}

	void start()
	{
		auto fileSourceErrHandler = [f = fName](C::FileSource::Err err) {
            outputCryptoPPWarning(f, err, "error");
            return tr("Unknown error from Crypto++");
		};
		auto fileSourceReadErrHandler = [f = fName](C::FileSource::ReadErr err) {
            outputCryptoPPWarning(f, err, "read error");
            return tr("Could not read the provided file");
		};
		auto fileSourceOpenErrHandler = [f = fName](C::FileSource::OpenErr err) {
            outputCryptoPPWarning(f, err, "open error");
            return tr("Failed to open the file provided");
		};
		auto filesystemErrHandler = [f = fName](FS::filesystem_error err) {
            qWarning() << "There was an error in the filesystem while accessing this file: " << f << "\n"
                       << "The reported error is: " << err.code().value() << err.code().message() << "\n"
			           << err.what();
            return tr("Filesystem interaction error");
		};
		auto badAllocHandler = [f = fName](std::bad_alloc err) {
            qWarning() << "The process ran out of memory while performing the hash task.\nFilename in progress: "
                       << f << "\n" << err.what();
            return tr("Hashing process ran out of memory");
		};
		auto unknownHandler = [f = fName] {
            qWarning() << "An unknown exception was thrown from the running hash task.\nFilename in progress: "
                       << f << "\n";
            return tr("An unknown error occurred during hash");
		};

		if (millis == -1)
		{
			watcher.setFuture(QtConcurrent::run(runHashNow, fName, algorithm));
			// Chaining onFailed() from the QFuture returned by run is broken for some reason. The returned QFuture
			// doesn't correctly maintain the values passed to QPromise::setProgressRange(), so I had to resort to the
			// below.
			watcher.future().onFailed(fileSourceOpenErrHandler);
			watcher.future().onFailed(fileSourceReadErrHandler);
			watcher.future().onFailed(fileSourceErrHandler);
			watcher.future().onFailed(filesystemErrHandler);
			watcher.future().onFailed(badAllocHandler);
			watcher.future().onFailed(unknownHandler);
			// This is required because the watcher.finished() signal is connected to hashComplete(), which calls the
			// watcher.result() method unconditionally, so there must be some kind of result.
            watcher.future().onCanceled([] { return tr("Canceled!"); });
			millis = 0;
			hashStr = tr("Calculating Hash...");
		}
	}

	HashTask *top;
	int millis = -1;
	Algo algorithm;
	size_t ind;
	QString hashStr = tr("Hash has not started"),
	        fName;
	QFutureWatcher<QString> watcher;
	std::atomic_flag canceled;
};

HashTask::HashTask(QString const &filepath, Algo algo, size_t index, bool startNow, QObject *parent) :
    QObject(parent),
    im(make_unique<HashTask::Impl>(this, filepath, algo, index, startNow))
{
	// No implementation.
}

HashTask::~HashTask()
{
	if (im->millis > -1 && !isComplete())
	{
		cancel();
	}
}

int HashTask::permilliComplete() const
{
	return im->watcher.future().progressValue();
}

QString const &HashTask::hash() const
{
	return im->hashStr;
}

QString HashTask::filename() const
{
	return im->fName.split(QDir::separator()).back();
}

QString const &HashTask::filepath() const
{
	return im->fName;
}

Algo HashTask::hashAlgo() const
{
	return im->algorithm;
}

QString HashTask::algoName() const
{
	return SFH::algoName(im->algorithm);
}

bool HashTask::isComplete() const
{
	return im->watcher.isFinished();
}

bool HashTask::isPaused() const
{
	return im->watcher.isSuspending() || im->watcher.isSuspended();
}

bool HashTask::started() const
{
	return im->millis > -1;
}

size_t HashTask::index() const
{
	return im->ind;
}

void HashTask::start()
{
	im->start();
}

void HashTask::pause()
{
	im->watcher.setSuspended(true);
}

void HashTask::unpause()
{
	im->watcher.setSuspended(false);
}

void HashTask::togglePause()
{
	im->watcher.toggleSuspended();
}

void HashTask::cancel()
{
	if (!im->canceled.test_and_set(std::memory_order_relaxed) && im->millis != 1000)
	{
		im->watcher.cancel();
		// This is set immediately, even though the onCanceled() handler does the same.
		im->hashStr = tr("Canceled!");
		emit canceled();
		emit dataChanged(im->ind);
	}
}

void HashTask::jobUpdate(int permilli)
{
	int old = im->millis;
	im->millis = permilli;
	emit updated(im->millis - old);
	emit dataChanged(im->ind);
}

void HashTask::hashComplete()
{
	im->hashStr = im->watcher.result();
}

void HashTask::finished()
{
    emit completed(im->hashStr, filename(), im->algorithm);
}

void HashTask::suspendOn()
{
	emit paused();
}

void HashTask::suspendOff()
{
	emit unpaused();
}

void HashTask::runHashNow(QPromise<QString> &promise, QString fName, Algo algorithm)
{
	Impl::runHashNow(promise, fName, algorithm);
}

}
