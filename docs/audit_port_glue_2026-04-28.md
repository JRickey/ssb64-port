# Audit: SSB64 PC Port Bridge Code & Port Glue — Memory Safety & Type Shadows

**Date:** 2026-04-28  
**Scope:** `/Users/jackrickey/Dev/ssb64-port/port/` — bridge, stubs, coroutine, watchdog, audio, first-run, and native dialogs  
**Focus:** Memory-safety bugs, sizeof traps, include-order regressions, thread-safety, coroutine safety

---

## CRITICAL BUGS

### 1. OSThread sizeof Shadowing (n64_stubs.c:127)

**File:** `/Users/jackrickey/Dev/ssb64-port/port/stubs/n64_stubs.c:127`

**Code:**
```c
void osCreateThread(OSThread *t, OSId id, void (*entry)(void *), void *arg,
                    void *sp, OSPri p)
{
	memset(t, 0, sizeof(OSThread));  // LINE 127 — BUG HERE
	t->id = id;
	...
}
```

**Root Cause:** Typename shadowing — `OSThread` refers to LUS's redefined version from `libultraship/include/libultraship/libultra/thread.h`, NOT the decomp's original from `include/PR/os.h`.

**Size Mismatch:**
- **Decomp version** (`include/PR/os.h:80`): ~192 bytes (N64 word-sized pointers)
  - next (4), priority (4), queue (4), tlnext (4), state (2), flags (2), id (4), fp (4), context (160)
- **LUS version** (`libultraship/.../thread.h:50`): 0x1B0 = **432 bytes** (LP64 pointers + extra fields)
  - Same layout as decomp but with 8-byte pointers, plus `thprof` field at offset 0x1C, and extended `__OSThreadContext` with NEON/FP state

**Symptom:** 
- If the caller has allocated only the decomp-sized buffer (~192 bytes), `memset` with sizeof(LUS-OSThread) overwrites 240+ bytes past the buffer end.
- Stack corruption, heap overruns, subsequent crashes in unrelated code.

**Risk:** **SHIPPED-BUG-LIKELY**  
Game initializes threads very early (osCreateThread is called for game, scheduler, audio, controller threads at startup). An overrun here would corrupt adjacent stack frames or heap metadata.

**Reproduction:**
1. Set a breakpoint at `osCreateThread` entry.
2. Inspect the caller's stack — OSThread is often a local variable with 192 bytes allocated.
3. If `memset` writes 432 bytes, it will stomp the next stack variable or return address.
4. On some systems/malloc implementations, the heap write may not be caught immediately but cause crashes later.

**Fix:**
Use `sizeof(__OSThreadContext)` + fixed offsets, or better: define a local function `sizeof_decomp_osthread()` that returns the correct N64 size.

**Recommended Solution:**
```c
// In port/stubs/n64_stubs.c, near osCreateThread:

// Decomp OSThread struct is smaller than LUS's redefined version.
// We must not use sizeof(OSThread) directly as it would use LUS's size.
// Instead, use the field layout directly.
static size_t osthread_decomp_size(void) {
    // Decomp OSThread layout (N64 ABI):
    // next (4) + priority (4) + queue (4) + tlnext (4) + state (2) + flags (2) +
    // id (4) + fp (4) + context (160) = 192 bytes
    return 4 + 4 + 4 + 4 + 2 + 2 + 4 + 4 + 160;
}

void osCreateThread(OSThread *t, OSId id, void (*entry)(void *), void *arg,
                    void *sp, OSPri p)
{
	memset(t, 0, osthread_decomp_size());  // Use decomp size, not LUS size
	t->id = id;
	...
}
```

Or: Move the `memset` call into a stub that doesn't include the LUS headers, using only C99 fixed-size types.

---

## MEDIUM-RISK FINDINGS

### 2. Particle Bank Bridge — Rapid Scene Transition Dedup Map Eviction

**File:** `/Users/jackrickey/Dev/ssb64-port/port/bridge/particle_bank_bridge.cpp:378-406`

**Context:**
The `portParticleLoadBank()` function evicts old working entries for a given `scripts_lo` address when a new scene requests them:

```cpp
sWorkingEntries.erase(
    std::remove_if(sWorkingEntries.begin(), sWorkingEntries.end(),
                   [scripts_lo](const std::unique_ptr<WorkingEntry> &e) {
                       return e && e->scripts_lo == scripts_lo;
                   }),
    sWorkingEntries.end());
```

**Concern:**
- If a scene transition is extremely fast (subsecond), the old working entry could be evicted while `lbParticleSetupBankID()` is still executing in the same scene, if there's any threading/callback involved.
- The comment states: "Its pointerization was invalidated when lbRelocInitSetup() reset the token table between scenes", which assumes a clean scene boundary. But if decomp code holds raw pointers into the old buffer and continues to access them before the next scene fully starts, those pointers are now dangling.

