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
/** @file CommonIO.cpp
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 */

#include <libsolutil/CommonIO.h>
#include <libsolutil/Assertions.h>

#include <fstream>
#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <cmath>
#endif

using namespace std;
using namespace solidity::util;

namespace
{

template <typename T>
inline T readFile(boost::filesystem::path const& _file)
{
	assertThrow(boost::filesystem::exists(_file), FileNotFound, _file.string());

	// ifstream does not always fail when the path leads to a directory. Instead it might succeed
	// with tellg() returning a nonsensical value so that std::length_error gets raised in resize().
	assertThrow(boost::filesystem::is_regular_file(_file), NotAFile, _file.string());

	T ret;
	size_t const c_elementSize = sizeof(typename T::value_type);
	std::ifstream is(_file.string(), std::ifstream::binary);

	// Technically, this can still fail even though we checked above because FS content can change at any time.
	assertThrow(is, FileNotFound, _file.string());

	// get length of file:
	is.seekg(0, is.end);
	streamoff length = is.tellg();
	if (length == 0)
		return ret; // do not read empty file (MSVC does not like it)
	is.seekg(0, is.beg);

	ret.resize((static_cast<size_t>(length) + c_elementSize - 1) / c_elementSize);
	is.read(const_cast<char*>(reinterpret_cast<char const*>(ret.data())), static_cast<streamsize>(length));
	return ret;
}

}

std::string solidity::util::BinToHexString(const std::string &value) {
	std::string result;
	result.resize(value.size() * 2);
	for (size_t i = 0; i < value.size(); i++) {
		uint8_t item = static_cast<uint8_t>(value[i]);
		uint8_t high = (item >> 4);
		uint8_t low = (item & 0x0F);
		int highChar = (high <= 9) ? (high + '0') : (high - 10 + 'a');
		result[2 * i] = static_cast<char>(highChar);
		int highChar2 = (low <= 9) ? (low + '0') : (low - 10 + 'a');
		result[2 * i + 1] = static_cast<char>(highChar2);
	}
	return result;
}

int solidity::util::Base58Decode(const std::string &strIn, std::string &strout) {
	const char* kBase58Dictionary = "123456789AbCDEFGHJKLMNPQRSTuVWXYZaBcdefghijkmnopqrstUvwxyz";
	std::size_t nZeros = 0;
	for (; nZeros < strIn.size() && strIn.at(nZeros) == kBase58Dictionary[0]; nZeros++);
	std::size_t left_size = strIn.size() - nZeros;
	std::size_t new_size = std::size_t(left_size * log2(58.0) / 8 + 2);
	std::string tmp_str(new_size, 0);
	int carry = 0;
	for (size_t i = nZeros; i < strIn.size(); i++) {
		carry = (int)solidity::util::kBase58digits[(int)strIn[i]];
		for (size_t j = new_size - 1; j >= 0; j--) {
			int tmp = (unsigned char)tmp_str[j] * 58 + carry;
			tmp_str[j] = static_cast<char>(tmp % 256);
			carry = tmp / 256;
		}
	}
	strout.clear();
	for (size_t i = 0; i < nZeros; i++)
		strout.push_back((unsigned char)0);
	size_t k = 0;
	for (; k < tmp_str.size() && tmp_str[k] == 0; k++);
	for (; k < tmp_str.size(); k++)
		strout.push_back(tmp_str[k]);
	return static_cast<int>(nZeros + tmp_str.size() - k);
}

std::string solidity::util::fromBidAddress(std::string const &_a)
{
	std::cout << "fromAddress fromAddress:" << _a << std::endl;
    //_a === did:bid:[加密类型]xxxxxxx
    std::string subAddress = _a.substr(8); //ef24GK3M5yShYgVddmv1u5r24wzoUYZbd
    std::string signatureType = subAddress.substr(0, 1);
    std::string encodeType = subAddress.substr(1, 1);
    std::string decodeAddress = "";

    std::cout << "fromAddress signatureType:" << signatureType << std::endl;
    std::cout << "fromAddress encodeType:" << encodeType << std::endl;

    std::string signatureTypeStr = "";
    if(signatureType.compare("z") == 0)//SIGNTYPE_CFCASM2
            signatureTypeStr = "7A";
    else//SIGNTYPE_ED25519
            signatureTypeStr = "65";

    std::string encodeTypeStr = "";
    if(encodeType.compare("s") == 0){//ENCODETYPE_BASE64
            //待填充逻辑
            encodeTypeStr = "73";
    }
    else if(encodeType.compare("t") == 0){//ENCODETYPE_BECH32
            //待填充逻辑
            encodeTypeStr = "74";
    }
    else{//ENCODETYPE_BASE58
            Base58Decode(subAddress.substr(2), decodeAddress);
            encodeTypeStr = "66";
    }
    std::cout << "fromAddress decodeAddress:" << signatureTypeStr + encodeTypeStr + BinToHexString(decodeAddress) << std::endl;

    return signatureTypeStr + encodeTypeStr + BinToHexString(decodeAddress);
}

