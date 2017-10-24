#include "ellog.h"
INITIALIZE_EASYLOGGINGPP

namespace mim {

el::base::type::StoragePointer ellog::shared()
{
	   return el::Helpers::storage();
}

bool ellog::config(const std::string& confile)
{
    if (_confile.empty())
    {
        _confile = confile;
        _conf = el::Configurations(_confile);
        el::Loggers::reconfigureAllLoggers(_conf);
        return true;
    }
    return false;
}

void ellog::defaultformat(el::Configurations& idConf)
{
    idConf.set(el::Level::Trace,   el::ConfigurationType::Format, LOGGER_COLOR(BOLD,   "[ %level | %datetime ] > %msg"));
    idConf.set(el::Level::Debug,   el::ConfigurationType::Format, LOGGER_COLOR(WHITE,  "[ %level | %datetime ] : %msg"));
    idConf.set(el::Level::Error,   el::ConfigurationType::Format, LOGGER_COLOR(RED,    "[ %level | %datetime ] ! %msg"));
    idConf.set(el::Level::Warning, el::ConfigurationType::Format, LOGGER_COLOR(YELLOW, "[ %level | %datetime ] ? %msg"));
    idConf.set(el::Level::Info,    el::ConfigurationType::Format, LOGGER_COLOR(GREEN,  "[ %level | %datetime ] $ %msg"));
}

bool ellog::log(el::Level lev, const std::string& msg)
{
    switch (lev) {
//    case el::Level::Global:
//        LOG(GLOBAL) << msg;
//        break;
    case el::Level::Trace:
        LOG(TRACE) << msg;
        break;
    case el::Level::Debug:
        LOG(DEBUG) << msg;
        break;
    case el::Level::Fatal:
        LOG(FATAL) << msg;
        break;
    case el::Level::Error:
        LOG(ERROR) << msg;
        break;
    case el::Level::Warning:
        LOG(WARNING) << msg;
        break;
//    case el::Level::Verbose:
//        LOG(VERBOSE) << msg;
//        break;
    case el::Level::Info:
        LOG(INFO) << msg;
        break;
    default:
        return false;
        break;
    }
    return true;
}
}
