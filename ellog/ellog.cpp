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

void ellog::logroll(const std::string& value)
{
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::MaxLogFileSize, value);
    el::Helpers::installPreRollOutCallback(this->rolloutHandler);
}
void ellog::rolloutHandler(const char* filename, std::size_t size)
{
    time_t cuurenttime = time(NULL);
    cuurenttime -= 60;
    struct::tm oneMinuteAgo;
    localtime_s(&oneMinuteAgo, &cuurenttime);
    std::string filenameTemp = filename;
    int pos = filenameTemp.rfind('_');
    filenameTemp = filenameTemp.substr(0, pos);
    char backupFile[MAX_PATH] = { 0 };
    sprintf_s(backupFile, MAX_PATH, "%s_%04d%02d%02d%02d%02d.log", filenameTemp.c_str(), oneMinuteAgo.tm_year + 1900
		, oneMinuteAgo.tm_mon + 1, oneMinuteAgo.tm_mday, oneMinuteAgo.tm_hour, oneMinuteAgo.tm_min);
    Stream::Rename(filename, backupFile);
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
