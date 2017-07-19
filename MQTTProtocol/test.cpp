#include "Stream.h"
#include "MQTTPacket.h"
using namespace std;
using namespace mqtter;

void test_ACK(msgTypes ptype, int dried)
{
    MQTTPacket pkt(ptype,dried);
    pkt.setPacketId(1000);
    pAck cn = (pAck)pkt.data();
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
    Stream fs("./packet.mq","w");
    fs.Write(sendPacket,packetSize,1);
    fs.Close();
    MQTTPacket dpkt;
    dpkt.decode(sendPacket,packetSize);
    std::cout << dpkt;
    fs.Open("./packet.mq","r");
    char fixheader = (char)fs.GetC();
    MQTTPacket dpktf(MQTTPacket::type(fixheader));
    // printf("%s",MQTTPacket::types(MQTTPacket::type((char)fs.GetC())));
    char* sendPacketf = NULL;
    sendPacketf = (char*)malloc(4);
    sendPacketf[0] = fixheader;
    fs.Read(&sendPacketf[1],4,1);
    MQTTInt::decode(sendPacketf,packetSize);
    dpktf.decode(sendPacketf,packetSize+1);
    std::cout << dpktf;
}

void test_onlyHeader(msgTypes ptype, int dried)
{
    MQTTPacket pkt(ptype,dried);
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
    Stream fs("./packet.mq","w");
    fs.Write(sendPacket,packetSize,1);
    fs.Close();
    MQTTPacket dpkt;
    dpkt.decode(sendPacket,packetSize);
    std::cout << dpkt;
    fs.Open("./packet.mq","r");
    char fixheader = (char)fs.GetC();
    MQTTPacket dpktf(MQTTPacket::type(fixheader));
    // printf("%s",MQTTPacket::types(MQTTPacket::type((char)fs.GetC())));
    char* sendPacketf = NULL;
    sendPacketf = (char*)malloc(4);
    sendPacketf[0] = fixheader;
    fs.Read(&sendPacketf[1],4,1);
    MQTTInt::decode(sendPacketf,packetSize);
    dpktf.decode(sendPacketf,packetSize+1);
    std::cout << dpktf;
}

void test_CONNECT(int dried)
{

}

void test_CONNACK(int dried)
{

}

void test_PUBLISH(int dried)
{

}

void test_PUBACK(int dried)
{
    test_ACK(PUBACK, dried);
}

void test_PUBREC(int dried)
{
    test_ACK(PUBREC, dried);
}

void test_PUBREL(int dried)
{
    test_ACK(PUBREL, dried);
}

void test_PUBCOMP(int dried)
{
    test_ACK(PUBCOMP, dried);
}

void test_SUBSCRIBE(int dried)
{

}

void test_SUBACK(int dried)
{

}

void test_UNSUBSCRIBE(int dried)
{

}

void test_UNSUBACK(int dried)
{
    test_ACK(UNSUBACK, dried);
}

void test_PINGREQ(int dried)
{
   test_onlyHeader(PINGREQ, dried);
}

void test_PINGRESP(int dried)
{
   test_onlyHeader(PINGRESP, dried);
}

void test_DISCONNECT(int dried)
{
    test_onlyHeader(DISCONNECT, dried);
}

