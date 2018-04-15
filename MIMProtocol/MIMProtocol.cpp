#include "MIMProtocol.h"
#include "Topic.h"

namespace mimer {

MIMProtocol::MIMProtocol(InterFaceTransfer* handler):
    _user(handler),_mqData(NULL),_ptype(-1),_packetId(INIT_PACKET_ID),_dried(0),_stream(NULL)
{
    _loger = new mim::ellog("MIMProtocol", "./logs");
    _monitor = new Controller();
}

MIMProtocol::MIMProtocol(char* content):_packetId(INIT_PACKET_ID),_dried(0),_stream(NULL)
{
    _loger = new mim::ellog("MIMProtocol", "./logs");
    _monitor = new Controller();
    _mqData = new MIMPacket(MIMPacket::type(content[0]));
    if(_mqData->decode(content)){
        _ptype = _mqData->type();
        _dried = tFMT(pHeaders)->header.bits.retain;
        _loger->debug("MIMPacket type: %v", _mqData->types());
    }else{
        _loger->error("MIMPacket type: %v decode failed", _mqData->types());
        printf("%s", "MIMpacket decode error!!!\n");
    }
    analyzer();
}

MIMProtocol::MIMProtocol(int ptype, int dried, int dup,int qos):
    _ptype(ptype),_packetId(INIT_PACKET_ID),_dried(dried),_stream(NULL)
{
    _loger = new mim::ellog("MIMProtocol", "./logs");
    _monitor = new Controller();
    _mqData = new MIMPacket(ptype, dried, dup, qos);
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
    if (_loger) {
        delete _loger;
        _loger = NULL;
    }
    if (_monitor) {
        delete _monitor;
        _monitor = NULL;
    }
}

bool MIMProtocol::analyzer()
{
    if(0 >= _ptype || 15 <= _ptype /*|| false == _mqData->finish()*/){
        _loger->error("MIMPacket type: %v packet type: %v", _mqData->types(), _ptype);
        printf("%s", "Invalid packet type or incomplete packet!!!\n");
        return false;
    }
    _ctrler.clear();
    _ctrler["TYPE"] = pVoid(_ptype);
    switch (_ptype)
    {
    case PUBLISH:
        _ctrler["pubQOS"]   = pVoid(HEADERFLAG.qos);
        _ctrler["packetId"] = pVoid(tFMT(pPublish)->packetId);
        _ctrler["topic"]    = (tFMT(pPublish)->topic);
        _ctrler["topiclen"] = pVoid(tFMT(pPublish)->topiclen);
        _ctrler["payload"]  = (tFMT(pPublish)->payload);
        _ctrler["payloadlen"] = pVoid(tFMT(pPublish)->payloadlen);
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
            if(CONNFLAG.willRetain){
                // must save will message after publish will message TODO
                _ctrler["willRetain"] = pVoid(CONNFLAG.willRetain);
            }else{
                // delete will message after publish will message TODO
                _ctrler["willRetain"] = pVoid(CONNFLAG.willRetain);
            }
        }
        /* get username and password then Verify it, if false return error code */
        if(CONNFLAG.username && CONNFLAG.password){
            _ctrler["username"] = (tFMT(pConnect)->userName);
            _ctrler["password"] = (tFMT(pConnect)->passwd);
            _ctrler["passwordlen"] = pVoid(tFMT(pConnect)->passwdlen);
        }else{
            // is test user
            _ctrler["username"] = "skybosi";
            _ctrler["password"] = "skybosi";
        }
        if(CONNFLAG.isregister){
            // create a new comer
            _ctrler["newer"] = "1111111111111111";
        }
        _ctrler["multicon"] = pVoid(tFMT(pHeaders)->header.bits.dup);
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
        _ctrler["cstatus"] =  pVoid(_mqData->ClientStatus());
        break;
    case PINGRESP:
    case DISCONNECT:
        //_ctrler["header"] = (tFMT(pHeaders)->header);
        _loger->warn("%v ONLY HEADER!!", MIMPacket::packet_names[_ptype]);
        break;
    default:
        printf("error packet type\n");
        break;
    }
    return true;
}

