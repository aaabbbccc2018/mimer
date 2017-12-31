#pragma once
#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

enum Type {
	SERVER = 0,                /* Just a server */
	CLIENT,                    /* Just a client */
	BOTH_SER,                  /* server or client,Tend to server */
	BOTH_CLI,                  /* server or client,Tend to client */
};
const static char* _type[5] = { "server","client","both_ser","both_cli","unknown" };
inline const static char* user(int t) { return _type[t]; }

#endif // !_COMMON_DEFINE_H_
