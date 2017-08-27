#ifndef LOADSO_H
#define LOADSO_H
#include <stdio.h>
#include "../../platform.h"
#ifdef OS_LINUX
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <dlfcn.h>
#endif

#ifdef OS_MSWIN
    #include <windows.h>
#endif

class Loadso
{
public:
    /**
     *  This function dynamically loads a shared object and returns a pointer
     *  to the object handle (or NULL if there was an error).
     *  The 'sofile' parameter is a system dependent name of the object file.
     */
    static UTIL_API void * LoadObject(const char *sofile);

    /**
     *  Given an object handle, this function looks up the address of the
     *  named function in the shared object and returns it.  This address
     *  is no longer valid after calling UnloadObject().
     */
    static UTIL_API void * LoadFunction(void *handle, const char *name);

    /**
     *  Unload a shared object from memory.
     */
    static UTIL_API void  UnloadObject(void *handle);
};

#endif // LOADSO_H
