#include <iostream>
#include <stdio.h>
#include "../../platform.h"
#include "threads.h"

using namespace std;

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
#define NUM 1000
int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;
    threads th[NUM];
		int i = 0;
		while(i < NUM){
			th[i].ThreadCreate(ThreadFunc,(void*)"hello");
			th[i].run();
			i++;
			usleep(100);
		}
    return 0;
}
