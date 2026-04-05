/**
 * lbreloc_bridge.cpp — PORT replacement for src/lb/lbreloc.c
 *
 * Replaces ROM DMA-based file loading with LUS ResourceManager calls.
 * Uses the token-based pointer indirection system: the relocation logic
 * computes real 64-bit pointers but stores 32-bit tokens in the 4-byte
 * data slots. Game code resolves tokens via RELOC_RESOLVE().
 *
 * This file is compiled as C++ (needs LUS headers) but exports C-linkage
 * functions matching the signatures in src/lb/lbreloc.h.
 */

#include <ship/Context.h>
#include <ship/resource/ResourceManager.h>
#include <spdlog/spdlog.h>

#include <cstring>
#include <memory>

#include "resource/RelocFile.h"
#include "resource/RelocFileTable.h"
#include "resource/RelocPointerTable.h"
#include "bridge/lbreloc_byteswap.h"

// Bridge-local type definitions.
// These MUST be ABI-compatible with the decomp definitions in lbtypes.h.
// We define them here to avoid including the decomp's include/ directory
// (which shadows system headers and breaks C++ standard library includes).
#include "bridge/port_types.h"

#define LBRELOC_CACHE_ALIGN(x) (((x) + 0xF) & ~0xF)

enum {
	nLBFileLocationExtern  = 0,
	nLBFileLocationDefault = 1,
	nLBFileLocationForce   = 2,
};

struct LBFileNode
{
	u32 id;
	void *addr;
};

struct LBRelocSetup
{
	uintptr_t table_addr;
	u32 table_files_num;
	void *file_heap;
	size_t file_heap_size;
	LBFileNode *status_buffer;
	size_t status_buffer_size;
	LBFileNode *force_status_buffer;
	size_t force_status_buffer_size;
};

struct LBInternBuffer
{
	uintptr_t rom_table_lo;
	u32 total_files_num;
	uintptr_t rom_table_hi;
	void *heap_start;
	void *heap_ptr;
	void *heap_end;
	s32 status_buffer_num;
	s32 status_buffer_max;
	LBFileNode *status_buffer;
	s32 force_status_buffer_num;
	s32 force_status_buffer_max;
	LBFileNode *force_status_buffer;
};

// Same size as the N64 LBTableEntry (12 bytes) — used for size calculation
struct LBTableEntry
{
	ub32 is_compressed : 1;
	u32 data_offset : 31;
	u16 reloc_intern_offset;
	u16 compressed_size;
	u16 reloc_extern_offset;
	u16 decompressed_size;
};

// Forward declarations (C linkage)
extern "C" {
extern void syDebugPrintf(const char *fmt, ...);
extern void scManagerRunPrintGObjStatus(void);

// Forward declarations for functions used in mutual recursion
void* lbRelocGetExternBufferFile(u32 id);
void* lbRelocGetInternBufferFile(u32 id);
void* lbRelocGetForceExternBufferFile(u32 id);
}

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

static LBInternBuffer sLBRelocInternBuffer;

static u32 *sLBRelocExternFileIDs;
static s32 sLBRelocExternFileIDsNum;
static s32 sLBRelocExternFileIDsMax;
static void *sLBRelocExternFileHeap;

// // // // // // // // // // // //
//                               //
//       RESOURCE LOADING        //
//                               //
// // // // // // // // // // // //

static std::shared_ptr<RelocFile> portLoadRelocResource(u32 file_id)
{
	if (file_id >= RELOC_FILE_COUNT || gRelocFileTable[file_id] == NULL)
	{
		spdlog::error("lbReloc bridge: invalid file_id {}", file_id);
		return nullptr;
	}

	auto ctx = Ship::Context::GetInstance();
	if (!ctx)
	{
		spdlog::error("lbReloc bridge: no Ship::Context");
		return nullptr;
	}

	std::string path(gRelocFileTable[file_id]);
	auto resource = ctx->GetResourceManager()->LoadResource(path);

	if (!resource)
	{
		spdlog::error("lbReloc bridge: failed to load '{}' (file_id={})", path, file_id);
		return nullptr;
	}

	auto relocFile = std::dynamic_pointer_cast<RelocFile>(resource);
	if (!relocFile)
	{
		spdlog::error("lbReloc bridge: '{}' is not a RelocFile", path);
		return nullptr;
	}

	return relocFile;
}

