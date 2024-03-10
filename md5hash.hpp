/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** md5hash.hpp
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

#include <cstdint>
#include <array>
#include <vector>

namespace KirHut
{

/*!
 * The number of bits in a byte.
 *
 * Some platforms may have differently sized bytes, and this can be used when that matters. The type used by
 * std::numeric_limits<T> cannot be std::byte because of a C++ limitation where std::numeric_limits<std::byte> is not
 * defined statically so cannot be used in constexpr expressions.
 */
constexpr int_fast16_t BYTE_BITS = std::numeric_limits<unsigned char>::digits;

constexpr uint_fast32_t bytesNeededForBits(uint_fast32_t numBits)
{
	return (numBits + BYTE_BITS - 1) / BYTE_BITS;
}

/*!
 * The total size in bytes of the returned value from getMd5().
 *
 * An MD5 hash is guaranteed to be 128 bits long, so regardless of the number of bits in a byte, the return value is
 * equal to whatever is necessary to store 128 bits.
 */
constexpr uint32_t MD5SUM_RETURN_SIZE = bytesNeededForBits(128);

/*!
 * Simple typedef of an array that will fit an MD5 Hash sum.
 *
 * Use it just like a std::array. The underlying type is std::byte, not char or unsigned char, so you will need to
 * convert if you need a char type.
 */
typedef std::array<std::byte, MD5SUM_RETURN_SIZE> Md5Sum;

/*!
 * The MD5 Hash sum generator class.
 *
 * This class is designed to have an efficient and easy to verify C++ hash implementation that never throws exceptions
 * and follows modern C++17 practices. This class does not allocate memory and is safe to use in memory constrained
 * situations. The object does not yet support byte lengths that aren't 8 bits, but other than that it should support
 * ints of arbitrary length (though it does require the platform to have an integer type of at least 32 and 64 bits).
 *
 * As an object following modern C++ practices, input is accepted as const std::byte rather than const char, allows
 * passing std::vector and std::array arguments of arbitrary length, and returning an object if needed instead of just
 * filling a buffer for the user.
 */
class Md5Hash
{
public:
	/*!
	 * Basic constructor that creates a Md5Hash with no input.
	 *
	 * The state is valid, so you can call getMd5() on this Md5Hash and get a valid hash with no input provided to the
	 * algorithm (as though it had an empty string as input).
	 */
	Md5Hash() noexcept;

	/*!
	 * Input consuming constructor for Md5Hash.
	 *
	 * This constructor allows you to combine a construction with a provideInput() call, so the arguments passed are
	 * simply redirected to provideInput() after a valid state has been initialized.
	 *
	 * \param length The length of the passed data byte array.
	 * \param data A pointer to an array of std::byte to be added as input to the this Md5Hash sum.
	 */
	Md5Hash(size_t length, std::byte const *data) noexcept;

	/*!
	 * Input consuming constructor for Md5Hash.
	 *
	 * This constructor allows you to combine a construction with a provideInput() call, so the argument passed is
	 * simply redirected to provideInput() after a valid state has been initialized.
	 *
	 * \param data A std::vector of the data to be added as input to the this Md5Hash sum.
	 */
	template <class Allocator = std::allocator<std::byte>>
	explicit Md5Hash(std::vector<std::byte, Allocator> const &data) noexcept : Md5Hash(data.size(), data.data())
	{
	}

	/*!
	 * Input consuming constructor for Md5Hash.
	 *
	 * This constructor allows you to combine a construction with a provideInput() call, so the argument passed is
	 * simply redirected to provideInput() after a valid state has been initialized.
	 *
	 * \param data A std::array of the data to be added as input to the this Md5Hash sum.
	 */
	template <size_t N>
	explicit Md5Hash(std::array<std::byte, N> const &data) noexcept : Md5Hash(data.size(), data.data())
	{
	}

	/*!
	 * Provides additional input to the Md5Hash sum.
	 *
	 * If you do not know the total size of the file or source you are hashing, this allows you to get multiple blocks
	 * of input that you process in series. This allows for arbitrarily long input.
	 *
	 * If getMd5() has been called on this Md5Hash, then this method will do nothing at all. This method returns zero
	 * instead of \a length if that is the case.
	 *
	 * \param length The length of the passed data byte array.
	 * \param data A pointer to an array of std::byte to be added as input to the this Md5Hash sum.
	 * \return The number of bytes processed. Should be equal to \a length, or 0 if Md5Hash::getMd5() has been called.
	 */
	size_t provideInput(size_t length, std::byte const *data) noexcept;

	/*!
	 * Provides additional input to the Md5Hash sum.
	 *
	 * If you do not know the total size of the file or source you are hashing, this allows you to get multiple blocks
	 * of input that you process in series. This allows for arbitrarily long input.
	 *
	 * If getMd5() has been called on this Md5Hash, then this method will do nothing at all. This method returns zero
	 * instead of \a length if that is the case.
	 *
	 * \param data A std::vector of the data to be added as input to the this Md5Hash sum.
	 * \return The number of bytes processed. Should be equal to \a length, or 0 if Md5Hash::getMd5() has been called.
	 */
	template <class Allocator = std::allocator<std::byte>>
	size_t provideInput(std::vector<std::byte, Allocator> const &data) noexcept
	{
		return provideInput(data.size(), data.data());
	}

