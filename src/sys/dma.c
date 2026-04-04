#include "common.h"
#include <sys/dma.h>
#include <sys/scheduler.h>

#include <macros.h>
#include <ssb_types.h>

#include <PR/mbi.h>
#include <PR/os.h>
#include <PR/sp.h>
#include <PR/rcp.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x80045040
OSPiHandle *gSYDmaRomPiHandle;

// 0x80045048
OSPiHandle sSYDmaSramPiHandle;

// 0x800450BC
OSMesg sSYDmaMesg;

// 0x800450C0
OSMesgQueue sSYDmaMesgQueue;

// 0x800450D8
void *sSYDmaVpkBuffer;

// 0x800450DC
size_t sSYDmaVpkBufferSize;

// 0x800450E0
uintptr_t sSYDmaVpkDevAddr;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

void syDmaCreateMesgQueue(void) 
{
    osCreateMesgQueue(&sSYDmaMesgQueue, &sSYDmaMesg, OS_MESG_BLOCK);
}

void syDmaCopy(OSPiHandle *handle, uintptr_t phys, uintptr_t virtual, size_t size, u8 direction)
{
    OSIoMesg mesg;

    if (direction == OS_WRITE)
    {
        osWritebackDCache((void*)virtual, size);
    } 
    else osInvalDCache((void*)virtual, size);
    
    mesg.hdr.pri      = OS_MESG_PRI_NORMAL;
    mesg.hdr.retQueue = &sSYDmaMesgQueue;
    mesg.size         = 0x10000;

    while (size > 0x10000)
    {
        mesg.dramAddr = (void*)virtual;
        mesg.devAddr  = phys;

        if (!(gSYSchedulerIsSoftReset))
        {
            osEPiStartDma(handle, &mesg, direction);
        }
        osRecvMesg(&sSYDmaMesgQueue, NULL, OS_MESG_BLOCK);
        size -= 0x10000;
        phys += 0x10000;
        virtual += 0x10000;
    }
    if (size != 0) 
    {
        mesg.dramAddr = (void*)virtual;
        mesg.devAddr  = phys;
        mesg.size     = size;

        if (!(gSYSchedulerIsSoftReset)) 
        { 
            osEPiStartDma(handle, &mesg, direction);
        }
        osRecvMesg(&sSYDmaMesgQueue, NULL, OS_MESG_BLOCK);
    }
}

// 0x80002BE4
void syDmaLoadOverlay(SYOverlay *ovl)
{
    if ((ovl->ram_text_end - ovl->ram_text_start) != 0)
    {
        osInvalICache((void*) ovl->ram_text_start, ovl->ram_text_end - ovl->ram_text_start);
        osInvalDCache((void*) ovl->ram_text_start, ovl->ram_text_end - ovl->ram_text_start);
    }
    if ((ovl->ram_data_end - ovl->ram_data_start) != 0)
    {
        osInvalDCache((void*) ovl->ram_data_start, ovl->ram_data_end - ovl->ram_data_start);
    }
    if ((ovl->rom_end - ovl->rom_start) != 0)
    {
        syDmaCopy(gSYDmaRomPiHandle, ovl->rom_start, ovl->ram_load_start, ovl->rom_end - ovl->rom_start, OS_READ);
    }
    if ((ovl->ram_noload_end - ovl->ram_noload_start) != 0)
    {
        bzero((void*) ovl->ram_noload_start, ovl->ram_noload_end - ovl->ram_noload_start);
    }
}

// 0x80002CA0
void syDmaReadRom(uintptr_t rom_src, void *ram_dst, size_t size) 
{
    syDmaCopy(gSYDmaRomPiHandle, rom_src, (uintptr_t)ram_dst, size, OS_READ);
}

void syDmaWriteRom(void *ram_src, uintptr_t rom_dst, size_t size) 
{
    syDmaCopy(gSYDmaRomPiHandle, rom_dst, (uintptr_t)ram_src, size, OS_WRITE);
}

OSPiHandle* syDmaSramPiInit(void)
{
    if (sSYDmaSramPiHandle.baseAddress == PHYS_TO_K1(PI_DOM2_ADDR2))
    { 
        return &sSYDmaSramPiHandle; 
    }
    sSYDmaSramPiHandle.type        = DEVICE_TYPE_SRAM;
    sSYDmaSramPiHandle.baseAddress = PHYS_TO_K1(PI_DOM2_ADDR2);
    sSYDmaSramPiHandle.latency     = 5;
    sSYDmaSramPiHandle.pulse       = 12;
    sSYDmaSramPiHandle.pageSize    = 13;
    sSYDmaSramPiHandle.relDuration = 2;
    sSYDmaSramPiHandle.domain      = PI_DOMAIN2;
    sSYDmaSramPiHandle.speed       = 0;
    // once again, not sizeof(sSYDmaSramPiHandle.transferInfo)...
    bzero(&sSYDmaSramPiHandle.transferInfo, 0x60);
    osEPiLinkHandle(&sSYDmaSramPiHandle);

    return &sSYDmaSramPiHandle;
}

