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
    //el::Logger* operator->(){ return _logger; }
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
#   define LOGGER_LEVEL_WRITERS_SIGNATURES(FUNCTION_NAME)\
    template <typename T, typename... Args>\
    inline void FUNCTION_NAME(const char * f, const T& t, const Args& ...) \
    { _logger->FUNCTION_NAME(f,t); } \
    template <typename T>\
    inline void FUNCTION_NAME(const T&t){ _logger->FUNCTION_NAME(t); }

	template <typename T, typename... Args>
	inline void verbose(int, const char*, const T&, const Args&...);

	template <typename T>
	inline void verbose(int, const T&);

	LOGGER_LEVEL_WRITERS_SIGNATURES(info)
	LOGGER_LEVEL_WRITERS_SIGNATURES(debug)
	LOGGER_LEVEL_WRITERS_SIGNATURES(warn)
	LOGGER_LEVEL_WRITERS_SIGNATURES(error)
	LOGGER_LEVEL_WRITERS_SIGNATURES(fatal)
	LOGGER_LEVEL_WRITERS_SIGNATURES(trace)
#   undef LOGGER_LEVEL_WRITERS_SIGNATURES
private:
    el::Configurations  _conf;
    std::string         _id;
    std::string         _confile;
    el::Logger*         _logger;
};

}

#endif // ELLOG_H