void MIMProtocol::request(void * data, size_t& size, packetTypes method)
{
    CallBack* cb = NULL;
    MIMPacket* reqpk = NULL;
    mimTemp* pktmp = NULL;
    if (CONNECT == method) {
        if (data == NULL) {
            goto req_next;
        }
        Login* login = (Login*)data;
        reqpk = new MIMPacket(CONNECT);
        connflags cf;
        cf.bits.cleanstart = 1;
        cf.bits.isregister = 0;
        cf.bits.password = 1;
        cf.bits.username = 1;
        cf.bits.will = 1;
        cf.bits.willQoS = 0;
        cf.bits.willRetain = 0;
        reqpk->setFlags(cf.all);
        reqpk->setKAT(10);
        reqpk->setClientId();
        reqpk->setWill(login->willTopic, login->willMsg, strlen(login->willTopic), strlen(login->willMsg));
        reqpk->setUserName(login->userName, strlen(login->userName));
        reqpk->setPasswd(login->passwd, strlen(login->passwd));
        reqpk->setMultiConnect();
    }
    else if (method >= SUBSCRIBE && method <= UNSUBSCRIBE) {
        ListSub* sublist = NULL;
        ListQos* subqoss = NULL;
        pktmp = (mimTemp*)data;
        Subitor itlist;
        Qositor itqoss;
        switch (method) {
        case SUBSCRIBE:
            reqpk = new MIMPacket(SUBSCRIBE);
            reqpk->setPacketId(_packetId);
            sublist = (ListSub*)(pktmp->topics());
            subqoss = (ListQos*)(pktmp->qoss());
            itlist = sublist->begin();
            itqoss = subqoss->begin();
            for (; itlist != sublist->end() && itqoss != subqoss->end(); ++itlist, ++itqoss) {
                reqpk->addTopics(*itqoss, itlist->_content, itlist->_size);
            }
            break;
        case SUBACK:
            reqpk = new MIMPacket(SUBACK);
            reqpk->setPacketId(_packetId);
            subqoss = (ListQos*)(pktmp->qoss());
            itlist = sublist->begin();
            itqoss = subqoss->begin();
            for (; itlist != sublist->end() && itqoss != subqoss->end(); ++itlist, ++itqoss) {
                reqpk->addTopics(*itqoss, itlist->_content, itlist->_size);
            }
            break;
        case UNSUBSCRIBE:
            reqpk = new MIMPacket(UNSUBSCRIBE);
            reqpk->setPacketId(_packetId);
            sublist = (ListSub*)(pktmp->topics());
            itlist = sublist->begin();
            for (; itlist != sublist->end(); ++itlist) {
                reqpk->addTopics(-1, itlist->_content, itlist->_size);
            }
            break;
        }
    }
    else {
        pktmp = (mimTemp*)data;
        switch (method)
        {
        case PUBLISH:
            reqpk = new MIMPacket(PUBLISH, 0, 0, 1);
            reqpk->addTopics(0, pktmp->topic(), (size_t)pktmp->tlen());
            reqpk->setPacketId(_packetId);
            reqpk->setPayload(pktmp->payload(), (size_t)pktmp->plen());
            break;
        case CONNACK:
            reqpk = new MIMPacket(CONNACK);
            // payload will save client id
            reqpk->setClientId(pktmp->payload(), pktmp->plen());
            reqpk->setRC(pktmp->rc());
            break;
        case PUBACK:
        case PUBREC:
        case PUBREL:
        case PUBCOMP:
            reqpk = new MIMPacket(method);
            reqpk->setPacketId(_packetId);
            break;
        case UNSUBACK:
            reqpk = new MIMPacket(UNSUBACK);
            reqpk->setPacketId(_packetId);
            break;
        case PINGREQ:
            reqpk = new MIMPacket(PINGREQ);
            reqpk->setPingStatus(PING_WRITING);
            break;
        case PINGRESP:
            reqpk = new MIMPacket(PINGRESP);
            break;
        case DISCONNECT:
            reqpk = new MIMPacket(DISCONNECT);
            // disconnect only header
            break;
        default:
            printf("error packet type\n");
            break;
        }
    }
req_next:
    cb = this->ret(reqpk, data, size);
    if (_user && cb) {        
        _user->SendHandle(cb->data, cb->size);
        DELETE(cb);
        _packetId++;
    }
}

