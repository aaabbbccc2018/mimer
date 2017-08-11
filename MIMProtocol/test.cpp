#include "Stream.h"
#include "MIMPacket.h"
#include "MIMProtocol.h"
using namespace std;
using namespace mimer;

void test_ACK(msgTypes ptype, int dried)
{
    MIMPacket pkt(ptype,dried);
    pkt.setPacketId(1000);
    pAck cn = (pAck)pkt.data();
    std::cout << cn << std::endl;
    char* sendPacket = NULL;
    int packetSize = 0;
    packetSize = pkt.size();
    sendPacket = (char*)malloc(packetSize);
    memset(sendPacket,0,packetSize);
    if(!pkt.encode(sendPacket)){
        return;
    }
    std::cout << pkt;
    MIMProtocol p(sendPacket,ptype);
    std::cout << p;
    std::cout << charStream(sendPacket,packetSize);
    Stream fs("./packet.mq","w");
    fs.Write(sendPacket,packetSize,1);
    fs.Close();
    MIMPacket dpkt(MIMPacket::type(sendPacket[0]));
    dpkt.decode(sendPacket);
    std::cout << dpkt;
    fs.Open("./packet.mq","r");
    char fixheader = (char)fs.GetC();
    MIMPacket dpktf(MIMPacket::type(fixheader));
    // printf("%s",MIMPacket::types(MIMPacket::type((char)fs.GetC())));
    char* sendPacketf = NULL;
    sendPacketf = (char*)malloc(4);
    sendPacketf[0] = fixheader;
    fs.Read(&sendPacketf[1],4,1);
    MIMInt::decode(sendPacketf,packetSize);
    dpktf.decode(sendPacketf);
    std::cout << dpktf;
    free(sendPacket);
    free(sendPacketf);
}

void test_onlyHeader(msgTypes ptype, int dried)
{
    MIMPacket pkt(ptype,dried);
    pPingReq cn = (pPingReq)pkt.data();
    std::cout << cn << std::endl;
    char* sendPacket = NULL;
    int packetSize = 0;
    packetSize = pkt.size();
    sendPacket = (char*)malloc(packetSize);
    memset(sendPacket,0,packetSize);
    if(!pkt.encode(sendPacket)){
        return;
    }
    std::cout << pkt;
    MIMProtocol p(sendPacket,ptype);
    std::cout << p;
    std::cout << charStream(sendPacket,packetSize);
    Stream fs("./packet.mq","w");
    fs.Write(sendPacket,packetSize,1);
    fs.Close();
    MIMPacket dpkt(MIMPacket::type(sendPacket[0]));
    dpkt.decode(sendPacket);
    std::cout << dpkt;
    fs.Open("./packet.mq","r");
    char fixheader = (char)fs.GetC();
    MIMPacket dpktf(MIMPacket::type(fixheader));
    // printf("%s",MIMPacket::types(MIMPacket::type((char)fs.GetC())));
    char* sendPacketf = NULL;
    sendPacketf = (char*)malloc(4);
    memset(sendPacketf,0,4);
    sendPacketf[0] = fixheader;
    fs.Read(&sendPacketf[1],4,1);
    MIMInt::decode(sendPacketf,packetSize);
    dpktf.decode(sendPacketf);
    std::cout << dpktf;
    free(sendPacket);
    free(sendPacketf);
}

