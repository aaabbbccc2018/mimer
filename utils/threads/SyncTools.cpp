#include <stdio.h>
#include "SyncTools.h"

//INITIALIZE_EASYLOGGINGPP
/* Mutex */

Mutex::Mutex():baseSync()
{
    _mutex = createMutex();
    loger->info("createMutex ...");
}

Mutex::~Mutex()
{
    if (_mutex){
        destroyMutex(_mutex);
    }
}

syncMutex* Mutex::createMutex(void)
{
    syncMutex *mutex;
#ifdef OS_LINUX
    pthread_mutexattr_t attr;
    /* Allocate the structure */
    mutex = (syncMutex *)calloc(1, sizeof(syncMutex));
    if (mutex) {
        pthread_mutexattr_init(&attr);
#if THREAD_PTHREAD_RECURSIVE_MUTEX
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#elif THREAD_PTHREAD_RECURSIVE_MUTEX_NP
        pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#else
        /* No extra attributes necessary */
#endif
        if (pthread_mutex_init(&mutex->id, &attr) != 0) {
            syncerrno = EINIT;
            loger->error("errno: %d pthread_mutex_init() failed", syncerrno);
            utilException::SetError("errno: %d pthread_mutex_init() failed", syncerrno);
            free(mutex);
            mutex = NULL;
        }
    } else {
        syncerrno = EALLOC;
        loger->error("errno: %d Allocate the initialize Mutex failed", syncerrno);
        utilException::SetError("errno: %d Allocate the initialize Mutex failed", syncerrno);
    }
#endif

#ifdef STD_THREAD
    /* Allocate and initialize the mutex */
    try {
        mutex = new syncMutex;
    } catch (std::system_error & ex) {
        syncerrn = EALLOC;
        loger->error("errno: %d unable to create a C++ mutex: code=%d; %s", ex.code(), ex.what());
        utilException::SetError("errno: %d unable to create a C++ mutex: code=%d; %s", ex.code(), ex.what());
        return NULL;
    } catch (std::bad_alloc &) {
        syncerrno = EALLOC;
        loger->error("errno: %d Allocate the initialize Mutex failed", syncerrno);
        utilException::SetError("errno: %d Allocate the initialize Mutex failed", syncerrno);
        return NULL;
    }
#endif

#ifdef OS_MSWIN
    /* Allocate mutex memory */
    mutex = (syncMutex *) calloc(1, sizeof(syncMutex));
    if (mutex) {
        /* Initialize */
        /* On SMP systems, a non-zero spin count generally helps performance */
#if __WINRT__
        InitializeCriticalSectionEx(&mutex->cs, 2000, 0);
#else
        InitializeCriticalSectionAndSpinCount(&mutex->cs, 2000);
#endif
    } else {
        syncerrno = EALLOC;
        loger->error("errno: %d Allocate the initialize Mutex failed", syncerrno);
        utilException::SetError("errno: %d Allocate the initialize Mutex failed", syncerrno);
    }
#endif
    return (mutex);
}

void   Mutex::destroyMutex(syncMutex * mutex)
{
#ifdef OS_LINUX
    if (mutex) {
        pthread_mutex_destroy(&mutex->id);
        free(mutex);
    }
#endif

#ifdef STD_THREAD
    if (mutex) {
        delete mutex;
    }
#endif

#ifdef OS_MSWIN
    if (mutex) {
        DeleteCriticalSection(&mutex->cs);
        free(mutex);
    }
#endif
}

int    Mutex::_lockMutex(syncMutex * mutex)
{
#ifdef OS_LINUX

#ifdef FAKE_RECURSIVE_MUTEX
    pthread_t this_thread;
#endif
    if (mutex == NULL) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL mutex", syncerrno);
        utilException::SetError("errno: %d Passed a NULL mutex", syncerrno);
        return syncerrno;
    }
#ifdef FAKE_RECURSIVE_MUTEX
    this_thread = pthread_self();
    if (mutex->owner == this_thread) {
        ++mutex->recursive;
    } else {
        /* The order of operations is important.
           We set the locking thread id after we obtain the lock
           so unlocks from other SyncTools will fail.
         */
        if (pthread_mutex_lock(&mutex->id) == 0) {
            mutex->owner = this_thread;
            mutex->recursive = 0;
        } else {
            syncerrno = EEXEC;
            loger->error("errno: %d pthread_mutex_lock() failed", syncerrno);
            utilException::SetError("errno: %d pthread_mutex_lock() failed", syncerrno);
            return syncerrno;
        }
    }
