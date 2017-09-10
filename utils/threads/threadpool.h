#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include "threads.h"
#include "SyncTools.h"

typedef struct _tasker{
    ThreadFunction _handler;
    void* _vargument;
}tasker;

class threadpool
{
public:
    threadpool(size_t size, int task_size);
    ~threadpool();
    int addpool(tasker* tsk);
    int addpool(ThreadFunction tskfun, void* args);

#ifdef STD_THREAD
#undef OS_MSWIN
#undef OS_LINUX
    static void* run(void* pool);
#endif
#ifdef OS_LINUX
    static void* run(void* pool);
#endif
#ifdef OS_MSWIN
    static unsigned int __stdcall run(void* pool);
#endif
private:
    std::vector<threads*> _workers;
    // the task queue
    std::queue<tasker> _taskers;
private:
    // synchronization
    Mutex* _tpMutex;
    Cond*  _tpConds;
    bool   _stop;
    int    _thsSize;    // max thread size
    int    _tskSize;    // max task queue
};

#endif // THREADPOOL_H
