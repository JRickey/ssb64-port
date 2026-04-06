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
    uintptr_t aligned_addr;
    uintptr_t offset = 0;

    if (alignment != 0) 
    {
        // alignment must be a power of two for mask-based rounding
        if ((alignment & (alignment - 1)) != 0)
        {
            syDebugPrintf("ml : invalid alignment #%d (%d)\n", bp->id, alignment);
            while (TRUE);
        }
        offset       = (uintptr_t)(alignment - 1);
        aligned_addr = ((uintptr_t)bp->ptr + offset) & ~offset;
        aligned      = (u8*)aligned_addr;
    } 
    else
    {
        aligned_addr = (uintptr_t)bp->ptr;
        aligned      = (u8*)bp->ptr;
    }

    bp->ptr = (void*)(aligned_addr + size);

    if (((uintptr_t)bp->end < (uintptr_t)bp->ptr) || ((uintptr_t)bp->ptr < (uintptr_t)bp->start))
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