// All game-facing functions have C linkage
extern "C" {

// // // // // // // // // // // //
//                               //
//      STATUS BUFFER FUNCS      //
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

	if (num >= (u32)sLBRelocInternBuffer.status_buffer_max)
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

	if (num >= (u32)sLBRelocInternBuffer.force_status_buffer_max)
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

// // // // // // // // // // // //
//                               //
//   BRIDGE: LOAD & RELOCATE     //
//                               //
// // // // // // // // // // // //

/**
 * Load a file from the .o2r archive, copy into ram_dst, and perform
 * token-based internal + external pointer relocation.
 *
 * Instead of writing 64-bit void* into 4-byte slots (which would corrupt
 * adjacent data), we register each target pointer in the token table and
 * write the 32-bit token into the slot.
 */
void lbRelocLoadAndRelocFile(u32 file_id, void *ram_dst, u32 bytes_num, s32 loc)
{
	auto relocFile = portLoadRelocResource(file_id);

	if (!relocFile)
	{
		spdlog::error("lbReloc bridge: cannot load file_id {} — halting", file_id);
		return;
	}

	// Copy decompressed data into the game's heap allocation
	size_t copySize = relocFile->Data.size();
	if (copySize > bytes_num && bytes_num > 0)
	{
		spdlog::warn("lbReloc bridge: file_id {} data ({} bytes) exceeds "
		             "buffer ({} bytes), truncating", file_id, copySize, bytes_num);
		copySize = bytes_num;
	}
	memcpy(ram_dst, relocFile->Data.data(), copySize);

	// Byte-swap from N64 big-endian to native little-endian.
	// Must happen BEFORE the reloc chain walk (which reads u16 fields
	// from u32 words using bit shifts that assume native byte order).
	portRelocByteSwapBlob(ram_dst, copySize);

	// Register in status buffer
	if (loc == nLBFileLocationForce)
	{
		lbRelocAddForceStatusBufferFile(file_id, ram_dst);
	}
	else
	{
		lbRelocAddStatusBufferFile(file_id, ram_dst);
	}

	// --- Internal pointer relocation (token-based) ---
	//
	// Each reloc descriptor is a 4-byte word in the data:
	//   bits [31:16] = next descriptor offset (in words), 0xFFFF = end
	//   bits [15:0]  = target offset within this file (in words)
	//
	// On N64, the code overwrites this 4-byte word with a void* pointer.
	// In the port, we compute the pointer, register it as a token, and
	// write the 32-bit token into the 4-byte word.

	u16 reloc_intern = relocFile->RelocInternOffset;

	while (reloc_intern != 0xFFFF)
	{
		u32 *slot = (u32 *)((uintptr_t)ram_dst + (reloc_intern * sizeof(u32)));

		// Read the reloc info before we overwrite the slot
		u16 next_reloc = (u16)(*slot >> 16);
		u16 words_num  = (u16)(*slot & 0xFFFF);

		// Compute the real target pointer (within this file's data)
		void *target = (void *)((uintptr_t)ram_dst + (words_num * sizeof(u32)));

		// Register in the token table and write the 32-bit token
		u32 token = portRelocRegisterPointer(target);
		*slot = token;

		reloc_intern = next_reloc;
	}

	// --- External pointer relocation (token-based) ---
	//
	// Same chain structure, but the target is in a DIFFERENT file.
	// The extern file IDs come from the RelocFile metadata (extracted
	// by Torch at ROM-extraction time), not from ROM DMA.

	u16 reloc_extern = relocFile->RelocExternOffset;
	u32 extern_idx = 0;

	while (reloc_extern != 0xFFFF)
	{
		u32 *slot = (u32 *)((uintptr_t)ram_dst + (reloc_extern * sizeof(u32)));

		u16 next_reloc = (u16)(*slot >> 16);
		u16 words_num  = (u16)(*slot & 0xFFFF);

		if (extern_idx >= relocFile->ExternFileIds.size())
		{
			spdlog::error("lbReloc bridge: file_id {} extern reloc overrun "
			              "(idx={}, count={})", file_id, extern_idx,
			              relocFile->ExternFileIds.size());
			break;
		}

		u16 dep_file_id = relocFile->ExternFileIds[extern_idx];
		void *vaddr_extern;

		// Check if dependency is already loaded
		if (loc == nLBFileLocationForce)
		{
			vaddr_extern = lbRelocFindForceStatusBufferFile(dep_file_id);
		}
		else
		{
			vaddr_extern = lbRelocFindStatusBufferFile(dep_file_id);
		}

		// Load dependency if not cached
		if (vaddr_extern == NULL)
		{
			switch (loc)
			{
			case nLBFileLocationExtern:
				vaddr_extern = lbRelocGetExternBufferFile(dep_file_id);
				break;
			case nLBFileLocationDefault:
				vaddr_extern = lbRelocGetInternBufferFile(dep_file_id);
				break;
			case nLBFileLocationForce:
				vaddr_extern = lbRelocGetForceExternBufferFile(dep_file_id);
				break;
			}
		}

		// Compute target pointer (offset into the dependency file's data)
		void *target = (void *)((uintptr_t)vaddr_extern + (words_num * sizeof(u32)));

		u32 token = portRelocRegisterPointer(target);
		*slot = token;

		extern_idx++;
		reloc_extern = next_reloc;
	}
}

// // // // // // // // // // // //
//                               //
//    BRIDGE: SIZE CALCULATION   //
//                               //
// // // // // // // // // // // //

size_t lbRelocGetExternBytesNum(u32 file_id)
{
	s32 i;

	if (lbRelocFindStatusBufferFile(file_id) != NULL)
	{
		return 0;
	}

	for (i = 0; i < sLBRelocExternFileIDsNum; i++)
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
			syDebugPrintf("Relocatable Data Manager: External Data is over %d!!\n",
			              sLBRelocExternFileIDsMax);
			scManagerRunPrintGObjStatus();
		}
	}

	auto relocFile = portLoadRelocResource(file_id);
	if (!relocFile) { return 0; }

	size_t bytes_read = (u32)LBRELOC_CACHE_ALIGN(relocFile->Data.size());
	sLBRelocExternFileIDs[sLBRelocExternFileIDsNum++] = file_id;

	for (u16 dep_id : relocFile->ExternFileIds)
	{
		bytes_read += lbRelocGetExternBytesNum(dep_id);
	}

	return bytes_read;
}

