#include "MIMProtocol.h"

namespace mimer {

MIMProtocol::MIMProtocol():_mqData(NULL),_ptype(0),_mtype(CLIENT),_dried(0),_stream(NULL)
{
}

MIMProtocol::MIMProtocol(char* content,int mtype):_mtype((Mtype)mtype),_dried(0),_stream(NULL)
{
    _mqData = new MIMPacket(MIMPacket::type(content[0]));
    if(_mqData->decode(content)){
        _ptype = _mqData->type();
        _dried = tFMT(pHeader)->bits.retain;
    }else{
        printf("%s", "MIMpacket decode error!!!\n");
    }
    analyzer();
}

MIMProtocol::MIMProtocol(int type, int dried, int dup,int qos):_ptype(type),_dried(dried),_stream(NULL)
{
    _mqData = new MIMPacket(type,dried,dup,qos);
}

MIMProtocol::~MIMProtocol()
{
    if(_mqData) {
        delete _mqData;
        _mqData = NULL;
    }
    if(_stream){
        delete _stream;
        _stream = NULL;
    }
}

bool MIMProtocol::analyzer()
{
    if(0 >= _ptype || 15 <= _ptype /*|| false == _mqData->finish()*/){
        printf("%s", "Invalid packet type or incomplete packet!!!\n");
        return false;
    }
    switch (_ptype)
    {
    case PUBLISH:
        _ctrler["packetId"] = pVoid(tFMT(pPublish)->packetId);
        _ctrler["topic"]    = (tFMT(pPublish)->topic);
        _ctrler["payload"]  = (tFMT(pPublish)->payload);
        break;
    case SUBSCRIBE:
        _ctrler["packetId"] = pVoid(tFMT(pSubscribe)->packetId);
        _ctrler["topics"]   = (tFMT(pSubscribe)->topics);
        _ctrler["qoss"]     = (tFMT(pSubscribe)->qoss);
        break;
    case SUBACK:
        _ctrler["packetId"] = pVoid(tFMT(pSubAck)->packetId);
        _ctrler["qoss"]     = (tFMT(pSubAck)->qoss);
        break;
    case CONNECT:
        _ctrler["Protocol"] = (tFMT(pConnect)->Protocol);
        _ctrler["version"]  = pVoid(tFMT(pConnect)->version);
        _ctrler["KAT"]      = pVoid(tFMT(pConnect)->KAT);
        _ctrler["clientID"] = (tFMT(pConnect)->clientID);
        /* connect flags byte */
        if(CONNFLAG.will){
            // get willTopic and willMsg TODO
            _ctrler["willTopic"] = (tFMT(pConnect)->willTopic);
            _ctrler["willMsg"]   = (tFMT(pConnect)->willMsg);
            _ctrler["willRetain"] = pVoid(CONNFLAG.willRetain);
            /*
            if(CONNFLAG.willRetain){
                // must save will message after publish will message TODO
                _ctrler["willRetain"] = pVoid(CONNFLAG.willRetain);
            }else{
                // delete will message after publish will message TODO
                _ctrler["willRetain"] = pVoid(CONNFLAG.willRetain);
            }
            */
        }
        /* get username and password then Verify it, if false return error code */
        if(CONNFLAG.username && CONNFLAG.password){
            _ctrler["username"] = (tFMT(pConnect)->userName);
            _ctrler["password"] = (tFMT(pConnect)->passwd);
        }else{
            // is test user
            _ctrler["username"] = "skybosi";
            _ctrler["password"] = "skybosi";
        }
        if(CONNFLAG.isregister){
            // create a new comer
            _ctrler["newer"] = "1111111111111111";
        }
        break;
    case CONNACK:
        if(CONAFLAG.isregister){
            _ctrler["clientID"] = (tFMT(pConnAck)->clientID);
        }
        if(CONAFLAG.sessionPresent){
            _ctrler["sessionPresent"] = pVoid(CONAFLAG.sessionPresent);
        }
        _ctrler["RC"] = pVoid(tFMT(pConnAck)->rc);
        break;
    case PUBACK:
    case PUBREC:
    case PUBREL:
    case PUBCOMP:
    case UNSUBACK:
        _ctrler["packetId"] = pVoid(tFMT(pAck)->packetId);
        break;
    case UNSUBSCRIBE:
        _ctrler["packetId"] = pVoid(tFMT(pUnsubscribe)->packetId);
        _ctrler["topics"]   = (tFMT(pUnsubscribe)->topics);
        break;
    case PINGREQ:
    case PINGRESP:
    case DISCONNECT:
        //_ctrler["header"] = (tFMT(pHeaders)->header);
        printf("packet type don't have Variable header && payload!\n");
        break;
    default:
        printf("error packet type\n");
        break;
    }
    return true;
}

bool MIMProtocol::controller()
{
    return true;
}

}//namespace mimer
