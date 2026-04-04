#ifdef MODERN_CC
.set gp=64
#endif

#include "PR/R4300.h"
#include "sys/asm.h"
#include "sys/regdef.h"
#include "PR/os.h"
#include "PR/rcp.h"
#include "PR/rdb.h"
#include "exceptasm.h"
#include "threadasm.h"
#include "PR/os_version.h"

#define KMC_CODE_ENTRY  0xBFF00014
#define KMC_WPORT       0xBFF08000
#define KMC_STAT        0xBFF08004

.rdata

#define REDISPATCH 0x00
#define SW1 0x04
#define SW2 0x08
#define RCP 0x0c
#define CART 0x10
#define PRENMI 0x14
#define IP6_HDLR 0x18
#define IP7_HDLR 0x1c
#define COUNTER 0x20

__osIntOffTable:
    .byte REDISPATCH
    .byte PRENMI
    .byte IP6_HDLR
    .byte IP6_HDLR
    .byte IP7_HDLR
    .byte IP7_HDLR
    .byte IP7_HDLR
    .byte IP7_HDLR
    .byte COUNTER
    .byte COUNTER
    .byte COUNTER
    .byte COUNTER
    .byte COUNTER
    .byte COUNTER
    .byte COUNTER
    .byte COUNTER
    .byte REDISPATCH
    .byte SW1
    .byte SW2
    .byte SW2
    .byte RCP
    .byte RCP
    .byte RCP
    .byte RCP
    .byte CART
    .byte CART
    .byte CART
    .byte CART
    .byte CART
    .byte CART
    .byte CART
    .byte CART

__osIntTable:
    .word redispatch, sw1, sw2, rcp, cart, prenmi, IP6_Hdlr, IP7_Hdlr, counter

#ifndef _FINALROM
EXPORT(__osCauseTable_pt)
    .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0
    .byte 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
#endif

.data

#if BUILD_VERSION >= VERSION_J
EXPORT(__osHwIntTable)
    .word 0, 0
    .word 0, 0
    .word 0, 0
    .word 0, 0
    .word 0, 0

EXPORT(__osPiIntTable)
    .word 0, 0
#else
EXPORT(__osHwIntTable)
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
#endif

#ifndef _FINALROM
__osRdb_DbgRead_Ct:
    .word 0

__osRdb_Mesg:
    .word 0

__os_Kdebug_Pkt:
    .word 0

#if BUILD_VERSION >= VERSION_K
__osPreviousThread:
    .word 0
#endif
#endif

.text

/**
 * The exception preamble is copied to the exception vectors at
 * UT_VEC, XUT_VEC, ECC_VEC, E_VEC, to direct execution to __osException
 */
LEAF(__osExceptionPreamble)
    la      k0, __osException
    jr      k0
END(__osExceptionPreamble)

#ifndef _FINALROM
LEAF(__ptExceptionPreamble)
.set noreorder
    sw      k0, -0x10(sp)
    b       pt_next
     lui    k0, %hi(__ptException)
    nop
    nop
pt_next:
    addiu   k0, k0, %lo(__ptException)
    jr      k0
     nop
.set reorder
END(__ptExceptionPreamble)

LEAF(__ptException)
.set noreorder
    mfc0    k0, C0_CAUSE
    andi    k0, k0, 0x7c
    bnez    k0, pt_not_Int
     srl    k0, k0, 2
    mfc0    k0, C0_CAUSE
    nop
    andi    k0, k0, CAUSE_IP7
    beqz    k0, __osException
     nop
    j       pt_break
     nop
pt_not_Int:
    sw      k1, -8(sp)
    /* determine if the pt exception handler should deal with this cause */
    la      k1, __osCauseTable_pt
    add     k1, k1, k0
    lbu     k0, (k1)
    beqz    k0, __osException
     nop
pt_break:
pi_ok_loop:
    lw      k0, PHYS_TO_K1(PI_STATUS_REG)
    nop
    andi    k0, k0, (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY)
    bnez    k0, pi_ok_loop
     nop
    lw      k0, KMC_STAT
    nop
    andi    k0, k0, 8
    bnez    k0, pt_prof
     li     k0, KMC_CODE_ENTRY
    lw      k1, -8(sp)
    jr      k0
     addiu  sp, sp, -0x60
pt_prof:
    mfc0    k0, C0_EPC
    /* the following two ways is a (necessarily) messy way to write "sw k0, KMC_WPORT" without using at */
    lui     k1, (KMC_WPORT >> 16 + 1)
    sw      k0, -(KMC_WPORT & 0xFFFF)(k1)
    lw      k1, -8(sp)
    lw      k0, -0x10(sp)
    eret
    nop
.set reorder
END(__ptException)
#endif

LEAF(__osException)
    /* Load scratch space for thread saving */
    la      k0, __osThreadSave
    /* Save AT */
.set noat
    sd      $1, THREAD_GP1(k0);
.set at
    /* Save SR */
    MFC0(   k1, C0_SR)
    sw      k1, THREAD_SR(k0)
    /* Disable interrupts */
    and     k1, k1, ~(SR_IE | SR_EXL)
    MTC0(   k1, C0_SR)
    /* Save some temp registers for use in the following */
    sd      $8, THREAD_GP8(k0)
    sd      $9, THREAD_GP9(k0)
    sd      $10, THREAD_GP10(k0)
    /* Mark FPU as unused */
    sw      zero, THREAD_FP(k0)
    /* This instruction is useless, leftover because of bad placement of an ifdef for the debug version */
    MFC0(   t0, C0_CAUSE)