void solidity::util::bidAddressReplace(std::string &_context)
{
	std::string::size_type posStart = 0;
	std::string::size_type posEnd = 0;
	while(true)
	{
		if( (posStart=_context.find("did:bid", posEnd)) != string::npos )
		{       
			posEnd = 0;
			posEnd = _context.find(";", posStart);
			std::string::size_type posEndTemp = 0;
			if((posEndTemp = _context.find(")", posStart)) != std::string::npos){
				if(posEndTemp < posEnd)
					posEnd = posEndTemp;				
			}
			if((posEndTemp = _context.find("]", posStart)) != std::string::npos){
				if(posEndTemp < posEnd)
					posEnd = posEndTemp;				
			}
			if((posEndTemp = _context.find("\"", posStart)) != std::string::npos){
				if(posEndTemp < posEnd)
					posEnd = posEndTemp;				
			}
			std::string oldbidAddress, newbidAddress;
			oldbidAddress.assign(_context, posStart, posEnd-posStart);
			std::cout << "oldbidAddress:" << oldbidAddress << std::endl;
			newbidAddress = fromBidAddress(oldbidAddress);
			std::cout << "newbidAddress:" << newbidAddress << std::endl;
			_context.replace(posStart,oldbidAddress.length(),"0x"+newbidAddress); 
		}
		else
		{
			break;
		}
	}
}

string solidity::util::readFileAsString(boost::filesystem::path const& _file)
{
	string context = readFile<string>(_file);
	bidAddressReplace(context);

	return context;
}

string solidity::util::readUntilEnd(istream& _stdin)
{
	ostringstream ss;
	ss << _stdin.rdbuf();
	return ss.str();
}

string solidity::util::readBytes(istream& _input, size_t _length)
{
	string output;
	output.resize(_length);
	_input.read(output.data(), static_cast<streamsize>(_length));
	// If read() reads fewer bytes it sets failbit in addition to eofbit.
	if (_input.fail())
		output.resize(static_cast<size_t>(_input.gcount()));
	return output;
}

#if defined(_WIN32)
class DisableConsoleBuffering
{
public:
	DisableConsoleBuffering()
	{
		m_stdin = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(m_stdin, &m_oldMode);
		SetConsoleMode(m_stdin, m_oldMode & (~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT)));
	}
	~DisableConsoleBuffering()
	{
		SetConsoleMode(m_stdin, m_oldMode);
	}
private:
	HANDLE m_stdin;
	DWORD m_oldMode;
};
#else
class DisableConsoleBuffering
{
public:
	DisableConsoleBuffering()
	{
		tcgetattr(0, &m_termios);
		m_termios.c_lflag &= ~tcflag_t(ICANON);
		m_termios.c_lflag &= ~tcflag_t(ECHO);
		m_termios.c_cc[VMIN] = 1;
		m_termios.c_cc[VTIME] = 0;
		tcsetattr(0, TCSANOW, &m_termios);
	}
	~DisableConsoleBuffering()
	{
		m_termios.c_lflag |= ICANON;
		m_termios.c_lflag |= ECHO;
		tcsetattr(0, TCSADRAIN, &m_termios);
	}
private:
	struct termios m_termios;
};
#endif

int solidity::util::readStandardInputChar()
{
	DisableConsoleBuffering disableConsoleBuffering;
	return cin.get();
}

string solidity::util::absolutePath(string const& _path, string const& _reference)
{
	boost::filesystem::path p(_path);
	// Anything that does not start with `.` is an absolute path.
	if (p.begin() == p.end() || (*p.begin() != "." && *p.begin() != ".."))
		return _path;
	boost::filesystem::path result(_reference);

	// If filename is "/", then remove_filename() throws.
	// See: https://github.com/boostorg/filesystem/issues/176
	if (result.filename() != boost::filesystem::path("/"))
		result.remove_filename();
	for (boost::filesystem::path::iterator it = p.begin(); it != p.end(); ++it)
		if (*it == "..")
			result = result.parent_path();
		else if (*it != ".")
			result /= *it;
	return result.generic_string();
}

string solidity::util::sanitizePath(string const& _path) {
	return boost::filesystem::path(_path).generic_string();
}