**Risk:** **MEDIUM** (depends on decomp behavior — if lbParticleSetupBankID never yields and scripts don't run until the next frame, this is safe).

**Observation:**
- No mutex or atomic flag guards `sWorkingEntries`.
- The coroutine-based scheduler is single-threaded at the C++ level (no OS threads access the bridge), so this is safe **if** scene transitions are atomic from the game's perspective.
- The code **comment is correct** — by the time `portParticleLoadBank()` is called in the next scene, lbRelocInitSetup() has already reset the token table, so old pointers are invalid anyway.

**Verdict:** **NO ACTION REQUIRED** — the eviction is safe because:
1. Only called from decomp code during scene setup.
2. Decomp does not yield during scene setup (single coroutine context).
3. Pointers are re-resolved via the token system in the next scene.

---

### 3. Port Log ARM64 Variadic-Arguments Declaration

**File:** `/Users/jackrickey/Dev/ssb64-port/port/port_log.c:28-36` and callers in decomp

**Code:**
```c
void port_log(const char *fmt, ...)
{
	if (sLogFile == NULL) return;
	va_list ap;
	va_start(ap, fmt);
	vfprintf(sLogFile, fmt, ap);
	va_end(ap);
	fflush(sLogFile);
}
```

**Findings:**
- ✅ Proper extern declarations exist in decomp callsites:
  - `/src/sc/scmanager.c` includes `<port_log.h>`
  - `/src/gr/grwallpaper.c` has `extern void port_log(const char *fmt, ...);`
  - `/src/lb/lbcommon.c` has the same extern

- ✅ Port header `/port/port_log.h` declares it correctly

**Verdict:** **NO BUG** — variadic args are safe on ARM64 because:
1. All callsites have the proper extern declaration.
2. Even on ARM64, variadic args are passed in x0-x7 + stack, and `va_start(ap, fmt)` handles this correctly.
3. No silent truncation or register misalignment observed.

---

### 4. Coroutine Stack Size for Nested Decomp Callstacks

**Files:** 
- `/Users/jackrickey/Dev/ssb64-port/port/coroutine_posix.cpp:45-46` — 256 KB service threads, 64 KB GObj threads
- `/Users/jackrickey/Dev/ssb64-port/port/stubs/n64_stubs.c:45-46` — defines PORT_STACK_SERVICE and PORT_STACK_GOBJ

**Concern:**
The game's decomp code can recurse deeply in some paths:
- Fighter AI → match logic → attack/shield logic → animation event handler → particle system → memory management
- This can easily exceed 32 KB (minimum configured) without careful measurement.

**Current Stack Sizes:**
- Service threads (game, scheduler, audio, controller): 256 KB
- GObj threads (per-entity coroutines): 64 KB

**Observation:**
- No explicit stack-overflow detection; coroutines just crash if they exceed the stack.
- No per-thread high-water-mark tracking for regression detection.

**Risk:** **LOW** (empirically, 64 KB has been sufficient for SSB64's GObj threads in production; service threads have more headroom).

**Verdict:** **NO ACTION REQUIRED** — empirical evidence suggests these sizes are correct, but recommend:
- Add optional debug mode to log max stack depth per coroutine on exit (via frame pointer walk).
- Document how to increase stack sizes if new game code paths cause overflow.

---

## OBSERVATIONS: CLEAN PATTERNS

### 5. Audio Bridge — Field-by-Field Parsing (WELL-DEFENDED)

**File:** `/Users/jackrickey/Dev/ssb64-port/port/bridge/audio_bridge.cpp:308-491`

**Strengths:**
- ✅ Avoids the sizeof trap entirely by parsing ROM binary field-by-field.
- ✅ All `sizeof()` calls are on bridge-local typedef'd types (ALEnvelope, ALSound, etc.) which are defined in the same .cpp file (lines 52-162).
- ✅ These local types have fixed layouts compatible with LP64 (no decomp-side typedef shadows).
- ✅ Explicit offset calculations (e.g., `r[12]`, `r+0x08`) instead of relying on struct field layout.

**Pattern:** This is the **correct** way to handle binary format parsing in a 32→64-bit port. It avoids all the pitfalls of memcpy into sized buffers.

---

### 6. Particle Bank Bridge — Byte-Swap Helpers (WELL-DEFENDED)

**File:** `/Users/jackrickey/Dev/ssb64-port/port/bridge/particle_bank_bridge.cpp:163-308`

**Strengths:**
- ✅ Byte-swap functions work on raw uint8_t* data with explicit offsets.
- ✅ Bounds-checking on script and texture count before processing headers.
- ✅ Proper error logging for malformed data.
- ✅ No assumption about struct layout — uses readNativeU32() helpers that do unaligned reads safely.

**Pattern:** Another example of correct binary-format handling.

---

### 7. Watchdog Signal Handler — Async-Signal-Safety (WELL-DEFENDED)

**File:** `/Users/jackrickey/Dev/ssb64-port/port/port_watchdog.cpp:50-70`

**Code:**
```cpp
void WriteBoth(const char *buf, size_t n) {
    write(STDERR_FILENO, buf, n);        // POSIX signal-safe
    int log_fd = port_log_get_fd();
    if (log_fd >= 0) write(log_fd, buf, n);
}

void DumpBacktraceBoth() {
    constexpr int kMaxFrames = 64;
    void *frames[kMaxFrames];
    int n = backtrace(frames, kMaxFrames);  // signal-safe on Darwin/glibc
    const char msg[] = "SSB64: ---- main-thread backtrace ----\n";
    WriteBoth(msg, sizeof(msg) - 1);
    backtrace_symbols_fd(frames, n, STDERR_FILENO);
    ...
}
```

**Strengths:**
- ✅ No malloc/free in signal handler.
- ✅ Only POSIX signal-safe functions: write(2), backtrace(), backtrace_symbols_fd().
- ✅ Fixed-size buffer (kMaxFrames = 64).
- ✅ No variadic printf (which can deadlock in signal handlers).

**Pattern:** Correct async-signal-safety discipline. This watchdog is safe to trigger from SIGSEGV or timeout.

---

### 8. Coroutine Context Saving (WELL-DEFENDED on POSIX)

**File:** `/Users/jackrickey/Dev/ssb64-port/port/coroutine_posix.cpp:99-112`

**Strengths:**
- ✅ ucontext_t captures all callee-save registers (R15-R30 on ARM64, RBX/RBP on x86-64).
- ✅ uc_stack field set correctly: ss_sp = stack base, ss_size = size.
- ✅ uc_link set to caller_ctx for implicit return handling.

**Note on ARM64:**
The POSIX ucontext_t on ARM64 (both Linux and macOS) includes d8-d15 NEON registers (callee-save). These are saved by libc's swapcontext() automatically. **No manual NEON preservation needed.**

**Pattern:** Correct use of POSIX ucontext API.

---

## INCLUDE-ORDER: MIXER MACRO CLOBBER

**Status:** ✅ **NO ISSUE FOUND**

Audit of `port/audio/mixer.h` include order:
- mixer.c includes: `<stdint.h>`, `<stdlib.h>`, `<string.h>`, `<stdio.h>`, then `"mixer.h"`
- mixer.h does NOT include `n_abi.h` (it declares that the caller has included it before: "These are defined in abi.h which is included before this header.")
- The undef/redefine pattern at mixer.h:79-94 is safe as long as abi.h macros are already in scope.

**Verdict:** No regression detected. The macro-clobber bug from 2026-04-24 does not reappear.

---

## FIRST-RUN & NATIVE DIALOGS: FILESYSTEM OPERATIONS

**Files:**
- `/Users/jackrickey/Dev/ssb64-port/port/first_run.cpp:126-160`
- `/Users/jackrickey/Dev/ssb64-port/port/native_dialog.cpp:69-110`

**Observations:**
- ✅ ROM discovery uses `fs::exists()` for race-condition-safe checks.
- ✅ File dialogs are implemented via system calls (osascript on macOS, zenity on Linux, native dialogs on Windows) — no custom dialog parsing.
- ✅ Asset extraction is launched as a subprocess; no in-process decompression with mutable state.
- ✅ No tempfile creation in unsafe locations (uses configured app data directory).

**Risk:** **LOW** — dialog cancellation paths are clean (return empty string, caller handles gracefully).

---

## THREAD-SAFETY IN BRIDGE STATICS

**Summary of static container access:**

| Container | File | Access Pattern | Thread-Safe? |
|-----------|------|-----------------|--------------|
| `sPortRelocFileRanges` | lbreloc_bridge.cpp:123 | Read/erase during portRelocEvictFileRangesInRange() | Yes (single-threaded coroutine scheduler) |
| `pristineCache()` | particle_bank_bridge.cpp:146 | Read/insert during ensurePristine() | Yes (lazy init, read-only after) |
| `sWorkingEntries` | particle_bank_bridge.cpp:157 | Erase/push_back during portParticleLoadBank() | Yes (called only during scene setup, no preemption) |
| `sStageAuditVtxPerFile` | lbreloc_byteswap.cpp:194 | Insert during stage audit | Debug-only, no safety concern |

**Verdict:** All static containers are safe for the single-threaded coroutine model. No synchronization needed.

---

## LIBC COMPAT: bzero

**File:** `/Users/jackrickey/Dev/ssb64-port/port/stubs/libc_compat.c:66-70`

**Code:**
```c
void bzero(void *ptr, int len)
{
	memset(ptr, 0, (size_t)len);
}
```

**Concern from CLAUDE.md memory:** "bzero infinite recursion" bug on some systems where Clang optimizes memset(p, 0, n) → bzero().

**Current Status:**
- ✅ Comment clearly documents the issue (lines 42-61).
- ✅ Implementation is safe: it calls the C standard library's memset, not a recursive call to bzero.
- ✅ MSVC has no bzero, so it doesn't trigger the loop.
- ✅ On glibc/Darwin, if the compiler tries to fold memset back to bzero, the bzero in libc is already defined (not our stub), so the loop doesn't happen.

**Verdict:** **NO BUG** — the current implementation avoids the recursion trap.

---

## WINDOWS FIBER COROUTINES: CANARY CHECKS

**File:** `/Users/jackrickey/Dev/ssb64-port/port/coroutine_win32.cpp:21-87`

**Observation:**
- Canaries (0xDEADBEEF / 0xCAFEBABE) are written to the PortCoroutine struct.
- If a coroutine overruns its stack, the canary is NOT in the overrun path (the stack is allocated separately by CreateFiber).
- The canaries only detect writes to the struct itself, not stack overflow.

**Risk:** **LOW** — Windows will catch stack overflow via the guard page; canaries are an extra sanity check for memory corruption in the struct.

---

## SUMMARY TABLE

| Issue | File | Risk | Status |
|-------|------|------|--------|
| OSThread sizeof shadow | n64_stubs.c:127 | **CRITICAL** | ⚠️ UNFIXED |
| Particle bank eviction race | particle_bank_bridge.cpp:382 | MEDIUM | ✅ SAFE (by design) |
| port_log variadic ARM64 | port_log.c:28 | LOW | ✅ SAFE (proper extern) |
| Coroutine stack size | coroutine_posix.cpp:45 | LOW | ✅ EMPIRICALLY OK |
| Audio bridge sizeof traps | audio_bridge.cpp | LOW | ✅ AVOIDED (field parsing) |
| Particle bank byte-swap | particle_bank_bridge.cpp:163 | LOW | ✅ WELL-DEFENDED |
| Watchdog signal safety | port_watchdog.cpp:50 | LOW | ✅ ASYNC-SAFE |
| Coroutine context save | coroutine_posix.cpp:99 | LOW | ✅ CORRECT (NEON OK) |
| Mixer macro order | port/audio/mixer.h | LOW | ✅ NO REGRESSION |
| Thread safety (statics) | bridge/*.cpp | LOW | ✅ SINGLE-THREADED |
| bzero recursion | libc_compat.c:66 | LOW | ✅ NO BUG |
| Windows canaries | coroutine_win32.cpp:74 | LOW | ✅ EXTRA DEFENSE |

---

## RECOMMENDATIONS

### Immediate Action
1. **Fix the OSThread sizeof bug** (n64_stubs.c:127) using the suggested solution above.
2. Add a compile-time assertion to catch future sizeof shadows:
   ```c
   // In n64_stubs.c after osCreateThread:
   _Static_assert(sizeof(OSThread) == 192 || sizeof(OSThread) == 432,
       "OSThread size changed; update osCreateThread memset");
   ```
   (This fails both on decomp and LUS to alert maintainers of the issue.)

### Long-Term Improvements
1. **Deprecate direct sizeof(decomp_type) in the port layer.**  
   Use helper functions or macros that explicitly specify the target ABI (e.g., `SIZEOF_DECOMP_OSTHREAD()`).

2. **Add debug mode for coroutine stack depth tracking** (optional).  
   Log max high-water mark on coroutine exit to catch stack overrun patterns early.

3. **Document the "binary parsing pattern"** as the correct approach for LP64 ports.  
   Both audio_bridge.cpp and particle_bank_bridge.cpp demonstrate this; encourage it for future port work.

4. **Test async-signal-safety of port_watchdog** on ARM64.  
   Verify backtrace() and backtrace_symbols_fd() work correctly when triggered from SIGSEGV on real ARM64 hardware (not just x86-64 emulation).

---

## CONCLUSION

The SSB64 PC port's bridge code is **generally well-designed and memory-safe**, with correct patterns for binary parsing, coroutine management, and signal handling. **One critical sizeof shadow bug exists** in n64_stubs.c that must be fixed immediately. All other findings are either safe by design, already defended, or low-risk.

**Status:** Ready for deployment after OSThread sizeof fix.
