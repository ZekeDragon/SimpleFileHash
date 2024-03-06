/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** md5hash.cpp
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
#include "md5hash.hpp"

#include <cstring>
#include <algorithm>

#include <QtSystemDetection>

// Ordinarily, you'd just detect if we are building with MSVC by checking if _MSC_VER is defined. The problem is that
// Clang is a massive PITA and defines _MSC_VER itself. To ONLY build this on MSVC, I also have to check if Clang is
// not defined. That said, because most language servers use Clang as a backend, it will almost always look like on your
// editor/IDE that this is using the bottom entry, but it does work for top on MSVC.
#if !defined(__clang__) && defined(_MSC_VER)
#define KSC_INLINE __forceinline
#else
#define KSC_INLINE inline
#endif

/*!
 * The number of numeric bits found in the u32 integer.
 *
 * On platforms with 32-bit std::uint_least32_t integers, this should be 32.
 */
constexpr int_fast16_t U32_BITS = std::numeric_limits<uint32_t>::digits;

constexpr std::byte PADDING_FRONT = static_cast<std::byte>(0b1 << (BYTE_BITS - 1));

constexpr static uint32_t ACCUM_A_INIT = 0x67452301,
                          ACCUM_B_INIT = 0xefcdab89,
                          ACCUM_C_INIT = 0x98badcfe,
                          ACCUM_D_INIT = 0x10325476;

constexpr std::array<std::array<uint32_t, 16>, 4> T {{
	{{ 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	   0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 }},
	{{ 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	   0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a }},
	{{ 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	   0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 }},
	{{ 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	   0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 }}
}};

constexpr std::array<std::array<uint32_t, 16>, 4> S {{
	{{ 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22 }},
	{{ 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20 }},
	{{ 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23 }},
	{{ 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 }}
}};

constexpr std::array<std::array<uint32_t, 16>, 4> X {{
	{{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }},
	{{ 1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12 }},
	{{ 5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2 }},
	{{ 0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9 }}
}};

class Md5Hash::Impl
{
public:
	template <uint_fast32_t uPos>
	constexpr static inline uint32_t
	cycleBits(uint32_t a, uint32_t b, uint32_t mod, uint32_t const *data, uint_fast32_t counter) noexcept
	{
		uint32_t value = a + mod;
		value += data[X[uPos][counter]];
		value += T[uPos][counter];
		value = (value << S[uPos][counter]) | (value >> (U32_BITS - S[uPos][counter]));
		return value + b;
	}

	constexpr static auto UnitF =
	[](uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t const *data, uint_fast32_t counter) {
		return cycleBits<0>(a, b, (b & c) | (~b & d), data, counter);
	};

	constexpr static auto UnitG =
	[](uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t const *data, uint_fast32_t counter) {
		return cycleBits<1>(a, b, (b & d) | (c & ~d), data, counter);
	};

	constexpr static auto UnitH =
	[](uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t const *data, uint_fast32_t counter) {
		return cycleBits<2>(a, b, b ^ c ^ d, data, counter);
	};

	constexpr static auto UnitI =
	[](uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t const *data, uint_fast32_t counter) {
		return cycleBits<3>(a, b, c ^ (b | ~d), data, counter);
	};

	template <uint_fast32_t VAL, typename Func>
	constexpr static KSC_INLINE void unitLoop(Func f, Md5Hash::Accums &self, uint32_t const *data) noexcept
	{
		self.A = f(self.A, self.B, self.C, self.D, data, (VAL * 4) + 0);
		self.D = f(self.D, self.A, self.B, self.C, data, (VAL * 4) + 1);
		self.C = f(self.C, self.D, self.A, self.B, data, (VAL * 4) + 2);
		self.B = f(self.B, self.C, self.D, self.A, data, (VAL * 4) + 3);
	}

	template <typename Func>
	constexpr static KSC_INLINE void doUnit(Func f, Md5Hash::Accums &self, uint32_t const *data) noexcept
	{
		unitLoop<0>(f, self, data);
		unitLoop<1>(f, self, data);
		unitLoop<2>(f, self, data);
		unitLoop<3>(f, self, data);
	}

	static void processBlock(Md5Hash::Accums &self, std::byte const *blockLoc) noexcept
	{
		uint32_t A = self.A,
		         B = self.B,
		         C = self.C,
		         D = self.D;

		uint32_t block[BLOCK_U32];
		std::memcpy(block, blockLoc, BLOCK_SIZE);

		doUnit(UnitF, self, block);
		doUnit(UnitG, self, block);
		doUnit(UnitH, self, block);
		doUnit(UnitI, self, block);

		self.A += A;
		self.B += B;
		self.C += C;
		self.D += D;
	}

