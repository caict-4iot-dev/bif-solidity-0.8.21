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
/** @file CommonIO.cpp
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 */

#include "CommonIO.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif
#include <boost/filesystem.hpp>
#include "Assertions.h"

using namespace std;
using namespace dev;

namespace
{

template <typename _T>
inline _T readFile(std::string const& _file)
{
	_T ret;
	size_t const c_elementSize = sizeof(typename _T::value_type);
	std::ifstream is(_file, std::ifstream::binary);
	if (!is)
		return ret;

	// get length of file:
	is.seekg(0, is.end);
	streamoff length = is.tellg();
	if (length == 0)
		return ret; // do not read empty file (MSVC does not like it)
	is.seekg(0, is.beg);

	ret.resize((length + c_elementSize - 1) / c_elementSize);
	is.read(const_cast<char*>(reinterpret_cast<char const*>(ret.data())), length);
	return ret;
}

}

std::string dev::BinToHexString(const std::string &value) {
	std::string result;
	result.resize(value.size() * 2);
	for (size_t i = 0; i < value.size(); i++) {
		uint8_t item = value[i];
		uint8_t high = (item >> 4);
		uint8_t low = (item & 0x0F);
		result[2 * i] = (high <= 9) ? (high + '0') : (high - 10 + 'a');
		result[2 * i + 1] = (low <= 9) ? (low + '0') : (low - 10 + 'a');
	}
	return result;
}

int dev::Base58Decode(const std::string &strIn, std::string &strout) {
	const char* kBase58Dictionary = "123456789AbCDEFGHJKLMNPQRSTuVWXYZaBcdefghijkmnopqrstUvwxyz";
	std::size_t nZeros = 0;
	for (; nZeros < strIn.size() && strIn.at(nZeros) == kBase58Dictionary[0]; nZeros++);
	std::size_t left_size = strIn.size() - nZeros;
	std::size_t new_size = std::size_t(left_size * log2(58.0) / 8 + 2);
	std::string tmp_str(new_size, 0);
	int carry = 0;
	for (size_t i = nZeros; i < strIn.size(); i++) {
		carry = (int)dev::kBase58digits[(int)strIn[i]];
		for (int j = new_size - 1; j >= 0; j--) {
			int tmp = (unsigned char)tmp_str[j] * 58 + carry;
			tmp_str[j] = (unsigned char)(tmp % 256);
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
	return nZeros + tmp_str.size() - k;
}

std::string dev::fromBidAddress(std::string const &_a)
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

void dev::bidAddressReplace(std::string &_context)
{
	std::string::size_type posStart = 0;
	std::string::size_type posEnd = 0;
	while(true)
	{
		if( (posStart=_context.find("did:bid", posEnd)) != string::npos )
		{       
			//²éÕÒµØÖ·µÄ½áÎ²±êÖ¾";"
			posEnd = 0;
			posEnd = _context.find(";", posStart);
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


string dev::readFileAsString(string const& _file)
{
	string context = readFile<string>(_file);
	bidAddressReplace(context);

	return context;
}

string dev::readStandardInput()
{
	string ret;
	while (!cin.eof())
	{
		string tmp;
		// NOTE: this will read until EOF or NL
		getline(cin, tmp);
		ret.append(tmp);
		ret.append("\n");
	}
	return ret;
}

void dev::writeFile(std::string const& _file, bytesConstRef _data, bool _writeDeleteRename)
{
	namespace fs = boost::filesystem;
	if (_writeDeleteRename)
	{
		fs::path tempPath = fs::unique_path(_file + "-%%%%%%");
		writeFile(tempPath.string(), _data, false);
		// will delete _file if it exists
		fs::rename(tempPath, _file);
	}
	else
	{
		// create directory if not existent
		fs::path p(_file);
		if (!fs::exists(p.parent_path()))
		{
			fs::create_directories(p.parent_path());
			try
			{
				fs::permissions(p.parent_path(), fs::owner_all);
			}
			catch (...)
			{
			}
		}

		ofstream s(_file, ios::trunc | ios::binary);
		s.write(reinterpret_cast<char const*>(_data.data()), _data.size());
		assertThrow(s, FileError, "Could not write to file: " + _file);
		try
		{
			fs::permissions(_file, fs::owner_read|fs::owner_write);
		}
		catch (...)
		{
		}
	}
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
		m_termios.c_lflag &= ~ICANON;
		m_termios.c_lflag &= ~ECHO;
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

int dev::readStandardInputChar()
{
	DisableConsoleBuffering disableConsoleBuffering;
	return cin.get();
}

boost::filesystem::path dev::weaklyCanonicalFilesystemPath(boost::filesystem::path const &_path)
{
	if (boost::filesystem::exists(_path))
		return boost::filesystem::canonical(_path);
	else
	{
		boost::filesystem::path head(_path);
		boost::filesystem::path tail;
		for (auto it = --_path.end(); !head.empty(); --it)
		{
			if (boost::filesystem::exists(head))
				break;
			tail = (*it) / tail;
			head.remove_filename();
		}
		head = boost::filesystem::canonical(head);
		return head / tail;
	}
}
