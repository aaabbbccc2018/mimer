#include <stdio.h>
#include "SyncTools.h"

SyncTools::SyncTools(TYPE type,int initv):
    _type(type),_mutex(NULL),_cond(NULL),_sem(NULL)
{
    switch (_type) {
    case MUTEX:
        _mutex = createMutex();
        break;
    case COND:
        _cond = createCond();
        break;
    case SEM:
        _sem = createSemaphore(initv);
        break;
    default:
        break;
    }
}

SyncTools::~SyncTools()
{
    switch (_type) {
    case MUTEX:
        destroyMutex(_mutex);
        _mutex = NULL;
        break;
    case COND:
        destroyCond(_cond);
        _cond = NULL;
        break;
    case SEM:
        destroySemaphore(_sem);
        _sem = NULL;
        break;
    default:
        break;
    }
}

/* Mutex */

Mutex* SyncTools::createMutex(void)
{
    Mutex *mutex;
#ifdef OS_LINUX
    pthread_mutexattr_t attr;
    /* Allocate the structure */
    mutex = (Mutex *)calloc(1, sizeof(*mutex));
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
            utilException::SetError("pthread_mutex_init() failed");
            free(mutex);
            mutex = NULL;
        }
    } else {
        utilException::SetError("Allocate the initialize Mutex failed");
    }
#endif

#ifdef STD_THREAD
    /* Allocate and initialize the mutex */
    try {
        mutex = new Mutex;
    } catch (std::system_error & ex) {
        utilException::SetError("unable to create a C++ mutex: code=%d; %s", ex.code(), ex.what());
        return NULL;
    } catch (std::bad_alloc &) {
        utilException::SetError("Allocate the initialize Mutex failed");
        return NULL;
    }
#endif

#ifdef OS_MSWIN
    /* Allocate mutex memory */
    mutex = (Mutex *) calloc(1, sizeof(*mutex));
    if (mutex) {
        /* Initialize */
        /* On SMP systems, a non-zero spin count generally helps performance */
#if __WINRT__
        InitializeCriticalSectionEx(&mutex->cs, 2000, 0);
#else
        InitializeCriticalSectionAndSpinCount(&mutex->cs, 2000);
#endif
    } else {
        utilException::SetError("Allocate the initialize Mutex failed");
    }
#endif
    return (mutex);
}

void   SyncTools::destroyMutex(Mutex * mutex)
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

int    SyncTools::_lockMutex(Mutex * mutex)
{
#ifdef OS_LINUX

#ifdef FAKE_RECURSIVE_MUTEX
    pthread_t this_thread;
#endif
    if (mutex == NULL) {
        return utilException::SetError("Passed a NULL mutex");
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
            return utilException::SetError("pthread_mutex_lock() failed");
        }
    }
#else
    if (pthread_mutex_lock(&mutex->id) < 0) {
        return utilException::SetError("pthread_mutex_lock() failed");
    }
#endif
    return 0;
#endif

#ifdef STD_THREAD
    if (mutex == NULL) {
        return utilException::SetError("Passed a NULL mutex");
    }

    try {
        mutex->cpp_mutex.lock();
        return 0;
    } catch (std::system_error & ex) {
        return utilException::SetError("unable to lock a C++ mutex: code=%d; %s", ex.code(), ex.what());
    }
#endif

#ifdef OS_MSWIN
    if (mutex == NULL) {
        return utilException::SetError("Passed a NULL mutex");
    }
    EnterCriticalSection(&mutex->cs);
    return (0);
#endif
}

int    SyncTools::_tryLockMutex(Mutex * mutex)
{
#ifdef OS_LINUX
    int retval;
#ifdef FAKE_RECURSIVE_MUTEX
    pthread_t this_thread;
#endif

    if (mutex == NULL) {
        return utilException::SetError("Passed a NULL mutex");
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
            retval = utilException::SetError("pthread_mutex_trylock() failed");
        }
    }
