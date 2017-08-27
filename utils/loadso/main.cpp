#include <iostream>
#include "../../platform.h"
#include "Loadso.h"

using namespace std;

int main(int argc, char *argv[])
{
#ifdef OS_MSWIN
    HMODULE hm = (HMODULE)Loadso::LoadObject("Kernel32.dll");
    if(hm){
        FARPROC fpFun = (FARPROC)Loadso::LoadFunction((void*)hm,"GetVersion");
        DWORD dwVersion = (*fpFun)();
        DWORD dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
        DWORD dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
        const int nBufSize = 512;
        char chBuf[nBufSize] = {0};
        sprintf(chBuf,("verson = %d.%d\n"),(int)dwWindowsMajorVersion,(int)dwWindowsMinorVersion);
        printf("%s",chBuf);
    }
    Loadso::UnloadObject(hm);
#endif

#ifdef OS_LINUX
		typedef double (*pFun)(double);
    void* hm = Loadso::LoadObject("libm.so");
    if(hm){
        pFun cosine = (pFun)Loadso::LoadFunction(hm,"cos");
        printf ("cos(2.0) = %f\n", (*cosine)(2.0));
    }
    Loadso::UnloadObject(hm);
#endif
    cout << "Hello World!" << endl;
}
