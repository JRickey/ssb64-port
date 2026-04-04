/************************************************************************
 Copyright (C) 1998,1999 NINTENDO Co,Ltd,
 Copyright (C) 1998,1999 MONEGI CORPORATION,
    All Rights Reserved
This program is a trade secret of NINTENDO Co,Ltd and MONEGI Corp. 
and it is not to be reproduced, published, disclosed to others, copied,
adapted, distributed, or displayed without the prior authorization of 
NINTENDO Co,Ltd. and MONEGI Corp. Licensee agrees to attach or embed 
this Notice on all copies of the program, including partial copies or 
modified versions thereof.
*************************************************************************/
/************************************************************************
 $Date: 1999/07/06 13:21:13 $
 $Revision: 1.1 $
 $Author: doseki $
************************************************************************/

#ifndef __ASM_H__
#define __ASM_H__

#ifdef __cplusplus
extern "C" {
#endif

#define _MIPS_ISA_MIPS1 1   /* R2/3K */
#define _MIPS_ISA_MIPS2 2   /* R4K/6K */
#define _MIPS_ISA_MIPS3 3   /* R4K */
#define _MIPS_ISA_MIPS4 4   /* TFP */

#define _MIPS_SIM_ABI32     1   /* MIPS MSIG calling convention */
#define _MIPS_SIM_NABI32    2   /* MIPS new 32-bit abi */
        /* NABI32 is 64bit calling convention but 32bit type sizes) */
#define _MIPS_SIM_ABI64     3   /* MIPS 64 calling convention */


/* libgultra doesn't match with the .type directive but iQue sdk asm.h uses it */
#ifdef BBPLAYER
#define ASM_TYPE_FUNC(x)    .type   x, @function
#else
#define ASM_TYPE_FUNC(x)
#endif

#define LEAF(x)             \
    .globl  x              ;\
    .align  2              ;\
    ASM_TYPE_FUNC(x)       ;\
    .ent    x,0            ;\
    x:                     ;\
    .frame  sp,0,ra

#if defined(BBPLAYER) || defined(__sgi)
#define XLEAF(x)    \
    .globl  x      ;\
    .aent   x,0    ;\
    x:
#else
#define XLEAF(x)    \
    .globl  x
#endif

#ifdef BBPLAYER
#define END(proc)           \
    .end    proc           ;\
    .size   proc, . - proc
#else
#define END(proc)   \
    .end    proc
#endif

#define ABS(x, y)   \
    .globl  x      ;\
    x   =   y

#define EXPORT(x)   \
    .globl  x      ;\
    x:

#if defined(BBPLAYER) || defined(__sgi)
#define WEAK(x, y)      \
    .weakext    x,  y
#else
#define WEAK(x, y)
#endif


#define NOP         \
    .set noreorder ;\
        nop        ;\
    .set reorder

#define CACHE(op, reg)  \
    .set noreorder     ;\
        cache op, reg  ;\
    .set reorder

#define MFC0(reg, op)  \
    .set noreorder     ;\
        mfc0 reg, op  ;\
    .set reorder

#define MTC0(reg, op)  \
    .set noreorder     ;\
        mtc0 reg, op  ;\
    .set reorder

#define CFC1(reg, op)  \
    .set noreorder     ;\
        cfc1 reg, op  ;\
    .set reorder

#define CTC1(reg, op)  \
    .set noreorder     ;\
        ctc1 reg, op  ;\
    .set reorder


#ifdef __cplusplus
}
#endif

#endif /* !__ASM_H__ */
