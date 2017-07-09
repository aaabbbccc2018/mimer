#ifndef _MQTTER_STREAM_H
#define _MQTTER_STREAM_H
#include <stdio.h>
#include <stdlib.h>
#include "MQTTInt.h"

class Stream
{
public:
    Stream(FILE* fp = NULL);
    Stream(const char* filename, const char* mode);
    virtual ~Stream();
public:
    bool Open(const char* filename, const char* mode);
    virtual bool Close();
    virtual size_t	Read(void *buffer, size_t size, size_t count);
    virtual size_t	Write(const void *buffer, size_t size, size_t count);
    virtual bool Seek(int offset, int origin);
    virtual int Tell();
    virtual int	Size();
    virtual bool Flush();
    virtual bool Eof();
    virtual int	Error();
    virtual bool PutC(char c);
    virtual int	GetC();
    virtual char* GetS(char *string, int n);
    virtual int	Scanf(const char *format, void* output);
protected:
    FILE *  _fp;
    bool    _isOpen;
};

#endif // Stream.h
