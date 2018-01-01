#include "transmitter.h"
using namespace mimer;
using namespace mm::Transmitter;

void* client_pack(void * data, ssize_t& size)
{
       printf("client Packer data: %s size:%d\n", (char*)data, size);
       MIMPacket* pkt = NULL;
       MIMProtocol* mp = NULL;
       if (!strncmp((char*)data, "CONNECT", strlen("CONNECT"))) {
           mp = new MIMProtocol(CONNECT, CLIENT);
           return mp->request(data, size);
       }
       else {
           mp = new MIMProtocol(PUBLISH, CLIENT);
           return mp->request(data, size);
       }
       return NULL;
}

void* client_unpack(void * data, ssize_t& size)
{
       printf("client Unpack data: %s size:%d\n", (char*)data, size);
       char* getData = (char*)data;
       MIMPacket dpkt(MIMPacket::type(getData[0]));
       dpkt.decode(getData);
       MIMProtocol mp(getData, CLIENT);
       return mp.request(data, size);
 }


int client(int argc,char* argv[]) 
{
    tTM client;
    client.set_packer(client_pack);
    client.set_unpack(client_unpack);
    //uTM client;
    client.Relate("127.0.0.1", 9130, CLIENT);
    //client.Sendto();
    return 0;
}