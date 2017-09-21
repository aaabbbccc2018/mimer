#ifndef VERIFY_H
#define VERIFY_H
#include "../../platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "crc.h"
#include "md5.h"
#include "sha1.h"
#include "base64.h"

static const char* MD5(const void *data, size_t size){
    md5 m;
    m.md5_update(data,size);
    return m.md5_final().c_str();
}
static const char* SHA1(const void *data, size_t size){
    sha1 s;
    s.sha1_update(data,size);
    return s.sha1_final().c_str();
}
static const char* MD5(const char *data){
    return MD5(data,strlen(data));
}
static const char* SHA1(const char *data){
    return SHA1(data,strlen(data));
}
/* BKDRHash */
static unsigned int HASH(const void *data, size_t size)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    char* str = (char*)data;
    size_t i = 0;
    while (i < size){ hash = hash * seed + (str[i++]); }
    return (hash & 0x7FFFFFFF);
}
static unsigned int HASH(const char *data)
{
    return HASH(data,strlen(data));
}
static const char* BASE64(const void *data,size_t size){
    return base64_encode(data,size).c_str();
}
static const char* BASE64(const char *data){
    return base64_encode(data,strlen(data)).c_str();
}
static const char* DEBASE64(const void *data,size_t size){
    const char* out = base64_decode(data,size);
    std::string ourstr = (std::string)(out);
    free((void*)out);
    return ourstr.c_str();
}
static const char* DEBASE64(const char *data){
    const char* out = base64_decode(data,strlen(data));
    std::string ourstr = (std::string)(out);
    free((void*)out);
    return ourstr.c_str();
}
static uint16 CHECKSUM(const void *data, size_t size)
{
    unsigned long cksum=0;
    char* buffer = (char*)data;
    while (size > 1)
    {
        cksum += *buffer++;
        size -= sizeof(unsigned short);
    }
    if (size)
    {
        cksum += *(unsigned char*)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    return (unsigned short)(~cksum);
}
static uint16 CHECKSUM(const char *data)
{
    return CHECKSUM(data,strlen(data));
}
static uint16 CRC16(const void *data, size_t size)
{
    return crc16(data,size);
}
static uint16 CRC16(const char *data)
{
    return crc16(data,strlen(data));
}
static uint32 CRC32(const void *data, size_t size)
{
    return crc32(data,size);
}
static uint32 CRC32(const char* data)
{
    return crc32(data,strlen(data));
}
static uint64 CRC64(const void *data, size_t size)
{
    return crc64(data,size);
}
static uint64 CRC64(const char* data)
{
    return crc64(data,strlen(data));
}
#endif // VERIFY_H
