/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** hashalgo.hpp
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
#pragma once

#include <QtGlobal>

namespace KirHut::SFH
{

enum class Algo
{
	None = 0,
	MD5,
	SHA1,
	SHA2_224,
	SHA2_256,
	SHA2_384,
	SHA2_512,
	SHA3_224,
	SHA3_256,
	SHA3_384,
	SHA3_512,
	BLAKE256,
	BLAKE512,
	BLAKE2s,
	BLAKE2b,
	BLAKE3_256,
	BLAKE3_512,
	Tiger,
	Whirlpool,
	MD2,
	MD4,
	MD6_256,
	RIPEMD,
	RIPEMD128,
	RIPEMD160,
	RIPEMD256,
	RIPEMD320,
	Grostl,
	SM3,
	GOST_R,
	Streebog,
	MD6_128,
	MD6_224,
	MD6_384,
	MD6_512,
	AlgoEnd
};

constexpr bool isImplemented(Algo algo)
{
	using enum Algo;

	switch (algo)
	{
	case MD5:
	case SHA1:
	case SHA2_224:
	case SHA2_256:
	case SHA2_384:
	case SHA2_512:
	case SHA3_224:
	case SHA3_256:
	case SHA3_384:
	case SHA3_512:
	case BLAKE2s:
	case BLAKE2b:
	case Tiger:
	case Whirlpool:
	case MD4:
	case MD2:
	case RIPEMD160:
	case RIPEMD256:
	case MD6_128:
	case MD6_224:
	case MD6_256:
	case MD6_384:
	case MD6_512:
	case SM3: return true;
	default: return false;
	}
}

const char *algoShort(Algo algo);
Algo shortToAlgo(char const *shortName);

const char *const *sNamesBegin();
const char *const *sNamesEnd();

size_t numImplementedAlgos();
Algo const *algosBegin();
Algo const *algosEnd();

QString algoName(Algo algo);

}
