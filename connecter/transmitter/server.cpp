#include <stdio.h>
#include "transmitter.h"
using namespace mm::Transmitter;

void* server_pack(void * data, ssize_t& size)
{
	printf("server Packer data: %s size:%d\n", (char*)data, size);
	return data;
}

void* server_unpack(void * data, ssize_t& size)
{
	printf("server Unpack data: %s size:%d\n", (char*)data, size);
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