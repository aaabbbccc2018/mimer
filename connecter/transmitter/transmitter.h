#pragma once
#ifndef _TRANSMITTER_H
#define _TRANSMITTER_H
#include "mmloop_utils.h"
#include "mmloop_files.h"
#include "mmloop.h"
#include "mmloop_net.h"
#include "interface.h"
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

namespace mm {
    namespace Transmitter {
        class MM_LOOP_API tTM;
        class MM_LOOP_API uTM;
        /*
            interface of Transmitter
        */
        class clock;
        class Stdio;

        class tTM :public iTransfer, public baseT
        {
            typedef std::pair<std::string, tTM*> OnlinePair;
            typedef std::map< std::string, OnlinePair > OnlineList;
            struct SocketPeer {
                std::string ipaddr;
                int         port;
            public:
                SocketPeer(char* ip, int port):ipaddr(ip), port(port) {}
                SocketPeer(const SocketPeer& sp) {
                    ipaddr = sp.ipaddr;
                    port = sp.port;
                }
                std::string toString() {
                    if (!ipaddr.empty()) {
                        char ports[6] = { 0 };
                        _itoa(port, ports, 10);
                        return ipaddr + ":" + ports;
                    }
                    return "";
                }
            };
        public:
            virtual int  Relate(const char* addr, const int port, Type type = Type::SERVER);
            virtual bool Bind(const char* protocol = "MIM1");            
            virtual int  Unlink();
            /* post */
            virtual void  Sendto(void* buf, size_t& count);
            /* get */
            virtual void  Recfrm(void* buf, size_t& count);
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
            inline const tTM* getClientHandle(const char* clientId = NULL);
            inline const SocketPeer getPeerByClientId(const char* clientId = NULL);
        private:
            Type userType;
            const char* _protocol = "";
            static OnlineList  _onliners;
        };
        typedef tTM TTM;

        class uTM :public iTransfer, public baseU
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
    }

}
#endif
