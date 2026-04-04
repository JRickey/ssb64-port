#ifndef _LBRELOC_H_
#define _LBRELOC_H_

#include <ssb_types.h>
#include <lb/lbdef.h>

/* 
// Can't actually use this in a matching build, newline memes :(
#define rdManagerSetupCommonFiles(status_buffer, force_status_buffer)                                   \
{                                                                                                       \
    LBRelocSetup rl_setup;                                                                              \
    rl_setup.table_addr = (uintptr_t)&lLBRelocTableAddr;                                                \
    rl_setup.table_files_num = (u32)&llRelocFileCount;                                                  \
    rl_setup.file_heap = NULL;                                                                          \
    rl_setup.file_heap_size = 0;                                                                        \
    rl_setup.status_buffer = status_buffer;                                                             \
    rl_setup.status_buffer_size = (status_buffer) ? ARRAY_COUNT(status_buffer) : 0;                     \
    rl_setup.force_status_buffer = force_status_buffer;                                                 \
    rl_setup.force_status_buffer_size = (force_status_buffer) ? ARRAY_COUNT(force_status_buffer) : 0;   \
    lbRelocInitSetup(&rl_setup);                                                                        \
    lbRelocLoadFilesExtern                                                                              \
    (                                                                                                   \
        dGMCommonFileIDs,                                                                               \
        ARRAY_COUNT(dGMCommonFileIDs),                                                                  \
        gGMCommonFiles,                                                                                 \
        syTaskmanMalloc                                                                                 \
        (                                                                                               \
            lbRelocGetAllocSize                                                                         \
            (                                                                                           \
                dGMCommonFileIDs,                                                                       \
                ARRAY_COUNT(dGMCommonFileIDs)                                                           \
            ),                                                                                          \
            0x10                                                                                        \
        )                                                                                               \
    );                                                                                                  \
}
*/

#define lbRelocLoadFilesListed(file_ids, out_ptrs)  \
lbRelocLoadFilesExtern                              \
(                                                   \
    file_ids,                                       \
    ARRAY_COUNT(file_ids),                          \
    out_ptrs,                                       \
    syTaskmanMalloc                                 \
    (                                               \
        lbRelocGetAllocSize                         \
        (                                           \
            file_ids,                               \
            ARRAY_COUNT(file_ids)                   \
        ),                                          \
        0x10                                        \
    )                                               \
)

extern uintptr_t lLBRelocTableAddr;         // 0x001AC870

extern void* lbRelocFindStatusBufferFile(u32 id);
extern void* lbRelocGetStatusBufferFile(u32 id);
extern void* lbRelocFindForceStatusBufferFile(u32 id);
extern void* lbRelocGetForceStatusBufferFile(u32 id);
extern void lbRelocAddStatusBufferFile(u32 id, void *addr);
extern void lbRelocAddForceStatusBufferFile(u32 id, void *addr);
extern void lbRelocReadDmaTableEntry(u32 entry_id);
extern void lbRelocLoadAndRelocFile(u32 id, void *ram_dst, u32 bytes_num, s32 loc);
extern size_t lbRelocGetExternBytesNum(u32 id);
extern size_t lbRelocGetFileSize(u32 id);
extern void* lbRelocGetExternBufferFile(u32 id);
extern void* lbRelocGetExternHeapFile(u32 id, void *heap);
extern void* lbRelocGetInternBufferFile(u32 id);
extern void* lbRelocGetForceExternBufferFile(u32 id);
extern void* lbRelocGetForceExternHeapFile(u32 id, void *heap);
extern size_t lbRelocLoadFilesExtern(u32 *ids, u32 len, void **files, void *heap);
extern size_t lbRelocLoadFilesIntern(u32 *ids, u32 len, void **files);
extern size_t lbRelocGetAllocSize(u32 *ids, u32 len);
extern void lbRelocInitSetup(LBRelocSetup *setup);

#endif