#ifndef _FINALROM
    lw      t2, __kmc_pt_mode
    bnez    t2, skip_kmc_mode
    andi    t1, t0, 0x7c
    li      t2, 0
    bne     t1, t2, savecontext
    and     t1, k1, t0
    andi    t2, t1, CAUSE_IP7
    beqz    t2, notIP7
    /* clear rdb write interrupt */
    la      t1, RDB_WRITE_INTR_REG
    sw      zero, (t1)
IP7check:
    MFC0(   t0, C0_CAUSE)
    andi    t0, t0, CAUSE_IP7
    bne     zero, t0, IP7check
    la      t2, RDB_BASE_REG
    lw      t0, (t2)
    srl     t1, t0, 0x1a
    andi    t1, t1, 0x3f
    li      t2, RDB_TYPE_HtoG_DATA
    beq     t1, t2, HandData
    li      t2, RDB_TYPE_HtoG_DEBUG
    beq     t1, t2, HandDbg
    li      t2, RDB_TYPE_HtoG_KDEBUG
    beq     t1, t2, HandKDebug
    li      t2, RDB_TYPE_HtoG_DEBUG_CT
    beq     t1, t2, DbgCnt
    li      t2, RDB_TYPE_HtoG_DATA_DONE
    beq     t1, t2, DataRead
    li      t2, RDB_TYPE_HtoG_LOG_DONE
    beq     t1, t2, LogRead
    li      t2, RDB_TYPE_HtoG_REQ_RAMROM
    beq     t1, t2, ReqRamrom
    li      t2, RDB_TYPE_HtoG_FREE_RAMROM
    beq     t1, t2, FreeRamrom
    li      t2, RDB_TYPE_HtoG_PROF_SIGNAL
    beq     t1, t2, SignalProf
    b       rdbout
SignalProf:
    li      t2, 1
    srl     t1, t0, 0x10
    andi    t1, t1, 0xff
    beq     t1, t2, AckProf
    li      t2, 0xa8
    sw      t2, __osRdb_Mesg
    b       savecontext
AckProf:
    li      t2, 0xb0
    sw      t2, __osRdb_Mesg
    b       savecontext
HandKDebug:
    sw      t0, __os_Kdebug_Pkt
    b       savecontext
DbgCnt:
    li      t2, 0xffffff
    and     t1, t0, t2
    sw      t1, __osRdb_DbgRead_Ct
    b       rdbout
DataRead:
    li      t2, 0x88
    sw      t2, __osRdb_Mesg
    b       savecontext
LogRead:
    li      t2, 0x80
    sw      t2, __osRdb_Mesg
    b       savecontext
ReqRamrom:
    li      t2, 0x90
    sw      t2, __osRdb_Mesg
    b       savecontext
FreeRamrom:
    li      t2, 0x98
    sw      t2, __osRdb_Mesg
    b       savecontext
HandData:
    srl     t1, t0, 0x18
    andi    t1, t1, 3
    beq     zero, t1, rdbout
    lw      t2, __osRdb_Read_Data_Ct
    subu    t2, t2, t1
    sw      t2, __osRdb_Read_Data_Ct
    sd      t3, THREAD_GP11(k0)
    lw      t3, __osRdb_Read_Data_Buf
    srl     t2, t0, 0x10
    andi    t2, t2, 0xff
    sb      t2, (t3)
    addi    t3, t3, 1
    addi    t1, t1, -1
    beq     zero, t1, doneData
    srl     t2, t0, 8
    andi    t2, t2, 0xff
    sb      t2, (t3)
    addi    t3, t3, 1
    addi    t1, t1, -1
    beq     zero, t1, doneData
    andi    t0, t0, 0xff
    sb      t0, (t3)
    addi    t3, t3, 1
doneData:
    sw      t3, __osRdb_Read_Data_Buf
    ld      t3, THREAD_GP11(k0)
    lw      t2, __osRdb_Read_Data_Ct
    bne     zero, t2, rdbout
    li      t2, 0x78
    sw      t2, __osRdb_Mesg
    b       savecontext
HandDbg:
    srl     t1, t0, 0x18
    andi    t1, t1, 3
    beq     zero, t1, rdbout
    lw      t2, __osRdb_DbgRead_Ct
    subu    t2, t2, t1
    sw      t2, __osRdb_DbgRead_Ct
    sd      t3, THREAD_GP11(k0)
    lw      t3, __osRdb_DbgRead_Buf
    bne     zero, t3, 1f
    ld      t3, THREAD_GP11(k0)
    b       rdbout
1:
    srl     t2, t0, 0x10
    andi    t2, t2, 0xff
    sb      t2, (t3)
    addi    t3, t3, 1
    addi    t1, t1, -1
    beq     zero, t1, doneDbg
    srl     t2, t0, 8
    andi    t2, t2, 0xff
    sb      t2, (t3)
    addi    t3, t3, 1
    addi    t1, t1, -1
    beq     zero, t1, doneDbg
    andi    t0, t0, 0xff
    sb      t0, (t3)
    addi    t3, t3, 1
doneDbg:
    sw      t3, __osRdb_DbgRead_Buf
    ld      t3, THREAD_GP11(k0)
    lw      t2, __osRdb_DbgRead_Ct
    bne     zero, t2, rdbout
    li      t2, 0xa0
    sw      t2, __osRdb_Mesg
    b       savecontext
notIP7:
    andi    t2, t1, CAUSE_IP6
    beqz    t2, savecontext
    /* clear rdb read interrupt */
    la      t1, RDB_READ_INTR_REG
    sw      zero, (t1)
    lw      t2, __osRdb_IP6_Ct
    bnez    t2, 2f
    li      t2, 1
    sw      t2, __osRdb_IP6_Empty
    b       rdbout