#else
    if (pthread_mutex_lock(&mutex->id) < 0) {
        syncerrno = EEXEC;
        loger->error("errno: %d pthread_mutex_lock() failed", syncerrno);
        utilException::SetError("errno: %d pthread_mutex_lock() failed", syncerrno);
        return syncerrno;
    }
#endif
    return EOKED;
#endif

#ifdef STD_THREAD
    if (mutex == NULL) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL mutex", syncerrno);
        utilException::SetError("errno: %d Passed a NULL mutex", syncerrno);
        return syncerrno;
    }

    try {
        mutex->cpp_mutex.lock();
        return EOKED;
    } catch (std::system_error & ex) {
        syncerrno = EEXEC;
        loger->error("errno: %d unable to lock a C++ mutex: code=%d; %s", ex.code(), ex.what());
        utilException::SetError("errno: %d unable to lock a C++ mutex: code=%d; %s", ex.code(), ex.what());
        return syncerrno;
    }
#endif

#ifdef OS_MSWIN
    if (mutex == NULL) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL mutex", syncerrno);
        utilException::SetError("errno: %d Passed a NULL mutex", syncerrno);
        return syncerrno;
    }
    EnterCriticalSection(&mutex->cs);
    return (0);
#endif
}

int    Mutex::_tryLockMutex(syncMutex * mutex)
{
#ifdef OS_LINUX
    int retval;
#ifdef FAKE_RECURSIVE_MUTEX
    pthread_t this_thread;
#endif

    if (mutex == NULL) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL mutex", syncerrno);
        utilException::SetError("errno: %d Passed a NULL mutex", syncerrno);
        return syncerrno;
    }
    retval = 0;
#ifdef FAKE_RECURSIVE_MUTEX
    this_thread = pthread_self();
    if (mutex->owner == this_thread) {
        ++mutex->recursive;
    } else {
        /* The order of operations is important.
         We set the locking thread id after we obtain the lock
         so unlocks from other SyncTools will fail.
         */
        if (pthread_mutex_lock(&mutex->id) == 0) {
            mutex->owner = this_thread;
            mutex->recursive = 0;
        } else if (errno == EBUSY) {
            retval = MUTEX_TIMEDOUT;
        } else {
            syncerrno = EEXEC;
            loger->error("errno: %d pthread_mutex_trylock() failed", syncerrno);
            utilException::SetError("errno: %d pthread_mutex_trylock() failed", syncerrno);
            return syncerrno;
        }
    }
#else
    if (pthread_mutex_trylock(&mutex->id) != 0) {
        if (errno == EBUSY) {
            retval = MUTEX_TIMEDOUT;
        } else {
            syncerrno = EEXEC;
            loger->error("errno: %d pthread_mutex_trylock() failed", syncerrno);
            utilException::SetError("errno: %d pthread_mutex_trylock() failed", syncerrno);
            return syncerrno;
        }
    }
#endif
    return retval;
#endif

#ifdef STD_THREAD
    int retval = 0;
    if (mutex == NULL) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL mutex", syncerrno);
        utilException::SetError("errno: %d Passed a NULL mutex", syncerrno);
        return syncerrno;
    }

    if (mutex->cpp_mutex.try_lock() == false) {
        syncerrno = EEXEC;
        retval = MUTEX_TIMEDOUT;
    }
    return retval;
#endif

#ifdef OS_MSWIN
    int retval = 0;
    if (mutex == NULL) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL mutex", syncerrno);
        utilException::SetError("errno: %d Passed a NULL mutex", syncerrno);
        return syncerrno;
    }

    if (TryEnterCriticalSection(&mutex->cs) == 0) {
        retval = MUTEX_TIMEDOUT;
    }
    return retval;
#endif
}

int    Mutex::_unlockMutex(syncMutex * mutex)
{
#ifdef OS_LINUX
    if (mutex == NULL) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL mutex", syncerrno);
        utilException::SetError("errno: %d Passed a NULL mutex", syncerrno);
        return syncerrno;
    }

#ifdef FAKE_RECURSIVE_MUTEX
    /* We can only unlock the mutex if we own it */
    if (pthread_self() == mutex->owner) {
        if (mutex->recursive) {
            --mutex->recursive;
        } else {
            /* The order of operations is important.
               First reset the owner so another thread doesn't lock
               the mutex and set the ownership before we reset it,
               then release the lock semaphore.
             */
            mutex->owner = 0;
            pthread_mutex_unlock(&mutex->id);
        }
    } else {
        syncerrno = ESUPPORT;
        loger->error("errno: %d mutex not owned by this thread", syncerrno);
        utilException::SetError("errno: %d mutex not owned by this thread", syncerrno);
        return syncerrno;
    }

