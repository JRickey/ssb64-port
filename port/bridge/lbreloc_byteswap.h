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

#ifdef __cplusplus
}
#endif

#endif /* LBRELOC_BYTESWAP_H */
