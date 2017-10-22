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
	mim::ellog elg("test");
	elg.info("===================INFO==================");
	elg.info("This is test info log %v", 99);
	elg.debug("===================DEBUG==================");
	elg.debug("This is test debug log %v", 99);
	elg.error("===================ERROR==================");
	elg.error("This is test error log %v", 99);
	elg.warn("===================WARN==================");
	elg.warn("This is test warn log %v", 99);
	int i = 1;
	while (i) {
		elg.warn("This is test warn log %v", i++);
	}
	return 0;
}