	static inline void processInput(Md5Hash &self, size_t length, std::byte const *input)
	{
		self.totalInput += length;

		if (self.bufferPos + length < BLOCK_SIZE)
		{
			std::memcpy(&self.buffer[self.bufferPos], input, length);
			self.bufferPos += length;
			return;
		}

		size_t frontChop = BLOCK_SIZE - self.bufferPos;

		std::memcpy(&self.buffer[self.bufferPos], input, frontChop);
		processBlock(self.accums, self.buffer);
		length -= frontChop;

		// Given the length, it is possible to precalculate the number of blocks needed to process, and to know when to
		// copy data into the buffer as opposed to simply passing a location from the user input.
		size_t numFullBlocks = length / BLOCK_SIZE + 1;
		self.bufferPos = length % BLOCK_SIZE;

		for (size_t fullBlockNum = 1; fullBlockNum < numFullBlocks; ++fullBlockNum)
		{
			processBlock(self.accums, &input[fullBlockNum * BLOCK_SIZE]);
		}

		std::memcpy(self.buffer, &input[numFullBlocks * BLOCK_SIZE], self.bufferPos);
	}

	static inline void finish(Md5Hash &self)
	{
		self.buffer[self.bufferPos++] = PADDING_FRONT;

		// This does include 56 because you are required to add at least one bit, so if the bytes remaining has exactly
		// enough space for an 8 byte block then you still need to use a second full buffer. The MD5 specification
		// requires that at least 1 bit is appended to the end of the input (effectively at least one byte), and an
		// additional number of bits necessary to fill at least 64 bits of additional data up to a multiple of 512.
		if (self.bufferPos > SIZE_LOCATION)
		{
			std::fill(&self.buffer[self.bufferPos], &self.buffer[BLOCK_SIZE], std::byte(0));
			processBlock(self.accums, self.buffer);
			std::fill(self.buffer, &self.buffer[SIZE_LOCATION], std::byte(0));
		}
		else
		{
			std::fill(&self.buffer[self.bufferPos], &self.buffer[SIZE_LOCATION], std::byte(0));
		}

		uint64_t bitLength = self.totalInput * BYTE_BITS;

		std::memcpy(&self.buffer[SIZE_LOCATION], &bitLength, bytesNeededForBits(64));
		processBlock(self.accums, self.buffer);

		self.finished = true;
	}

	constexpr static uint32_t BLOCK_U32 = BLOCK_SIZE / sizeof(uint32_t);
	constexpr static size_t SIZE_LOCATION = BLOCK_SIZE - sizeof(uint64_t);
};

Md5Hash::Md5Hash() noexcept :
    totalInput(0)
{
}

Md5Hash::Md5Hash(size_t length, std::byte const *data) noexcept :
    Md5Hash()
{
	Impl::processInput(*this, length, data);
}

Md5Sum Md5Hash::getMd5() noexcept
{
	Md5Sum sum;
	getMd5(sum.data());
	return sum;
}

void Md5Hash::getMd5(std::byte *output) noexcept
{
	if (!finished)
	{
		Impl::finish(*this);
	}

	// This doesn't work on anything other than processors that have 8 bit bytes. I'd like to support arbitrary bit
	// width bytes, but I'm not even close to ready to do that.
	//std::array<u32, 4> sigNums{{ A, B, C, D }};
	//constexpr ue32 copyBytes = bytesNeededForBits(32);

	//for (ue32 i = 0; i < 4; ++i)
	//{
	//	std::memcpy(&output[i * copyBytes], &sigNums[i], copyBytes);
	//}
	std::memcpy(output, &accums, MD5SUM_RETURN_SIZE);

	if constexpr (MD5SUM_RETURN_SIZE * BYTE_BITS > 128)
	{
		// Currently does nothing, but this should be implemented.
	}
}

size_t Md5Hash::provideInput(size_t length, std::byte const *data) noexcept
{
	if (!finished)
	{
		Impl::processInput(*this, length, data);

		return length;
	}

	return 0;
}

void getMd5(size_t length, std::byte const *data, std::byte *output) noexcept
{
	Md5Hash(length, data).getMd5(output);
}

Md5Sum getMd5(size_t length, std::byte const *data) noexcept
{
	return Md5Hash(length, data).getMd5();
}

Md5Hash::Accums::Accums() :
    A(ACCUM_A_INIT),
    B(ACCUM_B_INIT),
    C(ACCUM_C_INIT),
    D(ACCUM_D_INIT)
{
}