2:
    addi    t2, t2, -1
    sw      t2, __osRdb_IP6_Ct
    lw      t0, __osRdb_IP6_Data
    lw      t1, __osRdb_IP6_CurSend
    sll     t2, t1, 2
    add     t0, t2, t0
    lw      t2, (t0)
    addi    t1, t1, 1
    lw      t0, __osRdb_IP6_Size
    sub     t0, t0, t1
    bgtz    t0, 5f
    li      t1, 0
5:
    sw      t1, __osRdb_IP6_CurSend
checkIP6:
    MFC0(   t0, C0_CAUSE)
    andi    t0, t0, CAUSE_IP6
    bne     zero, t0, checkIP6
    la      t0, RDB_BASE_REG
    sw      t2, (t0)
rdbout:
    ld      t0, THREAD_GP8(k0)
    ld      t1, THREAD_GP9(k0)
    ld      t2, THREAD_GP10(k0)
.set noat
    ld      $1, THREAD_GP1(k0)
.set at
    lw      k1, THREAD_SR(k0)
    MTC0(   k1, C0_SR)
.set noreorder
    nop
    nop
    nop
    nop
    eret
.set reorder

skip_kmc_mode:
#endif

savecontext:
    /* Save the context of the previously running thread to be restored when it resumes */
    move    t0, k0
    lw      k0, __osRunningThread
#if !defined(_FINALROM) && BUILD_VERSION >= VERSION_K
    sw      k0, __osPreviousThread
#endif
    ld      t1, THREAD_GP1(t0)
    sd      t1, THREAD_GP1(k0)
    ld      t1, THREAD_SR(t0)
    sd      t1, THREAD_SR(k0)
    ld      t1, THREAD_GP8(t0)
    sd      t1, THREAD_GP8(k0)
    ld      t1, THREAD_GP9(t0)
    sd      t1, THREAD_GP9(k0)
    ld      t1, THREAD_GP10(t0)
    sd      t1, THREAD_GP10(k0)
3:
    sd      $2, THREAD_GP2(k0)
    sd      $3, THREAD_GP3(k0)
    sd      $4, THREAD_GP4(k0)
    sd      $5, THREAD_GP5(k0)
    sd      $6, THREAD_GP6(k0)
    sd      $7, THREAD_GP7(k0)
    sd      $11, THREAD_GP11(k0)
    sd      $12, THREAD_GP12(k0)
    sd      $13, THREAD_GP13(k0)
    sd      $14, THREAD_GP14(k0)
    sd      $15, THREAD_GP15(k0)
    sd      $16, THREAD_GP16(k0)
    sd      $17, THREAD_GP17(k0)
    sd      $18, THREAD_GP18(k0)
    sd      $19, THREAD_GP19(k0)
    sd      $20, THREAD_GP20(k0)
    sd      $21, THREAD_GP21(k0)
    sd      $22, THREAD_GP22(k0)
    sd      $23, THREAD_GP23(k0)
    sd      $24, THREAD_GP24(k0)
    sd      $25, THREAD_GP25(k0)
    sd      $28, THREAD_GP28(k0)
    sd      $29, THREAD_GP29(k0)
    sd      $30, THREAD_GP30(k0)
    sd      $31, THREAD_GP31(k0)

    mflo    t0
    sd      t0, THREAD_LO(k0)
    mfhi    t0
    sd      t0, THREAD_HI(k0)

    lw      k1, THREAD_SR(k0)
    andi    t1, k1, SR_IMASK
    beqz    t1, savercp

    /*
     * If any CPU interrupts are enabled in the SR of the previous thread, bitwise-OR in the
     *  disabled CPU interrupts from the global interrupt mask.
     * This is an attempt at reverting the effect of masking the SR of the thread with the
     *  global interrupt mask. This is however broken, see comments for osSetIntMask.
     */
    la      t0, __OSGlobalIntMask
    lw      t0, 0(t0)
    xor     t2, t0, ~0 /* not except not using not */
    andi    t2, t2, SR_IMASK
    or      ta0, t1, t2
    and     t3, k1, ~SR_IMASK
    or      t3, t3, ta0
    sw      t3, THREAD_SR(k0)
    andi    t0, t0, SR_IMASK
    and     t1, t1, t0
    and     k1, k1, ~SR_IMASK
    or      k1, k1, t1
savercp:

    /* Save the currently masked RCP interrupts. */
    lw      t1, PHYS_TO_K1(MI_INTR_MASK_REG)
    beqz    t1, endrcp

    /*
     * Similar to the above comment, but for RCP interrupt enable bits rather than CPU.
     * This suffers from the same problem as above.
     */
    la      t0, __OSGlobalIntMask
    lw      t0, 0(t0)

    srl     t0, t0, 0x10
    xor     t0, t0, ~0
    andi    t0, t0, 0x3f
    lw      ta0, THREAD_RCP(k0)
    and     t0, t0, ta0
    or      t1, t1, t0