#else
    if (pthread_mutex_trylock(&mutex->id) != 0) {
        if (errno == EBUSY) {
            retval = MUTEX_TIMEDOUT;
        } else {
            retval = utilException::SetError("pthread_mutex_trylock() failed");
        }
    }
#endif
    return retval;
#endif

#ifdef STD_THREAD
    int retval = 0;
    if (mutex == NULL) {
        return utilException::SetError("Passed a NULL mutex");
    }

    if (mutex->cpp_mutex.try_lock() == false) {
        retval = MUTEX_TIMEDOUT;
    }
    return retval;
#endif

#ifdef OS_MSWIN
    int retval = 0;
    if (mutex == NULL) {
        return utilException::SetError("Passed a NULL mutex");
    }

    if (TryEnterCriticalSection(&mutex->cs) == 0) {
        retval = MUTEX_TIMEDOUT;
    }
    return retval;
#endif
}

int    SyncTools::_unlockMutex(Mutex * mutex)
{
#ifdef OS_LINUX
    if (mutex == NULL) {
        return utilException::SetError("Passed a NULL mutex");
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
        return utilException::SetError("mutex not owned by this thread");
    }

#else
    if (pthread_mutex_unlock(&mutex->id) < 0) {
        return utilException::SetError("pthread_mutex_unlock() failed");
    }
#endif /* FAKE_RECURSIVE_MUTEX */

    return 0;
#endif

#ifdef STD_THREAD
    if (mutex == NULL) {
        return utilException::SetError("Passed a NULL mutex");
    }

    mutex->cpp_mutex.unlock();
    return 0;
#endif

#ifdef OS_MSWIN
    if (mutex == NULL) {
        return utilException::SetError("Passed a NULL mutex");
    }

    LeaveCriticalSection(&mutex->cs);
    return (0);
#endif
}

/* Cond */
Cond* SyncTools::createCond(void)
{
#ifdef OS_LINUX
    Cond *cond = (Cond *) malloc(sizeof(Cond));
    if (cond) {
        if (pthread_cond_init(&cond->cond, NULL) < 0) {
            utilException::SetError("pthread_cond_init() failed");
            free(cond);
            cond = NULL;
        }
    }
    return (cond);
#endif

#ifdef STD_THREAD
    /* Allocate and initialize the condition variable */
    try {
        Cond * cond = new Cond;
        return cond;
    } catch (std::system_error & ex) {
        utilException::SetError("unable to create a C++ condition variable: code=%d; %s", ex.code(), ex.what());
        return NULL;
    } catch (std::bad_alloc &) {
        utilException::SetError("bad_alloc a Cond");
        return NULL;
    }
#endif

#ifdef OS_MSWIN
    utilException::SetError("Not support Cond");
    return NULL;
#endif
}

void  SyncTools::destroyCond(Cond * cond)
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
    utilException::SetError("Not support Cond");
    return;
#endif
}

int   SyncTools::_condSignal(Cond * cond)
{
#ifdef OS_LINUX
    int retval;

    if (!cond) {
        return utilException::SetError("Passed a NULL condition variable");
    }

    retval = 0;
    if (pthread_cond_signal(&cond->cond) != 0) {
        return utilException::SetError("pthread_cond_signal() failed");
    }
    return retval;
#endif

#ifdef STD_THREAD
    if (!cond) {
        return utilException::SetError("Passed a NULL condition variable");
    }

    cond->cpp_cond.notify_one();
    return 0;
#endif

#ifdef OS_MSWIN
    return utilException::SetError("Not support Cond");
#endif
}

int   SyncTools::_condBroadcast(Cond * cond)
{
#ifdef OS_LINUX
    int retval;

    if (!cond) {
        return utilException::SetError("Passed a NULL condition variable");
    }

    retval = 0;
    if (pthread_cond_broadcast(&cond->cond) != 0) {
        return utilException::SetError("pthread_cond_broadcast() failed");
    }
    return retval;
#endif

#ifdef STD_THREAD
    if (!cond) {
        return utilException::SetError("Passed a NULL condition variable");
    }

    cond->cpp_cond.notify_all();
    return 0;
#endif

#ifdef OS_MSWIN
    return utilException::SetError("Not support Cond");
#endif
}

