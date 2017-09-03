#ifndef SYNCTOOLS_H
#define SYNCTOOLS_H
#include "../../platform.h"
#include "SyncType.h"

class SyncTools
{
public:
    SyncTools(TYPE type,int initv = 0);
    virtual ~SyncTools();
public:
    /* mutex */
    int   lockMutex(){ return this->_lockMutex(_mutex);}
    int   tryLockMutex(){ return this->_tryLockMutex(_mutex);}
    int   unlockMutex(){return this->_unlockMutex(_mutex);}
    /* cond */
    int   condSignal(){return this->_condSignal(_cond);}
    int   condBroadcast(){return this->_condBroadcast(_cond);}
    int   condWaitTimeout(int ms){return this->_condWaitTimeout(_cond,_mutex,ms);}
    int   condWait(){return this->_condWait(_cond,_mutex);}
    /* semaphore */
    int   semTryWait(){return this->_semTryWait(_sem);}
    int   semWait(){return this->_semWait(_sem);}
    int   semWaitTimeout(int timeout){return this->_semWaitTimeout(_sem,timeout);}
    int   semValue(){return this->_semValue(_sem);}
    int   semPost(){return this->_semPost(_sem);}
    inline TYPE type(){return _type;}
private:
    /* mutex */
    Mutex* createMutex(void);
    void  destroyMutex(Mutex * mutex);
    int   _lockMutex(Mutex * mutex);
    int   _tryLockMutex(Mutex * mutex);
    int   _unlockMutex(Mutex * mutex);
    /* cond */
    Cond* createCond(void);
    void  destroyCond(Cond * cond);
    int   _condSignal(Cond * cond);
    int   _condBroadcast(Cond * cond);
    int   _condWaitTimeout(Cond * cond, Mutex * mutex, int ms);
    int   _condWait(Cond * cond, Mutex * mutex);
    /* semaphore */
    Sem*  createSemaphore(int initial_value);
    void  destroySemaphore(Sem * sem);
    int   _semTryWait(Sem * sem);
    int   _semWait(Sem * sem);
    int   _semWaitTimeout(Sem * sem, int timeout);
    int   _semValue(Sem * sem);
    int   _semPost(Sem * sem);
private:
    TYPE    _type;
    Mutex*  _mutex;
    Cond*   _cond;
    Sem*    _sem;
};

#endif // SYNCTOOLS_H
