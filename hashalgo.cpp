/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** hashalgo.cpp
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
#include "hashalgo.hpp"

#include "sfhbase.hpp"
#include "hashtask.hpp"

#include <algorithm>

namespace KirHut::SFH
{
using std::lower_bound;
using std::find_if;
using std::distance;

constexpr size_t cNumImplementedAlgos()
{
	size_t amount = 0;
	for (Algo alg = Algo(1); alg != Algo::AlgoEnd; alg = Algo(int(alg) + 1))
	{
		if (isImplemented(alg))
		{
			++amount;
		}
	}

	return amount;
}

size_t numImplementedAlgos()
{
	return cNumImplementedAlgos();
}

constexpr array<Algo, cNumImplementedAlgos()> implementedAlgos()
{
	array<Algo, cNumImplementedAlgos()> retArray;
	size_t algoPos = 0;
	for (Algo algo = Algo(1); algo != Algo::AlgoEnd; algo = Algo(int(algo) + 1))
	{
		if (isImplemented(algo))
		{
			retArray[algoPos++] = algo;
		}
	}

	return retArray;
}

constexpr static array<Algo, cNumImplementedAlgos()> implementedArray = implementedAlgos();

constexpr static array<char const *, cNumImplementedAlgos()> implementedNames {
	"md5", "sha1", "sha2-224", "sha2-256", "sha2-384", "sha2-512", "sha3-224", "sha3-256", "sha3-384", "sha3-512",
	"blake2s", "blake2b", "tiger", "whirlpool", "md2", "md4", "md6-256", "ripemd-160", "ripemd-256", "sm3",
	"md6-128", "md6-224", "md6-384", "md6-512"
};

static_assert(implementedNames.back() != nullptr,
    "You are required to provide a short name for every implemented algorithm in the implementedNames array.");

constexpr Algo const *cAlgosBegin()
{
	return &implementedArray.front();
}

Algo const *algosBegin()
{
	return cAlgosBegin();
}

constexpr Algo const *cAlgosEnd()
{
	return (&implementedArray.back()) + 1;
}

Algo const *algosEnd()
{
	return cAlgosEnd();
}

constexpr char const *const *cSNamesBegin()
{
	return &implementedNames.front();
}

char const *const *sNamesBegin()
{
	return cSNamesBegin();
}

constexpr char const *const *cSNamesEnd()
{
	return (&implementedNames.back()) + 1;
}

char const *const *sNamesEnd()
{
	return cSNamesBegin();
}

constexpr const char *cAlgoShort(Algo algo)
{
	if (auto loc = lower_bound(cAlgosBegin(), cAlgosEnd(), algo); loc != cAlgosEnd())
	{
		return implementedNames[distance(cAlgosBegin(), loc)];
	}

	return nullptr;
}

const char *algoShort(Algo algo)
{
	return cAlgoShort(algo);
}

constexpr Algo cShortToAlgo(char const *shortName)
{
	auto test = [=](char const *tName) {
		char const *sn = shortName;
		while (*tName != '\0' && *sn != '\0' && *sn == *tName)
		{
			++sn;
			++tName;
		}

		return *tName == '\0' && *sn == '\0';
	};

	if (auto loc = find_if(cSNamesBegin(), cSNamesEnd(), test); loc != cSNamesEnd())
	{
		return implementedArray[distance(cSNamesBegin(), loc)];
	}

	return Algo::None;
}

Algo shortToAlgo(char const *shortName)
{
	return cShortToAlgo(shortName);
}

QString algoName(Algo algo)
{
	using enum Algo;

	// This is done this way because the Qt Translation system goes through the source code to know what translation
	// strings to generate in the .ts files. This system cannot determine that a random array of const char pointers
	// are supposed to be translated unless they are wrapped in QT_TRANSLATE_NOOP anyway, and that is a lot more to
	// maintain. This makes adding translated strings easier.
	switch (algo)
	{
	case MD5: return HashTask::tr("Message Digest 5");
	case SHA1: return HashTask::tr("Secure Hash Algorithm 1");
	case SHA2_224: return HashTask::tr("224-bit Secure Hash Algorithm 2");
	case SHA2_256: return HashTask::tr("256-bit Secure Hash Algorithm 2");
	case SHA2_384: return HashTask::tr("384-bit Secure Hash Algorithm 2");
	case SHA2_512: return HashTask::tr("512-bit Secure Hash Algorithm 2");
	case SHA3_224: return HashTask::tr("224-bit Secure Hash Algorithm 3");
	case SHA3_256: return HashTask::tr("256-bit Secure Hash Algorithm 3");
	case SHA3_384: return HashTask::tr("384-bit Secure Hash Algorithm 3");
	case SHA3_512: return HashTask::tr("512-bit Secure Hash Algorithm 3");
	case BLAKE2s: return HashTask::tr("BLAKE2s Hash");
	case BLAKE2b: return HashTask::tr("BLAKE2b Hash");
	case Tiger: return HashTask::tr("Tiger Hash");
	case Whirlpool: return HashTask::tr("Whirlpool Hash");
	case MD4: return HashTask::tr("Message Digest 4");
	case MD2: return HashTask::tr("Message Digest 2");
	case MD6_128: return HashTask::tr("128-bit Message Digest 6");
	case MD6_224: return HashTask::tr("224-bit Message Digest 6");
	case MD6_256: return HashTask::tr("256-bit Message Digest 6");
	case MD6_384: return HashTask::tr("384-bit Message Digest 6");
	case MD6_512: return HashTask::tr("512-bit Message Digest 6");
	case RIPEMD160: return HashTask::tr("RIPE Message Digest-160");
	case RIPEMD256: return HashTask::tr("RIPE Message Digest-256");
	case SM3: return HashTask::tr("ShangMi 3 Hash");
	default: return HashTask::tr("None", "The algorithm type 'None'");
	}
}

}