size_t lbRelocGetFileSize(u32 id)
{
	u32 file_ids_extern_buf[50];

	sLBRelocExternFileIDs = file_ids_extern_buf;
	sLBRelocExternFileIDsNum = 0;
	sLBRelocExternFileIDsMax = ARRAY_COUNT(file_ids_extern_buf);

	return lbRelocGetExternBytesNum(id);
}

// // // // // // // // // // // //
//                               //
//     BRIDGE: FILE LOADING      //
//                               //
// // // // // // // // // // // //

void* lbRelocGetExternBufferFile(u32 id)
{
	void *file = lbRelocFindStatusBufferFile(id);
	if (file != NULL) { return file; }

	auto relocFile = portLoadRelocResource(id);
	if (!relocFile) { return NULL; }

	void *file_alloc = (void *)LBRELOC_CACHE_ALIGN((uintptr_t)sLBRelocExternFileHeap);
	size_t file_size = relocFile->Data.size();
	sLBRelocExternFileHeap = (void *)((uintptr_t)file_alloc + file_size);

	lbRelocLoadAndRelocFile(id, file_alloc, (u32)file_size, nLBFileLocationExtern);
	return file_alloc;
}

void* lbRelocGetExternHeapFile(u32 id, void *heap)
{
	sLBRelocExternFileHeap = heap;
	return lbRelocGetExternBufferFile(id);
}

