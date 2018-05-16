#pragma once
#ifndef _INTERFACE_H_
#define _INTERFACE_H_

enum Type {
    SERVER = 0,                /* Just a server */
    CLIENT,                    /* Just a client */
    BOTH_SER,                  /* server or client,Tend to server */
    BOTH_CLI,                  /* server or client,Tend to client */
};
const static char* _type[5] = { "server","client","both_ser","both_cli","unknown" };
inline const static char* user(int t) { return _type[t]; }

class iTransfer
{
    /*
    protected:
    iTransfer():_packer(NULL),_unpack(NULL) {}
    ~iTransfer() {}
    */
public:
    /*
    - 传输器接口
    0. 联系 Relate   : 上层获取一个传输器
    1. 指定 Bind     : 指定一种协议打包或解析数据包
    2. 发送 Sendto   : 发送包装好的数据包
    3. 接收 Recfrm   : 发送包装好的数据包
    5. 取关 Unlink   : 本次结束，取消与传输器的联系
    */
    virtual int   Relate(const char* addr, const int port, Type type = SERVER) = 0;
    virtual bool  Bind(const char* protocol = "MIM1") = 0;
    virtual void  Sendto(void* buf, size_t& count) = 0;
    virtual void  Recfrm(void* buf, size_t& count) = 0;
    virtual int   Unlink() = 0;
};

#endif // !_INTERFACE_H_
