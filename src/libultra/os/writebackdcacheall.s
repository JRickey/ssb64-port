#include "PR/R4300.h"
#include "sys/asm.h"
#include "sys/regdef.h"

.text
LEAF(osWritebackDCacheAll)
    li      t0, K0BASE
    li      t2, DCACHE_SIZE
    addu    t1, t0, t2
    addiu   t1, t1, -DCACHE_LINESIZE
1:
    .set noreorder
    cache   (C_IWBINV | CACH_PD), (t0)
    bltu    t0, t1, 1b
     addiu  t0, t0, DCACHE_LINESIZE
    .set reorder
    jr      ra
END(osWritebackDCacheAll)
