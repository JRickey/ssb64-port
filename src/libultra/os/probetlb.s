#include "PR/R4300.h"
#include "sys/asm.h"
#include "sys/regdef.h"

.text
.set noreorder
/**
 *  u32 __osProbeTLB(void* vaddr);
 *
 *  Searches the TLB for the physical address associated with
 *  the virtual address `vaddr`.
 *
 *  Returns the physical address if found, or -1 if not found.
 */
LEAF(__osProbeTLB)
    /* Set C0_ENTRYHI based on supplied vaddr */
    mfc0    t0, C0_ENTRYHI
    andi    t1, t0, TLBHI_PIDMASK
    and     t2, a0, (TLBHI_VPN2MASK << 32) >> 32
    or      t1, t1, t2
    mtc0    t1, C0_ENTRYHI
    nop
    nop
    nop
    /*
     * TLB probe, sets C0_INX to a value matching C0_ENTRYHI.
     * If no match is found the TLBINX_PROBE bit is set to indicate this.
     */
    tlbp
    nop
    nop
    /* Read result */
    mfc0    t3, C0_INX
    and     t3, t3, TLBINX_PROBE
    /* Branch if no match was found */
    bnez    t3, 3f
     nop
    /*
     * Read TLB, sets C0_ENTRYHI, C0_ENTRYLO0, C0_ENTRYLO1 and C0_PAGEMASK for the TLB
     * entry indicated by C0_INX
     */
    tlbr
    nop
    nop
    nop
    /* Calculate page size = (page mask + 0x2000) >> 1 */
    mfc0    t3, C0_PAGEMASK
    addi    t3, t3, 0x2000
    srl     t3, t3, 0x1
    /* & with vaddr */
    and     ta0, t3, a0
    /* Select C0_ENTRYLO0 or C0_ENTRYLO1 */
    bnez    ta0, 1f
    /* make bitmask out of page size */
     addi   t3, t3, -1
    mfc0    v0, C0_ENTRYLO0
    b       2f
     nop
1:
    mfc0    v0, C0_ENTRYLO1
2:
    /* Check valid bit and branch if not valid */
    andi    ta1, v0, TLBLO_V
    beqz    ta1, 3f
     nop
    /* Extract the Page Frame Number from the entry */
    and     v0, v0, TLBLO_PFNMASK
    sll     v0, v0, TLBLO_PFNSHIFT
    /* Mask vaddr with page size mask */
    and     ta1, a0, t3
    /* Add masked vaddr to pfn to obtain the physical address */
    add     v0, v0, ta1
    b       4f
     nop
3:
    /*
     * No physical address for the supplied virtual address was found,
     * return -1
     */
    li      v0, -1
4:
    mtc0    t0, C0_ENTRYHI
    jr      ra
     nop
END(__osProbeTLB)
