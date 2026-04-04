#include <lb/library.h>
#include <sys/debug.h>
#include <sys/dma.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x800D62E0
LBInternBuffer sLBRelocInternBuffer;

// this buffer and pointers are used to DMA table entry info from the rom
// as needed for reading in file data
// 0x800D6310
LBTableEntry sLBRelocTableEntries[nLBFileLocationEnumCount];

// 0x800D6334
LBTableEntry *sLBRelocCurrentTableEntry;

// 0x800D6338
LBTableEntry *sLBRelocNextTableEntry;

// external data pointers and buffers
// loaded files in an external heap
// 0x800D633C
u32 *sLBRelocExternFileIDs;

// 0x800D6340
s32 sLBRelocExternFileIDsNum;

// 0x800D6344
s32 sLBRelocExternFileIDsMax;

// 0x800D6348
void *sLBRelocExternFileHeap;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

void* lbRelocFindStatusBufferFile(u32 id)
{
    s32 i;

    if (sLBRelocInternBuffer.status_buffer_num == 0)
    {
        return NULL;
    }
    else for (i = 0; i < sLBRelocInternBuffer.status_buffer_num; i++)
    {
        if (id == sLBRelocInternBuffer.status_buffer[i].id)
        {
            return sLBRelocInternBuffer.status_buffer[i].addr;
        }
    }
    return NULL;
}

void* lbRelocGetStatusBufferFile(u32 id)
{
    return lbRelocFindStatusBufferFile(id);
}

void* lbRelocFindForceStatusBufferFile(u32 id)
{
    s32 i;

    if (sLBRelocInternBuffer.force_status_buffer_num != 0)
    {
        for (i = 0; i < sLBRelocInternBuffer.force_status_buffer_num; i++)
        {
            if (id == sLBRelocInternBuffer.force_status_buffer[i].id)
            {
                return sLBRelocInternBuffer.force_status_buffer[i].addr;
            }
        }
    }
    return lbRelocFindStatusBufferFile(id);
}

void* lbRelocGetForceStatusBufferFile(u32 id)
{
    return lbRelocFindForceStatusBufferFile(id);
}

void lbRelocAddStatusBufferFile(u32 id, void *addr)
{
    u32 num = sLBRelocInternBuffer.status_buffer_num;

    if (num >= sLBRelocInternBuffer.status_buffer_max)
    {
        while (TRUE)
        {
            syDebugPrintf("Relocatable Data Manager: Status Buffer is full !!\n");
            scManagerRunPrintGObjStatus();
        }
    }
    sLBRelocInternBuffer.status_buffer[num].id = id;
    sLBRelocInternBuffer.status_buffer[num].addr = addr;
    sLBRelocInternBuffer.status_buffer_num++;
}

void lbRelocAddForceStatusBufferFile(u32 id, void *addr)
{
    u32 num = sLBRelocInternBuffer.force_status_buffer_num;

    if (num >= sLBRelocInternBuffer.force_status_buffer_max)
    {
        while (TRUE)
        {
            syDebugPrintf("Relocatable Data Manager: Force Status Buffer is full !!\n");
            scManagerRunPrintGObjStatus();
        }
    }
    sLBRelocInternBuffer.force_status_buffer[num].id = id;
    sLBRelocInternBuffer.force_status_buffer[num].addr = addr;
    sLBRelocInternBuffer.force_status_buffer_num++;
}

void lbRelocReadDmaTableEntry(u32 entry_id)
{
    syDmaReadRom
    (
        sLBRelocInternBuffer.rom_table_lo + (entry_id * sizeof(LBTableEntry)),
        sLBRelocCurrentTableEntry,
        sizeof(LBTableEntry) * 2
    );
}