#else
    if (pthread_mutex_unlock(&mutex->id) < 0) {
        syncerrno = EEXEC;
        loger->error("errno: %d pthread_mutex_unlock() failed", syncerrno);
        utilException::SetError("errno: %d pthread_mutex_unlock() failed", syncerrno);
        return syncerrno;
    }
#endif /* FAKE_RECURSIVE_MUTEX */

    return EOKED;
#endif

#ifdef STD_THREAD
    if (mutex == NULL) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL mutex", syncerrno);
        utilException::SetError("errno: %d Passed a NULL mutex", syncerrno);
        return syncerrno;
    }

    mutex->cpp_mutex.unlock();
    return EOKED;
#endif

#ifdef OS_MSWIN
    if (mutex == NULL) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL mutex", syncerrno);
        utilException::SetError("errno: %d Passed a NULL mutex", syncerrno);
        return syncerrno;
    }

    LeaveCriticalSection(&mutex->cs);
    return (0);
#endif
}

/* Cond */

Cond::Cond() :baseSync()
{
    _cond = createCond();
    _mutex = NULL;
#ifdef OS_LINUX
    pthread_mutexattr_t attr;
    /* Allocate the structure */
    _mutex = (syncMutex *)calloc(1, sizeof(syncMutex));
    if (_mutex) {
        pthread_mutexattr_init(&attr);
#if THREAD_PTHREAD_RECURSIVE_MUTEX
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#elif THREAD_PTHREAD_RECURSIVE_MUTEX_NP
        pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#else
        /* No extra attributes necessary */
#endif
        if (pthread_mutex_init(&_mutex->id, &attr) != 0) {
            syncerrno = EINIT;
            loger->error("errno: %d pthread_mutex_init() failed", syncerrno);
            utilException::SetError("errno: %d pthread_mutex_init() failed", syncerrno);
            free(_mutex);
        }
    } else {
        syncerrno = EALLOC;
        loger->error("errno: %d Allocate the initialize Mutex failed", syncerrno);
        utilException::SetError("errno: %d Allocate the initialize Mutex failed", syncerrno);
    }
#endif

#ifdef STD_THREAD
    /* Allocate and initialize the mutex */
    try {
        _mutex = new syncMutex;
    } catch (std::system_error & ex) {
        syncerrno = EALLOC;
        loger->error("errno: %d unable to create a C++ mutex: code=%d; %s", ex.code(), ex.what());
        utilException::SetError("errno: %d unable to create a C++ mutex: code=%d; %s", ex.code(), ex.what());
    } catch (std::bad_alloc &) {
        syncerrno = EALLOC;
        loger->error("errno: %d Allocate the initialize Mutex failed", syncerrno);
        utilException::SetError("errno: %d Allocate the initialize Mutex failed", syncerrno);
    }
#endif

#ifdef OS_MSWIN
    /* Allocate mutex memory */
    _mutex = (syncMutex *) calloc(1, sizeof(syncMutex));
    if (_mutex) {
        /* Initialize */
        /* On SMP systems, a non-zero spin count generally helps performance */
#if __WINRT__
        InitializeCriticalSectionEx(&_mutex->cs, 2000, 0);
#else
        InitializeCriticalSectionAndSpinCount(&_mutex->cs, 2000);
#endif
    } else {
        syncerrno = EALLOC;
        loger->error("errno: %d Allocate the initialize Mutex failed", syncerrno);
        utilException::SetError("errno: %d Allocate the initialize Mutex failed", syncerrno);
    }
#endif
    loger->info("createCond ...");
}

Cond::~Cond()
{
    if (_cond){
        destroyCond(_cond);
    }
    if (_mutex){
#ifdef OS_LINUX
    if (_mutex) {
        pthread_mutex_destroy(&_mutex->id);
        free(_mutex);
    }
#endif

#ifdef STD_THREAD
    if (_mutex) {
        delete _mutex;
    }
#endif

#ifdef OS_MSWIN
    if (_mutex) {
        DeleteCriticalSection(&_mutex->cs);
        free(_mutex);
    }
#endif
    }
}

