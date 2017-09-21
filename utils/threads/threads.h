#ifndef THREADS_H
#define THREADS_H
#include "../../platform.h"
#include "ThreadType.h"

class threads
{
    typedef enum ThreadState
    {
        THREAD_STATE_ALIVE,
        THREAD_STATE_DETACHED,
        THREAD_STATE_ZOMBIE,
        THREAD_STATE_CLEANED,
    } ThreadState;
    typedef unsigned int TLSID;
    typedef enum {
        THREAD_PRI_LOW = 0,
        THREAD_PRI_NORMAL,
        THREAD_PRI_HIGH
    } THREAD_PRI;
public:
    threads();
    threads(const char* name);
    virtual ~threads();
public:
#ifdef STD_THREAD
#undef OS_MSWIN
#undef OS_LINUX
    static void* hook(void* args);
#endif
#ifdef OS_LINUX
    static void* hook(void *args);
#endif
#ifdef OS_MSWIN
    static unsigned int __stdcall hook(void *args);
#endif
    UTIL_API void UTIL_CALL run();
    UTIL_API void UTIL_CALL bind(ThreadFunction RunThread,void *args);
    UTIL_API void UTIL_CALL setThreadname(const char* name);
public:
    UTIL_API int   UTIL_CALL ThreadCreate(ThreadFunction RunThread = NULL,void *args = NULL);
    UTIL_API const char *UTIL_CALL getThreadName();
    UTIL_API threadID UTIL_CALL getThreadID();
    UTIL_API int UTIL_CALL setPRI(THREAD_PRI priority);
    UTIL_API void UTIL_CALL waitThread();
    UTIL_API void UTIL_CALL detachThread();
private:
    SYS_ThreadHandle _handle;
    ThreadFunction   _fn;
    threadID         _tid;
    int              _status;
    char*            _name;
    /* 0 for default, >0 for user-specified stack size. */
    size_t           _stacksize;
    void *           _args;
};

#endif // THREADS_H
