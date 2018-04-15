#include <stdio.h>
#include <assert.h>
#include "transmitter.h"
//#include "log_config.h"
using namespace mimer;

namespace mm {
    namespace Transmitter {
        //tcp
        tTM::OnlineList tTM::_onliners = std::map<std::string, OnlinePair>();

        void tTM::SendHandle(void * buf, size_t & count)
        {
            this->write((char*)buf, count);
        }
        void tTM::RecvHandle(void * buf, size_t & count)
        {
            const Analyzer& ctrler = this->_monitor->getAnalyzer();
            //std::cout << *(this->_monitor);
            char* clientId = (char*)_monitor->get("clientID");
            int ptype = (int)_monitor->get("TYPE");
            if (NULL != buf && count != 0)
            {
                if (CONNECT == ptype) {                
                    char ip[17] = { 0 };
                    char ports[6] = { 0 };
                    int  port = 0;
                    this->getpeerIpPort(ip, port);
                    _itoa(port, ports, 10);
                    std::string ip_port = std::string(ip) + ":" + ports;
                    _onliners[ip_port].first = (char*)_monitor->get("clientID");
                }
                switch (ptype)
                {
                case PUBLISH:
                    publisHandle(ctrler, (char*)buf, count);
                    break;
                case PUBREC:
                    //pubrecHandle(ctrler, (char*)buf, count);
                    break;
                case PUBREL:
                    break;
                case PUBCOMP:
                    break;
                case SUBSCRIBE:
                    break;
                case SUBACK:
                    break;
                case UNSUBSCRIBE:
                    break;
                case PINGREQ:
                    break;
                case PUBACK:
                case PINGRESP:
                case UNSUBACK:
                case CONNACK:
                case DISCONNECT:
                    break;
                default:
                    printf("error packet type\n");
                    break;
                }
                this->write((char*)buf, count);
            }
            else {
                _loger->debug("RecvHandle no respose packet %v", (int)_monitor->get("TYPE"));
            }
        }

        void tTM::publisHandle(const Analyzer & ctrler, char* buf, size_t & count)
        {
            char* topic = (char*)_monitor->get("topic");
            char* payload = (char*)_monitor->get("payload");
            size_t payloadlen = (size_t)_monitor->get("payloadlen");            
            message* pmsg = new message(payload, payloadlen, MsgType::PURE_TEXT);
            // record publish payload to chat list
            _chating.push_back({topic, pmsg});
            // send publis ack to other peer
            this->write(buf, count);
            if (this->userType & 1) { //Type::CLIENT ||  Type::BOTH_CLI
                std::cout << "=================: " << payload << std::endl;
            }
            else { // server
                if (std::string(topic) == "echo") {
                    mimTemp* pkt = new mimTemp();
                    pkt->Payload(payload, payloadlen);
                    pkt->Topic(topic);
                    size_t size = sizeof(mimTemp);
                    this->_monitor->request((void*)pkt, size, PUBLISH);
                }
            }
        }

        int  tTM::Relate(const char* addr, const int port,Type type)
        {
            Loop loop(false);
            this->userType = type;
            this->init(loop);
            this->_stder = new Stdio(this, loop);
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
            _protocol = protocol;
            if (std::string(protocol) == "MIM1") {
                this->_monitor = new MIMProtocol(this);
                // _monitor->bind(this);
                _loger->debug("Bind monitor %v is success", user(userType));
            }
            return true;
        }

        int  tTM::Unlink()
        {
            this->close();
            return 0;
        }

        int  tTM::Sendto(void* buf, size_t& count)
        {
            write((char*)buf,count);
            return count;
        }

