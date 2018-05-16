#ifndef _MIMER_PROTOCOL_H
#define _MIMER_PROTOCOL_H
#include <iostream>
#include <string>
#include <map>
#include <string.h>
#include "ellog.h"
#include "comdefine.h"
#include "MIMPacket.h"
#include "Controller.h"
/*
    MIM1数据包解析，所有的处理操作以回调的方式，不向上层返回任何数据，数据只在该模块内流动

    接收到数据包的处理流程

                                                                    origin-data回调上层操作
                                                                        _____________
                                                                        |           |
      输入数据             解析数据包           处理及响应      / ===>  | CallBack  |
    _____________         _____________        _____________   |        |___________|
    |           |         |           |        |           |   |
    |  in-data  |   ===>  |  unpacket |   ===> | iTransfer | ==>        后续数据包处理
    |___________|         |___________|        |___________|   |        _____________
                                                               |        |           |
                                                                \ ===>  |save/delete|
                                                                        |___________|
    
    上层需要创建一种数据包，用于后续操作流程
    
                                                                    packet-data回调上层操作
                                                                        _____________
                                                                        |           |
      原始数据             包装成数据包         处理及响应      / ===>  | CallBack  |
    _____________         _____________        _____________   |        |___________|
    |           |         |           |        |           |   |
    |origin data|   ===>  |   packet  |   ===> | iTransfer | ==>        后续数据包处理
    |___________|         |___________|        |___________|   |        _____________
                                                               |        |           |
                                                                \ ===>  |save/delete|
                                                                        |___________|
    

*/
namespace mimer {

#define tFMT(PTYPE) ((PTYPE)(_mqData->_packet))
#define BITS(FLAGS) (FLAGS.bits)
#define HEADERFLAG  (tFMT(pHeader)->bits)
#define CONNFLAG    (BITS(tFMT(pConnect)->flags))
#define CONAFLAG    (BITS(tFMT(pConnAck)->flags))
#define pVoid        reinterpret_cast<void*>

#define DELETE(ptr) { if (ptr) { delete ptr; ptr = NULL; } }
typedef struct {
    void*   data;
    size_t  size;
    int     errcode;
}CallBack;
#define initCallBack(code) { NULL,code }

struct cmp_str
{
   bool operator()(const char *a, const char *b)
   {
      return (strcmp(a, b) < 0);
   }
};
#define CHARS     '0'
#define INTEGER   '1'
#define LIST1     '2'
#define LIST2     '3'
static std::map<std::string, char> meta =
{
    /* All */
    { "TYPE",      INTEGER },
    /* CONNECT */
    { "Protocol",  CHARS },
    { "version",   INTEGER },
    { "KAT",       INTEGER },
    { "clientID",  CHARS },
    { "username",  CHARS },
    { "password",  CHARS },    
    { "willTopic", CHARS },
    { "willMsg",   CHARS },
    { "willRetain",INTEGER },
    { "passwordlen",INTEGER },
    /* CONNACK */
    { "clientID",  CHARS },
    { "sessionPresent", INTEGER },
    { "RC",        INTEGER },
    /* PUBLISH SUBSCRIBE SUBACK PUBACK PUBREC PUBREL PUBCOMP UNSUBACK UNSUBSCRIBE */
    { "packetId",  INTEGER },
    /* SUBSCRIBE UNSUBSCRIBE */
    { "topics",    LIST1 },
    /* PINGREQ */
    { "cstatus",   INTEGER },
    /* PUBLISH */
    { "topic",     CHARS },
    { "topiclen",  INTEGER },
    { "payload",   CHARS },
    { "payloadlen",INTEGER },
    /* SUBSCRIBE SUBACK */
    { "qoss",      LIST2  }
};

class PINGTimer;
class MIMProtocol
{
    typedef std::pair<std::string, void*> PAnalyzer;
public:
    UTIL_API MIMProtocol(iTransfer* handler = NULL);
    UTIL_API MIMProtocol(char* content);
    UTIL_API MIMProtocol(int ptype, int dried = 0, int dup = 0,int qos = 0);
    UTIL_API ~MIMProtocol();
public:
    inline const void* operator[](const char* key) const
    {
        if (NULL != key) {
            const Analyzer::const_iterator it = _ctrler.find(key);
            if (it != _ctrler.end()) {
                return it->second;
            }            
        }
        else {
            return NULL;
        }
    }
    UTIL_API void  request(void * data, size_t& size, packetTypes method);
    UTIL_API void  response(void * data, size_t& size);
    UTIL_API void  bind(iTransfer* h) { _user = h; }
    UTIL_API Analyzer& getAnalyzer() { return _ctrler; }
    UTIL_API const void* get(const char* key) const {
        if (NULL != key) {
            Analyzer::const_iterator it = _ctrler.find(key);
            if (it != _ctrler.end()) {
                return it->second;
            }
        }
        else {
            return NULL;
        }
    }
    friend std::ostream & operator<<(std::ostream &out, const MIMProtocol &mp)
    {
        //out << *(mp._mqData);
        Analyzer::const_iterator iter;
        ListSub* sublist = NULL;
        Subitor  itlist;
        ListQos* subqoss = NULL;
        Qositor  itqoss;
        for(iter = mp._ctrler.begin();iter != mp._ctrler.end(); ++iter)
        {
            switch (meta[std::string(iter->first)])
            {
            case CHARS:
                out << iter->first << " :\t" << (char*)iter->second << "\n";
                break;
            case INTEGER:
                out << iter->first << " :\t" << (long)iter->second << "\n";
                break;
            case LIST1:
                sublist = (ListSub*)(iter->second);
                itlist = sublist->begin();
                for(; itlist != sublist->end(); ++itlist){
                    out << *itlist << "\n";
                }
                break;
            case LIST2:
                subqoss = (ListQos*)(iter->second);
                itqoss = subqoss->begin();
                for(; itqoss != subqoss->end(); ++itqoss){
                    out << *itqoss << "\n";
                }
                break;
            default:
                break;
            }
        }
        return out;
    }
private:
    bool analyzer(void* data, size_t& size);
    bool analyzer();
    CallBack* ret(MIMPacket* pkt, void* data, size_t& size);
    CallBack* ret_err(int ptype, int errcode, void* data);
private:
    iTransfer * _user;
    MIMPacket*  _mqData;    // packet's data
    int         _ptype;     // packet's type
    size_t      _packetId;  // packet id
    Analyzer    _ctrler;    // save each packet's controller
    Controller* _monitor;
    bool        _dried;
    Stream*     _stream;
    mim::ellog* _loger;
};

}//namespace mimer

#endif // MIMPROTOCOL_H
