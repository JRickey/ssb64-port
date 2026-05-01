// coroutine_aarch64.S — context-switch primitive for the aarch64 coroutine
// backend (Android, and any other aarch64 platform that opts in).
//
// The AAPCS64 calling convention defines this set of callee-saved state
// as the only registers a function must preserve across a call:
//
//   GP : x19..x29 (x29=fp), x30=lr, sp
//   FPU: d8..d15 (low 64 bits of v8..v15)
//
// Everything else (x0..x18, NZCV, x30 if not used as lr by callee, d0..d7,
// d16..d31, v* upper halves) is caller-saved — the compiler has already
// spilled anything it cared about at the call site of port_coroutine_swap,
// so we only need to save/restore the callee-saved set.
//
// Context layout (176 bytes, 16-byte aligned). Offsets are mirrored by
// static_asserts on the C `CoroCtx` struct in coroutine_android.cpp:
//
//      0   x19   |  64   x27   | 112   d8    | 144   d12
//      8   x20   |  72   x28   | 120   d9    | 152   d13
//     16   x21   |  80   x29   | 128   d10   | 160   d14
//     24   x22   |  88   x30   | 136   d11   | 168   d15
//     32   x23   |  96   sp    |
//     40   x24   | 104   pad   |
//     48   x25   |
//     56   x26   |
//
// The 8-byte pad at offset 104 keeps d8..d15 at 16-byte alignment, which
// is required for stp/ldp pair access on aarch64.

    .text
    .balign 4

// void port_coroutine_swap(CoroCtx *from, CoroCtx *to);
//   x0 = from, x1 = to.
//
// Saves callee-saved state into *from, then restores *to and returns
// (via the lr that *to had stashed). The "from" coroutine effectively
// pauses at the instruction after its caller's bl to this function;
// when something later swaps back into "from", that instruction
// re-executes the ret below.
    .globl  port_coroutine_swap
#if defined(__ELF__)
    .type   port_coroutine_swap, %function
#endif
port_coroutine_swap:
    // Save "from" (x0)
    stp     x19, x20, [x0, #0]
    stp     x21, x22, [x0, #16]
    stp     x23, x24, [x0, #32]
    stp     x25, x26, [x0, #48]
    stp     x27, x28, [x0, #64]
    stp     x29, x30, [x0, #80]
    mov     x9, sp
    str     x9, [x0, #96]
    stp     d8,  d9,  [x0, #112]
    stp     d10, d11, [x0, #128]
    stp     d12, d13, [x0, #144]
    stp     d14, d15, [x0, #160]

    // Restore "to" (x1)
    ldp     x19, x20, [x1, #0]
    ldp     x21, x22, [x1, #16]
    ldp     x23, x24, [x1, #32]
    ldp     x25, x26, [x1, #48]
    ldp     x27, x28, [x1, #64]
    ldp     x29, x30, [x1, #80]
    ldr     x9, [x1, #96]
    mov     sp, x9
    ldp     d8,  d9,  [x1, #112]
    ldp     d10, d11, [x1, #128]
    ldp     d12, d13, [x1, #144]
    ldp     d14, d15, [x1, #160]

    ret
#if defined(__ELF__)
    .size   port_coroutine_swap, .-port_coroutine_swap
#endif


// void port_coroutine_trampoline_aarch64(void);
//
// First entry point for a freshly-created coroutine. The C side arranges
// for x19 to hold the PortCoroutine* on first entry (x19 is callee-saved,
// so it survives port_coroutine_swap). We move it into x0 as the first
// argument to the C trampoline, which invokes the user entry function
// and performs a permanent yield. The brk traps if the C trampoline ever
// returns, which would indicate a bug in the swap logic.
    .globl  port_coroutine_trampoline_aarch64
#if defined(__ELF__)
    .type   port_coroutine_trampoline_aarch64, %function
#endif
port_coroutine_trampoline_aarch64:
    // Establish a clean frame so any debugger walking via x29 stops cleanly
    // at the trampoline rather than walking into uninitialized stack.
    mov     x29, #0
    mov     x30, #0
    mov     x0, x19
    bl      port_coroutine_trampoline_c
    brk     #0
#if defined(__ELF__)
    .size   port_coroutine_trampoline_aarch64, .-port_coroutine_trampoline_aarch64
#endif


// GNU/Linux ELF non-executable stack note. Android's linker rejects shared
// objects without it (W^X enforcement: "the linker has not been informed
// of the stack permissions of the .so").
#if defined(__linux__) && defined(__ELF__)
    .section .note.GNU-stack,"",%progbits
#endif
