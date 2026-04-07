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
 * Fix byte order for a MObjSub struct (0x78 bytes) after blanket u32 swap.
 *
 * Handles the mixed u16/u8 fields: pad00+fmt+siz, unk08-unk0E,
 * flags+block_fmt+block_siz, block_dxt+unk36, unk38+unk3A.
 * Idempotent: tracked to prevent double-fixup.
 */
void portFixupMObjSub(void *mobjsub);

#ifdef __cplusplus
}
#endif

#endif /* LBRELOC_BYTESWAP_H */