void* lbRelocGetInternBufferFile(u32 id)
{
	void *file = lbRelocFindStatusBufferFile(id);
	if (file != NULL) { return file; }

	auto relocFile = portLoadRelocResource(id);
	if (!relocFile) { return NULL; }

	void *file_alloc = (void *)LBRELOC_CACHE_ALIGN((uintptr_t)sLBRelocInternBuffer.heap_ptr);
	size_t file_size = relocFile->Data.size();

	if (((uintptr_t)file_alloc + file_size) > (uintptr_t)sLBRelocInternBuffer.heap_end)
	{
		while (TRUE)
		{
			syDebugPrintf("Relocatable Data Manager: Buffer is full !!\n");
			scManagerRunPrintGObjStatus();
		}
	}
	sLBRelocInternBuffer.heap_ptr = (void *)((uintptr_t)file_alloc + file_size);

	lbRelocLoadAndRelocFile(id, file_alloc, (u32)file_size, nLBFileLocationDefault);
	return file_alloc;
}

void* lbRelocGetForceExternBufferFile(u32 id)
{
	void *file = lbRelocFindForceStatusBufferFile(id);
	if (file != NULL) { return file; }

	auto relocFile = portLoadRelocResource(id);
	if (!relocFile) { return NULL; }

	void *file_alloc = (void *)LBRELOC_CACHE_ALIGN((uintptr_t)sLBRelocExternFileHeap);
	size_t file_size = relocFile->Data.size();
	sLBRelocExternFileHeap = (void *)((uintptr_t)file_alloc + file_size);

	lbRelocLoadAndRelocFile(id, file_alloc, (u32)file_size, nLBFileLocationForce);
	return file_alloc;
}

void* lbRelocGetForceExternHeapFile(u32 id, void *heap)
{
	sLBRelocExternFileHeap = heap;
	sLBRelocInternBuffer.force_status_buffer_num = 0;
	return lbRelocGetForceExternBufferFile(id);
}

// // // // // // // // // // // //
//                               //
//     BRIDGE: BATCH LOADING     //
//                               //
// // // // // // // // // // // //

size_t lbRelocLoadFilesExtern(u32 *ids, u32 len, void **files, void *heap)
{
	sLBRelocExternFileHeap = heap;

	while (len != 0)
	{
		*files = lbRelocGetExternBufferFile(*ids);
		ids++;
		files++;
		len--;
	}

	return (size_t)((uintptr_t)sLBRelocExternFileHeap - (uintptr_t)heap);
}

size_t lbRelocLoadFilesIntern(u32 *ids, u32 len, void **files)
{
	void *heap = sLBRelocInternBuffer.heap_ptr;

	while (len)
	{
		*files = lbRelocGetInternBufferFile(*ids);
		ids++;
		files++;
		len--;
	}

	return (size_t)((uintptr_t)sLBRelocInternBuffer.heap_ptr - (uintptr_t)heap);
}

size_t lbRelocGetAllocSize(u32 *ids, u32 len)
{
	u32 file_ids[50];
	size_t allocated = 0;

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

// // // // // // // // // // // //
//                               //
//    BRIDGE: INITIALIZATION     //
//                               //
// // // // // // // // // // // //

void lbRelocInitSetup(LBRelocSetup *setup)
{
	// Clear token table from previous scene — prevents unbounded growth
	// and stale tokens pointing to freed heap memory
	portRelocResetPointerTable();

	// ROM addresses (unused in port but stored for completeness)
	sLBRelocInternBuffer.rom_table_lo = setup->table_addr;
	sLBRelocInternBuffer.total_files_num = setup->table_files_num;
	sLBRelocInternBuffer.rom_table_hi = setup->table_addr +
		((setup->table_files_num + 1) * sizeof(LBTableEntry));

	// Heap management (still used — callers allocate and pass heaps)
	sLBRelocInternBuffer.heap_start = sLBRelocInternBuffer.heap_ptr = setup->file_heap;
	sLBRelocInternBuffer.heap_end = (void *)((uintptr_t)setup->file_heap + setup->file_heap_size);

	// Status buffers (file caching)
	sLBRelocInternBuffer.status_buffer_num = 0;
	sLBRelocInternBuffer.status_buffer_max = setup->status_buffer_size;
	sLBRelocInternBuffer.status_buffer = setup->status_buffer;

	sLBRelocInternBuffer.force_status_buffer_max = setup->force_status_buffer_size;
	sLBRelocInternBuffer.force_status_buffer = setup->force_status_buffer;
}

} // extern "C"
