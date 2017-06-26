#include "MQTTPacket.h"
namespace mqtter {

const char* MQTTPacket::packet_names[] =
{
   "RESERVED", "CONNECT", "CONNACK", "PUBLISH", "PUBACK", "PUBREC", "PUBREL",
   "PUBCOMP", "SUBSCRIBE", "SUBACK", "UNSUBSCRIBE", "UNSUBACK",
   "PINGREQ", "PINGRESP", "DISCONNECT"
};

MQTTPacket::MQTTPacket(void* packet)
{

}

MQTTPacket::MQTTPacket(int type,void* packet):_ptype(type)
{
    switch (_ptype)
    {
    case CONNECT:
        _size   = sizeof(Connect);
        _packet = (Connect*)malloc(_size);
        memcpy(_packet,packet,sizeof(Connect));
        break;
    case CONNACK:
        _size   = sizeof(ConnAck);
        _packet = (ConnAck*)malloc(_size);
        memcpy(_packet,packet,sizeof(ConnAck));
        break;
    case PUBLISH:
        _size   = sizeof(Publish);
        _packet = (Publish*)malloc(_size);
        memcpy(_packet,packet,sizeof(Publish));
        break;
    case PUBACK:
        _size   = sizeof(PubAck);
        _packet = (PubAck*)malloc(_size);
        memcpy(_packet,packet,sizeof(PubAck));
        break;
    case PUBREC:
        _size   = sizeof(PubRec);
        _packet = (PubRec*)malloc(_size);
        memcpy(_packet,packet,sizeof(PubRec));
        break;
    case PUBREL:
        _size   = sizeof(PubRel);
        _packet = (PubRel*)malloc(_size);
        memcpy(_packet,packet,sizeof(PubRel));
        break;
    case PUBCOMP:
        _size   = sizeof(PubComp);
        _packet = (PubComp*)malloc(_size);
        memcpy(_packet,packet,sizeof(PubComp));
        break;
    case SUBSCRIBE:
        _size   = sizeof(Subscribe);
        _packet = (Subscribe*)malloc(_size);
        memcpy(_packet,packet,sizeof(Subscribe));
        break;
    case SUBACK:
        _size   = sizeof(SubAck);
        _packet = (SubAck*)malloc(_size);
        memcpy(_packet,packet,sizeof(SubAck));
        break;
    case UNSUBSCRIBE:
        _size   = sizeof(Unsubscribe);
        _packet = (Unsubscribe*)malloc(_size);
        memcpy(_packet,packet,sizeof(Unsubscribe));
        break;
    case UNSUBACK:
        _size   = sizeof(UnsubAck);
        _packet = (UnsubAck*)malloc(_size);
        memcpy(_packet,packet,sizeof(UnsubAck));
        break;
    case PINGREQ:
        _size   = sizeof(PingReq);
        _packet = (PingReq*)malloc(_size);
        memcpy(_packet,packet,sizeof(PingReq));
        break;
    case PINGRESP:
        _size   = sizeof(PingResp);
        _packet = (PingResp*)malloc(_size);
        memcpy(_packet,packet,sizeof(PingResp));
        break;
    case DISCONNECT:
        _size   = sizeof(Disconnect);
        _packet = (Disconnect*)malloc(_size);
        memcpy(_packet,packet,sizeof(Disconnect));
        break;
    default:
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

}//namespace mqtter
