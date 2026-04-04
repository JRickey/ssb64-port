#include "PR/R4300.h"
#include "PR/rdb.h"
#include "sys/asm.h"
#include "sys/regdef.h"

.text
.set noreorder
LEAF(osMapTLBRdb)
    mfc0    t0, C0_ENTRYHI
    li      t1, NTLBENTRIES
    mtc0    t1, C0_INX
    mtc0    zero, C0_PAGEMASK /* 4k */
    li      t2, (TLBLO_UNCACHED | TLBLO_D | TLBLO_V | TLBLO_G)
    li      t1, K2BASE
    mtc0    t1, C0_ENTRYHI
    li      t1, RDB_BASE_VIRTUAL_ADDR
    srl     t3, t1, TLBLO_PFNSHIFT
    or      t3, t3, t2
    mtc0    t3, C0_ENTRYLO0
    li      t1, TLBLO_G
    mtc0    t1, C0_ENTRYLO1
    nop
    tlbwi
    nop
    nop
    nop
    nop
    mtc0    t0, C0_ENTRYHI
    jr      ra
     nop
END(osMapTLBRdb)
