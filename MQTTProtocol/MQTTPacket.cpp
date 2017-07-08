#include "MQTTPacket.h"
namespace mqtter {

const char* MQTTPacket::packet_names[] =
{
   "RESERVED", "CONNECT", "CONNACK", "PUBLISH", "PUBACK", "PUBREC", "PUBREL",
   "PUBCOMP", "SUBSCRIBE", "SUBACK", "UNSUBSCRIBE", "UNSUBACK",
   "PINGREQ", "PINGRESP", "DISCONNECT"
};

MQTTPacket::MQTTPacket(int type,int dried, int dup,int qos):
    _ptype(type),_size(0),_step(0),_dried(dried)
{
    printf("Packet Type:%s\n", packet_names[type]);
    switch (_ptype)
    {
    case CONNECT:
#if DEBUG
        _packet = (pConnect)malloc(sizeof(Connect));
        memset(_packet,0,sizeof(Connect));
        pFMT(pConnect)->header.byte = 0;
        pFMT(pConnect)->header.bits.type = CONNECT;
#else
        ALLOC0(pConnect,Connect)
        initHeader0(pConnect,CONNECT)
#endif
        pFMT(pConnect)->Protocol = MQTT_NAME;
        pFMT(pConnect)->version = MQTT_VER;
        _step += 3;
        break;
    case CONNACK:
#if DEBUG
        _packet = (pConnAck)malloc(sizeof(ConnAck));
        memset(_packet,0,sizeof(ConnAck));
        pFMT(pConnAck)->header.byte = 0;
        pFMT(pConnAck)->header.bits.type = CONNACK;
#else
        ALLOC0(pConnAck,ConnAck)
        initHeader0(pConnAck,CONNACK)
#endif
        _step++;
        break;
    case PUBLISH:
#if DEBUG
        _packet = (pPublish)malloc(sizeof(Publish));
        memset(_packet,0,sizeof(Publish));
        pFMT(pPublish)->header.byte = 0;
        pFMT(pPublish)->header.bits.type = PUBLISH;
        pFMT(pPublish)->header.bits.dup = dup;
        pFMT(pPublish)->header.bits.qos = qos;
#else
        ALLOC0(pPublish,Publish)
        initHeader3(pPublish,PUBLISH,dup,qos)
#endif
        _step++;
        break;
    case PUBACK:
#if DEBUG
        _packet = (pPubAck)malloc(sizeof(PubAck));
        memset(_packet,0,sizeof(PubAck));
        pFMT(pPubAck)->header.byte = 0;
        pFMT(pPubAck)->header.bits.type = PUBACK;
#else
        ALLOC0(pPubAck,PubAck)
        initHeader0(pPubAck,PUBACK)
#endif
        _step++;
        break;
    case PUBREC:
#if DEBUG
        _packet = (pPubRec)malloc(sizeof(PubRec));
        memset(_packet,0,sizeof(PubRec));
        pFMT(pPubRec)->header.byte = 0;
        pFMT(pPubRec)->header.bits.type = PUBREC;
#else
        ALLOC0(pPubRec,PubRec)
        initHeader0(pPubRec,PUBREC)
#endif
        _step++;
        break;
    case PUBREL:
#if DEBUG
        _packet = (pPubRel)malloc(sizeof(PubRel));
        memset(_packet,0,sizeof(PubRel));
        pFMT(pPubRel)->header.byte = 0;
        pFMT(pPubRel)->header.bits.type = PUBREL;
        pFMT(pPubRel)->header.bits.qos = 1;//default value
#else
        ALLOC0(pPubRel,PubRel)
        initHeader1(pPubRel,PUBREL)
#endif
        _step++;
        break;
    case PUBCOMP:
#if DEBUG
        _packet = (pPubComp)malloc(sizeof(PubComp));
        memset(_packet,0,sizeof(PubComp));
        pFMT(pPubComp)->header.byte = 0;
        pFMT(pPubComp)->header.bits.type = PUBCOMP;
#else
        ALLOC0(pPubComp,PubComp)
        initHeader0(pPubComp,PUBCOMP)
#endif
        _step++;
        break;
    case SUBSCRIBE:
#if DEBUG
        _packet = (pSubscribe)malloc(sizeof(Subscribe));
        memset(_packet,0,sizeof(Subscribe));
        pFMT(pSubscribe)->header.byte = 0;
        pFMT(pSubscribe)->header.bits.type = PUBREL;
        pFMT(pSubscribe)->header.bits.qos = 1;//default value
#else
        ALLOC0(pSubscribe,Subscribe)
        initHeader1(pSubscribe,SUBSCRIBE)
#endif
        _step++;
        break;
    case SUBACK:
#if DEBUG
        _packet = (pSubAck)malloc(sizeof(SubAck));
        memset(_packet,0,sizeof(SubAck));
        pFMT(pSubAck)->header.byte = 0;
        pFMT(pSubAck)->header.bits.type = SUBACK;
#else
        ALLOC0(pSubAck,SubAck)
        initHeader0(pSubAck,SUBACK)
#endif
        _step++;
        break;
    case UNSUBSCRIBE:
#if DEBUG
        _packet = (pUnsubscribe)malloc(sizeof(Unsubscribe));
        memset(_packet,0,sizeof(Unsubscribe));
        pFMT(pUnsubscribe)->header.byte = 0;
        pFMT(pUnsubscribe)->header.bits.type = UNSUBSCRIBE;
#else
        ALLOC0(pUnsubscribe,Unsubscribe)
        initHeader1(pUnsubscribe,UNSUBSCRIBE)
#endif
        _step++;
        break;
    case UNSUBACK:
#if DEBUG
        _packet = (pUnsubAck)malloc(sizeof(UnsubAck));
        memset(_packet,0,sizeof(UnsubAck));
        pFMT(pUnsubAck)->header.byte = 0;
        pFMT(pUnsubAck)->header.bits.type = UNSUBACK;
#else
        ALLOC0(pUnsubAck,UnsubAck)
        initHeader0(pUnsubAck,UNSUBACK)
#endif
        _step++;
        break;
    case PINGREQ:
#if DEBUG
        _packet = (pPingReq)malloc(sizeof(PingReq));
        memset(_packet,0,sizeof(PingReq));
        pFMT(pPingReq)->header.byte = 0;
        pFMT(pPingReq)->header.bits.type = PINGREQ;
#else
        ALLOC0(pPingReq,PingReq)
        initHeader0(pPingReq,PINGREQ)
#endif
        _step++;
        break;
    case PINGRESP:
#if DEBUG
        _packet = (pPingResp)malloc(sizeof(PingResp));
        memset(_packet,0,sizeof(PingResp));
        pFMT(pPingResp)->header.byte = 0;
        pFMT(pPingResp)->header.bits.type = PINGRESP;
#else
        ALLOC0(pPingResp,PingResp)
        initHeader0(pPingResp,PINGRESP)
#endif
        _step++;
        break;
    case DISCONNECT:
#if DEBUG
        _packet = (pDisconnect)malloc(sizeof(Disconnect));
        memset(_packet,0,sizeof(Disconnect));
        pFMT(pDisconnect)->header.byte = 0;
        pFMT(pDisconnect)->header.bits.type = DISCONNECT;
#else
        ALLOC0(pDisconnect,Disconnect)
        initHeader0(pDisconnect,DISCONNECT)
#endif
        _step++;
        break;
    default:
        printf("error packet type\n");
        break;
    }
    pFMT(pHeader)->bits.retain = _dried; // dried flag
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

void MQTTPacket::setKAT(Int kat)
{
    assert(_ptype == CONNECT);
    _size += sizeof(int16_t);
    _step++;
    pFMT(pConnect)->KAT = kat;
}

void MQTTPacket::setClientId(char* clientId, size_t size)
{
    assert(_ptype == CONNECT);
    _size += size;
    _step++;
    pFMT(pConnect)->clientID = clientId;
}

void MQTTPacket::setWill(char* willtopic, char* willmsg, size_t sizet, size_t sizem)
{
    assert(_ptype == CONNECT);
    _step += 2;
    assert(pFMT(pConnect)->flags.bits.will);
    _size += sizet += sizem;
    pFMT(pConnect)->willTopic = willtopic;
    pFMT(pConnect)->willMsg = willmsg;
}

void MQTTPacket::setUserName(char* userName, size_t size)
{
    assert(_ptype == CONNECT);
    _step++;
    assert(pFMT(pConnect)->flags.bits.username);
    _size += size;
    pFMT(pConnect)->userName = userName;
}

void MQTTPacket::setPasswd(char* passwd, size_t size)
{
    assert(_ptype == CONNECT);
    _step++;
    assert(pFMT(pConnect)->flags.bits.password);
    _size += size;
    pFMT(pConnect)->passwd = passwd;
}

void MQTTPacket::setSignUp()
{
    assert(_ptype == CONNECT);
    _step++;
    pFMT(pConnect)->clientID = 0;
    pFMT(pConnect)->header.bits.dup = 0;
    _size += 16;  // default clientID 16 byte
}

void MQTTPacket::setSignDel(char* clientId, size_t size)
{
    assert(_ptype == CONNECT);
    _step++;
    pFMT(pConnect)->clientID = clientId;
    pFMT(pConnect)->header.bits.dup = 1;
    _size += size;
}

void MQTTPacket::setRC(char rc)
{
    assert(_ptype == CONNACK);
    _size += 1;
    _step++;
    pFMT(pConnAck)->rc = rc;
}

void MQTTPacket::setFlags(MQ_byte flags)
{
    assert(HasFlags);
    _size++;
    _step++;
    switch (_ptype) {
    case CONNECT:
        pFMT(pConnect)->flags.all = flags;
        break;
    case CONNACK:
        pFMT(pConnAck)->flags.all = flags;
        break;
    default:
        break;
    }
}

void MQTTPacket::setTopics(void* contents, size_t size)
{
    assert(HasTopic);
    _size += size;
    _step++;
    if(_ptype ==  PUBLISH){
        TopicQos* tpqos = (TopicQos*)(contents);
        pFMT(pPublish)->topic = tpqos->topic;
    }else{
        List* tpqos = (List*)contents;
        switch (_ptype) {
        case SUBSCRIBE:
            pFMT(pSubscribe)->topics = tpqos;
            pFMT(pSubscribe)->qoss = tpqos;
            break;
        case UNSUBSCRIBE:
            pFMT(pUnsubscribe)->topics = tpqos;
            break;
        default:
            break;
        }
    }
}

void MQTTPacket::setPayload(char* payload, size_t size)
{
    assert(_ptype == PUBLISH);
    _size += size;
    _step++;
    pFMT(pPublish)->payload = payload;
}

void MQTTPacket::setPacketId(int packetId)
{
    assert(HasPktId);
    _size += sizeof(int);
    _step++;
    switch (_ptype) {
    case PUBLISH://QoS > 0
        pFMT(pPublish)->packetId = packetId;
        break;
    case PUBACK :
        pFMT(pPubAck)->packetId = packetId;
        break;
    case  PUBREC:
        pFMT(pPubRec)->packetId = packetId;
        break;
    case  PUBREL:
        pFMT(pPubRel)->packetId = packetId;
        break;
    case  PUBCOMP:
        pFMT(pPubComp)->packetId = packetId;
        break;
    case  SUBSCRIBE:
        pFMT(pSubscribe)->packetId = packetId;
        break;
    case  SUBACK:
        pFMT(pSubAck)->packetId = packetId;
        break;
    case  UNSUBSCRIBE:
        pFMT(pUnsubscribe)->packetId = packetId;
        break;
    case  UNSUBACK:
        pFMT(pUnsubAck)->packetId = packetId;
        break;
    default:
        break;
    }
}

bool MQTTPacket::encode(char* packet)
{
    if(NULL == packet){
        printf("packet is not a enough memory!\n");
        return false;
    }
    pHeader fixhead = pFMT(pHeader);
    int prefixByte = 0;   /* save a content size's byte */
    int prefixSize = 0;   /* save a content size */
    int cursor = 0;
    int16_t packetId = 0;
    int16_t commond_size = 0;
    int     payloadSize = _size;
    /* fixed header */
    packet[cursor++] = pFMT(pHeader)->byte;
    /* Remaining Length: packet size */
    if(CannotDried){
        /* add packet's size after encode, size MQTTInt::encode's return */
        cursor += MQTTInt::encode(&packet[cursor], _size-1);
    }else{
        if(!_dried){
            if(OnlyHeader){
                /* Only Header, RL == 0 */
                packet[cursor++] = 0;
            }else{
                /* fix RL, RL == 2 */
                packet[cursor++] = 2;
            }
        }
        /*else{
            //1. OnlyHeader: only fix header, delete 1-byte RL
            //2. FixRL:  fix 1-byte RL type, delete 1-byte RL
        }*/
    }
    /* Variable header & payload */
    switch (_ptype)
    {
    case PUBLISH:// TODO ?
        /* Variable header */
        /* topic name */
        if(_dried){
            /* topic name's size */
            prefixSize = MQTTInt::decode(pFMT(pPublish)->topic,prefixByte);
            memcpy(&packet[cursor], &pFMT(pPublish)->topic, prefixByte);
            cursor += prefixByte;
            /* topic name's content */
            memcpy(&packet[cursor], &pFMT(pPublish)->topic + prefixByte,prefixSize);
            cursor += prefixSize;
        }else{
            /* topic name's size */
            commond_size = sizeof(pFMT(pPublish)->topic);
            memcpy(&packet[cursor], (char*)&commond_size, 2);
            // itoa((int16_t)sizeof(pFMT(pPublish)->topic),&packet[cursor],10);
            cursor += 2;
            /* topic name's content */
            memcpy(&packet[cursor], &pFMT(pPublish)->topic, prefixByte);
        }
        /* packet ID */
        packetId = (int16_t)pFMT(pPublish)->packetId;
        memcpy(&packet[cursor++], (char*)&packetId, 2);
        // itoa((int16_t)pFMT(pPubAck)->packetId,&packet[cursor++],10);
        /* payload */
        prefixSize = payloadSize - cursor;
        memcpy(&packet[cursor], &pFMT(pPublish)->payload , pFMT(pPublish)->payloadlen);
        break;
    case SUBSCRIBE:// TODO ?
        /* Variable header */
        /* packet ID */
        packetId = (int16_t)pFMT(pSubscribe)->packetId;
        memcpy(&packet[cursor++], (char*)&packetId, 2);
        // itoa((int16_t)pFMT(pPubAck)->packetId,&packet[cursor++],10);
        /* payload */
        /* List of topic and qos */
        for(int i = 0; i < ((List*)(pFMT(pSubscribe)->topics))->size(); ++i)
        {

        }
        memcpy(&packet[cursor++], (char*)&packetId, 2);
        break;
    case SUBACK:// TODO
        /* Variable header */
        /* packet ID */
        packetId = (int16_t)pFMT(pSubAck)->packetId;
        memcpy(&packet[cursor++], (char*)&packetId, 2);
        // itoa((int16_t)pFMT(pPubAck)->packetId,&packet[cursor++],10);
        /* payload */
        break;
    case CONNECT:// TODO ?
        /* Variable header */
        /* add CONNECT Variable header, size (2-byte)*/
        packet[cursor++] = 0;
        packet[cursor++] = 4;
        /* add CONNECT MQTT Protocol, size(4-byte) */
        memcpy(&packet[cursor], &pFMT(pConnect)->Protocol,4);
        cursor += 4;
        /* add CONNECT MQTT version, size(1-byte) */
        packet[cursor++] = pFMT(pConnect)->version;
        /* add CONNECT flags, size(1-byte) */
        packet[cursor++] = pFMT(pConnect)->flags.all;
        /* add CONNECT KAT, size(1-byte) */
        memcpy(&packet[cursor], &pFMT(pConnect)->KAT,2);
        cursor += 2;
        /* payload */        
        if (_dried){
            /* 1. client ID */
            /* client ID's size */
            prefixSize = MQTTInt::decode(pFMT(pConnect)->clientID,prefixByte);
            memcpy(&packet[cursor], &pFMT(pConnect)->clientID, prefixByte);
            cursor += prefixByte;
            /* client ID's content */
            memcpy(&packet[cursor], &pFMT(pConnect)->clientID + prefixByte,prefixSize);
            cursor += prefixSize;
            if(pFMT(pConnect)->flags.bits.will){
                /* 2. Will Topic */
                /* Will Topic's szie */
                prefixSize = MQTTInt::decode(pFMT(pConnect)->willTopic,prefixByte);
                memcpy(&packet[cursor], &pFMT(pConnect)->willTopic, prefixByte);
                cursor += prefixByte;
                /* Will Topic's content */
                memcpy(&packet[cursor], &pFMT(pConnect)->willTopic + prefixByte,prefixSize);
                cursor += prefixSize;
                /* 3. Will message */
                /* Will message's size */
                prefixSize = MQTTInt::decode(pFMT(pConnect)->willMsg,prefixByte);
                memcpy(&packet[cursor], &pFMT(pConnect)->willMsg, prefixByte);
                cursor += prefixByte;
                /* Will message's content */
                memcpy(&packet[cursor], &pFMT(pConnect)->willMsg + prefixByte,prefixSize);
                cursor += prefixSize;
            }
            if(pFMT(pConnect)->flags.bits.username){
                /* 4. User Name */
                /* UserName's size */
                prefixSize = MQTTInt::decode(pFMT(pConnect)->userName,prefixByte);
                memcpy(&packet[cursor], &pFMT(pConnect)->userName, prefixByte);
                cursor += prefixByte;
                /* UserName's content */
                memcpy(&packet[cursor], &pFMT(pConnect)->userName + prefixByte,prefixSize);
                cursor += prefixSize;
                if(pFMT(pConnect)->flags.bits.password){
                    /* 5. password */
                    /* password's size */
                    prefixSize = MQTTInt::decode(pFMT(pConnect)->passwd,prefixByte);
                    memcpy(&packet[cursor], &pFMT(pConnect)->passwd, prefixByte);
                    cursor += prefixByte;
                    /* password's content */
                    memcpy(&packet[cursor], &pFMT(pConnect)->passwd + prefixByte,prefixSize);
                    cursor += prefixSize;
                }
            }
        }else{
            // TODO
        }
        break;
    case CONNACK:
        /* Variable header */
        /* packet ID */
         packet[cursor++] = pFMT(pConnAck)->flags.all;
         packet[cursor++] = pFMT(pConnAck)->rc;
        break;
    case PUBACK:
    case PUBREC:
    case PUBREL:
    case PUBCOMP:
    case UNSUBACK:
        /* Variable header */
        /* packet ID */
        packetId = (int16_t)pFMT(pAck)->packetId;
        memcpy(&packet[cursor++], (char*)&packetId, 2);
        // itoa((int16_t)pFMT(pPubAck)->packetId,&packet[cursor++],10);
        break;
    case UNSUBSCRIBE:// TODO
        /* Variable header */
        /* packet ID */
        packetId = (int16_t)pFMT(pUnsubscribe)->packetId;
        memcpy(&packet[cursor++], (char*)&packetId, 2);
        // itoa((int16_t)pFMT(pPubAck)->packetId,&packet[cursor++],10);
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

void MQTTPacket::decode(char* packet, int size)
{
    if(NULL == packet || 0 <= size){
        return;
    }
    int cursor = 0;
    Header fixhead;
    /* fixed header */
    fixhead.byte = packet[cursor++];
    _dried = fixhead.bits.retain;
    _ptype = fixhead.bits.type;
    _step  = 0;
    //_packet = (void*)malloc(_size);
    //memcpy(_packet,packet,_size);
    switch (_ptype) {
    case CONNECT:

        break;
    case CONNACK:
        _step++;
        break;
    case PUBLISH:
        _step++;
        break;
    case PUBACK:
        _step++;
        break;
    case PUBREC:
        _step++;
        break;
    case PUBREL:
        _step++;
        break;
    case PUBCOMP:
        _step++;
        break;
    case SUBSCRIBE:
        _step++;
        break;
    case SUBACK:
        _step++;
        break;
    case UNSUBSCRIBE:
        _step++;
        break;
    case UNSUBACK:
        _step++;
        break;
    case PINGREQ:
        _step++;
        break;
    case PINGRESP:
        _step++;
        break;
    case DISCONNECT:
        _step++;
        break;
    default:
        printf("error packet type\n");
        break;
    }
}

}//namespace mqtter