void lbRelocLoadAndRelocFile(u32 file_id, void *ram_dst, u32 bytes_num, s32 loc)
{
    u16 *file_id_extern;
    LBRelocDesc *intern_desc, *extern_desc;
    void *vaddr_extern;
    u16 reloc_intern, reloc_extern;
    u16 file_ids[8];
    uintptr_t data_rom_offset;

    data_rom_offset = sLBRelocInternBuffer.rom_table_hi + sLBRelocCurrentTableEntry->data_offset;

    if (sLBRelocCurrentTableEntry->is_compressed)
    {
        syDmaReadVpk0(data_rom_offset, ram_dst);
    }
    else syDmaReadRom(data_rom_offset, ram_dst, bytes_num);

    if (loc == nLBFileLocationForce)
    {
        lbRelocAddForceStatusBufferFile(file_id, ram_dst);
    }
    else lbRelocAddStatusBufferFile(file_id, ram_dst);
    
    reloc_intern = sLBRelocCurrentTableEntry->reloc_intern_offset;
    
    while (reloc_intern != 0xFFFF)
    {
        intern_desc = (LBRelocDesc*) ((uintptr_t)ram_dst + (reloc_intern * sizeof(u32)));
        reloc_intern = intern_desc->info.reloc;
        intern_desc->p = (void*) ((intern_desc->info.words_num * sizeof(u32)) + (uintptr_t)ram_dst);
    }
    reloc_extern = sLBRelocCurrentTableEntry->reloc_extern_offset;
    
    data_rom_offset += sLBRelocCurrentTableEntry->compressed_size * sizeof(u32);
    
    file_id_extern = (void*) LBRELOC_CACHE_ALIGN((uintptr_t)file_ids);
    
    while (reloc_extern != 0xFFFF)
    {
        extern_desc = (LBRelocDesc*) ((uintptr_t)ram_dst + (reloc_extern * sizeof(u32)));
        reloc_extern = extern_desc->info.reloc;

        syDmaReadRom(data_rom_offset, file_id_extern, sizeof(u16));
        
        if (loc == nLBFileLocationForce)
        {
            vaddr_extern = lbRelocFindForceStatusBufferFile(*file_id_extern);
        }
        else vaddr_extern = lbRelocFindStatusBufferFile(*file_id_extern);

        if (vaddr_extern == NULL)
        {
            switch (loc)
            {
            case nLBFileLocationExtern:
                vaddr_extern = lbRelocGetExternBufferFile(*file_id_extern);
                break;
                
            case nLBFileLocationDefault:
                vaddr_extern = lbRelocGetInternBufferFile(*file_id_extern);
                break;
                
            case nLBFileLocationForce:
                vaddr_extern = lbRelocGetForceExternBufferFile(*file_id_extern);
                break;
            }
        }
        extern_desc->p = (void*)((extern_desc->info.words_num * sizeof(u32)) + (uintptr_t)vaddr_extern);
        data_rom_offset += sizeof(u16);

        continue;
    }
}

size_t lbRelocGetExternBytesNum(u32 file_id)
{
    u16 *rom_extern_csr;
    u16 *file_id_read;
    size_t bytes_read;
    u16 *end;
    size_t compressed_size;
    void *rom_end;
    s32 unused;
    u8 file_ids_buf[16];
    s32 i;

    if (lbRelocFindStatusBufferFile(file_id) != NULL)
    {
        return 0;
    }
    else for (i = 0; i < sLBRelocExternFileIDsNum; i++)
    {
        if (file_id == sLBRelocExternFileIDs[i])
        {
            return 0;
        }
    }
    if (sLBRelocExternFileIDsNum >= sLBRelocExternFileIDsMax)
    {
        while (TRUE)
        {
            syDebugPrintf("Relocatable Data Manager: External Data is over %d!!\n", sLBRelocExternFileIDsMax);
            scManagerRunPrintGObjStatus();
        }
    }
    lbRelocReadDmaTableEntry(file_id);
    
    bytes_read = (u32) LBRELOC_CACHE_ALIGN(sLBRelocCurrentTableEntry->decompressed_size * sizeof(u32));
    compressed_size = sLBRelocCurrentTableEntry->compressed_size * sizeof(u32);
        
    sLBRelocExternFileIDs[sLBRelocExternFileIDsNum++] = file_id;
        
    rom_end = (void*) (sLBRelocInternBuffer.rom_table_hi + sLBRelocCurrentTableEntry->data_offset); // Ew...

    rom_extern_csr = (u16*) ((uintptr_t)rom_end + compressed_size);
    
    end = (u16*) (sLBRelocInternBuffer.rom_table_hi + sLBRelocNextTableEntry->data_offset);

    file_id_read = (void*) LBRELOC_CACHE_ALIGN((uintptr_t)file_ids_buf);
    
    while (rom_extern_csr < end)
    {
        syDmaReadRom((uintptr_t)rom_extern_csr, file_id_read, sizeof(u16));
        bytes_read += lbRelocGetExternBytesNum(*file_id_read);
        rom_extern_csr++;
    }
    return bytes_read;
}

// calc space needed to load `id`
size_t lbRelocGetFileSize(u32 id)
{
    u32 file_ids_extern_buf[50];

    sLBRelocExternFileIDs = file_ids_extern_buf;
    sLBRelocExternFileIDsNum = 0;
    sLBRelocExternFileIDsMax = ARRAY_COUNT(file_ids_extern_buf);

    return lbRelocGetExternBytesNum(id);
}

/* 
 * Get a pointer to the start of a file.
 * This will copy the file from ROM into RAM if necessary
 */
void* lbRelocGetExternBufferFile(u32 id)
{
    void *file_alloc;
    void *file;
    size_t file_size;

    file = lbRelocFindStatusBufferFile(id);

    if (file != NULL)
    {
        return file;
    }
    file_alloc = (void*) LBRELOC_CACHE_ALIGN((uintptr_t)sLBRelocExternFileHeap);
    lbRelocReadDmaTableEntry(id);

    file_size = sLBRelocCurrentTableEntry->decompressed_size * sizeof(u32);
    sLBRelocExternFileHeap = (void*) ((uintptr_t)file_alloc + file_size);

    lbRelocLoadAndRelocFile(id, file_alloc, file_size, nLBFileLocationExtern);

    return file_alloc;
}