endrcp:
    sw      t1, THREAD_RCP(k0)
    MFC0(   t0, C0_EPC)
    sw      t0, THREAD_PC(k0)
    lw      t0, THREAD_FP(k0)
    beqz    t0, 1f
    /* Save FP Registers if FPU was used by the thread */
    CFC1(   t0, fcr31)
    NOP
    sw      t0, THREAD_FPCSR(k0)
    sdc1    $f0, THREAD_FP0(k0)
    sdc1    $f2, THREAD_FP2(k0)
    sdc1    $f4, THREAD_FP4(k0)
    sdc1    $f6, THREAD_FP6(k0)
    sdc1    $f8, THREAD_FP8(k0)
    sdc1    $f10, THREAD_FP10(k0)
    sdc1    $f12, THREAD_FP12(k0)
    sdc1    $f14, THREAD_FP14(k0)
    sdc1    $f16, THREAD_FP16(k0)
    sdc1    $f18, THREAD_FP18(k0)
    sdc1    $f20, THREAD_FP20(k0)
    sdc1    $f22, THREAD_FP22(k0)
    sdc1    $f24, THREAD_FP24(k0)
    sdc1    $f26, THREAD_FP26(k0)
    sdc1    $f28, THREAD_FP28(k0)
    sdc1    $f30, THREAD_FP30(k0)
1:
    /*
     * Determine the cause of the exception or interrupt and
     *  enter appropriate handling routine
     */
    MFC0(   t0, C0_CAUSE)
    sw      t0, THREAD_CAUSE(k0)

.set noreorder
    li      t1, OS_STATE_RUNNABLE
    sh      t1, THREAD_STATE(k0)
.set reorder

#ifndef _FINALROM
    lw      a0, __os_Kdebug_Pkt
    beqz    a0, no_kdebug
    sw      zero, __os_Kdebug_Pkt
    jal     kdebugserver
    b       __osDispatchThreadSave
no_kdebug:
    lw      a0, __osRdb_Mesg
    beqz    a0, no_rdb_mesg
    sw      zero, __osRdb_Mesg
    jal     send_mesg
    lw      t0, 0x120(k0)
no_rdb_mesg:
#endif

    andi    t1, t0, CAUSE_EXCMASK
    /* Test for break exception */
    li      t2, EXC_BREAK
    beq     t1, t2, handle_break

    /* Test for CpU (coprocessor unusable) exception */
    li      t2, EXC_CPU
    beq     t1, t2, handle_CpU

    /* Test for interrupt, if it is not an interrupt, panic */
    li      t2, EXC_INT
    bne     t1, t2, panic
handle_interrupt:
    and     s0, k1, t0
next_interrupt:
    /*
     * Handle external interrupt causes, using a jump table
     *  to enter into the appropriate handler
     */
    andi    t1, s0, SR_IMASK
    srl     t2, t1, 0xc
    bnez    t2, 1f

    srl     t2, t1, SR_IMASKSHIFT
    addi    t2, t2, 16
1:

    lbu     t2, __osIntOffTable(t2)

    lw      t2, __osIntTable(t2)
    jr      t2

/**
 *  IP6 Interrupt
 *  Only signalled by development hardware
 */
IP6_Hdlr:
    /* Mask out interrupt and continue */
    and     s0, s0, ~CAUSE_IP6
    b       next_interrupt

/**
 *  IP7 Interrupt
 *  Only signalled by development hardware
 */
IP7_Hdlr:
    /* Mask out interrupt and continue */
    and     s0, s0, ~CAUSE_IP7
    b       next_interrupt

/**
 *  IP8/Counter Interrupt
 *  Once the cop0 count register reaches the value of the
 *  cop0 compare register, this interrupt is triggered
 */
counter:
    MFC0(   t1, C0_COMPARE)
    MTC0(   t1, C0_COMPARE)
    li      a0, MESG(OS_EVENT_COUNTER)
    /* Post counter message */
    jal     send_mesg
    /* Mask out interrupt and continue */
    and     s0, s0, ~CAUSE_IP8
    b       next_interrupt

/**
 *  IP4/Cartridge Interrupt
 *  Signalled by the N64 Disk Drive
 */
cart:
#if BUILD_VERSION >= VERSION_J
    /* Mask out interrupt */
    and     s0, s0, ~CAUSE_IP4
    /* Load cart callback set by __osSetHWIntrRoutine */
    la      t1, __osHwIntTable
    add     t1, HWINTR_SIZE
    lw      t2, HWINTR_CALLBACK(t1)

    /* If the callback is NULL, handling is done */
    beqz    t2, 1f

    /* Set up a stack and run the callback */
    lw      sp, HWINTR_SP(t1)
    jalr    t2
    /* Post a cart event message */
    li      a0, MESG(OS_EVENT_CART)

    beqz    v0, 1f
    /* Redispatch immediately if the callback returned nonzero */
    b       redispatch

1:
    jal     send_mesg
    /* Continue */
    b       next_interrupt
#else
    li      a0, MESG(OS_EVENT_CART)
    /* Mask out interrupt */
    and     s0, s0, ~CAUSE_IP4
    la      sp, leoDiskStack
    addiu   sp, 0x1000 - 0x10 # Stack size minus initial frame
    /* Load cart callback set by __osSetHWIntrRoutine */
    li      t2, HWINTR_SIZE
    lw      t2, __osHwIntTable(t2)

    /* If the callback is NULL, handling is done */
    beqz    t2, 1f

    /* Set up a stack and run the callback */
    jalr    t2
    li      a0, MESG(OS_EVENT_CART)

    beqz    v0, 1f
    /* Redispatch immediately if the callback returned nonzero */
    b       redispatch

1:
    /* Post a cart event message */
    jal     send_mesg
    /* Continue */
    b       next_interrupt
#endif

/**
 *  IP3/RCP Interrupt
 *  Signalled by the RCP for various reasons, described below
 */
rcp:
    /*
     * Load the MI interrupts and mask with the RCP bits in the global interrupt mask
     *
     * @bug this clobbers the t0 register which is expected to hold the value of the
     *  C0_CAUSE register in the sw1 and sw2 handlers. If the sw1 or sw2 handler runs
     *  after this, the interrupt will not be cleared properly.
     */
    lw      s1, PHYS_TO_K1(MI_INTR_REG)
    la      t0, __OSGlobalIntMask
    lw      t0, (t0)

    srl     t0, t0, 0x10
    and     s1, s1, t0

