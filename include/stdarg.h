#ifndef STDARG_H
#define STDARG_H

// On MSVC, define va_list etc. using MSVC intrinsics
#ifdef _MSC_VER
#include <vadefs.h>
#ifndef va_start
#define va_start __crt_va_start
#define va_arg __crt_va_arg
#define va_end __crt_va_end
#define va_copy __crt_va_copy
#endif
#else

// When not building with IDO, use the builtin vaarg macros for portability.
#ifndef __sgi
#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_end __builtin_va_end

// glibc's <stdio.h> (and other headers) declare functions in terms of
// __gnuc_va_list, expecting <stdarg.h> to typedef it. This shadow
// header pre-empts the system <stdarg.h> via the project's include
// path ordering, so we must provide __gnuc_va_list ourselves on
// Linux/glibc — otherwise the first system header that uses it
// triggers a cascade of "unknown type name '__gnuc_va_list'" errors.
// Darwin's libc and MSVC's CRT don't reference this typedef.
#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif

#else

typedef char* va_list;
#define _FP 1
#define _INT 0
#define _STRUCT 2

#define _VA_FP_SAVE_AREA 0x10
#define _VA_ALIGN(p, a) (((unsigned int)(((char*)p) + ((a) > 4 ? (a) : 4) - 1)) & -((a) > 4 ? (a) : 4))
#define va_start(vp, parmN) (vp = ((va_list)&parmN + sizeof(parmN)))

#define __va_stack_arg(list, mode)                                                                                     \
	(((list) = (char*)_VA_ALIGN(list, __builtin_alignof(mode)) + _VA_ALIGN(sizeof(mode), 4)),                          \
	 (((char*)list) - (_VA_ALIGN(sizeof(mode), 4) - sizeof(mode))))

#define __va_double_arg(list, mode)                                                                                    \
	((((long)list & 0x1) /* 1 byte aligned? */                                                                         \
		  ? (list = (char*)((long)list + 7), (char*)((long)list - 6 - _VA_FP_SAVE_AREA))                               \
		  : (((long)list & 0x2) /* 2 byte aligned? */                                                                  \
				 ? (list = (char*)((long)list + 10), (char*)((long)list - 24 - _VA_FP_SAVE_AREA))                      \
				 : __va_stack_arg(list, mode))))

#define va_arg(list, mode)                                                                                             \
	((mode*)(((__builtin_classof(mode) == _FP && __builtin_alignof(mode) == sizeof(double))                            \
				  ? __va_double_arg(list, mode)                                                                        \
				  : __va_stack_arg(list, mode))))[-1]
#define va_end(__list)

#endif
#endif /* _MSC_VER */
#endif