int   SyncTools::_condWaitTimeout(Cond * cond, Mutex * mutex, int ms)
{
#ifdef OS_LINUX
    int retval;
#ifndef HAVE_CLOCK_GETTIME
    struct timeval delta;
#endif
    struct timespec abstime;

    if (!cond) {
        return utilException::SetError("Passed a NULL condition variable");
    }

#ifdef HAVE_CLOCK_GETTIME
    clock_gettime(CLOCK_REALTIME, &abstime);

    abstime.tv_nsec += (ms % 1000) * 1000000;
    abstime.tv_sec += ms / 1000;
#else
    gettimeofday(&delta, NULL);

    abstime.tv_sec = delta.tv_sec + (ms / 1000);
    abstime.tv_nsec = (delta.tv_usec + (ms % 1000) * 1000) * 1000;
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
        retval = utilException::SetError("pthread_cond_timedwait() failed");
    }
    return retval;
#endif

#ifdef STD_THREAD
    if (!cond) {
        return utilException::SetError("Passed a NULL condition variable");
    }

    if (!mutex) {
        return utilException::SetError("Passed a NULL mutex variable");
    }

    try {
        std::unique_lock<std::recursive_mutex> cpp_lock(mutex->cpp_mutex, std::adopt_lock_t());
        if (ms == MUTEX_MAXWAIT) {
            cond->cpp_cond.wait(
                        cpp_lock
                        );
            cpp_lock.release();
            return 0;
        } else {
            auto wait_result = cond->cpp_cond.wait_for(
                        cpp_lock,
                        std::chrono::duration<int, std::milli>(ms)
                        );
            cpp_lock.release();
            if (wait_result == std::cv_status::timeout) {
                return MUTEX_TIMEDOUT;
            } else {
                return 0;
            }
        }
    } catch (std::system_error & ex) {
        return utilException::SetError("unable to wait on a C++ condition variable: code=%d; %s", ex.code(), ex.what());
    }
#endif

#ifdef OS_MSWIN
    return utilException::SetError("Not support Cond");
#endif
}

int   SyncTools::_condWait(Cond * cond, Mutex * mutex)
{
#ifdef OS_LINUX
    if (!cond) {
        return utilException::SetError("Passed a NULL condition variable");
    } else if (pthread_cond_wait(&cond->cond, &mutex->id) != 0) {
        return utilException::SetError("pthread_cond_wait() failed");
    }
    return 0;
#endif

#ifdef STD_THREAD
    return condWaitTimeout(cond, mutex, MUTEX_MAXWAIT);
#endif

#ifdef OS_MSWIN
    return utilException::SetError("Not support Cond");
#endif
}

/* Sem */
Sem* SyncTools::createSemaphore(int initial_value)
{
#ifdef OS_LINUX
    Sem *sem = (Sem *) malloc(sizeof(Sem));
    if (sem) {
        if (sem_init(&sem->sem, 0, initial_value) < 0) {
            utilException::SetError("sem_init() failed");
            free(sem);
            sem = NULL;
        }
    } else {
        utilException::SetError("Allocate the initialize semaphore failed");
    }
    return sem;
#endif

#ifdef STD_THREAD
    utilException::SetError("Not support Cond");
    return NULL;
#endif

#ifdef OS_MSWIN
    Sem *sem;
    /* Allocate sem memory */
    sem = (Sem*) malloc(sizeof(Sem));
    if (sem) {
        /* Create the semaphore, with max value 32K */
#if __WINRT__
        sem->id = CreateSemaphoreEx(NULL, initial_value, 32 * 1024, NULL, 0, SEMAPHORE_ALL_ACCESS);
#else
        sem->id = CreateSemaphore(NULL, initial_value, 32 * 1024, NULL);
#endif
        sem->count = initial_value;
        if (!sem->id) {
            utilException::SetError("Couldn't create semaphore");
            free(sem);
            sem = NULL;
        }
    } else {
        utilException::SetError("Allocate the initialize semaphore failed");
    }
    return (sem);
#endif

}

