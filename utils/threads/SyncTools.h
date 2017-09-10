#ifndef SYNCTOOLS_H
#define SYNCTOOLS_H
#include "../../platform.h"
#include "utilException.h"
//#include "SyncType.h"

#define MUTEX_TIMEDOUT  1
#define MUTEX_MAXWAIT   (~(unsigned int)0)

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
    /* mutex */
    typedef struct _Mutex{
        std::recursive_mutex cpp_mutex;
    } syncMutex;
    /* cond */
    typedef struct _Cond{
        std::condition_variable_any cpp_cond;
    } syncCond;
    /* semaphore */
    typedef struct _Sem{
        std::mutex  _mutex;
        std::condition_variable _condition;
        unsigned long _count; // Initialized as locked.
    } syncSem;

    class Mutex
    {
    public:
        Mutex();
        ~Mutex();
    public:
        /* mutex */
        int   lockMutex(){ return this->_lockMutex(_mutex);}
        int   tryLockMutex(){ return this->_tryLockMutex(_mutex);}
        int   unlockMutex(){return this->_unlockMutex(_mutex);}
    private:
        /* mutex */
        syncMutex* createMutex(void);
        void   destroyMutex(syncMutex * mutex);
        int   _lockMutex(syncMutex * mutex);
        int   _tryLockMutex(syncMutex * mutex);
        int   _unlockMutex(syncMutex * mutex);
    private:
        syncMutex* _mutex;
    };
    class Cond
    {
    public:
        Cond();
        ~Cond();
    public:
        /* cond */
        int   condSignal(){return this->_condSignal(_cond);}
        int   condBroadcast(){return this->_condBroadcast(_cond);}
        int   condWaitTimeout(int ms){return this->_condWaitTimeout(_cond,_mutex,ms);}
        int   condWait(){return this->_condWait(_cond,_mutex);}
    private:
        /* cond */
        syncCond* createCond(void);
        void  destroyCond(syncCond * cond);
        int   _condSignal(syncCond * cond);
        int   _condBroadcast(syncCond * cond);
        int   _condWaitTimeout(syncCond * cond, syncMutex * mutex, int ms);
        int   _condWait(syncCond * cond, syncMutex * mutex);
    private:
        syncCond*  _cond;
        syncMutex* _mutex;
    };
    class Sem
    {
    public:
        Sem();
        ~Sem();
    public:
        /* semaphore */
        int   semTryWait(){return this->_semTryWait(_sem);}
        int   semWait(){return this->_semWait(_sem);}
        int   semWaitTimeout(int timeout){return this->_semWaitTimeout(_sem,timeout);}
        int   semValue(){return this->_semValue(_sem);}
        int   semPost(){return this->_semPost(_sem);}
    private:
        /* semaphore */
        syncSem*  createSemaphore(int initial_value);
        void  destroySemaphore(syncSem * sem);
        int   _semTryWait(syncSem * sem);
        int   _semWait(syncSem * sem);
        int   _semWaitTimeout(syncSem * sem, int timeout);
        int   _semValue(syncSem * sem);
        int   _semPost(syncSem * sem);
    private:
        syncSem* _sem;
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
    /* mutex */
    typedef struct _Mutex{
        pthread_mutex_t  id;
    #ifdef FAKE_RECURSIVE_MUTEX
        int recursive;
        pthread_t owner;
    #endif
    } syncMutex;
    /* cond */
    typedef struct _Cond{
        pthread_cond_t cond;
    } syncCond;
    /* semaphore */
    typedef struct _Sem{
        sem_t sem;
    } syncSem;

    class Mutex
    {
    public:
        Mutex();
        ~Mutex();
    public:
        /* mutex */
        int   lockMutex(){ return this->_lockMutex(_mutex);}
        int   tryLockMutex(){ return this->_tryLockMutex(_mutex);}
        int   unlockMutex(){return this->_unlockMutex(_mutex);}
    private:
        /* mutex */
        syncMutex* createMutex(void);
        void  destroyMutex(syncMutex * mutex);
        int   _lockMutex(syncMutex * mutex);
        int   _tryLockMutex(syncMutex * mutex);
        int   _unlockMutex(syncMutex * mutex);
    private:
        syncMutex* _mutex;
    };
    class Cond
    {
    public:
        Cond();
        ~Cond();
    public:
        /* cond */
        int   condSignal(){return this->_condSignal(_cond);}
        int   condBroadcast(){return this->_condBroadcast(_cond);}
        int   condWaitTimeout(int ms){return this->_condWaitTimeout(_cond,_mutex,ms);}
        int   condWait(){return this->_condWait(_cond,_mutex);}
    private:
        /* cond */
        syncCond* createCond(void);
        void  destroyCond(syncCond * cond);
        int   _condSignal(syncCond * cond);
        int   _condBroadcast(syncCond * cond);
        int   _condWaitTimeout(syncCond * cond, syncMutex * mutex, int ms);
        int   _condWait(syncCond * cond, syncMutex * mutex);
    private:
        syncCond* _cond;
        syncMutex* _mutex;
    };
    class Sem
    {
    public:
        Sem();
        ~Sem();
    public:
        /* semaphore */
        int   semTryWait(){return this->_semTryWait(_sem);}
        int   semWait(){return this->_semWait(_sem);}
        int   semWaitTimeout(int timeout){return this->_semWaitTimeout(_sem,timeout);}
        int   semValue(){return this->_semValue(_sem);}
        int   semPost(){return this->_semPost(_sem);}
    private:
        /* semaphore */
        syncSem*  createSemaphore(int initial_value);
        void  destroySemaphore(syncSem * sem);
        int   _semTryWait(syncSem * sem);
        int   _semWait(syncSem * sem);
        int   _semWaitTimeout(syncSem * sem, int timeout);
        int   _semValue(syncSem * sem);
        int   _semPost(syncSem * sem);
    private:
        syncSem* _sem;
    };
#endif

#ifdef OS_MSWIN
    #include <windows.h>
    #include <process.h>
    #include <stdlib.h>
    #include <stdio.h>
    typedef HANDLE SYS_ThreadHandle;
    /* mutex */
    typedef struct _Mutex{
        CRITICAL_SECTION cs;
    } syncMutex;
    /* cond */
    typedef struct _Cond{
        HANDLE id;
        CONDITION_VARIABLE cv;
    } syncCond;
    /* semaphore */
    typedef struct _Sem
    {
        HANDLE id;
        LONG count;
    }syncSem;

    class Mutex
    {
    public:
        Mutex();
        ~Mutex();
    public:
         /* mutex */
         int   lockMutex(){ return this->_lockMutex(_mutex);}
         int   tryLockMutex(){ return this->_tryLockMutex(_mutex);}
         int   unlockMutex(){return this->_unlockMutex(_mutex);}
     private:
         /* mutex */
         syncMutex* createMutex(void);
         void  destroyMutex(syncMutex * mutex);
         int   _lockMutex(syncMutex * mutex);
         int   _tryLockMutex(syncMutex * mutex);
         int   _unlockMutex(syncMutex * mutex);
     private:
         syncMutex* _mutex;
    };
    class Cond
    {
    public:
        Cond();
        ~Cond();
    public:
        /* cond */
        int   condSignal(){return this->_condSignal(_cond);}
        int   condBroadcast(){return this->_condBroadcast(_cond);}
        int   condWaitTimeout(int ms){return this->_condWaitTimeout(_cond,_mutex,ms);}
        int   condWait(){return this->_condWait(_cond,_mutex);}
    private:
        syncCond* createCond(void);
        void  destroyCond(syncCond * cond);
        int   _condSignal(syncCond * cond);
        int   _condBroadcast(syncCond * cond);
        int   _condWaitTimeout(syncCond * cond, syncMutex * mutex, int ms);
        int   _condWait(syncCond * cond, syncMutex * mutex);
    private:
        syncCond* _cond;
        syncMutex* _mutex;
    };
    class Sem
    {
    public:
        Sem();
        ~Sem();
    public:
        /* semaphore */
        int   semTryWait(){return this->_semTryWait(_sem);}
        int   semWait(){return this->_semWait(_sem);}
        int   semWaitTimeout(int timeout){return this->_semWaitTimeout(_sem,timeout);}
        int   semValue(){return this->_semValue(_sem);}
        int   semPost(){return this->_semPost(_sem);}
    private:
        /* semaphore */
        syncSem*  createSemaphore(int initial_value);
        void  destroySemaphore(syncSem * sem);
        int   _semTryWait(syncSem * sem);
        int   _semWait(syncSem * sem);
        int   _semWaitTimeout(syncSem * sem, int timeout);
        int   _semValue(syncSem * sem);
        int   _semPost(syncSem * sem);
    private:
        syncSem* _sem;
    };
#endif

#endif // SYNCTOOLS_H