// 0x80002DA4
void syDmaReadSram(uintptr_t rom_src, void *ram_dst, size_t size)
{
    syDmaCopy(&sSYDmaSramPiHandle, rom_src, (uintptr_t)ram_dst, size, OS_READ);
}

// 0x80002DE0
void syDmaWriteSram(void *ram_src, uintptr_t rom_dst, size_t size)
{
    syDmaCopy(&sSYDmaSramPiHandle, rom_dst, (uintptr_t)ram_src, size, OS_WRITE);
}

// 0x80002E18 - vpk0 decoder
void syDmaDecodeVpk0(u16 *data, size_t size, void (*update_stream)(void), u8 *out_buf)
{
#define VPK0_UPDATE_STREAM()        \
    if ((uintptr_t) csr >= bound)   \
    {                               \
        update_stream();            \
        csr = data;                 \
    }

#define VPK0_READ_USHORT()  \
    temp_value <<= 0x10;    \
    temp_value |= *(csr++); \
    bits_num += 0x10;

#define VPK0_GET_BITS(var, n) \
    if (bits_num < n) {       \
        VPK0_UPDATE_STREAM(); \
        VPK0_READ_USHORT();   \
    }                         \
    bits_num -= n;            \
    var = ((temp_value << ((32 - n) - bits_num)) >> (32 - (u32) (n)));

#define VPK0_INIT_NODE(node)    \
    node = lengths_node;        \
    lengths_node->left = NULL;  \
    lengths_node->right = NULL; \
    lengths_node->value = 0;    \
    lengths_node++;

    uintptr_t bound = (uintptr_t) ((uintptr_t)data + size);
    SYHuffmanNode *sample1_node;
    SYHuffmanNode *lengths_node;
    SYHuffmanNode sp14C[64];
    u8* out_ptr;
    SYHuffmanNode *offsets_tree;
    SYHuffmanNode *lengths_tree;
    u8* copy_src;
    void* out_buf_end;
    u32 sample_method;
    SYHuffmanNode *off_stack[20];
    s32 off_stack_size;
    s32 value;
    SYHuffmanNode *offsets_node;
    SYHuffmanNode *off_node;
    SYHuffmanNode *lengths_stack[20];
    s32 lengths_stack_size;
    s32 unused2[2];
    SYHuffmanNode *length_node;
    s32 unused3[3];
    s32 sp64;
    s32 unused;
    u32 temp_value;
    s32 bits_num;
    u16* csr;

    bits_num = 0;
    temp_value = 0;
    lengths_node = sp14C;

    // read "vpk0" magic
    update_stream();

    csr = data;
    // clang-format off
    VPK0_READ_USHORT(); VPK0_UPDATE_STREAM(); VPK0_READ_USHORT(); bits_num -= 32;
    // clang-format on

    out_ptr = out_buf;

    // read size of decompressed data
    // clang-format off
    VPK0_UPDATE_STREAM(); VPK0_READ_USHORT(); VPK0_UPDATE_STREAM(); VPK0_READ_USHORT(); \
    bits_num -= 32; out_buf_end = ((temp_value << ((32 - 32) - bits_num)) >> (32 - (u32) (32))) + out_buf;
    // clang-format on

    // read sample method
    VPK0_GET_BITS(sample_method, 8);

    // read Huffman tree for offsets
    off_stack_size = 0;
    off_stack[0] = NULL;

    while (TRUE)
    {
        // leaf or node
        VPK0_GET_BITS(value, 1);

        // node, but less than 2 nodes on stack -> end of tree
        if ((value != 0) && (off_stack_size < 2))
        {
            break;
        }
        if (value != 0)
        {
            // node
            VPK0_INIT_NODE(off_node);

            // combine two nodes from stack
            off_node->left = off_stack[off_stack_size - 2];
            off_node->right = off_stack[off_stack_size - 1];

            // write current node on stack
            off_stack[off_stack_size - 2] = off_node;
            off_stack_size--;
        }
        else
        {
            // leaf
            VPK0_INIT_NODE(off_node);

            // read leaf node value
            VPK0_GET_BITS(off_node->value, 8);

            off_stack[off_stack_size] = off_node;
            off_stack_size++;
        }
    }
    offsets_tree = off_stack[0];

    // read Huffman tree for lengths
    lengths_stack_size = 0;
    lengths_stack[0] = 0;

    while (TRUE)
    {
        // leaf or node
        VPK0_GET_BITS(value, 1);

        // node, but less than 2 nodes on stack -> end of tree
        if ((value != 0) && (lengths_stack_size < 2))
        {
            break;
        }
        if (value != 0)
        {
            // node
            VPK0_INIT_NODE(length_node);

            // combine two nodes from stack
            length_node->left = lengths_stack[lengths_stack_size - 2];
            length_node->right = lengths_stack[lengths_stack_size - 1];

            // write current node on stack
            lengths_stack[lengths_stack_size - 2] = length_node;
            lengths_stack_size--;
        }
        else
        {
            // leaf
            VPK0_INIT_NODE(length_node);

            // read leaf node value
            VPK0_GET_BITS(length_node->value, 8);

            lengths_stack[lengths_stack_size] = length_node;
            lengths_stack_size++;
        }
    }
    lengths_tree = lengths_stack[0];

    while ((uintptr_t)out_ptr < (uintptr_t)out_buf_end)
    {
        VPK0_GET_BITS(value, 1);

        if (!value)
        {
            // byte value
            VPK0_GET_BITS(*out_ptr++, 8);
        }
        else
        {
            // encoded data
            lengths_node = lengths_tree;

            if (sample_method != 0)
            {
                // two samples
                sp64 = 0;
                sample1_node = offsets_tree;

                while (sample1_node->left != NULL)
                {
                    VPK0_GET_BITS(value, 1);
                    sample1_node = !value ? sample1_node->left : sample1_node->right;
                }
                VPK0_GET_BITS(value, sample1_node->value);

                if (value <= 2)
                {
                    sp64 = value + 1;
                    offsets_node = offsets_tree;

                    while (offsets_node->left != NULL)
                    {
                        VPK0_GET_BITS(value, 1);
                        offsets_node = !value ? offsets_node->left : offsets_node->right;
                    }
                    VPK0_GET_BITS(value, offsets_node->value);
                }
                copy_src = out_ptr - value * 4 - sp64 + 8;
            }
            else
            {
                // one sample
                offsets_node = offsets_tree;
                // get number of bytes to move back
                while (offsets_node->left != NULL)
                {
                    VPK0_GET_BITS(value, 1);
                    offsets_node = !value ? offsets_node->left : offsets_node->right;
                }
                VPK0_GET_BITS(value, offsets_node->value);
                // move back
                copy_src = out_ptr - value;
            }
            // get number of bytes to copy
            while (lengths_node->left != NULL)
            {
                VPK0_GET_BITS(value, 1);
                lengths_node = !value ? lengths_node->left : lengths_node->right;
            }
            VPK0_GET_BITS(value, lengths_node->value);

            while (value-- > 0)
            {
                *(out_ptr++) = *(copy_src++);
            }
        }
    }
}