void test_CONNECT(int dried)
{
    MIMPacket pkt(CONNECT,dried);
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
    pkt.setMultiConnect();
    pConnect cn = (pConnect)pkt.data();
    std::cout << cn << std::endl;
    char* sendPacket = NULL;
    int packetSize = 0;
    packetSize = pkt.size();
    sendPacket = (char*)malloc(packetSize);
    memset(sendPacket,0,packetSize);
    if(!pkt.encode(sendPacket)){
        return;
    }
    std::cout << pkt;
    MIMProtocol p(sendPacket,CONNECT);
    std::cout << p;
    std::cout << charStream(sendPacket,packetSize);
    Stream fs("./packet.mq","w");
    fs.Write(sendPacket,packetSize,1);
    fs.Close();
    MIMPacket dpkt(MIMPacket::type(sendPacket[0]));
    dpkt.decode(sendPacket);
    std::cout << dpkt;
    fs.Open("./packet.mq","r");
    char fixheader = (char)fs.GetC();
    MIMPacket dpktf(MIMPacket::type(fixheader));
    // printf("%s",MIMPacket::types(MIMPacket::type((char)fs.GetC())));
    char* sendPacketf = NULL;
    sendPacketf = (char*)malloc(4);
    //sendPacketf[0] = fixheader;
    fs.Read(sendPacketf,4,1);
    int packetlen = MIMInt::decode(sendPacketf,packetSize);
    fs.Rewind();
    char* sendPacket2 = (char*)malloc(packetlen+1);
    fs.Read(sendPacket2,packetlen+1,1);
    fs.Close();
    dpktf.decode(sendPacket2);
    std::cout << dpktf;
    free(sendPacket);
    free(sendPacketf);
    free(sendPacket2);
}

void test_CONNACK(int dried)
{
    MIMPacket pkt(CONNACK,dried);
    cackflags caf;
    caf.bits.reserved = 0;
    caf.bits.sessionPresent = 0;
    caf.bits.isregister = 1;
    pkt.setFlags(caf.all);
    pkt.setRC(1);
    pkt.setClientId("123456789",9);
    pConnAck cn = (pConnAck)pkt.data();
    std::cout << cn << std::endl;
    char* sendPacket = NULL;
    int packetSize = 0;
    packetSize = pkt.size();
    sendPacket = (char*)malloc(packetSize);
    memset(sendPacket,0,packetSize);
    if(!pkt.encode(sendPacket)){
        return;
    }
    std::cout << pkt;
    MIMProtocol p(sendPacket,CONNACK);
    std::cout << p;
    std::cout << charStream(sendPacket,packetSize);
    Stream fs("./packet.mq","w");
    fs.Write(sendPacket,packetSize,1);
    fs.Close();
    MIMPacket dpkt(MIMPacket::type(sendPacket[0]));
    dpkt.decode(sendPacket);
    std::cout << dpkt;
    fs.Open("./packet.mq","r");
    char fixheader = (char)fs.GetC();
    MIMPacket dpktf(MIMPacket::type(fixheader));
    // printf("%s",MIMPacket::types(MIMPacket::type((char)fs.GetC())));
    char* sendPacketf = NULL;
    sendPacketf = (char*)malloc(4);
    //sendPacketf[0] = fixheader;
    fs.Read(sendPacketf,4,1);
    int packetlen = MIMInt::decode(sendPacketf,packetSize);
    fs.Rewind();
    char* sendPacket2 = (char*)malloc(packetlen+1);
    fs.Read(sendPacket2,packetlen+1,1);
    fs.Close();
    dpktf.decode(sendPacket2);
    std::cout << dpktf;
    free(sendPacket);
    free(sendPacketf);
    free(sendPacket2);
}

