#include <iostream>
#include <stdio.h>
#include "../../platform.h"
#include "threads.h"
#include "threadpool.h"

using namespace std;

#ifdef STD_THREAD
#undef OS_MSWIN
#undef OS_LINUX
void*  ThreadFunc(void* args)
#endif
#ifdef OS_LINUX
void*  ThreadFunc(void* args)
#endif
#ifdef OS_MSWIN
unsigned  __stdcall ThreadFunc(void* args)
#endif
{
	char* param = (char*)args;
	for (int i =0;i < 4; i++){
		switch(i){
			case 0:
				printf("%s %s\n","00",param);
				break;
			case 1:
				printf("%s %s\n","111",param);
				break;
			case 2:
				printf("%s %s\n","2222",param);
				break;
			case 3:
				printf("%s %s\n","33333",param);
				break;
		}
	}
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
    threads th[NUM];
        int i = 0;
        while(i < NUM){
            th[i].ThreadCreate(ThreadFunc,(void*)"hello");
            th[i].run();
            i++;
        }

    threadpool tp(5,6);
    tp.addpool(work, (void*)"1");
    tp.addpool(work, (void*)"2");
    tp.addpool(work, (void*)"3");
    tp.addpool(work, (void*)"4");
    tp.addpool(work, (void*)"5");
    tp.addpool(work, (void*)"6");
    tp.run((void*)(&tp));
    return 0;
}
