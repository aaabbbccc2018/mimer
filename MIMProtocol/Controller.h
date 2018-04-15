#pragma once
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include <string>
#include "ellog.h"
#include "platform.h"

namespace mimer {
    class Controller
    {
    public:
        Controller();
        ~Controller();
    public:
        /*
            chack user permissions
        */
        UTIL_API bool check_permissions(const char* username, const char* password, size_t passwdLen);
    private:
        mim::ellog* _loger;
    };

}


#endif // !_CONTROLLER_H_