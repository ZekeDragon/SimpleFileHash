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
	BLAKE2s,
	BLAKE2b,
	Tiger,
	Whirlpool,
	MD4,
	MD2,
	RIPEMD160,
	RIPEMD256,
	SM3,
	AlgoEnd
};

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
