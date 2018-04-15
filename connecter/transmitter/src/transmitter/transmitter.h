#pragma once
#ifndef _TRANSMITTER_H
#define _TRANSMITTER_H
#include "mmloop_utils.h"
#include "mmloop_files.h"
#include "mmloop.h"
#include "MIMPacket.h"
#include "mmloop_net.h"
#include "MIMProtocol.h"
#include "comdefine.h"
#include <map>
#include <stdlib.h>
#ifdef  __cplusplus
extern "C" {
#endif
#include "memUtil.h"
#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

using namespace mm::uvbase;
using namespace mimer;
typedef  TCP  baseT;                       /*Provide a transport layer base class,Now(2017-3-14) is uv::base::TCP*/
typedef  UDP  baseU;                       /*Provide a transport layer base class,Now(2017-3-14) is uv::base::UDP*/
typedef  MIMProtocol baseP;

namespace mm {
    namespace Transmitter {
        class MM_LOOP_API tTM;
        class MM_LOOP_API uTM;
        /*
            interface of Transmitter
        */
        class clock;
        class Stdio;

        class tTM :public InterFaceTransfer, public baseT
        {
            typedef enum MsgType {
                DEFAULT = 0,
                PROFILE,  // will used at  user list
                PURE_TEXT, JSON_TEXT, HTML_TEXT, RICH_TEXT,
                IMAGE, IMAGE3D, AUDIO, VIDEO // will used at chat list
            };

            struct message {
                char*    _body;   // message body or content
                size_t   _size;   // message body's size
                MsgType  _type;   // message type, (Text, Json, Html, Picture, Audio, Video ...)
            public:
                message() :_body(NULL), _size(0), _type(MsgType::DEFAULT) {}
                message(MsgType mt) :_body(NULL), _size(0), _type(mt) {}
                message(char* body, size_t size, MsgType mt) :_size(size), _type(mt) {
                    _body = (char*)_malloc(_size);
                    _memcpy(_body, body, size);
                }
                ~message() { if (_body) { delete _body; _body = NULL; _size = 0; _type = MsgType::DEFAULT; } }
            };
            /*
                std::string: topic
                message*   : message struct
            */
            typedef std::pair<std::string, message*> TopicPair;
            
            typedef std::list< TopicPair > TopicList;
            /*
                while is chatting ,message is save chat content, so, named chat list
            */
            typedef TopicList ChatList;
            /*
                while is get user list from server ,message is save each user's profile, so, named user list
            */
            typedef TopicList UserList;

            typedef std::pair<std::string, tTM*> OnlinePair;
            typedef std::map< std::string, OnlinePair > OnlineList;
            /*

            public:
                tTM() {}
                ~tTM() {}
            */
            friend class clock;
            friend class Stdio;
        public:
            virtual void SendHandle(void* buf, size_t& count);
            virtual void RecvHandle(void* buf, size_t& count);
        public:
            virtual int  Relate(const char* addr, const int port, Type type = Type::SERVER);
            virtual bool Bind(const char* protocol = "MIM1");            
            virtual int  Unlink();
            /* post */
            virtual int  Sendto(void* buf, size_t& count);
            /* get */
            virtual int  Recfrm(void* buf, size_t& count);
        public:
            void publisHandle(const Analyzer& ctrler, char* buf, size_t & count);
            //void pubrecHandle(Analyzer& ctrler, char* buf, size_t & count);
        private:
            /* will be implement a client */
            virtual void OnConnected(mmerrno status);
            /* will be implement a server */
            virtual void doAccept(mmerrno status);
            /* I/O ,if read a data,will be save a buf*/
            virtual void OnRead(size_t nread, const char *buf);
            /* I/O ,if write a data*/
            virtual void OnWrote(mmerrno status);
        private:
            Type userType;
            clock*  _pinger;
            Stdio*  _stder;
            MIMProtocol* _monitor;
            const char* _protocol = "";
            ChatList    _chating;
            UserList    _userlist;
            static OnlineList  _onliners;
        };
        typedef tTM TTM;

        class uTM :public InterFaceTransfer, public baseU
        {
            /*
            public:
                uTM() {}
                ~uTM() {}
            */
        public:
            virtual int  Relate(const char* addr, const int port, Type type = Type::SERVER);
            virtual int  Unlink();
            /* post */
            virtual int  Sendto(void* buf, size_t count);
            /* get */
            virtual int  Recfrm(void* buf, size_t count);
        private:
            virtual void OnSent(mmerrno status);
            //    virtual void OnAllocate(UDP *self, size_t suggested_size, uv_buf_t *buf) {}
            virtual void OnReceived(size_t nread, const char *buf, const struct sockaddr *addr, unsigned flags);
        private:
            Type userType;
            const char* _addr;
            int  _port;
        };
        typedef uTM UTM;

        class clock : public Timer
        {
        public:
            clock(tTM* tmer, uint64_t timeout = 1000, uint64_t repeat = 1000) :_tmer(tmer){
                //Loop loop(false);
                Loop loop = _tmer->loop();
                this->init(loop);
                this->start(timeout, repeat);
                //loop.run(Loop::Default);
            }
            ~clock() {}
            virtual void OnTimer()
            {
                if (_tmer) {
                    void* data = NULL;
                    size_t size = -1;
                    _tmer->_monitor->request(NULL, size, PINGREQ);
                }
            }
        private:
            tTM * _tmer = NULL;
        };

        class Stdio : public File
        {
        public:
            Stdio(tTM* tmer, Loop& loop) :File(loop),_tmer(tmer) {}
            ~Stdio() {}
        public:
            Stdio & operator =(int fb)
            {
                this->setfb(fb);
            }
            virtual void OnWrite(int result)
            {
                printf("OnWrite result = %d\n", result);
            }
            virtual void OnOpen(int result)
            {
                printf("open end! result = %d\n", result);
                char* buf = "1234";
                this->write(buf, 4, -1);
            }

            virtual void OnClose(int result)
            {
                printf("OnClose!\n");
                this->clean();
            }

            virtual void OnRead(char* data, int len)
            {
                if (!_tmer) return;
                if (len > 0) {
                    if (data && data[0] != '\n' && data[0] != '\r') {                        
                        void* postdata = (void*)data;
                        mimTemp* pkt = new mimTemp();
                        pkt->Payload(data, len);
                        pkt->Topic("echo");
                        size_t size = sizeof(mimTemp);
                        _tmer->_monitor->request((void*)pkt, size, PUBLISH);
                    }
                    else {
                        memset(data, 0, len);
                    }
                }
                else {
                    this->clean();
                    this->close();
                }
            }
        private:
            tTM * _tmer = NULL;
            int _packetId = 0;
        };
    }

}
#endif
