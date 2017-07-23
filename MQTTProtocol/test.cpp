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
    delete sendPacket;
    delete sendPacketf;
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
    delete sendPacket;
    delete sendPacketf;
}

void test_CONNECT(int dried)
{
    MQTTPacket pkt(CONNECT,dried);
    connflags cf;
    cf.bits.cleanstart = 1;
    cf.bits.isregister = 0;
    cf.bits.password = 1;
    cf.bits.username = 1;
    cf.bits.will = 1;
    cf.bits.willQoS = 0;
    cf.bits.willRetain = 0;
    pkt.setFlags(cf.all);
    pkt.setKAT(10);
    pkt.setClientId("123456789",9);
    pkt.setWill("test","test",4,4);
    pkt.setUserName("skybosi",7);
    pkt.setPasswd("skybosi",7);
    pConnect cn = (pConnect)pkt.data();
    char* sendPacket = NULL;
    int packetSize = 0;
    if(pkt.finish()){
        packetSize = pkt.size();
        sendPacket = (char*)malloc(packetSize);
        memset(sendPacket,0,packetSize);
        pkt.encode(sendPacket);
    }
    std::cout << pkt;
    // std::cout << charStream(sendPacket,packetSize);
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
    //sendPacketf[0] = fixheader;
    fs.Read(sendPacketf,4,1);
    int packetlen = MQTTInt::decode(sendPacketf,packetSize);
    fs.Rewind();
    char* sendPacket2 = (char*)malloc(packetlen+1);
    fs.Read(sendPacket2,packetlen+1,1);
    fs.Close();
    dpktf.decode(sendPacket2,packetlen+1);
    std::cout << dpktf;
    delete sendPacket;
    delete sendPacketf;
    delete sendPacket2;
}

void test_CONNACK(int dried)
{
    MQTTPacket pkt(CONNACK,dried);
    cackflags caf;
    caf.bits.reserved = 0;
    caf.bits.sessionPresent = 0;
    caf.bits.isregister = 1;
    pkt.setFlags(caf.all);
    pkt.setRC(1);
    pkt.setClientId("123456789",9);
    pConnAck cn = (pConnAck)pkt.data();
    char* sendPacket = NULL;
    int packetSize = 0;
    if(pkt.finish()){
        packetSize = pkt.size();
        sendPacket = (char*)malloc(packetSize);
        memset(sendPacket,0,packetSize);
        pkt.encode(sendPacket);
    }
    std::cout << pkt;
    // std::cout << charStream(sendPacket,packetSize);
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
    //sendPacketf[0] = fixheader;
    fs.Read(sendPacketf,4,1);
    int packetlen = MQTTInt::decode(sendPacketf,packetSize);
    fs.Rewind();
    char* sendPacket2 = (char*)malloc(packetlen+1);
    fs.Read(sendPacket2,packetlen+1,1);
    fs.Close();
    dpktf.decode(sendPacket2,packetlen+1);
    std::cout << dpktf;
    delete sendPacket;
    delete sendPacketf;
    delete sendPacket2;
}

void test_PUBLISH(int dried)
{
    MQTTPacket pkt(PUBLISH,dried);
    pkt.addTopics(0,"hello",6);
    pkt.setPacketId(1);
    pkt.setPayload("This is a test",15);
    pPublish cn = (pPublish)pkt.data();
    char* sendPacket = NULL;
    int packetSize = 0;
    if(pkt.finish()){
        packetSize = pkt.size();
        sendPacket = (char*)malloc(packetSize);
        memset(sendPacket,0,packetSize);
        pkt.encode(sendPacket);
    }
    std::cout << pkt;
    // std::cout << charStream(sendPacket,packetSize);
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
    //sendPacketf[0] = fixheader;
    fs.Read(sendPacketf,4,1);
    int packetlen = MQTTInt::decode(sendPacketf,packetSize);
    fs.Rewind();
    char* sendPacket2 = (char*)malloc(packetlen+1);
    fs.Read(sendPacket2,packetlen+1,1);
    fs.Close();
    dpktf.decode(sendPacket2,packetlen+1);
    std::cout << dpktf;
    delete sendPacket;
    delete sendPacketf;
    delete sendPacket2;
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
    MQTTPacket pkt(SUBSCRIBE,dried);
    pkt.setPacketId(1);
    pkt.addTopics(0,"hello",6);
    pkt.addTopics(0,"world",6);
    pSubscribe cn = (pSubscribe)pkt.data();
    char* sendPacket = NULL;
    int packetSize = 0;
    if(pkt.finish()){
        packetSize = pkt.size();
        sendPacket = (char*)malloc(packetSize);
        memset(sendPacket,0,packetSize);
        pkt.encode(sendPacket);
    }
    std::cout << pkt;
    // std::cout << charStream(sendPacket,packetSize);
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
    //sendPacketf[0] = fixheader;
    fs.Read(sendPacketf,4,1);
    int packetlen = MQTTInt::decode(sendPacketf,packetSize);
    fs.Rewind();
    char* sendPacket2 = (char*)malloc(packetlen+1);
    fs.Read(sendPacket2,packetlen+1,1);
    fs.Close();
    dpktf.decode(sendPacket2,packetlen+1);
    std::cout << dpktf;
    delete sendPacket;
    delete sendPacketf;
    delete sendPacket2;
}

void test_SUBACK(int dried)
{
    MQTTPacket pkt(SUBACK,dried);
    pkt.setPacketId(1);
    pkt.addTopics(1);
    pkt.addTopics(2);
    pSubAck cn = (pSubAck)pkt.data();
    char* sendPacket = NULL;
    int packetSize = 0;
    if(pkt.finish()){
        packetSize = pkt.size();
        sendPacket = (char*)malloc(packetSize);
        memset(sendPacket,0,packetSize);
        pkt.encode(sendPacket);
    }
    std::cout << pkt;
    // std::cout << charStream(sendPacket,packetSize);
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
    //sendPacketf[0] = fixheader;
    fs.Read(sendPacketf,4,1);
    int packetlen = MQTTInt::decode(sendPacketf,packetSize);
    fs.Rewind();
    char* sendPacket2 = (char*)malloc(packetlen+1);
    fs.Read(sendPacket2,packetlen+1,1);
    fs.Close();
    dpktf.decode(sendPacket2,packetlen+1);
    std::cout << dpktf;
    delete sendPacket;
    delete sendPacketf;
    delete sendPacket2;
}

void test_UNSUBSCRIBE(int dried)
{
    MQTTPacket pkt(UNSUBSCRIBE,dried);
    pkt.setPacketId(1);
    pkt.addTopics(0,"good",5);
    pkt.addTopics(0,"bye",4);
    pUnsubscribe cn = (pUnsubscribe)pkt.data();
    char* sendPacket = NULL;
    int packetSize = 0;
    if(pkt.finish()){
        packetSize = pkt.size();
        sendPacket = (char*)malloc(packetSize);
        memset(sendPacket,0,packetSize);
        pkt.encode(sendPacket);
    }
    std::cout << pkt;
    // std::cout << charStream(sendPacket,packetSize);
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
    //sendPacketf[0] = fixheader;
    fs.Read(sendPacketf,4,1);
    int packetlen = MQTTInt::decode(sendPacketf,packetSize);
    fs.Rewind();
    char* sendPacket2 = (char*)malloc(packetlen+1);
    fs.Read(sendPacket2,packetlen+1,1);
    fs.Close();
    dpktf.decode(sendPacket2,packetlen+1);
    std::cout << dpktf;
    delete sendPacket;
    delete sendPacketf;
    delete sendPacket2;
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

