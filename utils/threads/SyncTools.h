#ifndef THREADS_H
#define THREADS_H
#include "../../platform.h"
#include "SyncType.h"
enum TYPE{
    MUTEX = 0,
    COND,
    SEM
};

class SyncTools
{
public:
    SyncTools(TYPE type,int initv);
    ~SyncTools();
public:
    /* mutex */
    Mutex* createMutex(void);
    void  destroyMutex(Mutex * mutex);
    int   lockMutex(Mutex * mutex);
    int   tryLockMutex(Mutex * mutex);
    int   unlockMutex(Mutex * mutex);
    /* cond */
    Cond* createCond(void);
    void  destroyCond(Cond * cond);
    int   condSignal(Cond * cond);
    int   condBroadcast(Cond * cond);
    int   condWaitTimeout(Cond * cond, Mutex * mutex, int ms);
    int   condWait(Cond * cond, Mutex * mutex);
    /* semaphore */
    Sem*  createSemaphore(int initial_value);
    void  destroySemaphore(Sem * sem);
    int   semTryWait(Sem * sem);
    int   semWait(Sem * sem);
    int   semWaitTimeout(Sem * sem, int timeout);
    int   semValue(Sem * sem);
    int   semPost(Sem * sem);
private:
    int   SetError(const char *fmt,...);
private:
    TYPE    _type;
    Mutex*  _mutex;
    Cond*   _cond;
    Sem*    _sem;
};

#endif // THREADS_H
