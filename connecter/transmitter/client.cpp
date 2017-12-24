#include "transmitter.h"
using namespace mm::Transmitter;

void* client_pack(void * data, ssize_t& size)
{
	printf("client Packer data: %s size:%d\n", (char*)data, size);
	return data;
}

void* client_unpack(void * data, ssize_t& size)
{
	printf("client Unpack data: %s size:%d\n", (char*)data, size);
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