void* lbRelocGetExternHeapFile(u32 id, void *heap)
{
    sLBRelocExternFileHeap = heap;
    return lbRelocGetExternBufferFile(id);
}

void* lbRelocGetInternBufferFile(u32 id)
{
    size_t file_size;
    void *file;
    void *file_alloc, *file_alloc_end, *heap_end;

    file = lbRelocFindStatusBufferFile(id);
    
    if (file != NULL)
    {
        return file;
    }
    file_alloc = (void*) LBRELOC_CACHE_ALIGN((uintptr_t)sLBRelocInternBuffer.heap_ptr);
    lbRelocReadDmaTableEntry(id);

    file_size = sLBRelocCurrentTableEntry->decompressed_size * sizeof(u32);
    
    if (((uintptr_t)file_alloc + file_size) > (uintptr_t)sLBRelocInternBuffer.heap_end)
    {
        while (TRUE)
        {
            syDebugPrintf("Relocatable Data Manager: Buffer is full !!\n");
            scManagerRunPrintGObjStatus();
        }
    }
    sLBRelocInternBuffer.heap_ptr = ((uintptr_t)file_alloc + file_size);
    
    lbRelocLoadAndRelocFile(id, file_alloc, file_size, nLBFileLocationDefault);

    return file_alloc;
}

void* lbRelocGetForceExternBufferFile(u32 id)
{
    void *file_alloc;
    void *file;
    size_t file_size;

    file = lbRelocFindForceStatusBufferFile(id);

    if (file != NULL)
    {
        return file;
    }
    file_alloc = (void*) LBRELOC_CACHE_ALIGN((uintptr_t)sLBRelocExternFileHeap);
    lbRelocReadDmaTableEntry(id);

    file_size = sLBRelocCurrentTableEntry->decompressed_size * sizeof(u32);
    sLBRelocExternFileHeap = (void*) ((uintptr_t)file_alloc + file_size);

    lbRelocLoadAndRelocFile(id, file_alloc, file_size, nLBFileLocationForce);

    return file_alloc;
}

void* lbRelocGetForceExternHeapFile(u32 id, void *heap)
{
    sLBRelocExternFileHeap = heap;
    sLBRelocInternBuffer.force_status_buffer_num = 0;

    return lbRelocGetForceExternBufferFile(id);
}

size_t lbRelocLoadFilesExtern(u32 *ids, u32 len, void **files, void *heap)
{
    sLBRelocExternFileHeap = heap;

    // doesn't match as for-loop..?
    while (len != 0) 
    {
        *files = lbRelocGetExternBufferFile(*ids);

        ids++;
        files++;
        len--;
    }

    // return the amount allocated
    return (size_t) ((uintptr_t)sLBRelocExternFileHeap - (uintptr_t)heap);
}

size_t lbRelocLoadFilesIntern(u32 *ids, u32 len, void **files)
{
    UNUSED s32 pad;
    void *heap = sLBRelocInternBuffer.heap_ptr;

    // doesn't match as for-loop..?
    while (len)
    {
        *files = lbRelocGetInternBufferFile(*ids);

        ids++;
        files++;
        len--;
    }

    // return the amount allocated
    return (size_t) ((uintptr_t)sLBRelocInternBuffer.heap_ptr - (uintptr_t)heap);
}

size_t lbRelocGetAllocSize(u32 *ids, u32 len)
{
    u32 file_ids[50];
    UNUSED u32 unused;
    size_t allocated;

    allocated = 0;

    sLBRelocExternFileIDs = file_ids;
    sLBRelocExternFileIDsNum = 0;
    sLBRelocExternFileIDsMax = ARRAY_COUNT(file_ids);

    while (len != 0)
    {
        allocated = LBRELOC_CACHE_ALIGN(allocated);
        allocated += lbRelocGetExternBytesNum(*ids);

        ids++;
        len--;
    }
    return allocated;
}

void lbRelocInitSetup(LBRelocSetup *setup)
{
    sLBRelocInternBuffer.rom_table_lo = setup->table_addr;
    sLBRelocInternBuffer.total_files_num = setup->table_files_num;
    sLBRelocInternBuffer.rom_table_hi = setup->table_addr + ((setup->table_files_num + 1) * sizeof(LBTableEntry));

    sLBRelocInternBuffer.heap_start = sLBRelocInternBuffer.heap_ptr = setup->file_heap;
    sLBRelocInternBuffer.heap_end = (void*) ((uintptr_t)setup->file_heap + setup->file_heap_size);

    sLBRelocCurrentTableEntry = (void*) LBRELOC_CACHE_ALIGN((uintptr_t)sLBRelocTableEntries);
    sLBRelocNextTableEntry = sLBRelocCurrentTableEntry + 1;

    sLBRelocInternBuffer.status_buffer_num = 0;
    sLBRelocInternBuffer.status_buffer_max = setup->status_buffer_size;
    sLBRelocInternBuffer.status_buffer = setup->status_buffer;

    sLBRelocInternBuffer.force_status_buffer_max = setup->force_status_buffer_size;
    sLBRelocInternBuffer.force_status_buffer = setup->force_status_buffer;
}
