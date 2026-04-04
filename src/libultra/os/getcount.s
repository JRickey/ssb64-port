#include "PR/R4300.h"
#include "sys/asm.h"
#include "sys/regdef.h"

.text
LEAF(osGetCount)
    MFC0(   v0, C0_COUNT)
    jr      ra
END(osGetCount)
