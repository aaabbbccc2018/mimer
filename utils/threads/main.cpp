#include <iostream>
#include <stdio.h>
#include "../../platform.h"
#include "threads.h"
#include "threadpool.h"

using namespace std;
int sum = 0;
Mutex mu;

#ifdef STD_THREAD
#undef OS_MSWIN
#undef OS_LINUX
void*  ThreadAdd(void* args)
#endif
#ifdef OS_LINUX
void*  ThreadAdd(void* args)
#endif
#ifdef OS_MSWIN
unsigned  __stdcall ThreadAdd(void* args)
#endif
{
    mu.lockMutex();
    int param = (long)args;
    for (int i = 0;i < param; i++){
        sum += i;
	}
    printf("after add thread sum: %d\n",sum);
#ifdef OS_MSWIN
    Sleep(1000);
#else
    sleep(1);
#endif
    mu.unlockMutex();
    return 0;
}

#ifdef STD_THREAD
#undef OS_MSWIN
#undef OS_LINUX
void*  ThreadDel(void* args)
#endif
#ifdef OS_LINUX
void*  ThreadDel(void* args)
#endif
#ifdef OS_MSWIN
unsigned  __stdcall ThreadDel(void* args)
#endif
{
    mu.lockMutex();
    int param = (long)args;
    for (int i =0;i < param; i++){
        sum -= i;
    }
    printf("after del thread sum: %d\n",sum);
    mu.unlockMutex();
    return 0;
}

#ifdef STD_THREAD
#undef OS_MSWIN
#undef OS_LINUX
void*  work(void* args)
#endif
#ifdef OS_LINUX
void*  work(void* args)
#endif
#ifdef OS_MSWIN
unsigned  __stdcall work(void* args)
#endif
{
    char *p = (char*) args;
    printf("threadpool callback fuction : %s.\n", p);
    return 0;
}

#define NUM 100
int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;
    threads th[2];
    th[0].ThreadCreate(ThreadAdd,(void*)1000);
    th[0].waitThread();
    th[1].ThreadCreate(ThreadDel,(void*)1000);
    th[1].waitThread();
    cout << "End of main!" << endl;
/*
    threadpool tp(5,6);
    tp.addpool(work, (void*)"1");
    tp.addpool(work, (void*)"2");
    tp.addpool(work, (void*)"3");
    tp.addpool(work, (void*)"4");
    tp.addpool(work, (void*)"5");
    tp.addpool(work, (void*)"6");
    tp.run((void*)(&tp));
*/
    return 0;
}
