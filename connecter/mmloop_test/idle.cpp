#include "mmloop.h"
#include "platform.h"
#ifdef OS_MSWIN
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace mm::uvbase;

class MyIdle : public Idle
{
public:
	MyIdle() {}
	~MyIdle() {}
	virtual void OnIdle()
	{
		printf("Idle test\n");

#ifdef OS_MSWIN
		Sleep(1000);
#else
		sleep(1);
#endif
	}
};

int idleTest()
{
	Loop loop(true);

	MyIdle idle;
	idle.init(loop);
	idle.start();

	loop.run(Loop::Default);

	return 0;
}
