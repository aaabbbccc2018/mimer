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
#include <iostream>
#include "List.h"
namespace mqtter {

#define bsIsBigEndian ((((const int*)"\0\x1\x2\x3\x4\x5\x6\x7")[0] & 0xff) != 0)

#if defined(bsIsBigEndian)
#define REVERSED 1
#endif
#define MQTT_VER   0x04
#define MQTT_NAME  "MQTT"
#define MQ_byte    unsigned char
#define BAD_MQTT_PACKET -4

enum msgTypes
{
    CONNECT = 1, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL,
    PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK,
    PINGREQ, PINGRESP, DISCONNECT
};

/**
 * Bitfields for the MQTT header byte.
 */
typedef union
{
    /*unsigned*/ char byte;	    /* the whole byte */
#if defined(REVERSED)
    struct
    {
        unsigned int type : 4;	/* message type nibble */
        bool dup : 1;			/* DUP flag bit */
        unsigned int qos : 2;	/* QoS value, 0, 1 or 2 */
        bool retain : 1;		/* retained flag bit */
    } bits;
#else
    struct
    {
        bool retain : 1;		/* retained flag bit */
        unsigned int qos : 2;	/* QoS value, 0, 1 or 2 */
        bool dup : 1;			/* DUP flag bit */
        unsigned int type : 4;	/* message type nibble */
    } bits;
#endif
} Header;

/**
 * Data for one of the ack packets.
 */
typedef struct
{
    Header header;      /* MQTT header byte */
    int    packetId;    /* MQTT packet id */
} Ack;

/**
 * Data for a packet with header only.
 */
typedef struct
{
    Header header;	    /* MQTT header byte */
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
    Header        header;   /* MQTT header byte */
    /* Variable header */
    const char*   Protocol; /* MQTT protocol name */
    const MQ_byte version;	/* MQTT version number */
    union
    {
        unsigned char all;	/* all connect flags */
#if defined(REVERSED)
        struct
        {
            bool username : 1;			/* 3.1 user name */
            bool password : 1; 			/* 3.1 password */
            bool willRetain : 1;		/* will retain setting */
            unsigned int willQoS : 2;	/* will QoS value */
            bool will : 1;			/* will flag */
            bool cleanstart : 1;	/* cleansession flag */
            int : 1;	/* unused */
        } bits;
#else
        struct
        {
            int : 1;	/* unused */
            bool cleanstart : 1;	/* cleansession flag */
            bool will : 1;			/* will flag */
            unsigned int willQoS : 2;	/* will QoS value */
            bool willRetain : 1;		/* will retain setting */
            bool password : 1; 			/* 3.1 password */
            bool username : 1;			/* 3.1 user name */
        } bits;
#endif
    } flags;	            /* connect flags byte */
    int           KAT;		/* keepalive timeout value in seconds */
    /* Payload */
    char*         clientID;	/* string client id */
    char*         willTopic;/* will topic */
    char*         willMsg;	/* will payload */
    char*         userName; /* user name */
    char*         passwd;   /* password */
} Connect;

/**
 * Data for a connack packet. 0x2
 */
typedef struct
{
    Header header; /* MQTT header byte */
    union
    {
        unsigned char all;	/* all connack flags */
#if defined(REVERSED)
        struct
        {
            unsigned int reserved : 7;	/* message type nibble */
            bool sessionPresent : 1;    /* was a session found on the server? */
        } bits;
#else
        struct
        {
            bool sessionPresent : 1;    /* was a session found on the server? */
            unsigned int reserved : 7;	/* message type nibble */
        } bits;
#endif
    } flags;	   /* connack flags byte */
    char rc;       /* connack return code */
} ConnAck;

/**
 * Data for a publish packet. 0x3
 */
typedef struct
{
    Header header;	    /* MQTT header byte */
    char*  topic;	    /* topic string */
    int    topiclen;
    int    packetId;	/* MQTT Packets id */
    char*  payload;	    /* binary payload, length delimited */
    int    payloadlen;	/* payload length */
} Publish;

/**
 * Data for a Puback packet. 0x4
 */
typedef Ack PubAck;

/**
 * Data for a Pubrec packet. 0x5
 */
typedef Ack PubRec;

/**
 * Data for a Pubrel packet. 0x6
 */
typedef Ack PubRel;

/**
 * Data for a Pubrel Pubcomp. 0x7
 */
typedef Ack PubComp;

/**
 * Data for a subscribe packet. 0x8
 */
typedef struct
{
    Header header;	   /* MQTT header byte */
    int    packetId;   /* MQTT Packets id */
    List*  topics;	   /* list of topic strings */
    List*  qoss;	   /* list of corresponding QoSs */
    int    noTopics;   /* topic and qos count */
} Subscribe;

/**
 * Data for a suback packet. 0x9
 */
typedef struct
{
    Header header;	   /* MQTT header byte */
    int    packetId;   /* MQTT Packets id */
    List*  qoss;      /* list of granted QoSs */
} SubAck;

/**
 * Data for an unsubscribe packet. 0x10
 */
typedef struct
{
    Header header;	    /* MQTT header byte */
    int    packetId;    /* MQTT Packets id */
    List*  topics;     /* list of topic strings */
    int    noTopics;    /* topic count */
} Unsubscribe;

/**
 * Data for an Unsuback packet. 0x11
 */
typedef Ack UnsubAck;

/**
 * Data for an PingReq packet. 0x12
 */
typedef header_s PingReq;

/**
 * Data for an PingResp packet. 0x13
 */
typedef header_s PingResp;

/**
 * Data for an Disconnect packet. 0x14
 */
typedef header_s Disconnect;

/**
 *
 */
class MQTTPacket
{
public:
    static const char* packet_names[];
public:
    MQTTPacket(void* packet);
    MQTTPacket(int type,void* packet);
    virtual ~MQTTPacket();
    msgTypes type(){return (msgTypes)_ptype;}
private:
    void*    _packet;
    int      _ptype;
    int      _size;
};

}//namespace mqtter

#endif // MQTTPACKET_H
