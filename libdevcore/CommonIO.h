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
/** @file CommonIO.h
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 *
 * File & stream I/O routines.
 */

#pragma once

#include <sstream>
#include <string>
#include <boost/filesystem.hpp>
#include "Common.h"

namespace dev
{

/// Retrieve and returns the contents of the given file as a std::string.
/// If the file doesn't exist or isn't readable, returns an empty container / bytes.
std::string readFileAsString(std::string const& _file);

/// Retrieve and returns the contents of standard input (until EOF).
std::string readStandardInput();

/// Retrieve and returns a character from standard input (without waiting for EOL).
int readStandardInputChar();

/// Write the given binary data into the given file, replacing the file if it pre-exists.
/// Throws exception on error.
/// @param _writeDeleteRename useful not to lose any data: If set, first writes to another file in
/// the same directory and then moves that file.
void writeFile(std::string const& _file, bytesConstRef _data, bool _writeDeleteRename = false);
/// Write the given binary data into the given file, replacing the file if it pre-exists.
inline void writeFile(std::string const& _file, bytes const& _data, bool _writeDeleteRename = false) { writeFile(_file, bytesConstRef(&_data), _writeDeleteRename); }
inline void writeFile(std::string const& _file, std::string const& _data, bool _writeDeleteRename = false) { writeFile(_file, bytesConstRef(_data), _writeDeleteRename); }
/// Converts arbitrary value to string representation using std::stringstream.
template <class _T>
std::string toString(_T const& _t)
{
	std::ostringstream o;
	o << _t;
	return o.str();
}

/// Partial implementation of boost::filesystem::weakly_canonical (available in boost>=1.60).
/// Should be replaced by the boost implementation as soon as support for boost<1.60 can be dropped.
boost::filesystem::path weaklyCanonicalFilesystemPath(boost::filesystem::path const &_path);

///Ìæ»»ºÏÔ¼ÖÐµÄÐÇ»ð¸ñÊ½µØÖ·did:bid:xxxxx -> 23×Ö½ÚµØÖ·
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
std::string fromBidAddress(std::string const &_a);
void bidAddressReplace(std::string &_context);

}
