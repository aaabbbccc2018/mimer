#ifndef _MQTTER_PROTOCOL_H
#define _MQTTER_PROTOCOL_H
#include "Stream.h"
#include "MQTTPacket.h"

namespace mqtter {

class MQTTProtocol
{
public:
    MQTTProtocol();
    MQTTProtocol(char* content, int size);
    MQTTProtocol(int type, int dried = 0, int dup = 0,int qos = 0);
public:
    bool analyzer();
    bool controller();
private:
    MQTTPacket* _mqData;
    int         _ptype;     // packet's type
    Stream*     _stream;
};

}//namespace mqtter

#endif // MQTTPROTOCOL_H
