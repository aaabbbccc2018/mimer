#include <stdio.h>
#include "transmitter.h"


using namespace mimer;
using namespace mm::Transmitter;

int server(int argc, char* argv[])
{
    tTM server;
    //uTM server;
    server.Relate(argv[2], 9130);
    return 0;
}
