/*	Copyright (c) 2015 IsoJumper
**	
**	Permission is granted to use, modify and redistribute this software.
**	Modified versions of this software MUST be marked as such.
**	
**	This software is provided "AS IS". In no event shall
**	the authors or copyright holders be liable for any claim,
**	damages or other liability. The above copyright notice
**	and this permission notice shall be included in all copies
**	or substantial portions of the software.
**	
**	File created on: 20/09/2015
*/

#ifndef CHECKSUM_H
#define CHECKSUM_H
#include <string>
enum ChecksumFlag {
	FLAG_WINANY		=0x00000001,
	FLAG_WIN64		=0x00000002,
	FLAG_WIN32		=0x00000004,
	FLAG_LINUX		=0x00000008,
	FLAG_MAC		=0x00000010
};

struct ChecksumStorageInfo {
	ChecksumStorageInfo(std::string =std::string(), std::uint32_t =0);
	bool Required() const;
	bool Compare(std::string const&) const;
	
	std::string m_checksum;
	std::uint32_t m_flags;
};

bool FileChecksum(std::string const& filename, std::string& checksum);
#endif // CHECKSUM_H