/**
 *  Signal Processor (SP) Interrupt
 */
    /* Test for sp interrupt */
    andi    t1, s1, MI_INTR_SP
    beqz    t1, vi

    /* Test for yielded or done signals in particular */

    /* Mask out SP interrupt */
    andi    s1, s1, (MI_INTR_SI | MI_INTR_AI | MI_INTR_VI | MI_INTR_PI | MI_INTR_DP)
    lw      ta0, PHYS_TO_K1(SP_STATUS_REG)
    li      t1, (SP_CLR_INTR | SP_CLR_SIG3)

    /* Clear interrupt and signal 3 */
    sw      t1, PHYS_TO_K1(SP_STATUS_REG)
    andi    ta0, ta0, (SP_STATUS_YIELDED | SP_STATUS_TASKDONE)
    beqz    ta0, sp_other_break

    /* Post an SP event message */
    li      a0, MESG(OS_EVENT_SP)
    jal     send_mesg

    beqz    s1, NoMoreRcpInts

    /* Step over sp_other_break handler */
    b       vi

sp_other_break:
    /*
     * An sp signal that is not due to yielding or task completion, such as
     *  an sp breakpoint. Post a different event message
     */
    li      a0, MESG(OS_EVENT_SP_BREAK)
    jal     send_mesg

    beqz    s1, NoMoreRcpInts

/**
 *  Video Interface (VI) Interrupt
 */
vi:
    /* Test for vi interrupt */
    andi    t1, s1, MI_INTR_VI
    beqz    t1, ai

    /* Mask out vi interrupt */
    andi    s1, s1, (MI_INTR_SP | MI_INTR_SI | MI_INTR_AI | MI_INTR_PI | MI_INTR_DP)

    /* Clear interrupt */
    sw      zero, PHYS_TO_K1(VI_CURRENT_REG)

    /* Post vi event message */
    li      a0, MESG(OS_EVENT_VI)
    jal     send_mesg
    beqz    s1, NoMoreRcpInts

/**
 *  Audio Interface (AI) Interrupt
 */
ai:
    /* Test for ai interrupt */
    andi    t1, s1, MI_INTR_AI
    beqz    t1, si

    /* Mask out ai interrupt */
    andi    s1, s1, (MI_INTR_SP | MI_INTR_SI | MI_INTR_VI | MI_INTR_PI | MI_INTR_DP)

    li      t1, 1
    /* Clear interrupt */
    sw      t1, PHYS_TO_K1(AI_STATUS_REG)

    /* Post ai event message */
    li      a0, MESG(OS_EVENT_AI)
    jal     send_mesg
    beqz    s1, NoMoreRcpInts

/**
 *  Serial Interface (SI) Interrupt
 */
si:
    /* Test for si interrupt */
    andi    t1, s1, MI_INTR_SI
    beqz    t1, pi

    /* Mask out si interrupt */
    andi    s1, s1, (MI_INTR_SP | MI_INTR_AI | MI_INTR_VI | MI_INTR_PI | MI_INTR_DP)
    /* Clear interrupt */
    sw      zero, PHYS_TO_K1(SI_STATUS_REG)

    /* Post si event message */
    li      a0, MESG(OS_EVENT_SI)
    jal     send_mesg
    beqz    s1, NoMoreRcpInts

/**
 *  Parallel Interface (PI) Interrupt
 */
pi:
    /* Test for pi interrupt */
    andi    t1, s1, MI_INTR_PI
    beqz    t1, dp

    /* Mask out pi interrupt */
    andi    s1, s1, (MI_INTR_SP | MI_INTR_SI | MI_INTR_AI | MI_INTR_VI | MI_INTR_DP)

    /* Clear interrupt */
    li      t1, PI_STATUS_CLR_INTR
    sw      t1, PHYS_TO_K1(PI_STATUS_REG)

#if 0
    /* Load pi callback */
    la      t1, __osPiIntTable
    lw      t2, (t1)
    /* Skip callback if NULL */
    beqz    t2, 1f

    /* Set up a stack and run the callback */
    lw      sp, 4(t1)
    move    a0, v0
    jalr    t2

    /* If the callback returns non-zero, do not post a pi event message */
    bnez    v0, 2f
1:
#endif
    /* Post pi event message */
    li      a0, MESG(OS_EVENT_PI)
    jal     send_mesg
#if BUILD_VERSION >= VERSION_J
2:
#endif
    beqz    s1, NoMoreRcpInts

/**
 *  Display Processor (DP) Interrupt
 */
dp:
    /* Test for dp interrupt */
    andi    t1, s1, MI_INTR_DP
    beqz    t1, NoMoreRcpInts

    /* Mask out dp interrupt */
    andi    s1, s1, (MI_INTR_SP | MI_INTR_SI | MI_INTR_AI | MI_INTR_VI | MI_INTR_PI)

    /* Clear dp interrupt */
    li      t1, MI_CLR_DP_INTR
    sw      t1, PHYS_TO_K1(MI_INIT_MODE_REG)

    /* Post dp event message */
    li      a0, MESG(OS_EVENT_DP)
    jal     send_mesg

NoMoreRcpInts:
    /* Mask out interrupt and continue */
    and     s0, s0, ~CAUSE_IP3
    b       next_interrupt

/**
 *  IP5/PreNMI Interrupt
 *  Reset button has been pressed
 */
