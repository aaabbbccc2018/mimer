#include <iostream>
#include <stdio.h>
#include "../../platform.h"
#include "threads.h"

using namespace std;

#ifdef OS_LINUX
void*  ThreadFunc(void* args)
#endif
#ifdef OS_MSWIN
unsigned  ThreadFunc(void* args)
#endif
{
    char* param = (char*)args;
    printf("%s\n",param);
}

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;
    threads th;
    th.ThreadCreate(ThreadFunc,(void*)"hello");
    th.run();
    return 0;
}
