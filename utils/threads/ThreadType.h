#pragma once
#ifndef THREAD_TYPE_H
#define THREAD_TYPE_H
#include "utilException.h"

typedef unsigned int threadID;
typedef unsigned int TLSID;

#ifdef STD_THREAD
#undef OS_MSWIN
#undef OS_LINUX
	#include <functional>
	#include <iostream>           // std::cout
	#include <thread>             // std::thread
	#include <chrono>             // std::chrono::seconds
	#include <mutex>              // std::mutex, std::unique_lock
	#include <condition_variable> // std::condition_variable, std::cv_status
	typedef void * SYS_ThreadHandle;
	typedef std::function< void*(void*)> ThreadFunction;
#endif

#ifdef OS_LINUX
	#include <stdlib.h>
	#include <errno.h>
	#include <pthread.h>
	#include <sys/time.h>
	#include <unistd.h>
	#include <semaphore.h>
	typedef pthread_t SYS_ThreadHandle;
	typedef void* (UTIL_CALL * ThreadFunction) (void *args);
#endif

#ifdef OS_MSWIN
	#include <windows.h>
	#include <process.h>
	#include <stdlib.h>
	#include <stdio.h>
	typedef HANDLE SYS_ThreadHandle;
	typedef unsigned int(__stdcall *ThreadFunction)(void *args);
#endif

#endif // ThreadType.h