void test_PUBLISH(int dried)
{
    MIMPacket pkt(PUBLISH,dried,0,1);
    pkt.addTopics(0,"hello",6);
    pkt.setPacketId(1);
    // pkt.setPayload("This is a test",15); //???? why crash
    pkt.setPayload("This is a test!",16);
    pPublish cn = (pPublish)pkt.data();
    std::cout << cn << std::endl;
    char* sendPacket = NULL;
    int packetSize = 0;
    packetSize = pkt.size();
    sendPacket = (char*)malloc(packetSize);
    memset(sendPacket,0,packetSize);
    if(!pkt.encode(sendPacket)){
        return;
    }
    std::cout << pkt;
    MIMProtocol p(sendPacket,PUBLISH);
    std::cout << p;
    std::cout << charStream(sendPacket,packetSize);
    Stream fs("./packet.mq","w");
    fs.Write(sendPacket,packetSize,1);
    fs.Close();
    MIMPacket dpkt(MIMPacket::type(sendPacket[0]));
    dpkt.decode(sendPacket);
    std::cout << dpkt;
    fs.Open("./packet.mq","r");
    char fixheader = (char)fs.GetC();
    MIMPacket dpktf(MIMPacket::type(fixheader));
    // printf("%s",MIMPacket::types(MIMPacket::type((char)fs.GetC())));
    char* sendPacketf = NULL;
    sendPacketf = (char*)malloc(4);
    //sendPacketf[0] = fixheader;
    fs.Read(sendPacketf,4,1);
    int packetlen = MIMInt::decode(sendPacketf,packetSize);
    fs.Rewind();
    char* sendPacket2 = (char*)malloc(packetlen+1);
    fs.Read(sendPacket2,packetlen+1,1);
    fs.Close();
    dpktf.decode(sendPacket2);
    std::cout << dpktf;
    free(sendPacket);
    free(sendPacketf);
    free(sendPacket2);
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
    MIMPacket pkt(SUBSCRIBE,dried);
    pkt.setPacketId(1);
    pkt.addTopics(0,"hello",6);
    pkt.addTopics(0,"world",5);
    pkt.addTopics(0,"This ",5);
    pkt.addTopics(0,"is ",3);
    pkt.addTopics(0,"a ",2);
    pkt.addTopics(1,"Test!",5);
    pSubscribe cn = (pSubscribe)pkt.data();
    std::cout << cn << std::endl;
    char* sendPacket = NULL;
    int packetSize = 0;
    packetSize = pkt.size();
    sendPacket = (char*)malloc(packetSize);
    memset(sendPacket,0,packetSize);
    if(!pkt.encode(sendPacket)){
        return;
    }
    std::cout << pkt;
    MIMProtocol p(sendPacket,SUBSCRIBE);
    std::cout << p;
    std::cout << charStream(sendPacket,packetSize);
    Stream fs("./packet.mq","w");
    fs.Write(sendPacket,packetSize,1);
    fs.Close();
    MIMPacket dpkt(MIMPacket::type(sendPacket[0]));
    dpkt.decode(sendPacket);
    std::cout << dpkt;
    fs.Open("./packet.mq","r");
    char fixheader = (char)fs.GetC();
    MIMPacket dpktf(MIMPacket::type(fixheader));
    // printf("%s",MIMPacket::types(MIMPacket::type((char)fs.GetC())));
    char* sendPacketf = NULL;
    sendPacketf = (char*)malloc(4);
    //sendPacketf[0] = fixheader;
    fs.Read(sendPacketf,4,1);
    int packetlen = MIMInt::decode(sendPacketf,packetSize);
    fs.Rewind();
    char* sendPacket2 = (char*)malloc(packetlen+1);
    fs.Read(sendPacket2,packetlen+1,1);
    fs.Close();
    dpktf.decode(sendPacket2);
    std::cout << dpktf;
    free(sendPacket);
    free(sendPacketf);
    free(sendPacket2);
}

void test_SUBACK(int dried)
{
    MIMPacket pkt(SUBACK,dried);
    pkt.setPacketId(1);
    pkt.addTopics(1);
    pkt.addTopics(2);
    pSubAck cn = (pSubAck)pkt.data();
    std::cout << cn << std::endl;
    char* sendPacket = NULL;
    int packetSize = 0;
    packetSize = pkt.size();
    sendPacket = (char*)malloc(packetSize);
    memset(sendPacket,0,packetSize);
    if(!pkt.encode(sendPacket)){
        return;
    }
    std::cout << pkt;
    MIMProtocol p(sendPacket,SUBACK);
    std::cout << p;
    std::cout << charStream(sendPacket,packetSize);
    Stream fs("./packet.mq","w");
    fs.Write(sendPacket,packetSize,1);
    fs.Close();
    MIMPacket dpkt(MIMPacket::type(sendPacket[0]));
    dpkt.decode(sendPacket);
    std::cout << dpkt;
    fs.Open("./packet.mq","r");
    char fixheader = (char)fs.GetC();
    MIMPacket dpktf(MIMPacket::type(fixheader));
    // printf("%s",MIMPacket::types(MIMPacket::type((char)fs.GetC())));
    char* sendPacketf = NULL;
    sendPacketf = (char*)malloc(4);
    //sendPacketf[0] = fixheader;
    fs.Read(sendPacketf,4,1);
    int packetlen = MIMInt::decode(sendPacketf,packetSize);
    fs.Rewind();
    char* sendPacket2 = (char*)malloc(packetlen+1);
    fs.Read(sendPacket2,packetlen+1,1);
    fs.Close();
    dpktf.decode(sendPacket2);
    std::cout << dpktf;
    free(sendPacket);
    free(sendPacketf);
    free(sendPacket2);
}

