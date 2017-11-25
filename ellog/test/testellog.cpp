#include <iostream>

#include "ellog.h"

// There are two ways to share repository
// one way is:
// SHARE_EASYLOGGINGPP(sharedLoggingRepository())

// Other way is
// INITIALIZE_NULL_EASYLOGGINGPP
// and in main function:
// int main(int argc, char** argv) {
//     el::Helpers::setStorage(sharedLoggingRepository());
//     ...
//     START_EASYLOGGINGPP(argc, argv);
//     ...
// }

INITIALIZE_NULL_EASYLOGGINGPP

int main() {
	mim::ellog elg("test", "logs");
	printf("time: %s", elg.datatime().c_str());
	printf("time stamp: %lld", mim::ellog::timestamp());
	elg.logroll("10000");
	elg.info("===================INFO==================");
	elg.info("This is test info log %v", 99);
	elg.debug("===================DEBUG==================");
	elg.debug("This is test debug log %v", 99);
	elg.error("===================ERROR==================");
	elg.error("This is test error log %v", 99);
	elg.warn("===================WARN==================");
	elg.warn("This is test warn log %v", 99);
	clog(DEBUG, elg) << "hello clog";
	int i = 1;
	char* p = "pointer";
	char* nil = NULL;
	while (i < 15) {
		elg.warn("%v This is test warn log %v %v",  __LINE__, i++, NULL);
		clog_if(i < 10, DEBUG, elg) << "hello clog";
	}
	elg.warn("%v This is test warn log %v pointer %v nil %v", __LINE__, i++, p, nil);
	return 0;
}
