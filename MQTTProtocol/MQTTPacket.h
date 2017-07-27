/*
 *
 *
 **************************************************************
 *   Name        * Value * 报文流动方向	 * 描述
 *   Reserved    * 0     * 禁止	         * 保留
 *   CONNECT     * 1     * 客户端到服务端	 * 客户端请求连接服务端
 *   CONNACK     * 2     * 服务端到客户端	 * 连接报文确认
 *   PUBLISH     * 3     * 两个方向都允许	 * 发布消息
 *   PUBACK      * 4     * 两个方向都允许	 * QoS 1消息发布收到确认
 *   PUBREC      * 5     * 两个方向都允许	 * 发布收到（保证交付第一步）
 *   PUBREL      * 6     * 两个方向都允许	 * 发布释放（保证交付第二步）
 *   PUBCOMP     * 7     * 两个方向都允许	 * QoS 2消息发布完成（保证交互第三步）
 *   SUBSCRIBE   * 8     * 客户端到服务端	 * 客户端订阅请求
 *   SUBACK      * 9     * 服务端到客户端	 * 订阅请求报文确认
 *   UNSUBSCRIBE * 10    * 客户端到服务端	 * 客户端取消订阅请求
 *   UNSUBACK    * 11    * 服务端到客户端	 * 取消订阅报文确认
 *   PINGREQ     * 12    * 客户端到服务端	 * 心跳请求
 *   PINGRESP    * 13    * 服务端到客户端	 * 心跳响应
 *   DISCONNECT  * 14    * 客户端到服务端	 * 客户端断开连接
 *   Reserved    * 15    * 禁止           * 保留
 **************************************************************
 *
 *
 */

#ifndef _MQTTER_PACKET_H
#define _MQTTER_PACKET_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <string.h>
//#include "List.cpp"
#include <list>
#include "MQTTInt.h"
using namespace std;