prenmi:
    /* Disable IP5/PreNMI interrupt for the previously running thread */
    lw      k1, THREAD_SR(k0)

    and     k1, k1, ~CAUSE_IP5
    sw      k1, THREAD_SR(k0)
    /* Test __osShutdown for first PreNMI event */
    la      t1, __osShutdown
    lw      t2, 0(t1)
    beqz    t2, firstnmi

    /* Mask out interrupt and redispatch immediately */
    and     s0, s0, ~CAUSE_IP5
    b       redispatch

firstnmi:
    /* Set __osShutdown */
    li      t2, 1
    sw      t2, (t1)
    /* Post a PreNMI event message */
    li      a0, MESG(OS_EVENT_PRENMI)
    jal     send_mesg

    /* Mask out and disable IP5/PreNMI interrupt for the highest priority thread */
    and     s0, s0, ~CAUSE_IP5
    lw      t2, __osRunQueue
    lw      k1, THREAD_SR(t2)
    and     k1, k1, ~CAUSE_IP5
    sw      k1, THREAD_SR(t2)
    /* Redispatch immediately */
    b       redispatch

sw2:
    /* Mask out interrupt */
    and     t0, t0, ~CAUSE_SW2
    MTC0(   t0, C0_CAUSE)

    /* Post sw2 event message */
    li      a0, MESG(OS_EVENT_SW2)
    jal     send_mesg

    /* Mask out interrupt and continue */
    and     s0, s0, ~CAUSE_SW2
    b       next_interrupt

sw1:
    /* Mask out interrupt */
    and     t0, t0, ~CAUSE_SW1
    MTC0(   t0, C0_CAUSE)

    /* Post sw1 event message */
    li      a0, MESG(OS_EVENT_SW1)
    jal     send_mesg

    /* Mask out interrupt and continue */
    and     s0, s0, ~CAUSE_SW1
    b       next_interrupt

handle_break:
    /* Set last thread as having hit a break exception */
    li      t1, OS_FLAG_CPU_BREAK
    sh      t1, THREAD_FLAGS(k0)
    /* Post a cpu break event message */
    li      a0, MESG(OS_EVENT_CPU_BREAK)
    jal     send_mesg

    /* Redispatch */
    b       redispatch

redispatch:
    /* Get priority of previously running thread */
    lw      t1, THREAD_PRI(k0)
    lw      t2, __osRunQueue
    /* Get highest priority from waiting threads */
    lw      t3, THREAD_PRI(t2)

    bge     t1, t3, enqueueRunning

    /*
     * The previously running thread is no longer the highest priority,
     *  enqueue it to the run queue to wait its turn again
     */
    move    a1, k0
    la      a0, __osRunQueue

    jal     __osEnqueueThread

    j       __osDispatchThread

/**
 *  Resume the previously running thread by placing it at the top of
 *  the run queue and dispatching it
 */
enqueueRunning:
    la      t1, __osRunQueue
    lw      t2, MQ_MTQUEUE(t1)
    sw      t2, THREAD_NEXT(k0)
    sw      k0, MQ_MTQUEUE(t1)
    j       __osDispatchThread

/**
 *  Unhandled exceptions & interrupts end up here,
 *  trap to software by posting a fault message
 */
panic:
    /* Mark the thread as having faulted */
    sw      k0, __osFaultedThread
    li      t1, OS_STATE_STOPPED
    sh      t1, THREAD_STATE(k0)
    li      t1, OS_FLAG_FAULT
    sh      t1, THREAD_FLAGS(k0)

    /* Save C0_BADVADDR */
    MFC0(   t2, C0_BADVADDR)

    sw      t2, THREAD_BADVADDR(k0)

    /* Post the fault message */
    li      a0, MESG(OS_EVENT_FAULT)
    jal     send_mesg

    /* Dispatch next thread */
    j       __osDispatchThread
END(__osException)

/**
 *  Handles posting event messages to the listening message queue, if there is one
 */
LEAF(send_mesg)
    /* Save return address */
    move    s2, ra

    /* Load pointer to listening message queue */
    la      t2, __osEventStateTab
    addu    t2, t2, a0
    lw      t1, OS_EVENTSTATE_MESSAGE_QUEUE(t2)

    /* If there is no listening message queue, done */
    beqz    t1, send_done

    /* Test if the message queue is full, if so do not post the message */
    lw      t3, MQ_VALIDCOUNT(t1)
    lw      ta0, MQ_MSGCOUNT(t1)

    bge     t3, ta0, send_done

    /* Add validcount to first and modulo with msgcount */
    lw      ta1, MQ_FIRST(t1)
    addu    ta1, ta1, t3

    rem     ta1, ta1, ta0
    lw      ta0, MQ_MSG(t1)
    mul     ta1, ta1, 4
    addu    ta0, ta0, ta1

    /* Fetch the message to post */
    lw      ta1, OS_EVENTSTATE_MESSAGE(t2)
    /* Post the message to the message queue */
    sw      ta1, 0(ta0)
    /* Increment the validCount */
    addiu   t2, t3, 1
    sw      t2, MQ_VALIDCOUNT(t1)
    /*
     * If there was a thread blocked on this message queue,
     *  wake it up
     */
    lw      t2, MQ_MTQUEUE(t1)
    lw      t3, THREAD_NEXT(t2)
    beqz    t3, send_done

    move    a0, t1
    jal     __osPopThread

    move    t2, v0

    move    a1, t2
    la      a0, __osRunQueue
    jal     __osEnqueueThread
send_done:
    jr      s2
END(send_mesg)

/**
 *  Handle coprocessor unusable exception
 */
