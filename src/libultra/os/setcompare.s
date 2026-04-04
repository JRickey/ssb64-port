#include "PR/R4300.h"
#include "sys/asm.h"
#include "sys/regdef.h"

.text
LEAF(__osSetCompare)
    MTC0(   a0, C0_COMPARE)
    jr      ra
END(__osSetCompare)
