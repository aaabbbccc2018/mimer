#include <stdio.h>
#include "transmitter.h"
#include "MIMPacket.h"
#include "MIMProtocol.h"

using namespace mimer;
using namespace mm::Transmitter;
void* server_pack(void * data, ssize_t& size)
{
       printf("server Packer data: %s size:%d\n", (char*)data, size);
       MIMPacket* pkt = NULL;
       if (!strncmp((char*)data, "CONNACK", strlen("CONNACK"))) {
               pkt = new MIMPacket(CONNACK);
               cackflags caf;
               caf.bits.reserved = 0;
               caf.bits.sessionPresent = 0;
               caf.bits.isregister = 1;
               pkt->setFlags(caf.all);
               pkt->setRC(1);
               pkt->setClientId();
       }
       else {
               pkt = new MIMPacket(PUBLISH, 0, 0, 1);
               pkt->addTopics(0, "test", strlen("test"));
               pkt->setPacketId(1);
               pkt->setPayload((char*)data, strlen((char*)data));
       }
       size = pkt->size();
       data = (char*)malloc(size);
       memset(data, 0, size);
       if (!pkt->encode((char*)data)) {
               return NULL;
       }
       return data;
}

void* server_unpack(void * data, ssize_t& size)
{
       printf("server Unpack data: %s size:%d\n", (char*)data, size);
       char* getData = (char*)data;
       std::cout << charStream(getData, size);
       MIMProtocol mp(getData, 1);
       MIMPacket dpkt(MIMPacket::type(getData[0]));
       dpkt.decode(getData);
       //std::cout << dpkt;
       size = dpkt.size();
       data = (char*)malloc(size);
       memset(data, 0, size);
       memcpy(data, dpkt.data(), size);
       return data;
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