syncCond* Cond::createCond(void)
{
    syncCond* cond = NULL;
#ifdef OS_LINUX
    cond = (syncCond *) malloc(sizeof(syncCond));
    if (cond) {
        if (pthread_cond_init(&cond->cond, NULL) < 0) {
            syncerrno = EINIT;
            loger->error("errno: %d pthread_cond_init() failed", syncerrno);
            utilException::SetError("errno: %d pthread_cond_init() failed", syncerrno);
            free(cond);
            cond = NULL;
        }
    }
#endif

#ifdef STD_THREAD
    /* Allocate and initialize the condition variable */
    try {
        cond = new syncCond;
    } catch (std::system_error & ex) {
        syncerrno = EALLOC;
        loger->error("errno: %d unable to create a C++ condition variable: code=%d; %s", ex.code(), ex.what());
        utilException::SetError("errno: %d unable to create a C++ condition variable: code=%d; %s", ex.code(), ex.what());
        return NULL;
    } catch (std::bad_alloc &) {
        syncerrno = EALLOC;
        loger->error("errno: %d bad_alloc a Cond", syncerrno);
        utilException::SetError("errno: %d bad_alloc a Cond", syncerrno);
        return NULL;
    }
#endif

#ifdef OS_MSWIN
    cond = new syncCond;
#if _WIN32_WINNT >= 0x0600
    InitializeConditionVariable (&cond->cv);
#else
    syncerrno = ESUPPORT;
    loger->error("errno: %d not support InitializeConditionVariable a Cond", syncerrno);
    utilException::SetError("errno: %d not support InitializeConditionVariable a Cond", syncerrno);
#endif
#endif
    return cond;
}

void  Cond::destroyCond(syncCond * cond)
{
#ifdef OS_LINUX
    if (cond) {
        pthread_cond_destroy(&cond->cond);
        free(cond);
    }
#endif

#ifdef STD_THREAD
    if (cond) {
        delete cond;
    }
#endif

#ifdef OS_MSWIN
    if(cond){
        delete cond;
    }
    return;
#endif
}

int   Cond::_condSignal(syncCond * cond)
{
#ifdef OS_LINUX
    int retval;

    if (NULL == cond) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL condition variable", syncerrno);
        utilException::SetError("errno: %d Passed a NULL condition variable", syncerrno);
        return syncerrno;
    }

    retval = 0;
    if (pthread_cond_signal(&cond->cond) != 0) {
        syncerrno = EEXEC;
        loger->error("errno: %d pthread_cond_signal() failed", syncerrno);
        utilException::SetError("errno: %d pthread_cond_signal() failed", syncerrno);
        return syncerrno;
    }
    return retval;
#endif

#ifdef STD_THREAD
    if (NULL == cond) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL condition variable", syncerrno);
        utilException::SetError("errno: %d Passed a NULL condition variable", syncerrno);
        return syncerrno;
    }

    cond->cpp_cond.notify_one();
    return EOKED;
#endif

#ifdef OS_MSWIN
    if (NULL == cond) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL condition variable", syncerrno);
        utilException::SetError("errno: %d Passed a NULL condition variable", syncerrno);
        return syncerrno;
    }
#if _WIN32_WINNT >= 0x0600
    WakeConditionVariable(&cond->cv);
#else
    syncerrno = ESUPPORT;
    loger->error("errno: %d not support WakeConditionVariable a Cond", syncerrno);
    utilException::SetError("errno: %d not support WakeConditionVariable a Cond", syncerrno);
    return syncerrno;
#endif
    return EOKED;
#endif
}

int   Cond::_condBroadcast(syncCond * cond)
{
#ifdef OS_LINUX
    int retval;
    if (NULL == cond) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL condition variable", syncerrno);
        utilException::SetError("errno: %d Passed a NULL condition variable", syncerrno);
        return syncerrno;
    }

    retval = 0;
    if (pthread_cond_broadcast(&cond->cond) != 0) {
        syncerrno = EEXEC;
        loger->error("errno: %d pthread_cond_broadcast() failed", syncerrno);
        utilException::SetError("errno: %d pthread_cond_broadcast() failed", syncerrno);
        return syncerrno;
    }
    return retval;
#endif

#ifdef STD_THREAD
    if (NULL == cond) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL condition variable", syncerrno);
        utilException::SetError("errno: %d Passed a NULL condition variable", syncerrno);
        return syncerrno;
    }
    cond->cpp_cond.notify_all();
    return EOKED;
#endif