namespace mqtter {

#define bsIsBigEndian ((((const int*)"\0\x1\x2\x3\x4\x5\x6\x7")[0] & 0xff) != 0)

#define MQTT_NAME  "MQTT"
#define MQ_byte    unsigned char
#define MQTT_VER   (0x04)
#define BAD_MQTT_PACKET -4
#define DEBUG      1
#define OFFICIAL_MQTT   1
enum msgTypes
{
    CONNECT = 1, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL,
    PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK,
    PINGREQ, PINGRESP, DISCONNECT
};
typedef short int  int16_t;
typedef int        int32_t;
typedef struct sub_s{
    size_t _size;
    char*  _content;
    sub_s(size_t size,char* content):_size(size){
        _content = (char*)malloc(_size);
        memcpy(_content,content,_size);
    }
    sub_s():_size(0),_content(NULL){}
}sub_t;
typedef list<sub_t> ListSub;
typedef ListSub::iterator Subitor;
typedef list<char>  ListQos;
typedef ListQos::iterator Qositor;
const int encodeStep[16] =
{
    /*  Reserved */
    0,
    /*  CONNECT
     *    Fix header
     *  1. set Fix header
     *    Variable header
     *  2. set Protocol name
     *  3. set Protocol Level
     *  4. set Connect flags
     *  5. set Keep alive time(KAT)
     *    Payload
     *  6. set Client ID
     *  7. set Will Topic
     *  8. set Will Message
     *  9. set User name
     *  10. set Password
     */
    10,
    /*  CONNACK
     *   Fix header
     *  1. set Fix header
     *   Variable header
     *  2. set Connect Ack flags
     *  3. set return code
     *  4. set return clientID(if sigin a new client)
     *   Payload: NULL
     */
    3,
    /*  PUBLISH
     *   Fix header
     *  1. set Fix header
     *   Variable header
     *  2. Set Topic Name
     *  3. set Packet ID
     *   Payload
     *  4. set payload message
     */
    4,
    /*  PUBACK
     *    Fix header
     *  1. set Fix header
     *   Variable header
     *  2. set Packet ID
     *   Payload: NULL
     */
    2,
    /*  PUBREC
     *  1. set Fix header
     *   Variable header
     *  2. set Packet ID
     *   Payload: NULL
     */
    2,
    /*  PUBREL
     *  1. set Fix header
     *   Variable header
     *  2. set Packet ID
     *   Payload: NULL
     */
    2,
    /*  PUBCOMP
     *  1. set Fix header
     *   Variable header
     *  2. set Packet ID
     *   Payload: NULL
     */
    2,
    /*  SUBSCRIBE
     *    Fix header
     *  1. set Fix header
     *   Variable header
     *  2. set Packet ID
     *   Payload
     *  3. set Topic Filter
     *  4. set Topic Qos
     *  ...(if not only a topic Filter)
     */
    4,
    /*  SUBACK
     *    Fix header
     *  1. set Fix header
     *  Variable header
     *  2. set Packet ID
     *   Payload
     *  3. set return code
     *  ...(if not only a topic Filter)
     */
    4,
    /*  UNSUBSCRIBE
     *    Fix header
     *  1. set Fix header
     *  Variable header
     *  2. set Packet ID
     *   Payload
     *  3. set Topic Filter
     * ...(if not only a topic Filter)
     */
    4,
    /*  UNSUBACK
     *    Fix header
     *  1. set Fix header
     *  Variable header
     *  2. set Packet ID
     *   Payload: NULL
     */
    2,
    /*  PINGREQ
     *    Fix header
     *  1. set Fix header
     */
    1,
    /*  PINGRESP
     *    Fix header
     *  1. set Fix header
     */
    1,
    /*  DISCONNECT
     *    Fix header
     *  1. set Fix header
     */
    1,
    /*  Reserved */
    0
};
const int isFixRLPacket[16] =
{
    0,0,1,0,1,1,1,1,0,0,0,1,1,1,1,0
};
const int isDriedPacket[16] =
{
    0,1,0,1,0,0,0,0,1,1,1,0,0,0,0,0
};
typedef unsigned int boolean;           /* use at bit-struct */
typedef unsigned short Int;             /* must 2 byte */

/**
 * Bitfields for the MQTT header byte.
 */
typedef union
{
    /*unsigned*/ char byte;             /* the whole byte */
#if REVERSED
    struct
    {
        unsigned int type : 4;          /* message type nibble */
        boolean dup : 1;                /* DUP flag bit */
        unsigned int qos : 2;           /* QoS value, 0, 1 or 2 */
        boolean retain : 1;             /* retained flag bit */
    } bits;
#else
    struct
    {
        boolean retain : 1;             /* retained flag bit */
        unsigned int qos : 2;           /* QoS value, 0, 1 or 2 */
        boolean dup : 1;                /* DUP flag bit */
        unsigned int type : 4;          /* message type nibble */
    } bits;
#endif
} Header;
typedef Header* pHeader;

/**
 * Data for one of the ack packets.
 */
typedef struct
{
    Header header;                      /* MQTT header byte */
    /* Variable header */
    int    packetId;                    /* MQTT packet id */
} Ack;
typedef Ack* pAck;

/**
 * Data for a packet with header only.
 */
typedef struct
{
    Header header;                      /* MQTT header byte */
} header_s;

/*
 * Packets
 *
 */

/* connect flags byte */
typedef union
{
    unsigned char all;              /* all connect flags */
    /* isregister == 1 && clientID == 0 : create a new user, will return a clientID
     * isregister == 0 && clientID != 0 : try to delete the user: clientID*/
#if REVERSED
    struct
    {
        boolean username : 1;       /* 3.1 user name */
        boolean password : 1;       /* 3.1 password */
        boolean willRetain : 1;     /* will retain setting */
        unsigned int willQoS : 2;   /* will QoS value */
        boolean will : 1;           /* will flag */
        boolean cleanstart : 1;     /* cleansession flag */
        boolean isregister : 1;     /* register(1) or delete(0) a user */
    } bits;
#else
    struct
    {
        boolean isregister : 1;     /* register(1) or delete(0) a user */
        boolean cleanstart : 1;     /* cleansession flag */
        boolean will : 1;           /* will flag */
        unsigned int willQoS : 2;	/* will QoS value */
        boolean willRetain : 1;     /* will retain setting */
        boolean password : 1;       /* 3.1 password */
        boolean username : 1;       /* 3.1 user name */
    } bits;
#endif
} connflags;

/**
 * Data for a connect packet. 0x1
 */
typedef struct
{
    Header        header;               /* MQTT header byte */
    /* Variable header */
    const char*   Protocol;             /* MQTT protocol name */
    MQ_byte       version;              /* MQTT version number */
    connflags     flags;                /* connect flags byte */
    Int           KAT;                  /* keepalive timeout value in seconds */
    /* Payload */
    /* All fields below here may be invisible characters */
    int           clientIDlen;          /* client id's size */
    char*         clientID;             /* string client id 23byte*/
    int           willTopiclen;         /* will topic's size */
    char*         willTopic;            /* will topic */
    int           willMsglen;           /* will message's size */
    char*         willMsg;              /* will message */
    int           userNamelen;          /* user name's size */
    char*         userName;             /* user name */
    int           passwdlen;            /* password's size */
    char*         passwd;               /* password */
} Connect;
typedef Connect* pConnect;

/**
 * Data for a connack packet. 0x2
 */
/* connack flags byte */
typedef union
{
    unsigned char all;              /* all connack flags */
#if REVERSED
    struct
    {
        unsigned int reserved : 6;  /* message type nibble */
        boolean isregister : 1;     /* register(1) */
        boolean sessionPresent : 1; /* was a session found on the server? */
    } bits;
#else
    struct
    {
        boolean sessionPresent : 1; /* was a session found on the server? */
        boolean isregister : 1;     /* register(1) */
        unsigned int reserved : 6;  /* message type nibble */
    } bits;
#endif
} cackflags;                            /* connack flags byte */

typedef struct
{
    Header header;                      /* MQTT header byte */
    /* Variable header */
    cackflags flags;                    /* connack flags byte */
    /* payload */
    char  rc;                           /* connack return code */
    int   clientIDlen;                  /* ClientId's size */
    char* clientID;                     /* create a new user, will return a new clientId */
} ConnAck;
typedef ConnAck* pConnAck;

/**
 * Data for a publish packet. 0x3
 */
typedef struct
{
    Header header;                      /* MQTT header byte */
    /* Variable header */
    int    topiclen;
    char*  topic;                       /* topic string */
    int    packetId;                    /* MQTT Packets id */
    /* payload */
    int    payloadlen;                  /* payload length */
    char*  payload;                     /* binary payload, length delimited */
} Publish;
typedef Publish* pPublish;

/**
 * Data for a Puback packet. 0x4
 */
typedef Ack PubAck;
typedef PubAck* pPubAck;

/**
 * Data for a Pubrec packet. 0x5
 */
typedef Ack PubRec;
typedef PubRec* pPubRec;

/**
 * Data for a Pubrel packet. 0x6
 */
typedef Ack PubRel;
typedef PubRel* pPubRel;

/**
 * Data for a Pubrel Pubcomp. 0x7
 */
typedef Ack PubComp;
typedef PubComp* pPubComp;

/**
 * Data for a subscribe packet. 0x8
 */
typedef struct
{
    Header header;                    /* MQTT header byte */
    /* Variable header */
    int    packetId;                  /* MQTT Packets id */
    /* payload */
    ListSub*  topics;                 /* list of topic strings */
    ListQos*  qoss;                   /* list of corresponding QoSs */
    // int    noTopics;               /* topic and qos count */
} Subscribe;
typedef Subscribe* pSubscribe;

/**
 * Data for a suback packet. 0x9
 */
typedef struct
{
    Header header;                    /* MQTT header byte */
    /* Variable header */
    int    packetId;                  /* MQTT Packets id */
    /* payload */
    ListQos*  qoss;                   /* list of granted QoSs */
} SubAck;
typedef SubAck* pSubAck;

/**
 * Data for an unsubscribe packet. 0x10
 */
typedef struct
{
    Header header;                    /* MQTT header byte */
    /* Variable header */
    int    packetId;                  /* MQTT Packets id */
    /* payload */
    ListSub*  topics;                 /* list of topic strings */
    // int    noTopics;               /* topic count */
} Unsubscribe;
typedef Unsubscribe* pUnsubscribe;

/**
 * Data for an Unsuback packet. 0x11
 */
typedef Ack UnsubAck;
typedef UnsubAck* pUnsubAck;

/**
 * Data for an PingReq packet. 0x12
 */
typedef header_s PingReq;
typedef PingReq* pPingReq;

/**
 * Data for an PingResp packet. 0x13
 */
typedef header_s PingResp;
typedef PingResp* pPingResp;

/**
 * Data for an Disconnect packet. 0x14
 */
typedef header_s Disconnect;
typedef Disconnect* pDisconnect;

typedef struct {
    char* topic;
    int   qos;
}TopicQos;


/* Delete the unimportant data in the packet as much as possible, keep the packet dry */
#define DRIED      (!_dried)

/*
 *  p* => pConnect,pConnAck,pPublish,
 *        pPubAck,pPubRec,pPubRel,
 *        pPubComp,pSubscribe,pSubAck,
 *        pUnsubscribe,pUnsubAck,pPingReq,
 *        pPingReq,pPingResp,pDisconnect
 *  Format void* _packet to p* pointer
 */
#define pFMT(pTYPE) ((pTYPE)(_packet))

#define ALLOC0(pTYPE,TYPE)                     \
    _packet = (pTYPE)malloc(sizeof(TYPE));     \
    memset(_packet,0,sizeof(TYPE));

#define initHeader0(pTYPE,TYPES)               \
    pFMT(pTYPE)->header.byte = 0;              \
    pFMT(pTYPE)->header.bits.type = TYPES;

#define initHeader1(pTYPE,TYPES)               \
    pFMT(pTYPE)->header.byte = 0;              \
    pFMT(pTYPE)->header.bits.type = TYPES;     \
    pFMT(pTYPE)->header.bits.qos = 1;//default value

#define initHeader3(pTYPE,TYPES,DUP,QOS)       \
    pFMT(pTYPE)->header.byte = 0;              \
    pFMT(pTYPE)->header.bits.type = TYPES;     \
    pFMT(pTYPE)->header.bits.dup = DUP;        \
    pFMT(pTYPE)->header.bits.qos = QOS;

#define MQNEW(PTYPE,KEY,SIZE)                  \
    pFMT(PTYPE)->KEY = (char*)malloc(SIZE)

#define MQDEL(PTYPE,KEY)                  \
    if(pFMT(PTYPE)->KEY) { free(pFMT(PTYPE)->KEY); pFMT(PTYPE)->KEY = NULL;}

#define HasFlags   ((_ptype == CONNECT)     || \
                    (_ptype == CONNACK))