void MIMProtocol::response(void * data, size_t& size)
{
    CallBack* cb = NULL;
    MIMPacket* repk = NULL;
    if (false == analyzer(data, size)) {
        goto rep_next;
    }
    switch (_ptype)
    {
    case PUBLISH:
        if (1 == (long)_ctrler["pubQOS"]) {
            repk = new MIMPacket(PUBACK);
            repk->setPacketId((long)_ctrler["packetId"]);
        }
        else if (2 == (long)_ctrler["pubQOS"]) {
            repk = new MIMPacket(PUBREC);
            repk->setPacketId((long)_ctrler["packetId"]);
        }
        else {
            _loger->error("%v QOS %v NO SUPPORT RESPONSE!!!", MIMPacket::packet_names[_ptype], (long)_ctrler["pubQOS"]);
        }
        break;
    case PUBREC:
        repk = new MIMPacket(PUBREL);
        repk->setPacketId((long)_ctrler["packetId"]);
        break;
    case PUBREL:
        repk = new MIMPacket(PUBCOMP);
        repk->setPacketId((long)_ctrler["packetId"]);
        break;
    case PUBCOMP:
        _loger->info("%v QOS = 2 is Finish !!!", MIMPacket::packet_names[_ptype]);
        break;
    case SUBSCRIBE:
        repk = new MIMPacket(SUBACK);
        repk->setPacketId((long)_ctrler["packetId"]);
        repk->addTopics(1);
        repk->addTopics(2);
        break;
    case SUBACK:
        _loger->warn("%v NO SUPPORT RESPONSE!!!", MIMPacket::packet_names[_ptype]);
        break;
    case CONNECT:
        repk = new MIMPacket(CONNACK);
        cackflags caf;
        caf.bits.reserved = 0;
        caf.bits.sessionPresent = 0;
        caf.bits.isregister = 0;
        repk->setFlags(caf.all);
        if (_monitor->check_permissions((char*)_ctrler["username"], (char*)_ctrler["password"], (size_t)_ctrler["passwordlen"])) {
            repk->setRC(CONNACK_ACCEPTED);
            repk->setClientId();
        }
        else {
            repk->setRC(CONNACK_REFUSED_BAD_USERNAME_PASSWORD);
            repk->setClientId();
        }
        break;
    case UNSUBSCRIBE:
        repk->setPacketId((long)_ctrler["packetId"]);
        break;
    case PINGREQ:
        repk = new MIMPacket(PINGRESP);
        // PINGRESP only header
        // _loger->warn("%v only header!!!", MIMPacket::packet_names[_ptype]);
        break;
    case PUBACK:
    case PINGRESP:
    case UNSUBACK:
    case CONNACK:
    case DISCONNECT:
        _loger->warn("%v NO SUPPORT RESPONSE!!!", MIMPacket::packet_names[_ptype]);
        break;
    default:
        printf("error packet type\n");
        break;
    }
rep_next:    
    if (NULL == repk) {
        cb = ret_err(_ptype, NO_SUPPORT_RESP, data);
    }
    else {
        cb = this->ret(repk, data, size);
    }
    if (_user && cb) {
        _user->RecvHandle(cb->data, cb->size);
        DELETE(cb);
    }
}

CallBack* MIMProtocol::ret(MIMPacket* pkt, void* data, size_t& size)
{
    if (!pkt) {
        return NULL;
    }
    size = pkt->size();
    data = (char*)malloc(size);
    memset(data, 0, size);
    CallBack* rtdata = new CallBack();
    if (!pkt->encode((char*)data)) {
        rtdata->data = NULL;
        rtdata->size = -1;
        rtdata->errcode = PACKET_ENCODE_ERROR;
    }
    else {
        rtdata->data = data;
        rtdata->size = size;
        rtdata->errcode = PACKET_ENCODE_OK;
    }    
    std::cout << charStream((char*)pkt->data(), size);
    DELETE(pkt);
    return rtdata;
}

CallBack* MIMProtocol::ret_err(int ptype, int errcode, void* data)
{
    if (0 >= ptype || 15 <= ptype /*|| false == _mqData->finish()*/) {
        _loger->error("MIMPacket type: %v packet type: %v", _mqData->types(), _ptype);
        return NULL;
    }
    data = malloc(sizeof(CallBack));
    ((CallBack*)data)->data = NULL;
    ((CallBack*)data)->errcode = errcode;
    size_t size = ((CallBack*)data)->size = sizeof(CallBack);
    return (CallBack*)data;
}

bool MIMProtocol::analyzer(void* data, size_t& size)
{
    char* content = (char*)data;
    if (0 >= MIMPacket::type(content[0]) || 15 <= MIMPacket::type(content[0]) /*|| false == _mqData->finish()*/) {
        _loger->error("MIMPacket type: %v packet type: %v", _mqData->types(), _ptype);
        printf("%s", "Invalid packet type or incomplete packet!!!\n");
        return false;
    }
    _mqData = new MIMPacket(MIMPacket::type(content[0]));
    if (_mqData->decode(content)) {
        _ptype = _mqData->type();
        _dried = tFMT(pHeaders)->header.bits.retain;
        _loger->debug("MIMPacket type: %v", _mqData->types());
    }
    else {
        _loger->error("MIMPacket type: %v decode failed", _mqData->types());
        printf("%s", "MIMpacket decode error!!!\n");
        return false;
    }
    return analyzer();
}

}//namespace mimer