#ifdef OS_MSWIN
    if (NULL == cond) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL condition variable", syncerrno);
        utilException::SetError("errno: %d Passed a NULL condition variable", syncerrno);
        return syncerrno;
    }
#if _WIN32_WINNT >= 0x0600
    WakeAllConditionVariable(&cond->cv);
#else
    syncerrno = ESUPPORT;
    loger->error("errno: %d not support WakeAllConditionVariable a Cond", syncerrno);
    utilException::SetError("errno: %d not support WakeAllConditionVariable a Cond", syncerrno);
    return syncerrno;
#endif
    return EOKED;
#endif
}

int   Cond::_condWaitTimeout(syncCond * cond, syncMutex * mutex, int timeout)
{
#ifdef OS_LINUX
    int retval;
#ifndef HAVE_CLOCK_GETTIME
    struct timeval delta;
#endif
    struct timespec abstime;

    if (NULL == cond) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL condition variable", syncerrno);
        utilException::SetError("errno: %d Passed a NULL condition variable", syncerrno);
        return syncerrno;
    }

#ifdef HAVE_CLOCK_GETTIME
    clock_gettime(CLOCK_REALTIME, &abstime);

    abstime.tv_nsec += (timeout % 1000) * 1000000;
    abstime.tv_sec += timeout / 1000;
#else
    gettimeofday(&delta, NULL);

    abstime.tv_sec = delta.tv_sec + (timeout / 1000);
    abstime.tv_nsec = (delta.tv_usec + (timeout % 1000) * 1000) * 1000;
#endif
    if (abstime.tv_nsec > 1000000000) {
        abstime.tv_sec += 1;
        abstime.tv_nsec -= 1000000000;
    }

tryagain:
    retval = pthread_cond_timedwait(&cond->cond, &mutex->id, &abstime);
    switch (retval) {
    case EINTR:
        goto tryagain;
        break;
    case ETIMEDOUT:
        retval = MUTEX_TIMEDOUT;
        break;
    case 0:
        break;
    default:
        syncerrno = EEXEC;
        loger->error("errno: %d pthread_cond_timedwait() failed", syncerrno);
        utilException::SetError("errno: %d pthread_cond_timedwait() failed", syncerrno);
        // return syncerrno;
    }
    return retval;
#endif

#ifdef STD_THREAD
    if (NULL == cond) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL condition variable", syncerrno);
        utilException::SetError("errno: %d Passed a NULL condition variable", syncerrno);
        return syncerrno;
    }

    if (NULL == mutex) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL mutex variable", syncerrno);
        utilException::SetError("errno: %d Passed a NULL mutex variable", syncerrno);
        return syncerrno;
    }

    try {
        std::unique_lock<std::recursive_mutex> cpp_lock(_mutex->cpp_mutex, std::adopt_lock_t());
        if (timeout == MUTEX_MAXWAIT) {
            cond->cpp_cond.wait( cpp_lock);
            cpp_lock.release();
            return EOKED;
        } else {
            auto wait_result = cond->cpp_cond.wait_for( cpp_lock, std::chrono::duration<int, std::milli>(timeout));
            cpp_lock.release();
            if (wait_result == std::cv_status::timeout) {
                return MUTEX_TIMEDOUT;
            } else {
                return EOKED;
            }
        }
    } catch (std::system_error & ex) {
        syncerrno = EEXEC;
        loger->error("errno: %d unable to wait on a C++ condition variable: code=%d; %s", ex.code(), ex.what());
        utilException::SetError("errno: %d unable to wait on a C++ condition variable: code=%d; %s", ex.code(), ex.what());
        return syncerrno;
    }
#endif

#ifdef OS_MSWIN
#if _WIN32_WINNT >= 0x0600
    SleepConditionVariableCS(&cond->cv, &mutex->cs, timeout);
	return timeout;
#else
    int retval;
    DWORD dwMilliseconds;
    if (NULL == cond) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL Cond", syncerrno);
        utilException::SetError("errno: %d Passed a NULL Cond", syncerrno);
        return syncerrno;
    }
    if(NULL == mutex){
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL Cond", syncerrno);
        utilException::SetError("errno: %d Passed a NULL Cond", syncerrno);
        return syncerrno;
    }else{
        EnterCriticalSection(&mutex->cs);
    }
    if ((unsigned int)timeout == MUTEX_MAXWAIT) {
        dwMilliseconds = INFINITE;
    } else {
        dwMilliseconds = (DWORD) timeout;
    }
