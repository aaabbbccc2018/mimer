#include "transmitter.h"
#include "MIMPacket.h"
using namespace mimer;
using namespace mm::Transmitter;

void* client_pack(void * data, ssize_t& size)
{
       printf("client Packer data: %s size:%d\n", (char*)data, size);
       MIMPacket* pkt = NULL;
       if (!strncmp((char*)data, "CONNECT", strlen("CONNECT"))) {
               pkt = new MIMPacket(CONNECT);
               connflags cf;
               cf.bits.cleanstart = 1;
               cf.bits.isregister = 0;
               cf.bits.password = 1;
               cf.bits.username = 1;
               cf.bits.will = 1;
               cf.bits.willQoS = 0;
               cf.bits.willRetain = 0;
               pkt->setFlags(cf.all);
               pkt->setKAT(10);
               pkt->setClientId();
               pkt->setWill("test", "test", strlen("test"), strlen("test"));
               pkt->setUserName("skybosi", strlen("skybosi"));
               pkt->setPasswd("skybosi", strlen("skybosi"));
               pkt->setMultiConnect();
       }
       else {
               pkt = new MIMPacket(PUBLISH,0,0,1);
               pkt->addTopics(0, "test", strlen("test"));
               pkt->setPacketId(1);
               pkt->setPayload((char*)data, strlen((char*)data));
       }
       size = pkt->size();
       data = (char*)malloc(size);
       memset(data, 0, size);
       //memcpy(data, pkt.data(), size);
       if (!pkt->encode((char*)data)) {
               return NULL;
       }
       //memcpy(data, pkt->data(), size);
       std::cout << charStream((char*)pkt->data(), size);
       return data;
}

void* client_unpack(void * data, ssize_t& size)
{
       printf("client Unpack data: %s size:%d\n", (char*)data, size);
       char* getData = (char*)data;
       MIMPacket dpkt(MIMPacket::type(getData[0]));
       dpkt.decode(getData);
       //std::cout << dpkt;
       size = dpkt.size();
       data = (char*)malloc(size);
       memset(data, 0, size);
       memcpy(data, dpkt.data(), size);
        return data;
 }


int client(int argc,char* argv[]) 
{
	tTM client;
	client.set_packer(client_pack);
	client.set_unpack(client_unpack);
	//uTM client;
	client.Relate("127.0.0.1", 9130, ITM::CLIENT);
	//client.Sendto();
	return 0;
}