#ifndef LBRELOC_BYTESWAP_H
#define LBRELOC_BYTESWAP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Byte-swap a decompressed reloc file blob from N64 big-endian to native
 * little-endian. Must be called AFTER memcpy and BEFORE the reloc chain walk.
 *
 * Two-pass approach:
 *   Pass 1: Blanket u32 swap of every word (fixes DL commands, struct fields,
 *           reloc chain descriptors, 32bpp textures, zeros).
 *   Pass 2: Parse now-native-endian DL commands to find vertex and texture
 *           regions, then apply targeted fixups for u16 and byte-granular data.
 *
 * @param data  Pointer to the decompressed blob in game memory.
 * @param size  Size in bytes (must be a multiple of 4).
 */
void portRelocByteSwapBlob(void *data, size_t size);

/**
 * Apply rotate16 fixup to a region of u16 fields within a ROM-overlay struct.
 *
 * After blanket u32 swap, adjacent u16 pairs within each u32 word are
 * position-swapped. This function corrects them by rotating each u32 word
 * by 16 bits. Tracks which regions have been fixed to prevent double-fixup
 * (the same struct pointer may be loaded multiple times from the cached blob).
 *
 * @param base         Pointer to the struct base in game memory.
 * @param byte_offset  Byte offset of the u16 region within the struct.
 * @param num_words    Number of u32 words in the u16 region to fix.
 */
void portFixupStructU16(void *base, unsigned int byte_offset, unsigned int num_words);

/**
 * Clear the u16 fixup tracking set. Call when file heaps are freed
 * (e.g. on scene change) to prevent stale entries.
 */
void portResetStructFixups(void);

/**
 * Fix byte order for a Sprite struct (68 bytes) after blanket u32 swap.
 *
 * rotate16 for s16/u16 pair words (x/y, width/height, etc.)
 * bswap32 for u8 quad words (rgba, bmfmt/bmsiz)
 * Idempotent: tracked to prevent double-fixup.
 */
void portFixupSprite(void *sprite);

/**
 * Fix byte order for a Bitmap struct (16 bytes) after blanket u32 swap.
 *
 * rotate16 for s16 pair words (width/width_img, s/t, actualHeight/LUToffset)
 * Idempotent: tracked to prevent double-fixup.
 */
void portFixupBitmap(void *bitmap);

/**
 * Fix byte order for an array of Bitmap structs.
 */
void portFixupBitmapArray(void *bitmaps, unsigned int count);

/**
 * Fix the texel data referenced by a Sprite's Bitmap array.
 *
 * Performs two passes per bitmap:
 *
 *   1. Restore N64 BE byte order. Pass2 of portRelocByteSwapBlob only finds
 *      textures referenced by an in-file SETTIMG/LOADBLOCK pair; sprites build
 *      their LOAD blocks at runtime from bitmap.buf and never embed those
 *      addresses in stored DLs, so pass2 misses them. Apply BSWAP32 again to
 *      undo pass1's blanket u32 swap. Fast3D's ImportTexture* readers expect
 *      bytes in N64 big-endian order (e.g. RGBA16: `(addr[0]<<8) | addr[1]`).
 *
 *   2. N64 RDP TMEM line-swizzle inverse. The N64 stores textures in DRAM
 *      pre-swizzled to avoid TMEM bank conflicts when sampled. The hardware
 *      XORs the byte address based on row parity:
 *        16bpp / IA / CI:  odd rows XOR with 0x4 (swap 4-byte halves of each
 *                          8-byte qword)
 *      LOAD_BLOCK with dxt=0 loads the data into TMEM as-is (still swizzled);
 *      the sampler unscrambles it during reads. Fast3D doesn't emulate TMEM
 *      addressing, so the swizzled data renders as a sheared/zigzag image.
 *      Pre-unswizzle each bitmap here so Fast3D sees a normal linear texture.
 *
 * Must be called AFTER portFixupSprite and portFixupBitmapArray (which fix
 * the struct fields the function reads), and BEFORE the texture data is read
 * by the renderer or the 4c→4b decompressor.
 *
 * Idempotent: tracks each buf pointer to prevent double-fixup.
 *
 * @param sprite_v   Pointer to a Sprite struct (struct fields already fixed up).
 * @param bitmaps_v  Pointer to the resolved Bitmap array referenced by sprite->bitmap.
 */
void portFixupSpriteBitmapData(void *sprite_v, void *bitmaps_v);

/**
 * Fix byte order for a MObjSub struct (0x78 bytes) after blanket u32 swap.
 *
 * Handles the mixed u16/u8 fields: pad00+fmt+siz, unk08-unk0E,
 * flags+block_fmt+block_siz, block_dxt+unk36, unk38+unk3A.
 * Idempotent: tracked to prevent double-fixup.
 */
void portFixupMObjSub(void *mobjsub);

/**
 * Fix byte order for an FTAttributes struct (0x348 bytes) after blanket u32 swap.
 *
 * rotate16 for u16 pair words (dead_fgm_ids, sfx, throw scales)
 * bswap32 for SYColorRGBA u8 quad words (shade_color[3], fog_color)
 * Idempotent: tracked to prevent double-fixup.
 */
void portFixupFTAttributes(void *attr);

#ifdef __cplusplus
}
#endif

#endif /* LBRELOC_BYTESWAP_H */
