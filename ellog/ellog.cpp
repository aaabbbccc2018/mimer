#include "ellog.h"
INITIALIZE_EASYLOGGINGPP

namespace mim {

el::base::type::StoragePointer ellog::shared()
{
	   return el::Helpers::storage();
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