	/*!
	 * Provides additional input to the Md5Hash sum.
	 *
	 * If you do not know the total size of the file or source you are hashing, this allows you to get multiple blocks
	 * of input that you process in series. This allows for arbitrarily long input.
	 *
	 * If getMd5() has been called on this Md5Hash, then this method will do nothing at all. This method returns zero
	 * instead of \a length if that is the case.
	 *
	 * \param data A std::array of the data to be added as input to the this Md5Hash sum.
	 * \return The number of bytes processed. Should be equal to \a length, or 0 if Md5Hash::getMd5() has been called.
	 */
	template <size_t N>
	size_t provideInput(std::array<std::byte, N> const &data) noexcept
	{
		return provideInput(data.size(), data.data());
	}

	/*!
	 * Finishes the hash sum computation if it hasn't been already and returns the complete Md5Sum.
	 *
	 * If the hash has already been finished this will simply return the computed result. Otherwise it will finish the
	 * Md5Hash computation which does change the object state, so this cannot be called on a const object. Md5Hash
	 * should never really be used const, as all methods can change the Md5Hash object's state, and if you need the hash
	 * to be const you just pass a const Md5Sum instead.
	 *
	 * \return A std::array of #SIMPLEMD5_RETURN_SIZE char values. On 8-bit byte platforms, a total size of 16 bytes.
	 */
	Md5Sum getMd5() noexcept;

	/*!
	 * Finishes the hash sum computation if it hasn't been already and writes the complete MD5 sum to output.
	 *
	 * If the hash has already been finished this will simply return the computed result. Otherwise it will finish the
	 * Md5Hash computation which does change the object state, so this cannot be called on a const object. Md5Hash
	 * should never really be used const, as all methods can change the Md5Hash object's state, and if you need the hash
	 * to be const you just pass a const Md5Sum instead.
	 *
	 * The output array must point to a memory location with at least 16 bytes of available data. This method will not
	 * write any more than 16 bytes.
	 *
	 * \param output A std::byte pointer to a location in memory at least 16 bytes long to write the completed hash sum.
	 */
	void getMd5(std::byte *output) noexcept;

private:
	// MD5 blocks are 512 bits in size per RFC1321. This holds that amount in bytes.
	constexpr static uint32_t BLOCK_SIZE = bytesNeededForBits(512);

	// The four accumulators for MD5.
	struct Accums
	{
		Accums();

		uint32_t A : 32,
		         B : 32,
		         C : 32,
		         D : 32;
	} accums;

	uint64_t totalInput : 64;

	std::byte buffer[BLOCK_SIZE] { std::byte(0) };
	size_t bufferPos = 0;

	bool finished = false;

	class Impl;
};

/*!
 * A quick and simple MD5 hash implementation that works on an arbitrary set of constant bytes passed as data.
 *
 * This implementation does not throw exceptions and should always successfully return an array of char data on all
 * supported platforms. This is intended to be very reliable so that it can work in all circumstances. The function is
 * guaranteed to return an array equal to the data's MD5 hash. On 8-bit byte platforms, the number of chars returned is
 * 16.
 *
 * While this implementation does its best to fully implement RFC1321, it is constrained in that this algorithm does NOT
 * accept any arbitrary number of bits, but rather the amount of data provided must be in bytes and therefore any MD5
 * hash of a value that takes a number of bits that isn't a multiple of #BYTE_BITS is impossible to generate with this
 * function.
 *
 * The data passed can be of arbitrary length, passed as the first argument. Since length is passed, data may contain
 * bytes with the value of 0 and the hash will still work correctly.
 *
 * This version of the function outputs the MD5 hash (as an array of 16 bytes, not hexadecimal) to the output char
 * array. If you do not have an array to write to, use the other function.
 *
 * \param length A std::size_t of the data size.
 * \param data A pointer to the <code>byte const</code> data to return a hash sum of.
 * \param[out] output A pointer to a buffer location to write the hash to. Must be at least #MD5SUM_RETURN_SIZE bytes in
 * length.
 */
void getMd5(size_t length, std::byte const *data, std::byte *output) noexcept;

/*!
 * A quick and simple MD5 hash implementation that works on an arbitrary set of constant bytes passed as data.
 *
 * This implementation does not throw exceptions and should always successfully return an array of char data on all
 * supported platforms. This is intended to be very reliable so that it can work in all circumstances. The function is
 * guaranteed to return an array equal to the data's MD5 hash. On 8-bit byte platforms, the number of chars returned is
 * 16.
 *
 * While this implementation does its best to fully implement RFC1321, it is constrained in that this algorithm does NOT
 * accept any arbitrary number of bits, but rather the amount of data provided must be in bytes and therefore any MD5
 * hash of a value that takes a number of bits that isn't a multiple of BYTE_BITS is impossible to generate with this
 * function.
 *
 * The data passed can be of arbitrary length, passed as the first argument. Since length is passed, data may contain
 * bytes with the value of 0 and the hash will still work correctly.
 *
 * This version of the function returns an Md5Sum of the completed MD5 hash sum. If you want to write to a buffer, use
 * the other function.
 *
 * \see Ksc::getMd5(size_t, byte const *, byte *)
 * \param length A std::size_t of the data size.
 * \param[in] data A pointer to the <code>char const</code> data to return a hash sum of.
 * \return A std::array of #MD5SUM_RETURN_SIZE char values. On 8-bit byte platforms, a total size of 16 bytes.
 */
Md5Sum getMd5(size_t length, std::byte const *data) noexcept;

}
