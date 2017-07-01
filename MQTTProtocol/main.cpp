#include <iostream>
#include "MQTTPacket.h"

using namespace std;
using namespace mqtter;
int main(int argc, char *argv[])
{
    MQTTPacket pkt(PUBREL);
    pPubRel cn = (pPubRel)pkt.data();
    std::cout << pkt;
    cout << "Hello World!" << endl;
    return 0;
}
