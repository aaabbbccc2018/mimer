#pragma once
#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_
#include <string.h>
#include <string>
#include <list>
#include "MIMErr.h"
#ifdef  __cplusplus
extern "C" {
#endif
#include "memUtil.h"
#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

using namespace std;

/**
* User login structure
*/
typedef struct
{
    char*         willTopic;            /* will topic */
    char*         willMsg;              /* will message */
    char*         userName;             /* client user name */
    char*         passwd;               /* client password */
    char*         userName2;            /* client user name */
}Login;
typedef Login* pLogin;
#define initLogin(userName, passwd) { NULL, NULL, userName, passwd, NULL }

typedef struct sub_s {
    size_t _size;
    char*  _content;
    sub_s(size_t size, const char* content) :_size(size) {
        _content = (char*)_malloc(_size);
        memcpy(_content, content, _size);
    }
    sub_s() :_size(0), _content(NULL) {}
    sub_s(const sub_s& rhs) :_size(rhs._size) {
        _content = (char*)_malloc(_size);
        memcpy(_content, rhs._content, _size);
    }
    ~sub_s() { if (_content) { _free(_content); _content = NULL; } }
    friend std::ostream & operator<<(std::ostream &out, const sub_s &sbs) {
        return out << sbs._content;
    }
}sub_t;
typedef list<sub_t> ListSub;
typedef ListSub::iterator Subitor;
typedef list<char>  ListQos;
typedef ListQos::iterator Qositor;

/*
    mimer packet templet, will use to create a new packet
*/
typedef class mimTemp
{
public:
    mimTemp() :_topics(NULL),_qoss(NULL) {}
    ~mimTemp() { if (_topics) { delete _topics; _topics = NULL; } 
                 if (_qoss) { delete _qoss; _qoss = NULL; } }
public:
    // update packet id
    mimTemp & operator++()
    {
        return *this;
    }
    mimTemp operator++(int)
    {
        mimTemp temp = *this;
        return temp;
    }
public: // set
    void push_back(sub_t s, char q = 1) {
        if (_topics && _qoss) {
            _topics->push_back(s);
            _qoss->push_back(q);
        }
        _topics = new ListSub();
        _qoss = new ListQos();
    }
    void Payload(const char* payload, size_t plen) {
        _payloadlen = plen;
        _payload = (char*)_malloc((size_t)_payloadlen);
        memcpy(_payload, payload, _payloadlen);
    }
    void Payload(const char* payload) {
        _payloadlen = strlen(payload);
        _payload = (char*)_malloc((size_t)_payloadlen);
        memcpy(_payload, payload, _payloadlen);
    }
    void Topic(const char* topic, size_t tlen) {
        _topiclen = tlen;
        _topic = (char*)_malloc((size_t)_topiclen);
        memcpy(_topic, topic, _topiclen);
    }
    void Topic(const char* topic) {
        _topiclen = strlen(topic);
        _topic = (char*)_malloc((size_t)_topiclen);
        memcpy(_topic, topic, _topiclen);
    }    
    void Rc(int rc) { _rc = rc; }
public: // get
    inline char* payload()  { return _payload; }
    inline int   plen()     { return _payloadlen; }
    inline char* topic()    { return _topic; }
    inline int   tlen()     { return _topiclen; }
    inline int   rc()       { return _rc; }
    inline ListSub* topics(){ return _topics; }
    inline ListQos* qoss()  { return _qoss; }
private:
    /* Variable header */
    int       _topiclen   = 0;
    char*     _topic      = NULL;               /* topic string */
    /* payload */
    int       _payloadlen = 0;                  /* payload length */
    char*     _payload    = NULL;               /* binary payload, length delimited, while registe a new client payload is client id */
    char      _rc         = CONNACK_DEFAULT;    /* connack return code */
    /* subscribe */
    ListSub*  _topics     = NULL;               /* list of topic strings */
    ListQos*  _qoss       = NULL;               /* list of corresponding QoSs */
};
typedef mimTemp* pmTemp;

typedef std::map<std::string, const void*>  Analyzer;

#endif // !_COMMON_DEFINE_H_
