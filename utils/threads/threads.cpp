#include <string.h>
#include "threads.h"

threads::threads():
    _handle(0),_threadid(0),_status(0),
    _name(NULL),_stacksize(0),_args(NULL)
{
    _sync = new SyncTools(MUTEX);
}

threads::threads(char* name):
    _handle(0),_threadid(0),_status(0),
    _name(NULL),_stacksize(0),_args(NULL)
{    
    _name = (char*)malloc(strlen(name));
    strcpy(_name, name);
    _sync = new SyncTools(MUTEX);
}

threads::~threads()
{
    if(_name){
        free(_name);
    }
    if(_sync){
        delete (_sync);
    }
}


UTIL_API void UTIL_CALL threads::run()
{
    _sync->lockMutex();
    this->_fn(_args);
    _sync->unlockMutex();
}

UTIL_API void UTIL_CALL threads::setThreadFunction(ThreadFunction fn)
{
    _fn = fn;
}

UTIL_API void UTIL_CALL threads::setThreadname(const char* name)
{
    if(_name){
        free(_name);
        _name = (char*)malloc(strlen(name));
        strcpy(_name, name);
    }else{
        _name = (char*)malloc(strlen(name));
        strcpy(_name, name);
    }
}

UTIL_API int  UTIL_CALL threads::ThreadCreate(ThreadFunction RunThread,void *args)
{
    _fn = RunThread;
    _args = args;
#ifdef OS_LINUX
    pthread_attr_t type;
    /* do this here before any threads exist, so there's no race condition. */
#if defined(__MACOSX__) || defined(__IPHONEOS__) || defined(__LINUX__)
    if (!checked_setname) {
        void *fn = dlsym(RTLD_DEFAULT, "pthread_setname_np");
#if defined(__MACOSX__) || defined(__IPHONEOS__)
        ppthread_setname_np = (int(*)(const char*)) fn;
#elif defined(__LINUX__)
        ppthread_setname_np = (int(*)(pthread_t, const char*)) fn;
#endif
        checked_setname = TRUE;
    }
#endif
    /* Set the thread attributes */
    if (pthread_attr_init(&type) != 0) {
        return SetError("Couldn't initialize pthread attributes");
    }
    pthread_attr_setdetachstate(&type, PTHREAD_CREATE_JOINABLE);
    /* Set caller-requested stack size. Otherwise: use the system default. */
    if (_stacksize) {
        pthread_attr_setstacksize(&type, (size_t) _stacksize);
    }
    /* Create the thread and go! */
    if (pthread_create(&_handle, &type, _fn, _args) != 0) {
        return SetError("Not enough resources to create thread");
    }
    return 0;
#endif

#ifdef STD_THREAD
    try {
        // !!! FIXME: no way to set a thread stack size here.
        std::thread cpp_thread(_fn, _args);
        _handle = (void *) new std::thread(std::move(cpp_thread));
        return 0;
    } catch (std::system_error & ex) {
        SetError("unable to start a C++ thread: code=%d; %s", ex.code(), ex.what());
        return -1;
    } catch (std::bad_alloc &) {
        OutOfMemory();
        return -1;
    }
#endif

#ifdef OS_MSWIN
    _handle = (SYS_ThreadHandle)_beginthreadex(
                NULL,
                (unsigned int)_stacksize,
                _fn,
                _args,
                NULL,
                &_threadid);
    return 0;
#endif
}

UTIL_API const char *UTIL_CALL threads::getThreadName()
{
    if (_name) {
        return _name;
    } else {
        return NULL;
    }
}

UTIL_API threadID UTIL_CALL threads::getThreadID(void)
{
#ifdef OS_LINUX
    return ((threadID) pthread_self());
#endif

#ifdef STD_THREAD
#ifdef __WINRT__
    return GetCurrentThreadId();
#else
    // HACK: Mimick a thread ID, if one isn't otherwise available.
    static thread_local threadID current_thread_id = 0;
    static threadID next_thread_id = 1;
    static std::mutex next_thread_id_mutex;
    if (current_thread_id == 0) {
        std::lock_guard<std::mutex> lock(next_thread_id_mutex);
        current_thread_id = next_thread_id;
        ++next_thread_id;
    }
    return current_thread_id;
#endif
#endif

#ifdef OS_MSWIN
    return ((threadID) GetCurrentThreadId());
#endif
}

UTIL_API int UTIL_CALL threads::setPRI(THREAD_PRI priority)
{
#ifdef OS_LINUX
    struct sched_param sched;
    int policy;
    pthread_t thread = pthread_self();

    if (pthread_getschedparam(thread, &policy, &sched) < 0) {        
        return SetError("pthread_getschedparam() failed");
    }
    if (priority == THREAD_PRI_LOW) {
        sched.sched_priority = sched_get_priority_min(policy);
    } else if (priority == THREAD_PRI_HIGH) {
        sched.sched_priority = sched_get_priority_max(policy);
    } else {
        int min_priority = sched_get_priority_min(policy);
        int max_priority = sched_get_priority_max(policy);
        sched.sched_priority = (min_priority + (max_priority - min_priority) / 2);
    }
    if (pthread_setschedparam(thread, policy, &sched) < 0) {        
        return SetError("pthread_setschedparam() failed");
    }
    return 0;
#endif

#ifdef STD_THREAD
    return (0);
#endif

#ifdef OS_MSWIN
    int value;
    if (priority == THREAD_PRI_LOW) {
        value = THREAD_PRIORITY_LOWEST;
    } else if (priority == THREAD_PRI_HIGH) {
        value = THREAD_PRIORITY_HIGHEST;
    } else {
        value = THREAD_PRIORITY_NORMAL;
    }
    if (!SetThreadPriority(GetCurrentThread(), value)) {
        //return SetError("SetThreadPriority()");
    }
    return 0;
#endif
}

UTIL_API void UTIL_CALL threads::waitThread(int *status)
{
#ifdef OS_LINUX
    pthread_join(_handle, 0);
#endif

#ifdef STD_THREAD
    if ( ! thread) {
        return;
    }
    try {
        std::thread * cpp_thread = (std::thread *) _handle;
        if (cpp__joinable()) {
            cpp__join();
        }
    } catch (std::system_error &) {
        // An error occurred when joining the thread.  THREAD_PRI_HIGHWaitThread does not,
        // however, seem to provide a means to report errors to its callers
        // though!
    }
#endif

#ifdef OS_MSWIN
    WaitForSingleObjectEx(_handle, INFINITE, FALSE);
    CloseHandle(_handle);
#endif
    if (status) {
        *status = _status;
    }
}

UTIL_API void UTIL_CALL threads::detachThread()
{

#ifdef OS_LINUX
    pthread_detach(_handle);
#endif

#ifdef STD_THREAD
    if ( ! thread) {
        return;
    }

    try {
        std::thread * cpp_thread = (std::thread *) _handle;
        if (cpp__joinable()) {
            cpp__detach();
        }
    } catch (std::system_error &) {
        // An error occurred when detaching the thread.  THREAD_PRI_HIGHDetachThread does not,
        // however, seem to provide a means to report errors to its callers
        // though!
    }
#endif

#ifdef OS_MSWIN
    CloseHandle(_handle);
#endif
}