#if __WINRT__
    switch (WaitForSingleObjectEx(cond->id, dwMilliseconds, FALSE)) {
#else
    switch (WaitForSingleObject(cond->id, dwMilliseconds)) {
#endif
    case WAIT_OBJECT_0:
        retval = 0;
        break;
    case WAIT_TIMEOUT:
        retval = MUTEX_TIMEDOUT;
        break;
    default:
        syncerrno = EEXEC;
        loger->error("errno: %d WaitForSingleObject() failed", syncerrno);
        utilException::SetError("errno: %d WaitForSingleObject() failed", syncerrno);
        // return syncerrno;
        break;
    }
    LeaveCriticalSection(&mutex->cs);
    return retval;
#endif
#endif
}

int   Cond::_condWait(syncCond * cond, syncMutex * mutex)
{
#ifdef OS_LINUX
    if (NULL == cond) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL condition variable", syncerrno);
        utilException::SetError("errno: %d Passed a NULL condition variable", syncerrno);
        return syncerrno;
    } else if (pthread_cond_wait(&cond->cond, &mutex->id) != 0) {
        syncerrno = EEXEC;
        loger->error("errno: %d pthread_cond_wait() failed", syncerrno);
        utilException::SetError("errno: %d pthread_cond_wait() failed", syncerrno);
        return syncerrno;
    }
    return EOKED;
#endif

#ifdef STD_THREAD
    return _condWaitTimeout(cond, mutex, MUTEX_MAXWAIT);
#endif

#ifdef OS_MSWIN
    return _condWaitTimeout(cond, mutex, MUTEX_MAXWAIT);
#endif
}

/* Sem */

Sem::Sem() :baseSync()
{
    _sem = createSemaphore(0);
    loger->info("createSemaphore ...");
}

Sem::~Sem()
{
    if (_sem){
        destroySemaphore(_sem);
    }
}

syncSem* Sem::createSemaphore(int initial_value)
{
    syncSem* sem = NULL;
#ifdef OS_LINUX
    sem = (syncSem *) malloc(sizeof(syncSem));
    if (sem) {
        if (sem_init(&sem->sem, 0, initial_value) < 0) {
            syncerrno = EINIT;
            loger->error("errno: %d sem_init() failed", syncerrno);
            utilException::SetError("errno: %d sem_init() failed", syncerrno);
            free(sem);
            sem = NULL;
        }
    } else {
        syncerrno = EALLOC;
        loger->error("errno: %d Allocate the initialize semaphore failed", syncerrno);
        utilException::SetError("errno: %d Allocate the initialize semaphore failed", syncerrno);
    }
#endif

#ifdef STD_THREAD
    sem  = (syncSem *) malloc(sizeof(syncSem));
    sem  = new syncSem;
 /*
    try {
        sem->_mutex = std::mutex();
    } catch (std::system_error & ex) {
        syncerrno = EALLOC;
        loger->error("errno: %d unable to create a C++ mutex: code=%d; %s", ex.code(), ex.what());
        utilException::SetError("errno: %d unable to create a C++ mutex: code=%d; %s", ex.code(), ex.what());
        return NULL;
    } catch (std::bad_alloc &) {
        syncerrno = EALLOC;
        loger->error("errno: %d Allocate the initialize Mutex failed", syncerrno);
        utilException::SetError("errno: %d Allocate the initialize Mutex failed", syncerrno);
        return NULL;
    }
    try {
        sem->_condition = syncCond;
    } catch (std::system_error & ex) {
        syncerrno = EALLOC;
        loger->error("errno: %d unable to create a C++ condition variable: code=%d; %s", ex.code(), ex.what());
        utilException::SetError("errno: %d unable to create a C++ condition variable: code=%d; %s", ex.code(), ex.what());
        return NULL;
    } catch (std::bad_alloc &) {
        syncerrno = EALLOC;
        loger->error("errno: %d bad_alloc a Cond", syncerrno);
        utilException::SetError("errno: %d bad_alloc a Cond", syncerrno);
        return NULL;
    }
*/
    sem->_count = 0; // Initialized as locked.
#endif

#ifdef OS_MSWIN
    /* Allocate sem memory */
    sem = (syncSem*) malloc(sizeof(syncSem));
    if (sem) {
        /* Create the semaphore, with max value 32K */
#if __WINRT__
        sem->id = CreateSemaphoreEx(NULL, initial_value, 32 * 1024, NULL, 0, SEMAPHORE_ALL_ACCESS);
#else
        sem->id = CreateSemaphore(NULL, initial_value, 32 * 1024, NULL);
#endif
        sem->count = initial_value;
        if (NULL == sem->id) {
            syncerrno = EINIT;
            loger->error("errno: %d Couldn't create semaphore", syncerrno);
            utilException::SetError("errno: %d Couldn't create semaphore", syncerrno);
            free(sem);
            sem = NULL;
        }
    } else {
        syncerrno = EALLOC;
        loger->error("errno: %d Allocate the initialize semaphore failed", syncerrno);
        utilException::SetError("errno: %d Allocate the initialize semaphore failed", syncerrno);
    }
#endif
    return sem;
}