void syDmaInitVpk0Stream(uintptr_t dev_addr, void *ram_addr, size_t size)
{
    sSYDmaVpkDevAddr = dev_addr;
    sSYDmaVpkBuffer = ram_addr;
    sSYDmaVpkBufferSize = size;
}

void syDmaFillVpk0Buf(void)
{
    syDmaReadRom(sSYDmaVpkDevAddr, sSYDmaVpkBuffer, sSYDmaVpkBufferSize);
    sSYDmaVpkDevAddr += sSYDmaVpkBufferSize;
}

#ifdef REGION_US
void syDmaReadVpk0Buf(uintptr_t dev_addr, void *ram_dst, void *ram_addr, size_t size)
{
    syDmaInitVpk0Stream(dev_addr, ram_addr, size);
    syDmaDecodeVpk0(ram_addr, size, syDmaFillVpk0Buf, ram_dst);
}

void syDmaReadVpk0(uintptr_t dev_addr, void *ram_dst)
{
    u16 buf[0x200];

    syDmaReadVpk0Buf(dev_addr, ram_dst, buf, sizeof(buf));
}
#else
void syDmaReadVpk0(uintptr_t dev_addr, void *ram_dst)
{
    u16 buf[0x200];

    syDmaInitVpk0Stream(dev_addr, &buf, sizeof(buf));
    syDmaDecodeVpk0(buf, sizeof(buf), syDmaFillVpk0Buf, ram_dst);
}
#endif

// Best I can do with this is functionally equivalent. Somewhat disappointing, but not a big deal; this function is unreferenced. It's also non-matching in Pokémon Snap.
#pragma GLOBAL_ASM("asm/nonmatchings/sys/dma/unref_800036B4.s")
