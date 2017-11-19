#pragma once
#include "mmloop_net.h"

using namespace mm::uvbase;
typedef  TCP  baseT;                       /*Provide a transport layer base class,Now(2017-3-14) is uv::base::TCP*/
typedef  UDP  baseU;                       /*Provide a transport layer base class,Now(2017-3-14) is uv::base::UDP*/

namespace mm {
	namespace Transmitter {
		/*
			interface of Transmitter
		*/
		class ITM
		{
		/*
		protected:
			ITM():_packer(NULL),_unpack(NULL) {}
			~ITM() {}
		*/
		public:
			enum Type {
				SERVER = 0,                /* Just a server */
				CLIENT,                    /* Just a client */
				BOTH_SER,                  /* server or client,Tend to server */
				BOTH_CLI,                  /* server or client,Tend to client */
			};
			typedef void* (packer)(void*, size_t);
			typedef void* (unpack)(void*, size_t);
		public:
			/*
			- 传输器接口
			0. 联系 Relate   : 上层获取一个传输器
			1. 打包 Packet   : 将上层的原数据打包，为发送准备(private)
			2. 发送 Sendto   : 发送包装好的数据包
			3. 接收 Recfrm   : 发送包装好的数据包
			4. 拆包 Unpack   : 将上层的原数据打包，为发送准备(private)
			5. 取关 Unlink   : 本次结束，取消与传输器的联系
			*/
			virtual int   Relate(const char* addr,const int port,Type type = SERVER) = 0;
			virtual int   Unlink() = 0;
			virtual void* Packer(void* data, size_t size) {
				if (_packer) {
					return _packer(data, size);
				}
				return data;
			}
			virtual void* Unpack(void* data, size_t size) {
				if (_unpack) {
					return _unpack(data, size);
				}
				return data;
			}
			virtual int   Sendto(void* buf, size_t count) = 0;
			virtual int   Recfrm(void* buf, size_t count) = 0;
		public:
			void set_packer(packer* method) { _packer = method; }
			void set_unpack(unpack* method) { _unpack = method; }
		protected:
			packer* _packer = NULL;
			unpack* _unpack = NULL;
		};
		
		class tTM :public ITM, public baseT
		{
		/*
		public:
			tTM() {}
			~tTM() {}
		*/
		public:
			const char* _type[5] = { "server","client","both_ser","both_cli","unknown" };
			inline const char* user() { return _type[userType]; }

		public:
			virtual int  Relate(const char* addr, const int port, Type type = Type::SERVER);
			virtual int  Unlink();
			/* post */
			virtual int  Sendto(void* buf, size_t count);
			/* get */
			virtual int  Recfrm(void* buf, size_t count);
		private:
			/* will be implement a client */
			virtual void OnConnected(mmerrno status);
			/* will be implement a server */
			virtual void doAccept(mmerrno status);
			/* I/O ,if read a data,will be save a buf*/
			virtual void OnRead(ssize_t nread, const char *buf);
			/* I/O ,if write a data*/
			virtual void OnWrote(mmerrno status);
		private:
			Type userType;
		};
		typedef tTM TTM;

		class uTM :public ITM, public baseU
		{
		/*
		public:
			uTM() {}
			~uTM() {}
		*/
		public:
			const char* _type[5] = { "server","client","both_ser","both_cli","unknown" };
			inline const char* user() { return _type[userType]; }

		public:
			virtual int  Relate(const char* addr, const int port, Type type = Type::SERVER);
			virtual int  Unlink();
			/* post */
			virtual int  Sendto(void* buf, size_t count);
			/* get */
			virtual int  Recfrm(void* buf, size_t count);
		private:
			virtual void OnSent(mmerrno status);
			//	virtual void OnAllocate(UDP *self, size_t suggested_size, uv_buf_t *buf) {}
			virtual void OnReceived(ssize_t nread, const char *buf, const struct sockaddr *addr, unsigned flags);
		private:
			Type userType;
			const char* _addr;
			int  _port;
		};
		typedef uTM UTM;
	}

}