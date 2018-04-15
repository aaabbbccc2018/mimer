#include "Controller.h"

namespace mimer {

    Controller::Controller()
    {
        _loger = new mim::ellog("Controller", "./logs");
    }

    Controller::~Controller()
    {
        if (_loger) {
            delete _loger;
            _loger = NULL;
        }
    }

    bool Controller::check_permissions(const char* username, const char* password, size_t passwdLen)
    {
        if (std::string(username) == "skybosi" &&  std::string(password) == "skybosi") {
            return true;
        }
        return false;
    }
}
