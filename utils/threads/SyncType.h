#ifndef LOCKTYPE_H
#define LOCKTYPE_H
#include "../../platform.h"
#include "utilException.h"

#define STD_THREAD
#ifdef STD_THREAD
#undef OS_MSWIN
#undef OS_LINUX
    #include <mutex>
    #include <condition_variable>
    #include <functional>
    #include <thread>
    typedef void * SYS_ThreadHandle;
    struct Mutex
    {
        std::recursive_mutex cpp_mutex;
    };
    struct Cond
    {
        std::condition_variable_any cpp_cond;
    };
    struct Sem
    {
    };
#endif

#ifdef OS_LINUX
    #include <stdlib.h>
    #include <errno.h>
    #include <pthread.h>
    #include <sys/time.h>
    #include <unistd.h>
    #include <semaphore.h>
    typedef pthread_t SYS_ThreadHandle;
    struct Mutex
    {
        pthread_mutex_t id;
    #ifdef FAKE_RECURSIVE_MUTEX
        int recursive;
        pthread_t owner;
    #endif
    };
    struct Cond
    {
        pthread_cond_t cond;
    };
    struct Sem
    {
        sem_t sem;
    };
#endif

#ifdef OS_MSWIN
    #include <windows.h>
    #include <process.h>
    typedef HANDLE SYS_ThreadHandle;
    struct Mutex
    {
        CRITICAL_SECTION cs;
    };
    struct Cond
    {
    };
    struct Sem
    {
        HANDLE id;
        LONG count;
    };
#endif

    enum TYPE{
        MUTEX = 0,
        COND,
        SEM
    };

#define MUTEX_TIMEDOUT  1
#define MUTEX_MAXWAIT   (~(unsigned int)0)

#endif // LOCKTYPE_H
