#ifndef _MIMER_PROTOCOL_H
#define _MIMER_PROTOCOL_H
#include <iostream>
#include <string>
#include <map>
#include <string.h>
#include "Stream.h"
#include "MIMPacket.h"

namespace mimer {

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
#define CHARS     '0'
#define INTEGER   '1'
static std::map<std::string, char> meta =
{
    /* CONNECT */
    { "Protocol",  CHARS },
    { "version",   INTEGER },
    { "KAT",       INTEGER },
    { "clientID",  CHARS },
    { "username",  CHARS },
    { "password",  CHARS },
    { "willTopic", CHARS }
};

class MIMProtocol
{
    typedef std::pair<std::string, void*> PAnalyzer;
    typedef std::map<const char*, const void*>  Analyzer;
    typedef enum MechineType{ CLIENT = 0, SERVER, SERCLI }Mtype;
public:
    UTIL_API MIMProtocol();
    UTIL_API MIMProtocol(char* content, int mtype = 0);
    UTIL_API MIMProtocol(int type, int dried = 0, int dup = 0,int qos = 0);
    UTIL_API ~MIMProtocol();
public:
    inline const void* operator[](const char* key)
    {
        if(NULL != key){
            return _ctrler[key];
        }else{
            return NULL;
        }
    }
    friend std::ostream & operator<<(std::ostream &out, const MIMProtocol &mp)
    {
        out << *(mp._mqData);
        Analyzer::const_iterator iter;
        for(iter = mp._ctrler.begin();iter != mp._ctrler.end(); ++iter)
        {
            switch (meta[std::string(iter->first)])
            {
            case CHARS:
                out << iter->first << " :\t" << (char*)iter->second << "\n";
                break;
            case INTEGER:
                out << iter->first << " :\t" << (int)iter->second << "\n";
                break;
            default:
                break;
            }
        }
        return out;
    }
private:
    bool analyzer();
    bool controller();
private:
    MIMPacket*  _mqData;    // packet's data
    int         _ptype;     // packet's type
    Mtype       _mtype;     // mechine type
    Analyzer    _ctrler;    // save each packet's controller
    bool        _dried;
    Stream*     _stream;
};

}//namespace mimer

#endif // MIMPROTOCOL_H
