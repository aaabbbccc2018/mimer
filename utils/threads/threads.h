#ifndef THREADS_H
#define THREADS_H
#include "../../platform.h"
#include "SyncType.h"
#include "SyncTools.h"

typedef unsigned int threadID;
typedef unsigned int TLSID;
#ifdef OS_LINUX
typedef void* (UTIL_CALL * ThreadFunction) (void *args);
#endif
#ifdef OS_MSWIN
typedef unsigned int (__stdcall *ThreadFunction)(void *args);
#endif

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
        THREAD_PRI_LOW,
        THREAD_PRI_NORMAL,
        THREAD_PRI_HIGH
    } THREAD_PRI;
public:
    threads();
    threads(char* name);
    ~threads();
public:
    UTIL_API void UTIL_CALL run();
    UTIL_API void UTIL_CALL setThreadFunction(ThreadFunction fn);
    UTIL_API void UTIL_CALL setThreadname(const char* name);
public:
    UTIL_API int   UTIL_CALL ThreadCreate(ThreadFunction fn,void *args);
    UTIL_API const char *UTIL_CALL getThreadName();
    UTIL_API threadID UTIL_CALL getThreadID();
    UTIL_API int UTIL_CALL setPRI(THREAD_PRI priority);
    UTIL_API void UTIL_CALL waitThread(int *status);
    UTIL_API void UTIL_CALL detachThread();
private:
    SYS_ThreadHandle _handle;
    ThreadFunction   _fn;
    SyncTools*       _sync;
    threadID         _threadid;
    int              _status;
    char*            _name;
    /* 0 for default, >0 for user-specified stack size. */
    size_t           _stacksize;
    void *           _args;
};

#endif // THREADS_H