LEAF(handle_CpU)
    and     t1, t0, CAUSE_CEMASK
    srl     t1, t1, CAUSE_CESHIFT
    /* if not coprocessor 1, panic */
    li      t2, 1
    bne     t1, t2, panic

    /* Mark cop1 as usable for previous thread */
    li      t1, 1
    sw      t1, THREAD_FP(k0)
    lw      k1, THREAD_SR(k0)
    or      k1, k1, SR_CU1
    sw      k1, THREAD_SR(k0)
    b       enqueueRunning
END(handle_CpU)

/**
 *  void __osEnqueueAndYield(OSThread** threadQueue);
 *
 *  Voluntary thread yielding.
 *  Enqueues the currently running thread to the top of the
 *   thread queue `threadQueue` and yields to the highest priority
 *   unblocked runnable thread.
 */
LEAF(__osEnqueueAndYield)
    lw      a1, __osRunningThread
#if !defined(_FINALROM) && BUILD_VERSION >= VERSION_K
    sw      a1, __osPreviousThread
#endif
    /* Save SR */
    MFC0(   t0, C0_SR)
    ori     t0, t0, SR_EXL
    sw      t0, THREAD_SR(a1)

    /* Save callee-saved registers */
    sd      s0, THREAD_GP16(a1)
    sd      s1, THREAD_GP17(a1)
    sd      s2, THREAD_GP18(a1)
    sd      s3, THREAD_GP19(a1)
    sd      s4, THREAD_GP20(a1)
    sd      s5, THREAD_GP21(a1)
    sd      s6, THREAD_GP22(a1)
    sd      s7, THREAD_GP23(a1)
    sd      gp, THREAD_GP28(a1)
    sd      sp, THREAD_GP29(a1)
    sd      s8, THREAD_GP30(a1)
    sd      ra, THREAD_GP31(a1)
    sw      ra, THREAD_PC(a1)

    /* Save FPU callee-saved registers if the current thread has used the FPU */
    lw      k1, THREAD_FP(a1)
    beqz    k1, 1f
    cfc1    k1, fcr31
    sw      k1, THREAD_FPCSR(a1)
    sdc1    $f20, THREAD_FP20(a1)
    sdc1    $f22, THREAD_FP22(a1)
    sdc1    $f24, THREAD_FP24(a1)
    sdc1    $f26, THREAD_FP26(a1)
    sdc1    $f28, THREAD_FP28(a1)
    sdc1    $f30, THREAD_FP30(a1)
1:
    lw      k1, THREAD_SR(a1)
    andi    t1, k1, SR_IMASK
    beqz    t1, 2f

    /*
     * This code does the same thing as the block just above the `savercp` label.
     * See the comment there for more about this.
     */
    la      t0, __OSGlobalIntMask
    lw      t0, 0(t0)
    xor     t0, t0, ~0
    andi    t0, t0, SR_IMASK
    or      t1, t1, t0
    and     k1, k1, ~SR_IMASK
    or      k1, k1, t1
    sw      k1, THREAD_SR(a1)
2:
    lw      k1, PHYS_TO_K1(MI_INTR_MASK_REG)
    beqz    k1, 3f

    /*
     * This code does the same thing as the block just below the `savercp` label.
     * See the comment there for more about this.
     */
    la      k0, __OSGlobalIntMask
    lw      k0, 0(k0)

    srl     k0, k0, RCP_IMASKSHIFT
    xor     k0, k0, ~0
    andi    k0, k0, (RCP_IMASK >> RCP_IMASKSHIFT)
    lw      t0, THREAD_RCP(a1)
    and     k0, k0, t0
    or      k1, k1, k0
3:
    /*
     * If the specified thread queue is null, skip
     *  straight to dispatching
     */
    sw      k1, THREAD_RCP(a1)
    beqz    a0, noEnqueue
    jal     __osEnqueueThread
noEnqueue:
    j       __osDispatchThread
END(__osEnqueueAndYield)

/**
 *  void __osEnqueueThread(OSThread** threadQueue, OSThread* thread);
 *
 *  Enqueues `thread` to the thread queue `threadQueue`, inserted by priority
 */
LEAF(__osEnqueueThread)
    move    t9, a0
    lw      t8, 0(a0)
    lw      ta3, THREAD_PRI(a1)
    lw      ta2, THREAD_PRI(t8)
    /* 
     * If the current highest priority thread is a lower priority than
     *  the new thread, skip searching the queue
     */
    blt     ta2, ta3, 2f
1:
    /*
     * Search the queue for the position to insert the thread to maintain
     *  ordering by priority
     */
    move    t9, t8
    lw      t8, THREAD_NEXT(t8)
    lw      ta2, THREAD_PRI(t8)
    bge     ta2, ta3, 1b
2:
    /* Insert the thread into the queue */
    lw      t8, THREAD_NEXT(t9)
    sw      t8, THREAD_NEXT(a1)
    sw      a1, THREAD_NEXT(t9)
    sw      a0, THREAD_QUEUE(a1)
    jr      ra
END(__osEnqueueThread)

/**
 *  OSThread* __osPopThread(OSThread** threadQueue);
 *
 *  Pops the highest priority thread from the top of the
 *   thread queue `threadQueue` and returns it
 */
LEAF(__osPopThread)
    lw      v0, 0(a0) /* a0 is OSThread** */
    lw      t9, THREAD_NEXT(v0)
    sw      t9, 0(a0)
    jr      ra
END(__osPopThread)

#if BUILD_VERSION >= VERSION_K
LEAF(__osNop)
    jr      ra
END(__osNop)
#endif

