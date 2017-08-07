#ifndef _MQTTER_PROTOCOL_H
#define _MQTTER_PROTOCOL_H
#include <iostream>
#include <string>
#include <map>
#include <string.h>
#include "Stream.h"
#include "MQTTPacket.h"

namespace mqtter {

#define tFMT(PTYPE) ((PTYPE)(_mqData->_packet))
#define BITS(FLAGS) (FLAGS.bits)
#define CONNFLAG    (BITS(tFMT(pConnect)->flags))
#define CONAFLAG    (BITS(tFMT(pConnAck)->flags))
#define pVoid        reinterpret_cast<void*>

struct cmp_str
{
   bool operator()(const char *a, const char *b)
   {
      return (strcmp(a, b) < 0);
   }
};

class MQTTProtocol
{
    typedef std::pair<std::string, void*> PAnalyzer;
    typedef std::map<const char*, const void*, cmp_str>  Analyzer;
    typedef enum MechineType{ CLIENT = 0, SERVER, SERCLI }Mtype;
public:
    MQTTProtocol();
    MQTTProtocol(char* content, int mtype = 0);
    MQTTProtocol(int type, int dried = 0, int dup = 0,int qos = 0);
    ~MQTTProtocol();
public:
    friend std::ostream & operator<<(std::ostream &out, const MQTTProtocol &mp)
    {
        out << *(mp._mqData);
        Analyzer::const_iterator iter;
        for(iter = mp._ctrler.begin();iter != mp._ctrler.end(); ++iter)
        {
            out << iter->first << " :\t" << iter->second << "\n";
        }
        return out;
    }

private:
    bool analyzer();
    bool controller();
private:
    MQTTPacket* _mqData;    // packet's data
    int         _ptype;     // packet's type
    Mtype       _mtype;     // mechine type
    Analyzer    _ctrler;    // save each packet's controller
    bool        _dried;
    Stream*     _stream;
};

}//namespace mqtter

#endif // MQTTPROTOCOL_H
