/**
 * libc_compat.c — Compatibility wrappers for IDO compiler built-ins and
 * BSD libc functions used by the decomp.
 *
 * The original N64 code was compiled with the SGI IDO 7.1 compiler, which
 * provides __sinf/__cosf as compiler built-ins and links against a BSD-
 * flavored libc.  On modern toolchains (MSVC, GCC, Clang) these symbols
 * don't exist, so we provide thin wrappers to the standard equivalents.
 */

#include <ssb_types.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <PR/xstdio.h>

/* ========================================================================= */
/*  IDO math built-ins                                                       */
/* ========================================================================= */

/*
 * IDO's __sinf and __cosf are single-precision math built-ins.
 * Declared in include/common.h as: f32 __sinf(f32); f32 __cosf(f32);
 */

f32 __cosf(f32 angle)
{
	return cosf(angle);
}

f32 __sinf(f32 angle)
{
	return sinf(angle);
}

/* ========================================================================= */
/*  BSD libc                                                                 */
/* ========================================================================= */

/*
 * bzero is the BSD equivalent of memset(ptr, 0, len).
 *
 * Platform handling:
 *
 *   - macOS / Linux (clang/gcc): every libc we target (glibc, musl, macOS
 *     libSystem) already exports `bzero`, so we must NOT provide our own.
 *     A previous version of this file shipped
 *         void bzero(void *p, unsigned int n) { memset(p, 0, n); }
 *     which had two fatal problems on macOS/arm64:
 *
 *       1. `unsigned int` dropped the upper 32 bits of the `size_t` length
 *          system callers pass in register x1.
 *       2. Clang recognises `memset(ptr, 0, len)` and lowers it back to a
 *          `bzero()` call — which re-entered our stub, recursed forever,
 *          and blew the thread stack inside `new Interpreter()` in Fast3D.
 *          See the "bzero infinite recursion" entry in CLAUDE.md.
 *
 *   - MSVC (Windows): the UCRT does NOT export `bzero` (Microsoft removed
 *     the deprecated POSIX names long ago), and MSVC does not perform the
 *     clang `memset(p,0,n) -> bzero` peephole, so a thin `memset` wrapper
 *     is safe and necessary. Use `size_t` for the length so 64-bit callers
 *     don't get truncated, matching the BSD prototype.
 */
#if defined(_MSC_VER)
/* Signature matches `extern void bzero(void*, int)` declared in
 * include/PR/os.h, which is what every decomp call site sees. */
void bzero(void *ptr, int len)
{
	memset(ptr, 0, (size_t)len);
}
#endif

/* ========================================================================= */
/*  IDO printf backend                                                       */
/* ========================================================================= */

/*
 * _Printf is IDO's internal formatted-output engine, used by the decomp's
 * debug printf system (src/sys/debug.c).  The full IDO implementation lives
 * in src/libultra/libc/xprintf.c with dependencies on xlitob, xldtob, etc.
 *
 * Rather than pulling in the full IDO printf chain (which has its own
 * integer/float formatting with non-standard quirks), we provide a minimal
 * wrapper that delegates to vsnprintf.  The outfun callback is called once
 * with the formatted result.
 *
 * Signature from include/PR/xstdio.h:
 *   typedef char* outfun(char*, const char*, size_t);
 *   int _Printf(outfun prout, char* arg, const char* fmt, va_list args);
 */

int _Printf(outfun prout, char *arg, const char *fmt, va_list args)
{
	char buf[512];
	int n = vsnprintf(buf, sizeof(buf), fmt, args);

	if (n > 0)
	{
		if ((size_t)n >= sizeof(buf))
		{
			n = sizeof(buf) - 1;
		}
		prout(arg, buf, (size_t)n);
	}

	return n;
}
