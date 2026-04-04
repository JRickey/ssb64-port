#include "PR/R4300.h"
#include "sys/asm.h"
#include "sys/regdef.h"

.text
LEAF(__osGetSR)
    MFC0(   v0, C0_SR)
    jr      ra
END(__osGetSR)
