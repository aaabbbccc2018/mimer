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
#include "List.h"
using namespace std;

namespace mqtter {

#define bsIsBigEndian ((((const int*)"\0\x1\x2\x3\x4\x5\x6\x7")[0] & 0xff) != 0)

#if defined(bsIsBigEndian)
#define REVERSED 1
#endif

#define MQTT_NAME  "MQTT"
#define MQ_byte    unsigned char
#define MQTT_VER   (0x04)
#define BAD_MQTT_PACKET -4
#define DEBUG 1
enum msgTypes
{
    CONNECT = 1, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL,
    PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK,
    PINGREQ, PINGRESP, DISCONNECT
};
const int encodeStep[16]
{
    0,10,3,4,2,2,2,2,4,3,3,2,1,1,1,0
};
typedef unsigned int boolean;           /* use at bit-struct */
typedef unsigned short Int;             /* must 2 byte */

/**
 * Bitfields for the MQTT header byte.
 */
typedef union
{
    /*unsigned*/ char byte;             /* the whole byte */
#if defined(REVERSED)
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

/**
 * Data for one of the ack packets.
 */
typedef struct
{
    Header header;                      /* MQTT header byte */
    /* Variable header */
    int    packetId;                    /* MQTT packet id */
} Ack;

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

/**
 * Data for a connect packet. 0x1
 */
typedef struct
{
    Header        header;               /* MQTT header byte */
    /* Variable header */
    const char*   Protocol;             /* MQTT protocol name */
    MQ_byte       version;              /* MQTT version number */
    union
    {
        unsigned char all;              /* all connect flags */
#if defined(REVERSED)
        struct
        {
            boolean username : 1;       /* 3.1 user name */
            boolean password : 1;       /* 3.1 password */
            boolean willRetain : 1;     /* will retain setting */
            unsigned int willQoS : 2;   /* will QoS value */
            boolean will : 1;           /* will flag */
            boolean cleanstart : 1;     /* cleansession flag */
            int : 1;                    /* unused */
        } bits;
#else
        struct
        {
            int : 1;                    /* unused */
            boolean cleanstart : 1;     /* cleansession flag */
            boolean will : 1;           /* will flag */
            unsigned int willQoS : 2;	/* will QoS value */
            boolean willRetain : 1;     /* will retain setting */
            boolean password : 1;       /* 3.1 password */
            boolean username : 1;       /* 3.1 user name */
        } bits;
#endif
    } flags;                            /* connect flags byte */
    Int           KAT;                  /* keepalive timeout value in seconds */
    /* Payload */
    /* All fields below here may be invisible characters */
    char*         clientID;             /* string client id 23byte*/
    char*         willTopic;            /* will topic */
    char*         willMsg;              /* will payload */
    char*         userName;             /* user name */
    char*         passwd;               /* password */
} Connect;

typedef Connect* pConnect;
/**
 * Data for a connack packet. 0x2
 */
typedef struct
{
    Header header;                      /* MQTT header byte */
    union
    {
        unsigned char all;              /* all connack flags */
#if defined(REVERSED)
        struct
        {
            unsigned int reserved : 7;  /* message type nibble */
            boolean sessionPresent : 1; /* was a session found on the server? */
        } bits;
#else
        struct
        {
            boolean sessionPresent : 1; /* was a session found on the server? */
            unsigned int reserved : 7;  /* message type nibble */
        } bits;
#endif
    } flags;                            /* connack flags byte */
    /* payload */
    char rc;                            /* connack return code */
} ConnAck;

typedef ConnAck* pConnAck;
/**
 * Data for a publish packet. 0x3
 */
typedef struct
{
    Header header;                      /* MQTT header byte */
    /* Variable header */
    char*  topic;                       /* topic string */
    int    topiclen;
    int    packetId;                    /* MQTT Packets id */
    /* payload */
    char*  payload;                     /* binary payload, length delimited */
    int    payloadlen;                  /* payload length */
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
    List*  topics;                    /* list of topic strings */
    List*  qoss;                      /* list of corresponding QoSs */
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
    List*  qoss;                      /* list of granted QoSs */
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
    List*  topics;                    /* list of topic strings */
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

/**
 *
 */
class MQTTPacket
{
public:
    static const char* packet_names[];
public:
    MQTTPacket(int type,int dup = 0,int qos = 0);
    virtual ~MQTTPacket();
public:
    friend std::ostream & operator<<(std::ostream &out, const MQTTPacket &c);
public://get
    inline bool finish(){ return (_step == encodeStep[_ptype]);}
    inline int size() {return _size;}
    inline msgTypes type(){return (msgTypes)_ptype;}
    inline const char* types(){return packet_names[_ptype];}
    void*  data(){return _packet;}
    char* ClientId(){ return pFMT(pConnect)->clientID;}
    char* willTopic(){return pFMT(pConnect)->willTopic;}
    char* willMsg(){return pFMT(pConnect)->willMsg;}
    char* username(){return pFMT(pConnect)->userName;}
    char* password(){return pFMT(pConnect)->passwd;}
    char* publish(char* payload, size_t& size)
    {
        size = pFMT(pPublish)->payloadlen;
        memcpy(payload, pFMT(pPublish)->payload, size);
        return payload;
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
     * @param size
     */
    void setClientId(char* clientId, size_t size);
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
    /**
     * @brief setTopics, use at PUBLISH/SUBSCRIBE/UNSUBSCRIBE
     * @param content (topic and qos)
     * @param size
     * @return
     */
    void setTopics(void* contents, size_t size);
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
public:
    void encode();
    void decode();
private:
    void*    _packet;    // packet's data
    int      _ptype;     // packet's type
    int      _size;      // packet's size
    int      _step;      // measure the encoding / decoding progress
};

}//namespace mqtter

#endif // MQTTPACKET_H