void test_UNSUBSCRIBE(int dried)
{
    MIMPacket pkt(UNSUBSCRIBE,dried);
    pkt.setPacketId(1);
    pkt.addTopics(0,"good",5);
    pkt.addTopics(0,"bye",4);
    pUnsubscribe cn = (pUnsubscribe)pkt.data();
    std::cout << cn << std::endl;
    char* sendPacket = NULL;
    int packetSize = 0;
    packetSize = pkt.size();
    sendPacket = (char*)malloc(packetSize);
    memset(sendPacket,0,packetSize);
    if(!pkt.encode(sendPacket)){
        return;
    }
    std::cout << pkt;
    MIMProtocol p(sendPacket,UNSUBSCRIBE);
    std::cout << p;
    std::cout << charStream(sendPacket,packetSize);
    Stream fs("./packet.mq","w");
    fs.Write(sendPacket,packetSize,1);
    fs.Close();
    MIMPacket dpkt(MIMPacket::type(sendPacket[0]));
    dpkt.decode(sendPacket);
    std::cout << dpkt;
    fs.Open("./packet.mq","r");
    char fixheader = (char)fs.GetC();
    MIMPacket dpktf(MIMPacket::type(fixheader));
    // printf("%s",MIMPacket::types(MIMPacket::type((char)fs.GetC())));
    char* sendPacketf = NULL;
    sendPacketf = (char*)malloc(4);
    //sendPacketf[0] = fixheader;
    fs.Read(sendPacketf,4,1);
    int packetlen = MIMInt::decode(sendPacketf,packetSize);
    fs.Rewind();
    char* sendPacket2 = (char*)malloc(packetlen+1);
    fs.Read(sendPacket2,packetlen+1,1);
    fs.Close();
    dpktf.decode(sendPacket2);
    std::cout << dpktf;
    free(sendPacket);
    free(sendPacketf);
    free(sendPacket2);
}

void test_UNSUBACK(int dried)
{
    test_ACK(UNSUBACK, dried);
}

void test_PINGREQ(int dried)
{
    MIMPacket pkt(PINGREQ,dried);
    pPingReq cn = (pPingReq)pkt.data();
    std::cout << cn << std::endl;
    pkt.setPingStatus(PING_WRITING);
    char* sendPacket = NULL;
    int packetSize = 0;
    packetSize = pkt.size();
    sendPacket = (char*)malloc(packetSize);
    memset(sendPacket,0,packetSize);
    if(!pkt.encode(sendPacket)){
        return;
    }
    std::cout << pkt;
    MIMProtocol p(sendPacket,PINGREQ);
    std::cout << p;
    std::cout << charStream(sendPacket,packetSize);
    Stream fs("./packet.mq","w");
    fs.Write(sendPacket,packetSize,1);
    fs.Close();
    MIMPacket dpkt(MIMPacket::type(sendPacket[0]));
    dpkt.decode(sendPacket);
    std::cout << dpkt;
    fs.Open("./packet.mq","r");
    char fixheader = (char)fs.GetC();
    MIMPacket dpktf(MIMPacket::type(fixheader));
    // printf("%s",MIMPacket::types(MIMPacket::type((char)fs.GetC())));
    char* sendPacketf = NULL;
    sendPacketf = (char*)malloc(4);
    memset(sendPacketf,0,4);
    sendPacketf[0] = fixheader;
    fs.Read(&sendPacketf[1],4,1);
    MIMInt::decode(sendPacketf,packetSize);
    dpktf.decode(sendPacketf);
    std::cout << dpktf;
    free(sendPacket);
    free(sendPacketf);
}

void test_PINGRESP(int dried)
{
    test_onlyHeader(PINGRESP, dried);
}

void test_DISCONNECT(int dried)
{
    test_onlyHeader(DISCONNECT, dried);
}

