# CMakeLists warning-suppression audit — Next-session plan (2026-04-20)

## Why

`CMakeLists.txt:198-217` disables 15 clang warnings for the `ssb64_game` target to let decomp code compile unmodified on modern clang. That's reasonable for cosmetic/decomp-idiom warnings, but some of those flags silence diagnostics that *actively prevent* silent pointer truncation on LP64. Today's dropped-item-despawn crash (`docs/bugs/item_arrow_gobj_implicit_int_2026-04-20.md`) was exactly that kind of bug — clang would have errored at the call site, but `-Wno-implicit-function-declaration` hid it, so the `GObj*` return from `ifCommonItemArrowMakeInterface` was truncated to 32 bits on every assignment to `ip->arrow_gobj`.

This class of bug corrupts data at *runtime*, far from the source, and the fingerprint (fault address with upper 32 bits zeroed) only becomes obvious after the right diagnostic logging.

Updated `CLAUDE.md` directive #5: decomp preservation is about **game behavior**, not compiler compat shims. When a suppression masks real stability bugs on LP64, fix the root cause (usually a missing include) rather than keep the suppression.

## Current suppression list, categorized

### Category A — LP64 data-corruption landmines (promote to `-Werror=`)
Each of these can silently truncate a 64-bit pointer or integer to 32 bits. Leaving them as `-Wno-*` on a 64-bit host is a latent bug farm.

| Flag | Class of bug it hides |
|------|-----------------------|
| `-Wno-implicit-function-declaration` | undeclared wide-return → truncated to `int` (today's bug) |
| `-Wno-int-conversion` | `int` ⇄ pointer assignments lose upper 32 bits |
| `-Wno-incompatible-pointer-types` | struct-layout punning via mismatched pointers |
| `-Wno-incompatible-library-redeclaration` | libc redeclared with wrong signature (e.g., `malloc` → `int`) |
| `-Wno-return-mismatch` | `return 0;` inside a pointer-returning function → NULL-ish with UB tail bytes |
| `-Wno-return-type` | falling off a non-void function → whatever's in `x0` |

### Category B — real-bug hints, unlikely to silently corrupt LP64 data
Worth auditing eventually, but not emergency-grade.

- `-Wno-implicit-int` — variable declared without type (usually a typo)
- `-Wno-shift-negative-value` — UB but deterministic on our targets
- `-Wno-constant-conversion` — narrowing int literal
- `-Wno-tautological-constant-out-of-range-compare` — decomp macro artifacts

### Category C — cosmetic / decomp-idiom (leave off)
Decomp has many of these for matching; they're not hiding bugs.

- `-Wno-parentheses-equality`
- `-Wno-pointer-sign` (`char*` vs `unsigned char*`)
- `-Wno-unused-value`, `-Wno-unused-variable`, `-Wno-unused-but-set-variable`, `-Wno-unused-function`

## Proposed execution (one flag at a time)

1. Pick the first Category A flag. Change `-Wno-X` → `-Werror=X` in `CMakeLists.txt`.
2. `cmake --build build --target ssb64 2>&1 | grep -c error:` to scope the damage.
3. For each error, fix at the call site — usually a missing `#include` for the canonical prototype header. Occasionally the fix is to add a `#ifdef PORT` local prototype or correct the decomp declaration.
4. Rebuild clean. Verify game still boots + battle works.
5. Commit (`Promote -W<flag> to error + fixes for <N> callers`).
6. Repeat for the next Category A flag.

Order of attack (easiest → hardest, subjective):
1. `-Werror=implicit-function-declaration` — **DONE (2026-04-20)**. 521 errors across ~140 files, ~60 unique undeclared functions. Fix was almost entirely adding missing `#include`s to call-site files plus adding ~10 missing prototypes to existing headers (`efmanager.h`, `ftmanager.h`, `scheduler.h`, `gmcamera.h`, `objanim.h`, `sc1pgameboss.h`, `ftcommonfunctions.h`, `ftparam.h`). Custom `include/string.h` and `include/stdlib.h` shims needed LP64 branches (pull `size_t` from `<stddef.h>`, declare `abort`/`malloc`/`free`). Two Samus UB call sites (`ftCommonEscapeSetStatus` with 2 args instead of 3) were made deterministic by passing 0 — documented in the comment at each site. `func_800269C0_275C0` / `func_80026*_*` family (n_audio internals with no header) got local externs at each caller; ~50 files.
2. `-Werror=incompatible-library-redeclaration` — **DONE (2026-04-20)**. 3 unique decls (`bcopy`, `bcmp`, `bzero`) in `include/PR/os.h` had `int` as the size argument; POSIX/clang-builtin prototype uses `size_t`. Under LP64 these are truly incompatible — a huge size argument would be truncated. Fix: `#ifdef PORT` branch uses `size_t`; the original N64 `int` signatures are preserved for non-port builds. No caller changes needed.
3. `-Werror=return-type` (easy to fix per site; may need `return 0;` or `return NULL;` added)
4. `-Werror=return-mismatch` (usually paired with return-type)
5. `-Werror=int-conversion` (likely many; may need more care to distinguish real bugs from "decomp passes a constant that happens to be a pointer")
6. `-Werror=incompatible-pointer-types` (likely the largest blast radius; save for last)

## Fingerprint to recognize in future crash reports

A fault address with **upper 32 bits zero** where the lower 32 bits match the low half of a real heap pointer in the same run's log is the LP64-truncation signature. If you see that pattern and this audit isn't complete yet, suspect an undeclared wide-return function in the call stack before chasing anything else.

## Related

- `docs/bugs/item_arrow_gobj_implicit_int_2026-04-20.md` — the motivating incident
- `docs/bugs/controller_motorevt_lp64_2026-04-11.md` — earlier LP64 truncation, different vector
- `docs/bugs/osmesg_union_2026-04-11.md` — uninitialized upper-bytes via union alias
- `MEMORY.md` → *Implicit-int LP64 trunc trap*
