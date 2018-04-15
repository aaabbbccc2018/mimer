#include "transmitter.h"
using namespace mimer;
using namespace mm::Transmitter;


int client(int argc,char* argv[]) 
{
    tTM client;
    //uTM client;
    client.Relate(argv[2], 9130, CLIENT);
    //client.Sendto();
    return 0;
}