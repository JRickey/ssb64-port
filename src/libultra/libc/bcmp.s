#include "PR/R4300.h"
#include "sys/asm.h"
#include "sys/regdef.h"

.text
#ifdef __sgi
WEAK(bcmp, _bcmp)
#else
#define _bcmp bcmp
#endif
LEAF(_bcmp)
    xor     v0, a0, a1
    blt     a2, 16, bytecmp

    andi    v0, v0, 0x3
    negu    t8, a0
    bnez    v0, unalgncmp

    andi    t8, t8, 0x3
    subu    a2, a2, t8
    beqz    t8, wordcmp
    
    move    v0, v1
    lwl     v0, 0(a0)
    lwl     v1, 0(a1)
    addu    a0, a0, t8
    addu    a1, a1, t8
    bne     v0, v1, cmpne

wordcmp:
    and     a3, a2, ~3
    subu    a2, a2, a3
    beqz    a3, bytecmp

    addu    a3, a3, a0
1:
    lw      v0, 0(a0)
    lw      v1, 0(a1)
    addiu   a0, a0, 4
    addiu   a1, a1, 4
    bne     v0, v1, cmpne
    bne     a0, a3, 1b

    b       bytecmp
unalgncmp:
    negu    a3, a1
    andi    a3, a3, 0x3
    subu    a2, a2, a3
    beqz    a3, partaligncmp

    addu    a3, a3, a0

1:
    lbu     v0, 0(a0)
    lbu     v1, 0(a1)
    addiu   a0, a0, 1
    addiu   a1, a1, 1
    bne     v0, v1, cmpne
    bne     a0, a3, 1b 

partaligncmp:
    and     a3, a2, ~3
    subu    a2, a2, a3
    beqz    a3, bytecmp

    addu    a3, a3, a0
    
1:
    lwl     v0, 0(a0)
    lwr     v0, 3(a0)
    lw      v1, 0(a1)
    addiu   a0, a0, 4
    addiu   a1, a1, 4
    bne     v0, v1, cmpne
    bne     a0, a3, 1b 
    
bytecmp:
    addu    a3, a2, a0
    blez    a2, cmpdone

1:
    lbu     v0, 0(a0)
    lbu     v1, 0(a1)
    addiu   a0, a0, 1
    addiu   a1, a1, 1
    bne     v0, v1, cmpne
    bne     a0, a3, 1b 

cmpdone:
    move    v0, zero
    jr      ra
cmpne:
    li      v0, 1
    jr      ra

.end _bcmp
