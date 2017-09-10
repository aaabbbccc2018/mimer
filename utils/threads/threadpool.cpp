#include "threadpool.h"

// the constructor just launches some amount of workers
threadpool::threadpool(size_t thread_count, int task_size):
    _stop(false),_thsSize(thread_count),_tskSize(task_size)
{
    _tpMutex = new Mutex();
    _tpConds = new  Cond();
    for(size_t i = 0; i < thread_count; i++) {
        threads* worker = new threads("thread");
        if(worker){
            utilException::SetError("Allocate a initialize threads failed");
        }else{
            worker->ThreadCreate(run,(void*)this);
            _workers.push_back(worker);
        }
    }
}

int threadpool::addpool(ThreadFunction tskfun, void* args)
{
    int err = 0;
    if(!tskfun || !args){
        err = -1;
    }
    _tpMutex->lockMutex();
    if (_tskSize == (int)_taskers.size())
    {
        err = 1;
    }else{
        tasker tsk;
        tsk._handler = tskfun;
        tsk._vargument = args;
        _taskers.push(tsk);
        _tpConds->condSignal();
        err = 0;
    }
    _tpMutex->unlockMutex();
    return err;
}


int threadpool::addpool(tasker* tsk)
{
    if(!tsk){
        return -1;
    }
    _tpMutex->lockMutex();
    _taskers.push(*tsk);
    _tpConds->condSignal();
    _tpMutex->unlockMutex();
    return 0;
}

#ifdef STD_THREAD
#undef OS_MSWIN
#undef OS_LINUX
void* threadpool::run(void* pool)
#endif
#ifdef OS_LINUX
void* threadpool::run(void* pool)
#endif
#ifdef OS_MSWIN
unsigned int __stdcall threadpool::run(void* pool)
#endif
{
    threadpool* tp = (threadpool*)pool;
    tasker task;
    task._handler = NULL;
    for(;;) {
        /* Lock must be taken to wait on conditional variable */
        tp->_tpMutex->lockMutex();
        while(tp->_taskers.empty()) {
            tp->_tpConds->condWait();
        }
        /* Grab our task */
        task = tp->_taskers.front();
        tp->_taskers.pop();
        tp->_taskers.push(task);
        /* Unlock */
        tp->_tpMutex->unlockMutex();
        /* Get to work */
        if(task._handler){
            ((task._handler))(task._vargument);
        }
    }
    tp->_tpMutex->unlockMutex();
    return(NULL);
}

// the destructor joins all threads
threadpool::~threadpool()
{
    _tpMutex->lockMutex();
    _stop = true;
    _tpConds->condBroadcast();
    size_t size = _workers.size();
    for(size_t i = 0;i < size;++i)
    {
        _workers[i]->waitThread();
    }
    _tpMutex->unlockMutex();
    delete _tpMutex;
    delete _tpConds;
}
