#ifndef ELLOG_H
#define ELLOG_H
#include <iostream>
#include <string>
#include "../platform.h"
#include "easylogging++.h"

namespace mim {

class UTIL_API ellog
{
public:
    ellog(const std::string& id = "default", const std::string& confile = "") : _confile(confile), _id(id) {
        el::Helpers::setStorage(mim::ellog::shared());
        if (!_confile.empty())
        {
            _conf = el::Configurations(_confile);
            el::Loggers::reconfigureAllLoggers(_conf);
        }
        _logger = el::Loggers::getLogger(id, true);
    }
    virtual ~ellog() {
    }
public:
    static el::base::type::StoragePointer shared();
    bool log(el::Level lev, const std::string& msg);
    el::Logger* operator->(){ return _logger; }
public:
    bool config(const std::string& confile){
        if(_confile.empty()){
            _confile = confile;
            _conf = el::Configurations(_confile);
            el::Loggers::reconfigureAllLoggers(_conf);
            return true;
        }
        return false;
    }
public:
    void glog(const std::string& msg){ log(el::Level::Global, msg); }
    void trace(const std::string& msg){ log(el::Level::Trace, msg);}
    void debug(const std::string& msg){ log(el::Level::Debug, msg); }
    void fatal(const std::string& msg){ log(el::Level::Fatal, msg);}
    void error(const std::string& msg){ log(el::Level::Error, msg); }
    void warn(const std::string& msg){ log(el::Level::Warning, msg); }
    void verbose(const std::string& msg){ log(el::Level::Verbose, msg); }
    void info(const std::string& msg){ log(el::Level::Info, msg); }
private:
    el::Configurations  _conf;
    std::string         _id;
    std::string         _confile;
    el::Logger*         _logger;
};

}

#endif // ELLOG_H
