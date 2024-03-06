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
	MD6,
	RIPEMD,
	RIPEMD128,
	RIPEMD160,
	RIPEMD256,
	RIPEMD320,
	Grostl,
	SM3,
	GOST_R,
	Streebog,
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
	case SM3: return true;
	default: return false;
	}
}

constexpr const char *algoShort(Algo algo)
{
	using enum Algo;

	switch (algo)
	{
	case MD5: return "md5";
	case SHA1: return "sha1";
	case SHA2_224: return "sha224";
	case SHA2_256: return "sha256";
	case SHA2_384: return "sha384";
	case SHA2_512: return "sha512";
	case SHA3_224: return "sha3224";
	case SHA3_256: return "sha3256";
	case SHA3_384: return "sha3384";
	case SHA3_512: return "sha3512";
	case BLAKE2s: return "blake2s";
	case BLAKE2b: return "blake2b";
	case Tiger: return "tiger";
	case Whirlpool: return "whirlpool";
	case MD4: return "md4";
	case MD2: return "md2";
	case RIPEMD160: return "ripemd160";
	case RIPEMD256: return "ripemd256";
	case SM3: return "sm3";
	default: return "none";
	}
}

constexpr const char *algoName(Algo algo)
{
	using enum Algo;

	switch (algo)
	{
	case MD5: return QT_TRANSLATE_NOOP("HashTask", "Message Digest 5");
	case SHA1: return QT_TRANSLATE_NOOP("HashTask", "Secure Hash Algorithm 1");
	case SHA2_224: return QT_TRANSLATE_NOOP("HashTask", "224-bit Secure Hash Algorithm 2");
	case SHA2_256: return QT_TRANSLATE_NOOP("HashTask", "256-bit Secure Hash Algorithm 2");
	case SHA2_384: return QT_TRANSLATE_NOOP("HashTask", "384-bit Secure Hash Algorithm 2");
	case SHA2_512: return QT_TRANSLATE_NOOP("HashTask", "512-bit Secure Hash Algorithm 2");
	case SHA3_224: return QT_TRANSLATE_NOOP("HashTask", "224-bit Secure Hash Algorithm 3");
	case SHA3_256: return QT_TRANSLATE_NOOP("HashTask", "256-bit Secure Hash Algorithm 3");
	case SHA3_384: return QT_TRANSLATE_NOOP("HashTask", "384-bit Secure Hash Algorithm 3");
	case SHA3_512: return QT_TRANSLATE_NOOP("HashTask", "512-bit Secure Hash Algorithm 3");
	case BLAKE2s: return QT_TRANSLATE_NOOP("HashTask", "BLAKE2s Hash");
	case BLAKE2b: return QT_TRANSLATE_NOOP("HashTask", "BLAKE2b Hash");
	case Tiger: return QT_TRANSLATE_NOOP("HashTask", "Tiger Hash");
	case Whirlpool: return QT_TRANSLATE_NOOP("HashTask", "Whirlpool Hash");
	case MD4: return QT_TRANSLATE_NOOP("HashTask", "Message Digest 4");
	case MD2: return QT_TRANSLATE_NOOP("HashTask", "Message Digest 2");
	case RIPEMD160: return QT_TRANSLATE_NOOP("HashTask", "RIPE Message Digest-160");
	case RIPEMD256: return QT_TRANSLATE_NOOP("HashTask", "RIPE Message Digest-256");
	case SM3: return QT_TRANSLATE_NOOP("HashTask", "ShangMi 3 Hash");
	default: return QT_TRANSLATE_NOOP("HashTask", "None");
	}
}

Algo shortToAlgo(char const *shortName);
