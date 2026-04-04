#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include <PR/ultralog.h>
#include <PR/viint.h>

// TODO: this comes from a header
#ident "$Revision: 1.17 $"

void osViSetYScale(f32 value) {
	register u32 saveMask;

#ifdef _DEBUG
	if (!__osViDevMgr.active) {
		__osError(ERR_OSVISETYSCALE_VIMGR, 0);
		return;
	}

	if ((value < 0.05f) || (value > 1.0f)) {
		__osError(ERR_OSVISETYSCALE_VALUE, 1, OS_LOG_FLOAT(value));

		if (value < 0.05f) {
			value = 0.05f;
		} else {
			value = 1.0f;
		}
	}
#endif

	saveMask = __osDisableInt();

	__osViNext->y.factor = value;
	__osViNext->state |= VI_STATE_YSCALE_UPDATED;
	__osRestoreInt(saveMask);
}
