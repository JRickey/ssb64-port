#include <PR/os_internal.h>
#include <osint.h>

OSPri osGetThreadPri(OSThread* thread) {
	if (thread == NULL) {
		thread = __osRunningThread;
	}

	return thread->priority;
}