#define HasTopic   ((_ptype == PUBLISH)     || \
                    (_ptype == SUBSCRIBE)   || \
                    (_ptype == UNSUBSCRIBE))

#define HasPayload ((_ptype == CONNECT)     || \
                    (_ptype == PUBLISH)     || \
                    (_ptype == SUBSCRIBE)   || \
                    (_ptype == SUBACK)      || \
                    (_ptype == UNSUBSCRIBE))

#define HasPktId   ((_ptype >= PUBLISH)     && \
                    (_ptype <= UNSUBACK))

#define SER_CLI(T) ((T >= PUBLISH)          && \
                    (T <= UNSUBACK))

#define SERVERS(T) ((SER_CLI)               || \
                    (T == CONNACK))

#define CLIENTS(T) ((T == PINGREQ)          || \
                     (T == CONNECT)         || \
                     (SER_CLI)              || \
                     (T == DISCONNECT))

/* Only Header Remaining Length 0,
 * PINGREQ, PINGRESP, DISCONNECT just a fixed head */
#define OnlyHeader ((_ptype >= PINGREQ)     && \
                    (_ptype <= DISCONNECT))

/* fix Remaining Length 2 */
#define FixRL      (isFixRLPacket[_ptype])
/*
#define FixRL      ((_ptype == CONNACK)     || \
                    (_ptype >= PUBACK       && \
                     _ptype <= PUBCOMP)     || \
                    (_ptype >= UNSUBACK     && \
                     _ptype <= DISCONNECT))
*/

