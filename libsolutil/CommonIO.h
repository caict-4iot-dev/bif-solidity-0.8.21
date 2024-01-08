/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0
/** @file CommonIO.h
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 *
 * File & stream I/O routines.
 */

#pragma once

#include <boost/filesystem.hpp>

#include <libsolutil/Common.h>
#include <iostream>
#include <sstream>
#include <string>

namespace solidity
{

inline std::ostream& operator<<(std::ostream& os, bytes const& _bytes)
{
	std::ostringstream ss;
	ss << std::hex;
	std::copy(_bytes.begin(), _bytes.end(), std::ostream_iterator<int>(ss, ","));
	std::string result = ss.str();
	result.pop_back();
	os << "[" + result + "]";
	return os;
}

namespace util
{

/// Retrieves and returns the contents of the given file as a std::string.
/// If the file doesn't exist, it will throw a FileNotFound exception.
/// If the file exists but is not a regular file, it will throw NotAFile exception.
/// If the file is empty, returns an empty string.
std::string readFileAsString(boost::filesystem::path const& _file);

/// Retrieves and returns the whole content of the specified input stream (until EOF).
std::string readUntilEnd(std::istream& _stdin);

/// Tries to read exactly @a _length bytes from @a _input.
/// Returns a string containing as much data as has been read.
std::string readBytes(std::istream& _input, size_t _length);

/// Retrieves and returns a character from standard input (without waiting for EOL).
int readStandardInputChar();

/// Converts arbitrary value to string representation using std::stringstream.
template <class T>
std::string toString(T const& _t)
{
	std::ostringstream o;
	o << _t;
	return o.str();
}

/// @returns the absolute path corresponding to @a _path relative to @a _reference.
std::string absolutePath(std::string const& _path, std::string const& _reference);

/// Helper function to return path converted strings.
std::string sanitizePath(std::string const& _path);

///add by zhangbo
static const int8_t kBase58digits[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, -1, -1, -1, -1, -1, -1,
	-1, 9, 34, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1,
	22, 23, 24, 25, 26, 52, 28, 29, 30, 31, 32, -1, -1, -1, -1, -1,
	-1, 33, 10, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46,
	47, 48, 49, 50, 51, 27, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1
};
std::string BinToHexString(const std::string &value);
int Base58Decode(const std::string &strIn, std::string &strout);
std::string toBidAddress(std::string const &_a);
std::string fromBidAddress(std::string const &_a);
void bidAddressReplace(std::string &_context);

}
}
