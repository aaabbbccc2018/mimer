#ifndef LOCKTYPE_H
#define LOCKTYPE_H
#include "../../platform.h"

#ifdef STD_THREAD
#undef OS_MSWIN
#undef OS_LINUX
    #include <mutex>
    #include <condition_variable>
    typedef void * SYS_ThreadHandle;
    struct Mutex
    {
        std::recursive_mutex cpp_mutex;
    };
    struct Cond
    {
        std::condition_variable_any cpp_cond;
    };
    struct Sem
    {
    };
#endif

#ifdef OS_LINUX
    #include <stdlib.h>
    #include <errno.h>
    #include <pthread.h>
    #include <sys/time.h>
    #include <unistd.h>
    #include <semaphore.h>
    typedef pthread_t SYS_ThreadHandle;
    struct Mutex
    {
        pthread_mutex_t id;
    #ifdef FAKE_RECURSIVE_MUTEX
        int recursive;
        pthread_t owner;
    #endif
    };
    struct Cond
    {
        pthread_cond_t cond;
    };
    struct Sem
    {
        sem_t sem;
    };
#endif

#ifdef OS_MSWIN
    #include <windows.h>
    #include <process.h>
    typedef HANDLE SYS_ThreadHandle;
    struct Mutex
    {
        CRITICAL_SECTION cs;
    };
    struct Cond
    {
    };
    struct Sem
    {
        HANDLE id;
        LONG count;
    };
#endif

    enum TYPE{
        MUTEX = 0,
        COND,
        SEM
    };

#define MUTEX_TIMEDOUT  1
#define MUTEX_MAXWAIT   (~(unsigned int)0)

#include <stdarg.h>
#include <string.h>
typedef struct _error
{
#define ERR_MAX_STRLEN 256
#define ERR_MAX_ARGS   10
    /* This is a numeric value corresponding to the current error */
    int error;
    int argc;
    union
    {
        void *value_ptr;
        unsigned char value_c;
        int value_i;
        double value_f;
        char buf[ERR_MAX_STRLEN];
    } args[ERR_MAX_ARGS];
} Error;

static int SetError(const char *fmt,...)
{
    va_list ap;
    Error* error = (Error*)malloc(sizeof(Error));
    /* Ignore call if invalid format pointer was passed */
    if (fmt == NULL) return -1;
    error->error = 1;
    va_start(ap, fmt);
    error->argc = 0;
    while (*fmt) {
        if (*fmt++ == '%') {
            while (*fmt == '.' || (*fmt >= '0' && *fmt <= '9')) {
                ++fmt;
            }
            switch (*fmt++) {
            case 0:            /* Malformed format string.. */
                --fmt;
                break;
            case 'c':
            case 'i':
            case 'd':
            case 'u':
            case 'o':
            case 'x':
            case 'X':
                error->args[error->argc++].value_i = va_arg(ap, int);
                break;
            case 'f':
                error->args[error->argc++].value_f = va_arg(ap, double);
                break;
            case 'p':
                error->args[error->argc++].value_ptr = va_arg(ap, void *);
                break;
            case 's':
            {
                int i = error->argc;
                const char *str = va_arg(ap, const char *);
                if (str == NULL)
                    str = "(null)";
                strcpy((char *) error->args[i].buf, str);
                error->argc++;
            }
                break;
            default:
                break;
            }
            if (error->argc >= ERR_MAX_ARGS) {
                break;
            }
        }
    }
    va_end(ap);
    free(error);
    error = NULL;
    return -1;
}

#endif // LOCKTYPE_H
