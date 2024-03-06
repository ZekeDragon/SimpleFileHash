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

#include <cassert>
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

constexpr size_t PUMP_AMOUNT = 4096;

struct Hasher
{
	virtual ~Hasher() = default;
	virtual bool run(QPromise<QString> &promise, std::string &result, std::filesystem::path const &path) = 0;
};

struct MD5Hasher : public Hasher
{
	constexpr static char const hexAlphabet[] = "0123456789abcdef";

	inline void fillHexFromBytes(std::string &result, Md5Sum sum)
	{
		result.clear();
		result.reserve(sum.size() * 2);

		for (int i = 0; i < 16; ++i)
		{
			std::string::value_type c = std::to_integer<std::string::value_type>(sum[i]);

			result.push_back(hexAlphabet[(c >> 4) & 0b1111]);
			result.push_back(hexAlphabet[(c >> 0) & 0b1111]);
		}
	}

	bool run(QPromise<QString> &promise, std::string &result, std::filesystem::path const &p) override
	{
		Md5Hash hashObj;
		uintmax_t completed = 0, fileSize = std::filesystem::file_size(p);
		size_t numSteps = fileSize / PUMP_AMOUNT;
		int lastReport = 0;
		if (fileSize % PUMP_AMOUNT)
		{
			++numSteps;
		}

		// This uses std::ifstream to avoid dependency on how Crypto++ opens files. This should work for all
		// localities on all OS's.
		std::basic_ifstream<std::byte> stream(p, std::ios::binary);
		std::byte buffer[PUMP_AMOUNT];
		while (!promise.isCanceled() && numSteps--)
		{
			stream.read(buffer, PUMP_AMOUNT);
			completed += hashObj.provideInput(stream.gcount(), buffer);
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

	std::unique_ptr<CryptoPP::HashTransformation> getTransform(Algo algo)
	{
		namespace C = CryptoPP;
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

	bool run(QPromise<QString> &promise, std::string &result, std::filesystem::path const &p) override
	{
		using namespace CryptoPP;

		HashFilter filter(*tPtr, new HexEncoder(new StringSink(result), false));
		uintmax_t completed = 0, fileSize = std::filesystem::file_size(p);
		size_t numSteps = fileSize / PUMP_AMOUNT;
		int lastReport = 0;
		if (fileSize % PUMP_AMOUNT)
		{
			++numSteps;
		}

		// This uses std::ifstream to avoid dependency on how Crypto++ opens files. This should work for all
		// localities on all OS's.
		std::ifstream stream(p, std::ios::binary);
		FileSource src(stream, false, new Redirector(filter));
		while (!promise.isCanceled() && numSteps--)
		{
			completed += src.Pump(PUMP_AMOUNT);
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

	std::unique_ptr<CryptoPP::HashTransformation> tPtr;
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

	static std::unique_ptr<Hasher> makeHasher(Algo algo)
	{
		if (algo == Algo::MD5)
		{
			return std::make_unique<MD5Hasher>();
		}
		else if (isImplemented(algo))
		{
			return std::make_unique<CryptoPPHasher>(algo);
		}
		else
		{
			return {};
		}
	}

	static void runHashNow(QPromise<QString> &promise, QString fName, Algo algorithm)
	{
		try
		{
			std::filesystem::path filePath(fName.toStdString());
			promise.setProgressRange(0, 1000);
			if (!std::filesystem::exists(filePath))
			{
				promise.addResult(HashTask::tr("The file path provided does not exist."));
			}
			else if (!std::filesystem::is_regular_file(filePath))
			{
				promise.addResult(HashTask::tr("The file provided is not a regular file."));
			}
			else if (auto hasher = makeHasher(algorithm))
			{
				std::string result;
				if (hasher->run(promise, result, filePath))
				{
					promise.setProgressValue(1000);
					promise.addResult(result.c_str());
				}
			}
			else
			{
				promise.addResult(HashTask::tr("Invalid Hash Function: ") + HashTask::tr(::algoName(algorithm)));
			}
		}
		catch (...)
		{
			// This is a convenient way to handle any exceptions thrown and propagate them to the main thread.
			promise.setException(std::current_exception());
		}
	}

	static void outputCryptoPPWarning(QString fName, CryptoPP::FileSource::Err const &err, QString errType)
	{
		QDebug warn = qWarning();
		warn << HashTask::tr("Received Crypto++ file %1 from attempt to read file: ").arg(errType) << fName << "\n";
		switch (err.GetErrorType())
		{
		case CryptoPP::Exception::ErrorType::IO_ERROR:
			warn << HashTask::tr("This is due to an underlying Input error.") << "\n";
			break;
		case CryptoPP::Exception::ErrorType::CANNOT_FLUSH:
			warn << HashTask::tr("This is because a data buffer cannot be flushed correctly.") << "\n";
			break;
		case CryptoPP::Exception::ErrorType::DATA_INTEGRITY_CHECK_FAILED:
			warn << HashTask::tr("There was a data integrity check that failed.") << "\n";
			break;
		case CryptoPP::Exception::ErrorType::INVALID_ARGUMENT:
			warn << HashTask::tr("This is because an argument provided was invalid.") << "\n";
			break;
		case CryptoPP::Exception::ErrorType::INVALID_DATA_FORMAT:
			warn << HashTask::tr("This is because the data was in an invalid format.") << "\n";
			break;
		case CryptoPP::Exception::ErrorType::NOT_IMPLEMENTED:
			warn << HashTask::tr("This is reportedly because the functionality is not implemented.") << "\n";
			break;
		case CryptoPP::Exception::ErrorType::OTHER_ERROR:
			warn << HashTask::tr("This is because of an unknown error within Crypto++.") << "\n";
		}

		warn << HashTask::tr("The error reports: ") << err.GetWhat() << "\n";
	}

	void start()
	{
		auto fileSourceErrHandler = [f = fName](CryptoPP::FileSource::Err err) {
			outputCryptoPPWarning(f, err, tr("error"));
			return HashTask::tr("Unknown error from Crypto++");
		};
		auto fileSourceReadErrHandler = [f = fName](CryptoPP::FileSource::ReadErr err) {
			outputCryptoPPWarning(f, err, tr("read error"));
			return HashTask::tr("Could not read the provided file");
		};
		auto fileSourceOpenErrHandler = [f = fName](CryptoPP::FileSource::OpenErr err) {
			outputCryptoPPWarning(f, err, tr("open error"));
			return HashTask::tr("Failed to open the file provided");
		};
		auto filesystemErrHandler = [f = fName](std::filesystem::filesystem_error err) {
			qWarning() << HashTask::tr("There was an error in the filesystem while accessing this file: ") << f << "\n"
			           << HashTask::tr("The reported error is: ") << err.code().value() << err.code().message() << "\n"
			           << err.what();
			return HashTask::tr("Filesystem interaction error");
		};
		auto badAllocHandler = [f = fName](std::bad_alloc err) {
			qWarning() << HashTask::tr("The process ran out of memory while performing the hash task.") << "\n"
			           << HashTask::tr("Filename in progress: ") << f << "\n"
			           << err.what();
			return HashTask::tr("Hashing process ran out of memory");
		};
		auto unknownHandler = [f = fName] {
			qWarning() << HashTask::tr("An unknown exception was thrown from the running hash task.") << "\n"
			           << HashTask::tr("Filename in progress: ") << f << "\n";
			return HashTask::tr("An unknown error occurred during hash");
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
			watcher.future().onCanceled([] { return HashTask::tr("Canceled!"); });
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
    im(std::make_unique<HashTask::Impl>(this, filepath, algo, index, startNow))
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
	return tr(::algoName(im->algorithm));
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
	emit completed();
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