        int  tTM::Recfrm(void* buf, size_t& count)
        {
            //printf("%s recv data:%s,len = %d\n", user(userType), (char*)buf, count);
            _loger->debug("tTM is Recfrm size: %v buf: %v", count, (char*)buf);
            return count;
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
                // client connect, should send a connect packet to server
                Login* login = new Login();
                login->userName = "skybosi";
                login->passwd = "skybosi";
                login->willTopic = "test";
                login->willMsg = "test";
                size_t size = sizeof(Login);
                _monitor->request((void*)login, size, CONNECT);
                /*
                callback* cbd = _monitor->request(condata, size);
                if (cbd->data) {
                    Sendto(cbd->data, cbd->size);
                    //_pinger = new clock(this, 1000, 10000);
                }
                else {
                    _loger->error("User %v login in falied!!!", user(userType));
                }*/
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
                char ip[17] = { 0 };
                char ports[6] = { 0 };
                int  port = 0;
                s2c->getpeerIpPort(ip, port);
                _itoa(port, ports, 10);
                std::string ip_port = std::string(ip) + ":" + ports;
                printf("client accept client ip = %s,port = %d\n", ip, port);
                _loger->debug("tTM is doAccept client ip:%v port: %v", ip, port);
                // start to read data from a client
                if (s2c->Bind("MIM1")) {
                    s2c->read_start();
                    _loger->debug("Server Monitor is runing ...");
                }
                else {
                    _loger->error("Bind %v Monitor is error!!!", user(userType));
                }
                _onliners.insert({ ip_port, { ip_port, s2c} });
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
                if(_pinger){
                    _pinger->stop();
                    _pinger = NULL;
                }
                if (_stder) {
                    _stder->close();
                    _stder = NULL;
                }
                this->close();
                return;
            }
            void* getdata = (void*)buf;
            size_t size = nread;
            Recfrm(getdata, size);
            if (this->userType & 1) { //Type::CLIENT ||  Type::BOTH_CLI
                _monitor->response((void*)buf, nread);
                /*
                _monitor->setPtype(MIMPacket::type(buf[0]));
                if (_monitor->analyzer(getdata, size)) {
                    void* reqdata = NULL;
                    callback* cbd = _monitor->request(reqdata, size);
                    if (cbd->data) {
                        Sendto(cbd->data, cbd->size);
                    }
                    else {
                        _loger->error("tTM %v request failed code: %v!!!", user(userType), cbd->errcode);
                    }
                }
                else {
                    _loger->error("tTM %v is get packet analyzer failed!!!", user(userType));
                }
                */
            }
            else { //Type::SERVER ||  Type::BOTH_SER
                _monitor->response((void*)buf, nread);
                /*
                _monitor->setPtype(MIMPacket::type(buf[0]));
                if (_monitor->analyzer(getdata, size)) {
                    //void* reqdata = NULL;
                    callback* cbd = _monitor->response(getdata, size);
                    if (cbd->data) {
                        Sendto(cbd->data, cbd->size);
                    }
                    else {
                        _loger->error("tTM %v response failed code: %v!!!", user(userType), cbd->errcode);
                    }
                }
                else {
                    _loger->error("tTM %v is get packet analyzer failed!!!", user(userType));
                }*/
            }
        }

        //client or client need implement, when it needs to send data, it id called
        void tTM::OnWrote(mmerrno status) {
            if (this->userType & 1) { //Type::CLIENT ||  Type::BOTH_CLI
                _loger->debug("tTM is OnWrote client");
                if(_stder){
                    _stder->read(1024, -1);
                }                
                /*
                char sendbuf[1024];
                memset(sendbuf, 0, 1024);
                scanf("%s", sendbuf);
                size_t size = strlen(sendbuf) + 1;
                void* postdata = (void*)sendbuf;
                _monitor->setPtype(PUBLISH);
                callback* cbd = _monitor->request(postdata, size);
                if(cbd->data){
                    Sendto(cbd->data, cbd->size);
                }
                else {
                    _loger->error("tTM %v request failed code: %v!!!", user(userType), cbd->errcode);
                }*/
            }
            else { //Type::SERVER ||  Type::BOTH_SER
                _loger->debug("tTM is OnWrote server");
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
