#include "easylogging++.h"  
#include "Stream.h"  
INITIALIZE_EASYLOGGINGPP  
  
static unsigned int idx;  
  
void rolloutHandler(const char* filename, std::size_t size)   
{ 
	/*
    std::stringstream ss;  
    // ss << "mv " << filename << " log_backup_" << ++idx;  
    ss << "logs/log_backup_" << ++idx;  
    // system(ss.str().c_str());  
	Stream::Rename(filename, ss.str().c_str());
	*/
    time_t cuurenttime = time(NULL);
    cuurenttime -= 60; 
    struct::tm*  oneMinuteAgo = localtime(&cuurenttime);
    std::string filenameTemp = filename;
    int pos = filenameTemp.rfind('.');
    filenameTemp = filenameTemp.substr(0, pos);
#ifndef MAX_PATH
#define MAX_PATH 1024
#endif
    char backupFile[MAX_PATH] = { 0 };
    snprintf(backupFile, MAX_PATH, "%s_%04d%02d%02d%02d%02d%02d.log", filenameTemp.c_str(),  oneMinuteAgo->tm_year + 1900
        ,oneMinuteAgo->tm_mon + 1, oneMinuteAgo->tm_mday, oneMinuteAgo->tm_hour, oneMinuteAgo->tm_min, oneMinuteAgo->tm_sec);
    if(Stream::Rename(filename, backupFile))
    {   
        printf("rename %s to %s ok\n", filename, backupFile);
    }else{
        printf("rename %s to %s failed\n", filename, backupFile);
        exit(0);
    }
}  
  
int main(int, char**)  
{  
    idx = 0;  
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);  
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::MaxLogFileSize, "100000");  
  
    /// 注册回调函数  
    el::Helpers::installPreRollOutCallback(rolloutHandler);  
  
    for (int i = 0; i < 1000000; ++i)  
    {  
        LOG(INFO) << "Test";  
    }  
  
    /// 注销回调函数  
    el::Helpers::uninstallPreRollOutCallback();  
    return 0;  
}  
