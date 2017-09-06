#include <iostream>
#include "crc.h"
#include "md5.h"
#include "sha1.h"
#include "verify.h"
using namespace std;

int main(int argc, char *argv[])
{
    md5 m;
    m.md5_update("hello",5);
    printf("%s\n",(m.md5_final().c_str()));
    sha1 s;
    s.sha1_update("hello",5);
    printf("%s\n",(s.sha1_final().c_str()));
    printf("%s\n",MD5("hello",5));
    printf("%s\n",MD5("hello"));
    printf("%s\n",SHA1("hello",5));
    printf("%s\n",SHA1("hello"));
    printf("%d\n",HASH("hello",5));
    printf("%d\n",HASH("hello"));
    printf("%s\n",BASE64("hello",5));
    printf("%s\n",BASE64("中国"));
    printf("%s\n",DEBASE64("5Lit5Zu9",8));
    printf("%s\n",DEBASE64("5Lit5Zu9"));
    printf("%d\n",CHECKSUM("hello",5));
    printf("%d\n",CHECKSUM("hello"));
    printf("%0x\n",CRC16("hello",5));
    printf("%0x\n",CRC16("hello"));
    printf("%0x\n",CRC32("hello",5));
    printf("%0x\n",CRC32("hello"));
    printf("%0x\n",CRC64("hello",5));
    printf("%0x\n",CRC64("hello"));
    return 0;
}