/* cannot Dried packet type */
#define CannotDried (isDriedPacket[_ptype])
/*
#define CannotDried ((_ptype == CONNECT)    || \
                     (_ptype == PUBLISH)    || \
                     (_ptype >= SUBSCRIBE   && \
                      _ptype <= UNSUBSCRIBE))
*/

/**
 *
 */
class MQTTPacket
{
public:
    static const char* packet_names[];
public:
    MQTTPacket(int type, int dried = 0, int dup = 0,int qos = 0);
    virtual ~MQTTPacket();
public:
    friend std::ostream & operator<<(std::ostream &out, const MQTTPacket &c);
protected:
    /**
     * @brief dry: set this packet's DRIED flag. The fixed Header 0 bit is 1
     * represent this packet is DRIED.
     */
    inline void dry(){ pFMT(pHeader)->bits.retain = 1; _dried = 1;}
    inline bool dried() {return (1 == _dried && 1 == pFMT(pHeader)->bits.retain);}
public://get
    inline bool finish(){ return (_step == encodeStep[_ptype]);}
    inline int size() {return _size;}
    inline msgTypes type(){return (msgTypes)_ptype;}
    inline const char* types(){return packet_names[_ptype];}
    void*  data(){return _packet;}
    static msgTypes type(char header)
    {
#if REVERSED
        return (msgTypes)(header & 0x0F);
#else
        return (msgTypes)((header & 0xF0) >> 4);
#endif
    }
    static const char* types(msgTypes ptype){return packet_names[ptype];}
    static bool dried(char header){ return (1 == (header & 0x1)); }
public://get
    char*  clientId()const{ return pFMT(pConnect)->clientID;}
    char*  willTopic()const{return pFMT(pConnect)->willTopic;}
    char*  willMsg()const{return pFMT(pConnect)->willMsg;}
    char*  username()const{return pFMT(pConnect)->userName;}
    char*  password()const{return pFMT(pConnect)->passwd;}
    char*  payload(char* payload, size_t& size)const{
        size = pFMT(pPublish)->payloadlen;
        memcpy(payload, pFMT(pPublish)->payload, size);
        return payload;
    }
    char*  s_payload()const{ return pFMT(pPublish)->payload; }
    char*  topic()const{assert(PUBLISH == _ptype); return pFMT(pPublish)->topic;}
    Int    KAT()const{ assert(CONNECT == _ptype); return pFMT(pConnect)->KAT; }
    int    packetId()const{assert(HasPktId); return pFMT(pAck)->packetId;}
    int    RC()const{assert(CONNACK == _ptype); return (int)pFMT(pConnAck)->rc;}
    char*  AnewClientId()const
    {
        assert(CONNACK == _ptype);
        if(pFMT(pConnAck)->flags.bits.isregister)
            return pFMT(pConnAck)->clientID;
        return "";
    }
public://set
    /**
     * @brief setKAT, use at CONNECT
     * @param kat
     */
    void setKAT(Int kat);
    /**
     * @brief setClientId, use at CONNECT
     * @param clientId
     * @param size default clientId is 16 byte
     */
    void setClientId(char* clientId, size_t size = 16);
    /**
     * @brief setWill, use at CONNECT
     * @param willtopic
     * @param willmsg
     * @param size
     */
    void setWill(char* willtopic, char* willmsg, size_t sizet, size_t sizem);
    /**
     * @brief setUserName, use at CONNECT
     * @param userName
     * @param size
     */
    void setUserName(char* userName, size_t size);
    /**
     * @brief setPasswd, use at CONNECT
     * @param passwd
     * @param size
     */
    void setPasswd(char* passwd, size_t size);
    /**
     * @brief setSignUp, a special CONNECT mode, create a new user,
     * a special setClientId type
     */
    void setSignUp();
    /**
     * @brief setSignDel, a special CONNECT mode, delete a exist user,
     * a special setClientId type
     */
    void setSignDel(char* clientId, size_t size = 16);
    /**
     * @brief setRC, use at CONNACK
     * @param rc
     */
    void setRC(char rc);
    /**
     * @brief setFlags, use at CONNECT/CONNACK
     * @param value
     * @return
     */
    void setFlags(MQ_byte value);
    void setCleanstart(boolean value);
    void setWillflag(boolean value);
    void setWillQos(unsigned int value);
    void setisPassword(boolean value);
    void setisUsername(boolean value);
    /**
     * @brief setTopics, use at PUBLISH/SUBSCRIBE/UNSUBSCRIBE
     * @param content (topic and qos)
     * @param size
     * @return
     */
    void addTopics(char qos = 0, char* contents = NULL, size_t size = 0);
    /**
     * @brief setPayload, use at CONNECT/PUBLISH/SUBSCRIBE/SUBACK/UNSUBSCRIBE
     * @param payload
     * @param size
     * @return
     */
    void setPayload(char* payload, size_t size);
    /**
     * @brief setPacketId, use at PUBLISH/PUBACK/PUBREC/
     *                            PUBREL/PUBCOMP/SUBSCRIBE/
     *                            SUBACK/UNSUBSCRIBE/UNSUBACK
     * @param packetId
     * @param size
     * @return
     */
    void setPacketId(int packetId);
private:
    /* header.bits.dup == 0 && clientID == 0 : create a new user, will return a clientID */
    inline bool isSignin()
    {
        return (_ptype == CONNECT &&
                '\0' == (*pFMT(pConnect)->clientID) &&
                1 == pFMT(pConnect)->flags.bits.isregister);
    }
    inline bool signOk(){
        return (_ptype == CONNACK && pFMT(pConnAck)->flags.bits.isregister);
    }
    /* header.bits.dup == 1 && clientID != 0 : try to delete the user: clientID */
    inline bool isSigndel()
    {
        return (_ptype == CONNECT &&
                '\0' != (*pFMT(pConnect)->clientID) &&
                0 == pFMT(pConnect)->flags.bits.isregister);
    }
public:
    /**
     * @brief encode: a packet to bit-datas packet for send
     * @param packet
     * @return
     */
    bool encode(char* packet);
    int    decode(char* packet);
private:
    void*    _packet;    // packet's data
    int      _ptype;     // packet's type
    int      _size;      // packet's size, == fix Header's + Remaining Length
    int      _step;      // measure the encoding / decoding progress
    int      _dried;     // packet's DRIED flag
};

}//namespace mqtter

#endif // MQTTPACKET_H
