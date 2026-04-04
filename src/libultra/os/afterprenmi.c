#include <PR/os_internal.h>

s32 osAfterPreNMI() {
	return __osSpSetPc(0);
}
