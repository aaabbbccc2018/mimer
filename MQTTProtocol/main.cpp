#include <iostream>
#include "Stream.h"
#include "MQTTPacket.h"
#include "List.h"
using namespace std;
using namespace mqtter;
int main(int argc, char *argv[])
{
    MQTTPacket pkt(PINGREQ,1);
    pPingReq cn = (pPingReq)pkt.data();
    char* sendPacket = NULL;
    int packetSize = 0;
    if(pkt.finish()){
        packetSize = pkt.size();
        sendPacket = (char*)malloc(packetSize);
        memset(sendPacket,0,packetSize);
        pkt.encode(sendPacket);
    }
    std::cout << pkt;
    std::cout << charStream(sendPacket,packetSize);
    MQTTPacket dpkt;
    dpkt.decode(sendPacket,packetSize);
    std::cout << dpkt;
    cout << "Hello World!" << endl;
    return 0;
}
