#include "Core.h"

#if defined flagWIN32
	#include <synchapi.h>
#endif

NAMESPACE_SDK_BEGIN

bool Thread::shutdown;
AtomicInt Thread::thrd_count;


void thread_runner(Callback* cb) {
	cb->Execute();
}

void Thread::Run(Callback cb) {
	this->cb = cb;
	t = new std::thread(thread_runner, &this->cb);
}

void Thread::ShutdownThreads() {
	shutdown = true;
	while (thrd_count > 0)
		Sleep(10);
}

void Sleep(int ms) {
#if defined(flagWIN32)
	::Sleep(ms);
#elif defined(flagPOSIX)
	usleep(ms * 1000);
#endif
}

NAMESPACE_SDK_END
