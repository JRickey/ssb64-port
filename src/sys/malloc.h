#ifndef _ML_H_
#define _ML_H_

#include <PR/ultratypes.h>

// SYMallocRegion?
typedef struct SYMallocRegion 
{
    u32 id;
    void *start;
    void *end;
    void *ptr;

} SYMallocRegion;

extern void syMallocInit(SYMallocRegion *buf, u32 id, void *start, size_t size);
extern void* syMallocSet(SYMallocRegion *buf, size_t size, u32 alignment);
extern void syMallocReset(SYMallocRegion *buf);

#endif /* _ML_H_ */