void  Sem::destroySemaphore(syncSem * sem)
{
#ifdef OS_LINUX
    if (sem) {
        sem_destroy(&sem->sem);
        free(sem);
    }
#endif

#ifdef STD_THREAD
    if (sem){
        delete sem;
    }
#endif

#ifdef OS_MSWIN
    if (sem) {
        if (sem->id) {
            CloseHandle(sem->id);
            sem->id = 0;
        }
        free(sem);
    }
#endif
}

int   Sem::_semTryWait(syncSem * sem)
{
#ifdef OS_LINUX
    int retval;

    if (NULL == sem) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL semaphore", syncerrno);
        utilException::SetError("errno: %d Passed a NULL semaphore", syncerrno);
        return syncerrno;
    }
    retval = MUTEX_TIMEDOUT;
    if (sem_trywait(&sem->sem) == 0) {
        syncerrno = EEXEC;
        retval = 0;
    }
    return retval;
#endif

#ifdef STD_THREAD
    std::unique_lock<decltype(sem->_mutex)> lock(sem->_mutex);
    if(sem->_count) {
        --(sem->_count);
        return EOKED;
    }
    return -1;
#endif

#ifdef OS_MSWIN
    return _semWaitTimeout(sem, 0);
#endif
}

int   Sem::_semWait(syncSem * sem)
{
#ifdef OS_LINUX
    int retval;

    if (NULL == sem) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL semaphore", syncerrno);
        utilException::SetError("errno: %d Passed a NULL semaphore", syncerrno);
        return syncerrno;
    }

    retval = sem_wait(&sem->sem);
    if (retval < 0) {
        syncerrno = EEXEC;
        loger->error("errno: %d sem_wait() failed", syncerrno);
        utilException::SetError("errno: %d sem_wait() failed", syncerrno);
        // return syncerrno;
    }
    return retval;
#endif

#ifdef STD_THREAD
    std::unique_lock<decltype(sem->_mutex)> lock(sem->_mutex);
    while(NULL == sem->_count) // Handle spurious wake-ups.
    {
        sem->_condition.wait(lock);
    }
    --(sem->_count);
    return EOKED;
#endif

#ifdef OS_MSWIN
    return _semWaitTimeout(sem, MUTEX_MAXWAIT);
#endif
}

int   Sem::_semWaitTimeout(syncSem * sem, int timeout)
{
    int retval = -1;
#ifdef OS_LINUX
#ifdef HAVE_SEM_TIMEDWAIT
#ifndef HAVE_CLOCK_GETTIME
    struct timeval now;
#endif
    struct timespec ts_timeout;
#else
    int end;
#endif

    if (NULL == sem)
    {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL semaphore", syncerrno);
        utilException::SetError("errno: %d Passed a NULL semaphore", syncerrno);
        return syncerrno;
    }

    /* Try the easy cases first */
    if (timeout == 0)
    {
        return _semTryWait(sem);
    }
    if (timeout == MUTEX_MAXWAIT)
    {
        return _semWait(sem);
    }

#ifdef HAVE_SEM_TIMEDWAIT
    /* Setup the timeout. sem_timedwait doesn't wait for
    * a lapse of time, but until we reach a certain time.
    * This time is now plus the timeout.
    */
#ifdef HAVE_CLOCK_GETTIME
    clock_gettime(CLOCK_REALTIME, &ts_timeout);

    /* Add our timeout to current time */
    ts_timeout.tv_nsec += (timeout % 1000) * 1000000;
    ts_timeout.tv_sec += timeout / 1000;
#else
    gettimeofday(&now, NULL);

    /* Add our timeout to current time */
    ts_timeout.tv_sec = now.tv_sec + (timeout / 1000);
    ts_timeout.tv_nsec = (now.tv_usec + (timeout % 1000) * 1000) * 1000;
#endif

    /* Wrap the second if needed */
    if (ts_timeout.tv_nsec > 1000000000)
    {
        ts_timeout.tv_sec += 1;
        ts_timeout.tv_nsec -= 1000000000;
    }

    /* Wait. */
    do
    {
        retval = sem_timedwait(&sem->sem, &ts_timeout);
    } while (retval < 0 && errno == EINTR);

    if (retval < 0)
    {
        if (errno == ETIMEDOUT)
        {
            retval = MUTEX_TIMEDOUT;
        }
        else
        {
            syncerrno = EEXEC;
            loger->error("errno: %d sem_timedwait returned an error: %s", strerror(errno));
            utilException::SetError("errno: %d sem_timedwait returned an error: %s", strerror(errno));
            return syncerrno;
        }
    }
#endif /* HAVE_SEM_TIMEDWAIT */
#endif

#ifdef STD_THREAD
    std::unique_lock<decltype(sem->_mutex)> lock(sem->_mutex);
    while( sem->_condition.wait_for(lock,std::chrono::seconds(timeout))
          == std::cv_status::timeout )
    --(sem->_count);
#endif

#ifdef OS_MSWIN
    DWORD dwMilliseconds;

    if (NULL == sem) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL sem", syncerrno);
        utilException::SetError("errno: %d Passed a NULL sem", syncerrno);
        return syncerrno;
    }

    if ((unsigned int)timeout == MUTEX_MAXWAIT) {
        dwMilliseconds = INFINITE;
    } else {
        dwMilliseconds = (DWORD) timeout;
    }
