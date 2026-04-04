#ifndef __STDDEF_H__
#define __STDDEF_H__

#include <PR/ultratypes.h>

#ifndef __sgi
/* use built-in offsetof macro */
#define offsetof(type, member) __builtin_offsetof(type, member)
#else
/* use macro from Indy headers */
#define offsetof(s, m) (size_t)(&(((s*)0)->m))
#endif

#endif /* __STDDEF_H__ */
