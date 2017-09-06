#ifndef CRC_H
#define CRC_H
#include "../../platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint16 crc16(const void* data, size_t size);
uint32 crc32(const void* data, size_t size);
uint64 crc64(const void* data, size_t size);

#endif // CRC_H
