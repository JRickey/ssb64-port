#include "malloc.h"

#include <sys/debug.h>
#include <ssb_types.h>
#include <PR/ultratypes.h>

void syMallocReset(SYMallocRegion *bp)
{
    bp->ptr = bp->start;
}

void* syMallocSet(SYMallocRegion *bp, size_t size, u32 alignment)
{
    u8 *aligned;
    u32 offset;

    if (alignment != 0) 
    {
        offset  = alignment - 1;
        aligned = (u8*) (((uintptr_t)bp->ptr + (offset)) & ~(offset));
    } 
    else aligned = (u8*) bp->ptr;

    bp->ptr = (void*) (aligned + size);

    if (bp->end < bp->ptr)
    {
        syDebugPrintf("ml : alloc overflow #%d\n", bp->id);

        while (TRUE);
    }
    return (void*) aligned;
}

void syMallocInit(SYMallocRegion *bp, u32 id, void *start, size_t size)
{
    bp->id    = id;
    bp->ptr   = start;
    bp->start = start;
    bp->end   = (void*) ((uintptr_t)start + size);
}
