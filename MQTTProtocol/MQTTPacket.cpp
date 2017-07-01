#include "MQTTPacket.h"
namespace mqtter {

const char* MQTTPacket::packet_names[] =
{
   "RESERVED", "CONNECT", "CONNACK", "PUBLISH", "PUBACK", "PUBREC", "PUBREL",
   "PUBCOMP", "SUBSCRIBE", "SUBACK", "UNSUBSCRIBE", "UNSUBACK",
   "PINGREQ", "PINGRESP", "DISCONNECT"
};

MQTTPacket::MQTTPacket(int type,int dup,int qos):_ptype(type),_size(1)
{
    printf("Packet Type:%s\n", packet_names[type]);
    switch (_ptype)
    {
    case CONNECT:
        _packet = (pConnect)malloc(sizeof(Connect));
        memset(_packet,0,sizeof(Connect));
        initHeader0(pConnect,CONNECT)
        break;
    case CONNACK:
        _packet = (pConnAck)malloc(sizeof(ConnAck));
        memset(_packet,0,sizeof(ConnAck));
        initHeader0(pConnAck,CONNACK)
        break;
    case PUBLISH:
        _packet = (pPublish)malloc(sizeof(Publish));
        memset(_packet,0,sizeof(Publish));
        initHeader3(pPublish,PUBLISH,dup,qos)
        break;
    case PUBACK:
        _size   = sizeof(PubAck);
        _packet = (pPubAck)malloc(sizeof(PubAck));
        memset(_packet,0,sizeof(PubAck));
        initHeader0(pPubAck,PUBACK)
        break;
    case PUBREC:
        _packet = (pPubRec)malloc(sizeof(PubRec));
        memset(_packet,0,sizeof(PubRec));
        initHeader0(pPubRec,PUBREC)
        break;
    case PUBREL:
        _packet = (pPubRel)malloc(sizeof(PubRel));
        memset(_packet,0,sizeof(PubRel));
        initHeader1(pPubRel,PUBREL)
        break;
    case PUBCOMP:
        _packet = (pPubComp)malloc(sizeof(PubComp));
        memset(_packet,0,sizeof(PubComp));
        initHeader0(pPubComp,PUBCOMP)
        break;
    case SUBSCRIBE:
        _size   = sizeof(Subscribe);
        _packet = (pSubscribe)malloc(sizeof(Subscribe));
        memset(_packet,0,sizeof(Subscribe));
        initHeader1(pSubscribe,SUBSCRIBE)
        break;
    case SUBACK:
        _packet = (pSubAck)malloc(sizeof(SubAck));
        memset(_packet,0,sizeof(SubAck));
        initHeader0(pSubAck,SUBACK)
        break;
    case UNSUBSCRIBE:
        _packet = (pUnsubscribe)malloc(sizeof(Unsubscribe));
        memset(_packet,0,sizeof(Unsubscribe));
        initHeader1(pUnsubscribe,UNSUBSCRIBE)
        break;
    case UNSUBACK:
        _packet = (pUnsubAck)malloc(sizeof(UnsubAck));
        memset(_packet,0,sizeof(UnsubAck));
        initHeader0(pUnsubAck,UNSUBACK)
        break;
    case PINGREQ:
        _packet = (pPingReq)malloc(sizeof(PingReq));
        memset(_packet,0,sizeof(PingReq));
        initHeader0(pPingReq,PINGREQ)
        break;
    case PINGRESP:
        _packet = (pPingResp)malloc(sizeof(PingResp));
        memset(_packet,0,sizeof(PingResp));
        initHeader0(pPingResp,PINGRESP)
        break;
    case DISCONNECT:
        _packet = (pDisconnect)malloc(sizeof(Disconnect));
        memset(_packet,0,sizeof(Disconnect));
        initHeader0(pDisconnect,DISCONNECT)
        break;
    default:
        printf("error packet type\n");
        break;
    }
}

MQTTPacket::~MQTTPacket()
{
    if(_packet){
        free(_packet);
        _packet = NULL;
    }
}

std::ostream & operator<<(std::ostream &out, const MQTTPacket &mp)
{
    return out << "Packet Type: " << mp.packet_names[mp._ptype]
               << "["<< mp._ptype << "] Packet Size: " << mp._size << std::endl;
}

}//namespace mqtter
