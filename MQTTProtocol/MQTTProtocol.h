#ifndef _MQTTER_PROTOCOL_H
#define _MQTTER_PROTOCOL_H
#include "Stream.h"
#include "MQTTPacket.h"

namespace mqtter {

class MQTTProtocol
{
    typedef enum MechineType{
        SERVER = 0,
        CLIENT,
        SERCLI
    }Mtype;
public:
    MQTTProtocol();
    MQTTProtocol(char* content, int size, int mtype = 0);
    MQTTProtocol(int type, int dried = 0, int dup = 0,int qos = 0);
public:
    bool analyzer();
    bool controller();
private:
    MQTTPacket* _mqData;
    int         _ptype;     // packet's type
    Mtype       _mtype;
    Stream*     _stream;
};

}//namespace mqtter

#endif // MQTTPROTOCOL_H
