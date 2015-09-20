/*	Copyright (c) 2015 SGH
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
#include "checksum.h"
#include <fstream>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <iomanip>

#ifdef USE_OPENSSL_MD5
#define HAS_MD5_METHOD
// If missing, check the .pro file.
#include <openssl/evp.h>
#endif

static std::uint32_t const g_currentFlags =
#if defined(Q_OS_WIN64)
	FLAG_WINANY|FLAG_WIN64;
#elif defined(Q_OS_WIN32)
	FLAG_WINANY|FLAG_WIN32;
#elif defined(Q_OS_LINUX)
	FLAG_LINUX;
#elif defined(Q_OS_OSX)
	FLAG_MAC;
#else
	0;
#endif

ChecksumStorageInfo::ChecksumStorageInfo(std::string checksum, uint32_t flags)
	: m_checksum(std::move(checksum)), m_flags(flags) {}
bool ChecksumStorageInfo::Required() const { return (m_flags & g_currentFlags) != 0; }
bool ChecksumStorageInfo::Compare(const std::string& computedChecksum) const {
	if(m_checksum.empty())
		return true;
#ifndef HAS_MD5_METHOD
	if(computedChecksum.empty())
		return true;
#endif
	return m_checksum == computedChecksum;
}

#ifdef HAS_MD5_METHOD
template <size_t Size>
struct ByteBuffer {
	std::uint8_t m_data[Size];
};
enum { g_file_buffer_size = 32768 };
#endif

bool FileChecksum(std::string const& filename, std::string& checksum) {
	checksum.clear();
	std::ifstream file(filename,std::ios_base::in|std::ios_base::binary);
	if(!file)
		return false;
#ifdef USE_OPENSSL_MD5
	if(!file.seekg(0,std::ios_base::end))
		return false;
	std::fstream::pos_type size = file.tellg();
	if(size < 0)
		return false;
	if(!file.seekg(0,std::ios_base::beg))
		return false;
	
	std::string contents;
	contents.resize(size);
	if(!file.read(&contents[0],size))
		return false;
	file.close();
	
	EVP_MD const* md = EVP_md5();
	if(!md)
		throw std::domain_error("OpenSSL has not been configured for MD5 support.");
	
	EVP_MD_CTX* mdctx = EVP_MD_CTX_create();
	if(!mdctx)
		return false;
	
	if(!EVP_DigestInit_ex(mdctx,md,nullptr))
	{
		EVP_MD_CTX_destroy(mdctx);
		return false;
	}
	
	if(!EVP_DigestUpdate(mdctx, contents.c_str(), contents.size()))
	{
		EVP_MD_CTX_destroy(mdctx);
		return false;
	}
	contents.clear();
	
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len =0;
	if(!EVP_DigestFinal_ex(mdctx,md_value,&md_len))
	{
		EVP_MD_CTX_destroy(mdctx);
		return false;
	}
	EVP_MD_CTX_destroy(mdctx);
	
	std::ostringstream oss;
	for(unsigned int i=0; i < md_len; ++i)
		oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(md_value[i]);
	checksum = oss.str();
#endif
	return true;
}
