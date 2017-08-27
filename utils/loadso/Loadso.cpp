#include "Loadso.h"

void* Loadso::LoadObject(const char *sofile)
{
#ifdef OS_LINUX
    void *handle;
    const char *loaderror;
    handle = dlopen(sofile, RTLD_NOW|RTLD_LOCAL);
    loaderror = (char *) dlerror();
    if (handle == NULL) {
        fprintf(stderr, "Failed loading %s: %s", sofile, loaderror);
    }
#endif

#ifdef OS_MSWIN

#define XMLDATA_LEN 256
    //LPTSTR tstr = ((WCHAR*)(sofile));
    TCHAR tstr[XMLDATA_LEN]={0};

#if defined _UNICODE || defined UNICODE
    #define _stprintf   _swprintf
    #define _tcsstr     wcsstr
    #define _T(x) L##x
    MultiByteToWideChar(
                CP_ACP,          // code page
                0,               // character-type options
                sofile,          // address of string to map
                strlen(sofile),  // number of bytes in string
                tstr,            // address of wide-character buffer
                XMLDATA_LEN);            // size of buffer);
#else
    #define	_T(x) x
    _stprintf(tstr,"%s",sofile);
#endif
    if(_tcsstr(tstr,_T("%") )!= NULL )
    {
        TCHAR szTemp[XMLDATA_LEN]={0};
        ExpandEnvironmentStrings(tstr, szTemp, XMLDATA_LEN);
        memset(tstr,0,sizeof(tstr));
        _stprintf(tstr,_T("%s"),szTemp);
    }
#ifdef __WINRT__
    /* WinRT only publically supports LoadPackagedLibrary() for loading .dll
       files.  LoadLibrary() is a private API, and not available for apps
       (that can be published to MS' Windows Store.)
    */
    void *handle = (void *) LoadPackagedLibrary(tstr, 0);
#else
    void *handle = (void *) LoadLibrary(tstr);
#endif
    /* Generate an error message if all loads failed */
    if (handle == NULL) {
        fprintf(stderr,"Failed loading %s",sofile);
    }
#endif
    return (handle);
}

void* Loadso::LoadFunction(void *handle, const char *name)
{
#ifdef OS_LINUX
    void *symbol = dlsym(handle, name);
    if (symbol == NULL) {
        /* append an underscore for platforms that need that. */
        size_t len = 1 + strlen(name) + 1;
        char *_name = (char*)calloc(sizeof(char), len);
        _name[0] = '_';
        strcpy(&_name[1], name);
        symbol = dlsym(handle, _name);
        free(_name);
        if (symbol == NULL) {
            fprintf(stderr, "Failed loading %s: %s", name,(const char *)dlerror());
        }
    }
#endif

#ifdef OS_MSWIN
    void *symbol = (void *) GetProcAddress((HMODULE) handle, name);
    if (symbol == NULL) {
        fprintf(stderr,"Failed loading %s",name);
    }
#endif
    return (symbol);
}

void Loadso::UnloadObject(void *handle)
{
#ifdef OS_LINUX
    if (handle != NULL) {
        dlclose(handle);
    }
#endif

#ifdef OS_MSWIN
    if (handle != NULL) {
        FreeLibrary((HMODULE) handle);
    }
#endif
}
