#include "MQTTProtocol.h"

namespace mqtter {

MQTTProtocol::MQTTProtocol():_mqData(NULL),_ptype(0)
{
}

MQTTProtocol::MQTTProtocol(char* content,int size)
{
    _mqData = new MQTTPacket();
    if(_mqData->decode(content,size)){
        _ptype = _mqData->type();
    }else{
        printf("%s", "MQTTpacket decode error!!!\n");
    }
}

MQTTProtocol::MQTTProtocol(int type, int dried, int dup,int qos):_ptype(type)
{
    _mqData = new MQTTPacket(type,dried,dup,qos);
}

bool MQTTProtocol::analyzer()
{
    if(0 == _mqData->type() || 15 == _mqData->type() || false == _mqData->finish()){
        printf("%s", "Invalid packet type or incomplete packet!!!\n");
        return false;
    }
    switch (_ptype)
    {
    case PUBLISH:// TODO
        /* Variable header */
        /* payload */
        break;
    case SUBSCRIBE:// TODO
        /* Variable header */
        /* payload */
        break;
    case SUBACK:// TODO
        /* Variable header */
        /* payload */
        break;
    case CONNECT:// TODO
        /* Variable header */
        /* payload */
        break;
    case CONNACK:
        /* Variable header */
        break;
    case PUBACK:
    case PUBREC:
    case PUBREL:
    case PUBCOMP:
    case UNSUBACK:
        /* Variable header */
        break;
    case UNSUBSCRIBE:// TODO
        /* Variable header */
        /* payload */
        break;
    case PINGREQ:
    case PINGRESP:
    case DISCONNECT:
        printf("packet type don't have Variable header && payload!\n");
        break;
    default:
        printf("error packet type\n");
        break;
    }
    return true;
}

bool MQTTProtocol::controller()
{
    return true;
}

}//namespace mqtter