#if __WINRT__
    switch (WaitForSingleObjectEx(sem->id, dwMilliseconds, FALSE)) {
#else
    switch (WaitForSingleObject(sem->id, dwMilliseconds)) {
#endif
    case WAIT_OBJECT_0:
        InterlockedDecrement(&sem->count);
        retval = 0;
        break;
    case WAIT_TIMEOUT:
        retval = MUTEX_TIMEDOUT;
        break;
    default:
        syncerrno = EEXEC;
        loger->error("errno: %d WaitForSingleObject() failed", syncerrno);
        utilException::SetError("errno: %d WaitForSingleObject() failed", syncerrno);
        // return syncerrno;
        break;
    }
#endif
    return retval;
}

int   Sem::_semValue(syncSem * sem)
{
#ifdef OS_LINUX
    int ret = 0;
    if (sem) {
        sem_getvalue(&sem->sem, &ret);
        if (ret < 0) {
            ret = 0;
        }
    }
    return (int) ret;
#endif

#ifdef STD_THREAD
    return sem->_count;
#endif

#ifdef OS_MSWIN
    if (NULL == sem) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL sem", syncerrno);
        utilException::SetError("errno: %d Passed a NULL sem", syncerrno);
        return syncerrno;
    }
    return (int)sem->count;
#endif
}

int   Sem::_semPost(syncSem * sem)
{
#ifdef OS_LINUX
    int retval;

    if (NULL == sem) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL semaphore", syncerrno);
        utilException::SetError("errno: %d Passed a NULL semaphore", syncerrno);
        return syncerrno;
    }

    retval = sem_post(&sem->sem);
    if (retval < 0) {
        syncerrno = EEXEC;
        loger->error("errno: %d sem_post() failed", syncerrno);
        utilException::SetError("errno: %d sem_post() failed", syncerrno);
        // return syncerrno;
    }
    return retval;
#endif

#ifdef STD_THREAD
    std::unique_lock<decltype(sem->_mutex)> lock(sem->_mutex);
    ++(sem->_count);
    sem->_condition.notify_one();
    return EOKED;
#endif

#ifdef OS_MSWIN
    if (NULL == sem) {
        syncerrno = ENULL;
        loger->error("errno: %d Passed a NULL sem", syncerrno);
        utilException::SetError("errno: %d Passed a NULL sem", syncerrno);
        return syncerrno;
    }
    /* Increase the counter in the first place, because
     * after a successful release the semaphore may
     * immediately get destroyed by another thread which
     * is waiting for this semaphore.
     */
    InterlockedIncrement(&sem->count);
    if (ReleaseSemaphore(sem->id, 1, NULL) == FALSE) {
        InterlockedDecrement(&sem->count);      /* restore */
        syncerrno = EEXEC;
        loger->error("errno: %d ReleaseSemaphore() failed", syncerrno);
        utilException::SetError("errno: %d ReleaseSemaphore() failed", syncerrno);
        return syncerrno;
    }
    return EOKED;
#endif
}

