#include <stdio.h>
#include "transmitter.h"


using namespace mimer;
using namespace mm::Transmitter;
void* server_pack(void * data, ssize_t& size)
{
       printf("server Packer data: %s size:%d\n", (char*)data, size);
       MIMProtocol* mp = NULL;
       if (!strncmp((char*)data, "CONNACK", strlen("CONNACK"))) {
           mp = new MIMProtocol(CONNECT, SERVER);
           return mp->response(data, (ssize_t)size);
       }
       else {
           mp = new MIMProtocol(PUBLISH, SERVER);
           return mp->response(data, (ssize_t)size);
       }
       return NULL;
}

void* server_unpack(void * data, ssize_t& size)
{
       printf("server Unpack data: %s size:%d\n", (char*)data, size);
       char* getData = (char*)data;
       std::cout << charStream(getData, size);
       MIMProtocol mp(getData, 1);
       char* rtData = NULL;
       return mp.response(data, size);

 }

int server(int argc, char* argv[])
{
    tTM server;
    server.set_packer(server_pack);
    server.set_unpack(server_unpack);
    //uTM server;
    server.Relate("127.0.0.1", 9130);
    return 0;
}