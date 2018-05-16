#include <stdio.h>
#include <assert.h>
#include "transmitter.h"
//#include "log_config.h"
using namespace mimer;

namespace mm {
    namespace Transmitter {
        //tcp

        tTM::OnlineList tTM::_onliners = std::map<std::string, OnlinePair>();

        int  tTM::Relate(const char* addr, const int port,Type type)
        {
            Loop loop(false);
            this->userType = type;
            this->init(loop);
            _loger->debug("tTM is Relate addr: %v port: %v type: %v", addr, port, user(userType));
            switch (type)
            {
            case SERVER:
                this->bind(addr, port);
                this->wait();
                break;
            case CLIENT:
                this->connect(addr, port);
                break;
            case BOTH_SER:
                break;
            case BOTH_CLI:
                break;
            default:
                break;
            }
            loop.run(Loop::Default);
            return 0;
        }

        bool tTM::Bind(const char* protocol)
        {
            return true;
        }

        int  tTM::Unlink()
        {
            this->close();
            return 0;
        }

        void tTM::Sendto(void* buf, size_t& count)
        {
            write((char*)buf,count);
        }

        void tTM::Recfrm(void* buf, size_t& count)
        {
            //printf("%s recv data:%s,len = %d\n", user(userType), (char*)buf, count);
            _loger->debug("tTM is Recfrm size: %v buf: %v", count, (char*)buf);
        }

        //client need implement
        void tTM::OnConnected(mmerrno status) {
            assert(this->userType & 1);
            if (status != mmerrno::mmSuccess) {
                fprintf(stderr, "on_connect error %s\n", Handle::errCode(status));
                _loger->error("tTM is OnConnected error: %v", Handle::errCode(status));
                return;
            }
            if (Bind("MIM1")) {
                read_start();      
            }
            else {
                _loger->error("Bind %v Monitor is error!!!", user(userType));
            }
        }

        //server need implement
        void tTM::doAccept(mmerrno status) {
            assert(!(this->userType & 1));
            if (status != mmerrno::mmSuccess) {
                fprintf(stderr, "New connection error %s\n", Handle::errCode(status));
                _loger->error("tTM is doAccept error: %v", Handle::errCode(status));
                return;
            }
            // create a new server for a client side service
            tTM* s2c = new tTM();
            s2c->init(loop());
            if (accept((Stream*)s2c) == 0) {             
                tTM::SocketPeer sp = s2c->getPeerByClientId();
                std::string ip_port = sp.toString();                
                printf("client accept client ip = %s,port = %d\n", sp.ipaddr.c_str(), sp.port);
                _loger->debug("tTM is doAccept client ip:%v port: %v", sp.ipaddr.c_str(), sp.port);
                // start to read data from a client
                if (s2c->Bind("MIM1")) {
                    s2c->read_start();
                    _loger->debug("Server Monitor is runing ...");
                }
                else {
                    _loger->error("Bind %v Monitor is error!!!", user(userType));
                }
            }
            else {
                s2c->close();
                delete s2c;
            }
        }

        //client or server need implement, When data comes in, it is called
        void tTM::OnRead(size_t nread, const char *buf)
        {
            _loger->debug("tTM is OnRead size: %v context: %v", nread, (char*)buf);
            if (nread < 0) {
                fprintf(stderr, "%s Read error %s\n", user(userType), Handle::errType(nread));
                this->read_stop();
                this->close();
                return;
            }
            if (this->userType & 1) { //Type::CLIENT ||  Type::BOTH_CLI
                Recfrm((void*)buf, nread);
            }
            else { //Type::SERVER ||  Type::BOTH_SER
                Recfrm((void*)buf, nread);
            }
        }

        //client or client need implement, when it needs to send data, it id called
        void tTM::OnWrote(mmerrno status) {
            if (this->userType & 1) { //Type::CLIENT ||  Type::BOTH_CLI
                _loger->debug("tTM is OnWrote client");            
            }
            else { //Type::SERVER ||  Type::BOTH_SER
                _loger->debug("tTM is OnWrote server");
            }
        }

        inline const tTM* tTM::getClientHandle(const char* clientId)
        {
            if (!_onliners.empty()) {
                OnlineList::iterator user = _onliners.find(clientId);
                if (user != _onliners.end()){
                    return (user->second).second;
                }
                else {
                    return NULL;
                }                
            }
            return this;
        }

        inline const tTM::SocketPeer tTM::getPeerByClientId(const char* clientId)
        {
            if (!_onliners.empty() && NULL != clientId) {
                OnlineList::iterator user = _onliners.find(clientId);
                if (user != _onliners.end()) {
                    tTM* up = (user->second).second;
                    up->getPeerByClientId(NULL);
                }
            }
            else {
                char ip[17] = { 0 };
                int  port = 0;
                this->getpeerIpPort(ip, port);
                return tTM::SocketPeer(ip, port);
            }            
        }

        //udp
        int  uTM::Relate(const char* addr, const int port, Type type)
        {
            Loop loop(false);
            this->userType = type;
            this->_addr = addr;
            this->_port = port;
            this->init(loop);
            _loger->debug("uTM is Relate addr: %v port: %v type :%v", addr, port, user(userType));
            switch (type)
            {
            case SERVER:
                this->bind(addr, port, UDP::Reuseaddr);
                this->recv_start();
                break;
            case CLIENT:
                this->bind(addr, port,UDP::Reuseaddr);
                this->recv_start();
                this->Sendto((void*)"hello",6);
                break;
            case BOTH_SER:
                break;
            case BOTH_CLI:
                break;
            default:
                break;
            }
            loop.run(Loop::Default);
            return 0;
        }

        int  uTM::Unlink()
        {
            this->close();
            return 0;
        }

        int  uTM::Sendto(void* buf, size_t count)
        {
            _loger->debug("uTM is Sendto size: %v buf: %v", count, (char*)buf);
            send((char*)buf, count, _addr,_port);
            return count;
        }

        int  uTM::Recfrm(void* buf, size_t count)
        {
            printf("%s recv data:%s,len = %d\n", user(userType), (char*)buf, count);
            _loger->debug("uTM is Recfrm size: %v buf: %v", count, (char*)buf);
            return count;
        }

        void uTM::OnSent(mmerrno status)
        {
            if (status != mmerrno::mmSuccess) {
                fprintf(stderr, "Send error %s\n", Handle::errCode(status));
                _loger->error("uTM is OnSent");
                return;
            }
        }

        void uTM::OnReceived(size_t nread, const char *buf, const struct sockaddr *addr, unsigned flags)
        {
            sockaddr_in* psin = (sockaddr_in*)addr;
            printf("Recv from ip:%s,port:%d\n", inet_ntoa(psin->sin_addr), ntohs(psin->sin_port));
            _loger->debug("uTM is OnReceived from ip:%v port:%v size: %v buf: %v", inet_ntoa(psin->sin_addr), ntohs(psin->sin_port), nread, (char*)buf);
            printf("recv data:%s,len = %d\n", buf, nread);
            Sendto((void*)buf, nread);
            //send4(buf,nread,"192.168.100.90",1800);
        }
    }
}