void  SyncTools::destroySemaphore(Sem * sem)
{
#ifdef OS_LINUX
    if (sem) {
        sem_destroy(&sem->sem);
        free(sem);
    }
#endif

#ifdef STD_THREAD
    return;
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

int   SyncTools::_semTryWait(Sem * sem)
{
#ifdef OS_LINUX
    int retval;

    if (!sem) {
        return utilException::SetError("Passed a NULL semaphore");
    }
    retval = MUTEX_TIMEDOUT;
    if (sem_trywait(&sem->sem) == 0) {
        retval = 0;
    }
    return retval;
#endif

#ifdef STD_THREAD
    return utilException::SetError("Not support semaphore");
#endif

#ifdef OS_MSWIN
    return _semWaitTimeout(sem, 0);
#endif
}

int   SyncTools::_semWait(Sem * sem)
{
#ifdef OS_LINUX
    int retval;

    if (!sem) {
        return utilException::SetError("Passed a NULL semaphore");
    }

    retval = sem_wait(&sem->sem);
    if (retval < 0) {
        retval = utilException::SetError("sem_wait() failed");
    }
    return retval;
#endif

#ifdef STD_THREAD
    return utilException::SetError("Not support semaphore");
#endif

#ifdef OS_MSWIN
    return _semWaitTimeout(sem, MUTEX_MAXWAIT);
#endif
}

int   SyncTools::_semWaitTimeout(Sem * sem, int timeout)
{
#ifdef OS_LINUX
    int retval;
#ifdef HAVE_SEM_TIMEDWAIT
#ifndef HAVE_CLOCK_GETTIME
    struct timeval now;
#endif
    struct timespec ts_timeout;
#else
    int end;
#endif

    if (!sem)
    {
        return utilException::SetError("Passed a NULL semaphore");
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
            utilException::SetError("sem_timedwait returned an error: %s", strerror(errno));
        }
    }
#endif /* HAVE_SEM_TIMEDWAIT */
#endif

#ifdef STD_THREAD
    return utilException::SetError("Not support semaphore");
#endif

#ifdef OS_MSWIN
    int retval;
    DWORD dwMilliseconds;

    if (!sem) {
        return utilException::SetError("Passed a NULL sem");
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
        retval = utilException::SetError("WaitForSingleObject() failed");
        break;
    }
    return retval;
#endif
}

int   SyncTools::_semValue(Sem * sem)
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
    return utilException::SetError("Not support semaphore");
#endif

#ifdef OS_MSWIN
    if (!sem) {
        utilException::SetError("Passed a NULL sem");
        return 0;
    }
    return (int)sem->count;
#endif
}

int   SyncTools::_semPost(Sem * sem)
{
#ifdef OS_LINUX
    int retval;

    if (!sem) {
        return utilException::SetError("Passed a NULL semaphore");
    }

    retval = sem_post(&sem->sem);
    if (retval < 0) {
        utilException::SetError("sem_post() failed");
    }
    return retval;
#endif

#ifdef STD_THREAD
    return utilException::SetError("Not support semaphore");
#endif

#ifdef OS_MSWIN
    if (!sem) {
        return utilException::SetError("Passed a NULL sem");
    }
    /* Increase the counter in the first place, because
     * after a successful release the semaphore may
     * immediately get destroyed by another thread which
     * is waiting for this semaphore.
     */
    InterlockedIncrement(&sem->count);
    if (ReleaseSemaphore(sem->id, 1, NULL) == FALSE) {
        InterlockedDecrement(&sem->count);      /* restore */
        return utilException::SetError("ReleaseSemaphore() failed");
    }
    return 0;
#endif
}
