#include "utilException.h"

int utilException::SetError(const char *fmt,...)
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
