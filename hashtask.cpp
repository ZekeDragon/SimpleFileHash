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

#include <filesystem>
#include <fstream>

#include <cassert>

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

void exceptionWith(QPromise<QString> &promise, QString const &message, std::exception_ptr ptr)
{
	promise.addResult(message);
	promise.setException(ptr);
}

std::unique_ptr<CryptoPP::HashTransformation> getTransform(Algo algo)
{
	using namespace CryptoPP;
	using std::make_unique;

	switch (algo)
	{
	case Algo::MD5: return make_unique<Weak::MD5>();
	case Algo::SHA1: return make_unique<SHA1>();
	case Algo::SHA2_224: return make_unique<SHA224>();
	case Algo::SHA2_256: return make_unique<SHA256>();
	case Algo::SHA2_384: return make_unique<SHA384>();
	case Algo::SHA2_512: return make_unique<SHA512>();
	case Algo::SHA3_224: return make_unique<SHA3_224>();
	case Algo::SHA3_256: return make_unique<SHA3_256>();
	case Algo::SHA3_384: return make_unique<SHA3_384>();
	case Algo::SHA3_512: return make_unique<SHA3_512>();
	case Algo::BLAKE2b: return make_unique<BLAKE2b>();
	case Algo::BLAKE2s: return make_unique<BLAKE2s>();
	case Algo::Tiger: return make_unique<Tiger>();
	case Algo::Whirlpool: return make_unique<Whirlpool>();
	case Algo::MD4: return make_unique<Weak::MD4>();
	case Algo::MD2: return make_unique<Weak::MD2>();
	case Algo::RIPEMD160: return make_unique<RIPEMD160>();
	case Algo::RIPEMD256: return make_unique<RIPEMD256>();
	case Algo::SM3: return make_unique<SM3>();
	default: return {};
	}
}

bool taskBody(QPromise<QString> &promise, CryptoPP::HashFilter &filter, std::filesystem::path p)
{
	uintmax_t completed = 0, fileSize = std::filesystem::file_size(p);
	size_t numSteps = fileSize / PUMP_AMOUNT;
	int lastReport = 0;
	if (fileSize % PUMP_AMOUNT)
	{
		++numSteps;
	}

	// This uses std::ifstream to avoid dependency on how Crypto++ opens files. This should work for all localities
	// on all OS's.
	std::ifstream stream(p, std::ios::binary);
	CryptoPP::FileSource src(stream, false, new CryptoPP::Redirector(filter));
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

void outputCryptoPPWarning(QString fName, CryptoPP::FileSource::Err const &err, QString errType)
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

void runHashNow(QPromise<QString> &promise, QString fName, Algo algorithm)
{
	using namespace CryptoPP;

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
		else if (auto transform = getTransform(algorithm))
		{
			std::string result;
			HashFilter filter(*transform, new HexEncoder(new StringSink(result), false));

			if (taskBody(promise, filter, filePath))
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

HashTask::HashTask(QString filename, Algo algo, size_t index, bool startNow, QObject *parent) :
    QObject(parent),
    millis(-1),
    algorithm(algo),
    ind(index),
    hashStr(tr("Hash has not started")),
    fName(filename)
{
	connect(&watcher, SIGNAL(suspended()), SLOT(suspendOn()));
	connect(&watcher, SIGNAL(resumed()), SLOT(suspendOff()));
	connect(&watcher, SIGNAL(progressValueChanged(int)), SLOT(jobUpdate(int)));
	connect(&watcher, SIGNAL(resultReadyAt(int)), SLOT(hashComplete()));
	connect(&watcher, SIGNAL(finished()), SLOT(finished()));

	if (startNow)
	{
		start();
	}
}

HashTask::~HashTask()
{
	if (millis > -1 && !isComplete())
	{
		cancel();
	}
}

int HashTask::permilliComplete() const
{
	return watcher.future().progressValue();
}

QString const &HashTask::hash() const
{
	return hashStr;
}

QString const &HashTask::filename() const
{
	return fName;
}

Algo HashTask::hashAlgo() const
{
	return algorithm;
}

QString HashTask::algoName() const
{
	return tr(::algoName(algorithm));
}

bool HashTask::isComplete() const
{
	return watcher.isFinished();
}

bool HashTask::isPaused() const
{
	return watcher.isSuspending() || watcher.isSuspended();
}

bool HashTask::started() const
{
	return millis > -1;
}

size_t HashTask::index() const
{
	return ind;
}

void HashTask::start()
{
	auto fileSourceErrHandler = [f = fName](CryptoPP::FileSource::Err err) {
		outputCryptoPPWarning(f, err, tr("Error"));
		return HashTask::tr("Unknown error from Crypto++");
	};
	auto fileSourceReadErrHandler = [f = fName](CryptoPP::FileSource::ReadErr err) {
		outputCryptoPPWarning(f, err, tr("Read Error"));
		return HashTask::tr("Could not read the provided file");
	};
	auto fileSourceOpenErrHandler = [f = fName](CryptoPP::FileSource::OpenErr err) {
		outputCryptoPPWarning(f, err, tr("Open Error"));
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
		watcher.setFuture(QtConcurrent::run(::runHashNow, fName, algorithm));
		// Chaining onFailed() from the QFuture returned by run is broken for some reason. The returned QFuture doesn't
		// correctly maintain the values passed to QPromise::setProgressRange(), so I had to resort to the below.
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

void HashTask::pause()
{
	watcher.setSuspended(true);
}

void HashTask::unpause()
{
	watcher.setSuspended(false);
}

void HashTask::togglePause()
{
	watcher.toggleSuspended();
}

void HashTask::cancel()
{
	watcher.cancel();
	// This is set immediately, even though the onCanceled() handler does the same.
	hashStr = tr("Canceled!");
	emit canceled();
}

void HashTask::jobUpdate(int permilli)
{
	int old = millis;
	millis = permilli;
	emit updated(millis - old);
	emit dataChanged(ind);
}

void HashTask::hashComplete()
{
	hashStr = watcher.result();
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
	::runHashNow(promise, fName, algorithm);
}
