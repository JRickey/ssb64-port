#ifndef __STRING_H__
#define __STRING_H__
#ifdef __cplusplus
extern "C"
{
#endif
#ifdef PORT
	/* LP64 host build: pull size_t from <stddef.h>. The original N64-targeted
	 * definitions below are keyed on _MIPS_SZLONG which isn't defined in the
	 * host build. */
	#include <stddef.h>
#else
#if !defined(_SIZE_T) && !defined(_SIZE_T_)
#define _SIZE_T
#if (_MIPS_SZLONG == 32)
	typedef unsigned int size_t;
#endif
#if (_MIPS_SZLONG == 64)
	typedef unsigned long size_t;
#endif
#endif
#endif

#ifndef NULL
#define NULL 0
#endif

	extern void* memcpy(void*, const void*, size_t);
	extern void* memset(void*, int, size_t);
	extern char* strchr(const char*, int);
	extern size_t strlen(const char*);

#ifdef __cplusplus
}
#endif
#endif /* !__STRING_H__ */