/**
 *  void __osDispatchThread(void);
 *
 *  Dispatches the next thread to run after restoring the context
 */
LEAF(__osDispatchThread)
    /* Obtain highest priority thread from the active run queue */
    la      a0, __osRunQueue
    jal     __osPopThread
    /* Set thread as running */
    sw      v0, __osRunningThread
    li      t0, OS_STATE_RUNNING
    sh      t0, THREAD_STATE(v0)
    /*
     * Restore SR, masking out any interrupts that are not also
     *  enabled in the global interrupt mask
     */
    move    k0, v0

#if !defined(_FINALROM) && BUILD_VERSION >= VERSION_K
    la      t0, __osThprofFunc
    lw      t0, (t0)
    beqz    t0, __osDispatchThreadSave
1:
    lw      a0, __osPreviousThread
    lw      sp, __osThprofStack
    jalr    t0
#endif

/* There is another 1: label somewhere around here in version J and below */
#if BUILD_VERSION < VERSION_K
1:
#endif

__osDispatchThreadSave:
    lw      k1, THREAD_SR(k0)
    la      t0, __OSGlobalIntMask
    lw      t0, 0(t0)
    andi    t0, t0, SR_IMASK
    andi    t1, k1, SR_IMASK
    and     t1, t1, t0
    and     k1, k1, ~SR_IMASK
    or      k1, k1, t1
    MTC0(   k1, C0_SR)
/* Restore GPRs */
.set noat
    ld      $1, THREAD_GP1(k0)
.set at
    ld      $2, THREAD_GP2(k0)
    ld      $3, THREAD_GP3(k0)
    ld      $4, THREAD_GP4(k0)
    ld      $5, THREAD_GP5(k0)
    ld      $6, THREAD_GP6(k0)
    ld      $7, THREAD_GP7(k0)
    ld      $8, THREAD_GP8(k0)
    ld      $9, THREAD_GP9(k0)
    ld      $10, THREAD_GP10(k0)
    ld      $11, THREAD_GP11(k0)
    ld      $12, THREAD_GP12(k0)
    ld      $13, THREAD_GP13(k0)
    ld      $14, THREAD_GP14(k0)
    ld      $15, THREAD_GP15(k0)
    ld      $16, THREAD_GP16(k0)
    ld      $17, THREAD_GP17(k0)
    ld      $18, THREAD_GP18(k0)
    ld      $19, THREAD_GP19(k0)
    ld      $20, THREAD_GP20(k0)
    ld      $21, THREAD_GP21(k0)
    ld      $22, THREAD_GP22(k0)
    ld      $23, THREAD_GP23(k0)
    ld      $24, THREAD_GP24(k0)
    ld      $25, THREAD_GP25(k0)
    ld      $28, THREAD_GP28(k0)
    ld      $29, THREAD_GP29(k0)
    ld      $30, THREAD_GP30(k0)
    ld      $31, THREAD_GP31(k0)
    ld      k1, THREAD_LO(k0)
    mtlo    k1
    ld      k1, THREAD_HI(k0)
    mthi    k1
    /* Move thread pc to EPC so that eret will return execution to where the thread left off */
    lw      k1, THREAD_PC(k0)
    MTC0(   k1, C0_EPC)

    /* Check if the FPU was used by this thread and if so also restore the FPU registers */
    lw      k1, THREAD_FP(k0)
    beqz    k1, 1f

    lw      k1, THREAD_FPCSR(k0)
    CTC1(   k1, fcr31)
    ldc1    $f0, THREAD_FP0(k0)
    ldc1    $f2, THREAD_FP2(k0)
    ldc1    $f4, THREAD_FP4(k0)
    ldc1    $f6, THREAD_FP6(k0)
    ldc1    $f8, THREAD_FP8(k0)
    ldc1    $f10, THREAD_FP10(k0)
    ldc1    $f12, THREAD_FP12(k0)
    ldc1    $f14, THREAD_FP14(k0)
    ldc1    $f16, THREAD_FP16(k0)
    ldc1    $f18, THREAD_FP18(k0)
    ldc1    $f20, THREAD_FP20(k0)
    ldc1    $f22, THREAD_FP22(k0)
    ldc1    $f24, THREAD_FP24(k0)
    ldc1    $f26, THREAD_FP26(k0)
    ldc1    $f28, THREAD_FP28(k0)
    ldc1    $f30, THREAD_FP30(k0)

1:
    /*
     * Restore RCP interrupt mask, masking out any RCP interrupts that
     *  are not also enabled in the global interrupt mask
     */
.set noreorder
    lw      k1, THREAD_RCP(k0)
    la      k0, __OSGlobalIntMask
    lw      k0, 0(k0)
    srl     k0, k0, 0x10
    and     k1, k1, k0
    sll     k1, k1, 0x1
    la      k0, __osRcpImTable
    addu    k1, k1, k0
    lhu     k1, 0(k1)
    la      k0, PHYS_TO_K1(MI_INTR_MASK_REG)
    sw      k1, 0(k0)

    /* Empty pipeline */
    nop
    nop
    nop
    nop

    /* Resume thread execution */
    eret
.set reorder
END(__osDispatchThread)

/**
 *  void __osCleanupThread(void);
 *
 *  When a thread entrypoint function returns, it returns to this function.
 *  This function is responsible for cleaning up the thread, signalling for the
 *  current thread to be destroyed.
 */
LEAF(__osCleanupThread)
    move    a0, zero
#if !defined(BBPLAYER) && !defined(__sgi)
    nop
#endif
    /* Despite being a jal, this function does not return as the thread will have been destroyed */
    jal     osDestroyThread
END(__osCleanupThread)
