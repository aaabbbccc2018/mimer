#ifndef UTILEXCEPTION_H
#define UTILEXCEPTION_H
#include <string>
#include <stdarg.h>
#include <string.h>

class utilException
{
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
public:
    utilException();
    utilException(const char* str, ...)
    {
        _msg = str;
        this->SetError(str);
    }
    utilException(utilException& rhs)
    {
        this->_msg = rhs._msg;
    }
    virtual ~ utilException() throw(){}
    std::string what()const{return _msg;}
    static int SetError(const char *fmt,...);
private:
    std::string _msg;
};

#endif // UTILEXCEPTION_H
