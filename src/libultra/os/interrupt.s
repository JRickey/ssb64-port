#include "PR/R4300.h"
#include "sys/asm.h"
#include "sys/regdef.h"
#include "PR/os_version.h"
#include "threadasm.h"

.text
.set noreorder
LEAF(__osDisableInt)
#if BUILD_VERSION >= VERSION_J
    la      t2, __OSGlobalIntMask
    lw      t3, (t2)
    andi    t3, SR_IMASK
    mfc0    t0, C0_SR
    and     t1, t0, ~SR_IE
    mtc0    t1, C0_SR
    andi    v0, t0, SR_IE
    lw      t0, (t2)
    andi    t0, SR_IMASK
    beq     t0, t3, No_Change_Global_Int
    # @bug this la should be lw, it may never come up in practice as to reach this code
    # the CPU bits of __OSGlobalIntMask must have changed while this function is running.
     la     t2, __osRunningThread # this is intentionally a macro in the branch delay slot
    lw      t1, THREAD_SR(t2)
    andi    t2, t1, SR_IMASK
    and     t2, t0
    and     t1, ~SR_IMASK
    or      t1, t2
    and     t1, ~SR_IE
    mtc0    t1, C0_SR
    nop
    nop
No_Change_Global_Int:
    jr      ra
     nop
#else
    mfc0    t0, C0_SR
    and     t1, t0, ~SR_IE
    mtc0    t1, C0_SR
    andi    v0, t0, SR_IE
    nop
    jr      ra
     nop
#endif
END(__osDisableInt)

LEAF(__osRestoreInt)
    mfc0    t0, C0_SR
    or      t0, t0, a0
    mtc0    t0, C0_SR
    nop
    nop
    jr      ra
     nop
END(__osRestoreInt)
