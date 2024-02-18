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

#include "hashtasktag.hpp"
#include "hashtaskreporter.hpp"

#include <filesystem>

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

HashTask::HashTask(QString filename, Algo algo) :
    fName(filename),
    algorithm(algo),
    tag(nullptr)
{
	// No implementation.
}

HashTask::~HashTask()
{
	// No implementation.
}

void HashTask::run()
{
	if (tag)
	{
		HashTaskReporter reporter(*tag);
		doTask(&reporter);
	}
	else
	{
		doTask(nullptr);
	}
}

QString const &HashTask::filename() const
{
	return fName;
}

Algo HashTask::hashAlgo() const
{
	return algorithm;
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

void endReportingWith(HashTaskReporter *reporter, QString const &message)
{
	if (reporter)
	{
		reporter->update(-1);
		reporter->complete(message);
	}
}

void HashTask::doTask(HashTaskReporter *reporter)
{
	using namespace CryptoPP;
	constexpr size_t PUMP_AMOUNT = 4096;

	try
	{
		std::string result, stdFileName = fName.toStdString();
		std::filesystem::path filePath(stdFileName);
		if (!std::filesystem::exists(filePath))
		{
			endReportingWith(reporter, QObject::tr("The file path provided does not exist.", "HashTask"));
		}
		else if (!std::filesystem::is_regular_file(filePath))
		{
			endReportingWith(reporter, QObject::tr("The file provided is not a regular file.", "HashTask"));
		}
		else if (auto transform = getTransform(algorithm))
		{
			uintmax_t completed = 0, fileSize = std::filesystem::file_size(filePath);
			size_t numSteps = fileSize / PUMP_AMOUNT;
			int lastReport = 0;
			HashFilter filter(*transform, new HexEncoder(new StringSink(result), false));
			if (fileSize % PUMP_AMOUNT)
			{
				++numSteps;
			}

			for (FileSource src(stdFileName.c_str(), false, new Redirector(filter));
			     numSteps-- && !src.SourceExhausted();)
			{
				completed += src.Pump(PUMP_AMOUNT);
				// This ensures that "update(1000)" isn't called until complete is ready (IE the hash is truly done).
				int permilliEst = std::min(int((double(completed) / double(fileSize)) * 1000.), 999);
				if (permilliEst > lastReport)
				{
					lastReport = permilliEst;
					reporter->update(permilliEst);
				}
			}

			filter.MessageEnd();
			reporter->update(1000);
			reporter->complete(result.c_str());
		}
		else
		{
			endReportingWith(reporter,
			                 QObject::tr("Invalid Hash Function: ", "HashTask") + QObject::tr(algoName(algorithm)));
		}
	}
	catch (FileSource::OpenErr err)
	{
		endReportingWith(reporter, QObject::tr("Failed to open the file provided: ", "HashTask") + err.what());
	}
	catch (FileSource::ReadErr err)
	{
		endReportingWith(reporter, QObject::tr("Could not read the provided file: ", "HashTask") + err.what());
	}
	catch (FileSource::Err err)
	{
		endReportingWith(reporter, QObject::tr("Unknown error from Crypto++: ", "HashTask") + err.what());
	}
	catch (std::filesystem::filesystem_error err)
	{
		endReportingWith(reporter, QObject::tr("Filesystem interaction error: ", "HashTask") + err.what());
	}
	catch (std::bad_alloc err)
	{
		endReportingWith(reporter, QObject::tr("Hashing process ran out of memory: ", "HashTask") + err.what());
	}
	catch (...)
	{
		// We do a catch any so that if an unknown exception is thrown in the thread, this will keep the application
		// stable. Calling std::terminate is unacceptable.
		endReportingWith(reporter, QObject::tr("Unknown exception in hash task thread.", "HashTask"));
	}
}
