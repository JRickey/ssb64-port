/**
 * lbreloc_byteswap.cpp — Big-endian to native byte-swap for reloc file blobs
 *
 * N64 reloc files are stored in big-endian byte order. On little-endian PC,
 * every multi-byte field is read incorrectly. This module performs a two-pass
 * byte-swap:
 *
 *   Pass 1: Blanket u32 swap of the entire blob. This correctly handles
 *           display list commands, float/int struct fields, reloc chain
 *           descriptors, 32bpp texture pixels, and zeros.
 *
 *   Pass 2: Parse the now-native-endian display list commands to identify
 *           vertex buffers and texture/palette regions, then apply targeted
 *           fixups for data types that u32 swap got wrong.
 *
 * Called from lbRelocLoadAndRelocFile() between memcpy and the reloc chain walk.
 */

#include "bridge/lbreloc_byteswap.h"

#include <ship/utils/binarytools/endianness.h>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <cstring>
#include <vector>

// F3DEX2 GBI opcodes (from gbi.h with F3DEX_GBI_2 defined)
#define GBI_VTX         0x01
#define GBI_DL          0xDE
#define GBI_ENDDL       0xDF
#define GBI_SETTIMG     0xFD
#define GBI_LOADBLOCK   0xF3
#define GBI_LOADTLUT    0xF0

// Texture size constants (bits per pixel)
#define G_IM_SIZ_4b     0
#define G_IM_SIZ_8b     1
#define G_IM_SIZ_16b    2
#define G_IM_SIZ_32b    3

// Segment ID used for intra-file display list references
#define FILE_SEGMENT_ID 0x0E

// Fixup types for marked regions
enum RegionFixup
{
	FIXUP_VERTEX,       // Per-Vtx: rotate16 words 0-2, bswap32 word 3
	FIXUP_TEX_BYTES,    // 4bpp/8bpp texture: undo u32 swap (bswap32 back)
	FIXUP_TEX_U16,      // 16bpp texture or palette: rotate16 each word
	// 32bpp: no fixup needed (u32 swap was correct)
};

struct FixupRegion
{
	uint32_t byte_offset;
	uint32_t byte_size;
	RegionFixup type;
};

// ============================================================
//  Pass 1: Blanket u32 swap
// ============================================================

static void pass1_swap_u32(void *data, size_t size)
{
	uint32_t *words = static_cast<uint32_t *>(data);
	size_t count = size / 4;

	for (size_t i = 0; i < count; i++)
	{
		words[i] = BSWAP32(words[i]);
	}
}

// ============================================================
//  Pass 2: DL-guided fixup scan
// ============================================================

static void scan_display_lists(const uint32_t *words, size_t num_words,
                               size_t file_size, std::vector<FixupRegion> &regions)
{
	// Pending texture image state from the most recent gDPSetTextureImage
	bool has_pending_tex = false;
	uint32_t pending_tex_offset = 0;
	uint32_t pending_tex_siz = 0;

	for (size_t i = 0; i + 1 < num_words; i += 2)
	{
		uint32_t w0 = words[i];
		uint32_t w1 = words[i + 1];
		uint8_t opcode = (w0 >> 24) & 0xFF;

		switch (opcode)
		{
		case GBI_VTX:
		{
			uint32_t num_vtx = (w0 >> 12) & 0xFF;
			uint8_t seg = (w1 >> 24) & 0xFF;
			uint32_t offset = w1 & 0x00FFFFFF;

			if (seg == FILE_SEGMENT_ID && num_vtx > 0 &&
			    offset + num_vtx * 16 <= file_size)
			{
				regions.push_back({offset, num_vtx * 16, FIXUP_VERTEX});
			}
			break;
		}

		case GBI_SETTIMG:
		{
			uint8_t seg = (w1 >> 24) & 0xFF;
			uint32_t siz = (w0 >> 19) & 0x03;

			if (seg == FILE_SEGMENT_ID)
			{
				pending_tex_offset = w1 & 0x00FFFFFF;
				pending_tex_siz = siz;
				has_pending_tex = true;
			}
			else
			{
				has_pending_tex = false;
			}
			break;
		}

		case GBI_LOADBLOCK:
		{
			if (!has_pending_tex)
				break;

			uint32_t lrs = (w1 >> 12) & 0xFFF;
			uint32_t num_texels = lrs + 1;
			uint32_t bpp = 0;

			switch (pending_tex_siz)
			{
			case G_IM_SIZ_4b:  bpp = 4;  break;
			case G_IM_SIZ_8b:  bpp = 8;  break;
			case G_IM_SIZ_16b: bpp = 16; break;
			case G_IM_SIZ_32b: bpp = 32; break;
			}

			if (bpp == 0)
				break;

			uint32_t tex_bytes = (num_texels * bpp + 7) / 8;
			// Align to 4 bytes for u32 word processing
			tex_bytes = (tex_bytes + 3) & ~3u;

			if (pending_tex_offset + tex_bytes > file_size)
				tex_bytes = file_size - pending_tex_offset;

			RegionFixup fixup;
			switch (pending_tex_siz)
			{
			case G_IM_SIZ_4b:
			case G_IM_SIZ_8b:
				fixup = FIXUP_TEX_BYTES;
				break;
			case G_IM_SIZ_16b:
				fixup = FIXUP_TEX_U16;
				break;
			default:
				// 32b: u32 swap was correct, no fixup
				goto skip_tex;
			}

			regions.push_back({pending_tex_offset, tex_bytes, fixup});
		skip_tex:
			has_pending_tex = false;
			break;
		}

		case GBI_LOADTLUT:
		{
			if (!has_pending_tex)
				break;

			uint32_t count = ((w1 >> 14) & 0x3FF) + 1;
			uint32_t palette_bytes = count * 2;
			// Align to 4 bytes
			palette_bytes = (palette_bytes + 3) & ~3u;

			if (pending_tex_offset + palette_bytes > file_size)
				palette_bytes = file_size - pending_tex_offset;

			regions.push_back({pending_tex_offset, palette_bytes, FIXUP_TEX_U16});
			has_pending_tex = false;
			break;
		}

		default:
			break;
		}
	}
}

// ============================================================
//  Fixup application
// ============================================================

// Rotate each u32 word by 16 bits: converts u32-swapped u16 pairs
// to correctly u16-swapped data.
// After u32 swap, BE bytes [A B C D] became [D C B A].
// rotate16 produces [B A D C] which is correct u16 LE for (AB, CD).
static inline uint32_t rotate16(uint32_t w)
{
	return (w << 16) | (w >> 16);
}

static void apply_fixup_vertex(uint32_t *words, uint32_t num_words)
{
	// Vtx is 4 u32 words (16 bytes):
	//   Word 0: s16 ob[0] | s16 ob[1]   -> rotate16
	//   Word 1: s16 ob[2] | u16 flag     -> rotate16
	//   Word 2: s16 tc[0] | s16 tc[1]    -> rotate16
	//   Word 3: u8 cn[0-3]               -> bswap32 (restore byte order)
	for (uint32_t i = 0; i + 3 < num_words; i += 4)
	{
		words[i + 0] = rotate16(words[i + 0]);
		words[i + 1] = rotate16(words[i + 1]);
		words[i + 2] = rotate16(words[i + 2]);
		words[i + 3] = BSWAP32(words[i + 3]);
	}
}

static void apply_fixup_tex_bytes(uint32_t *words, uint32_t num_words)
{
	// 4bpp/8bpp texture data: byte-granular pixels.
	// u32 swap reversed byte order within each 4-byte group.
	// Undo by swapping back to original byte order.
	for (uint32_t i = 0; i < num_words; i++)
	{
		words[i] = BSWAP32(words[i]);
	}
}

static void apply_fixup_tex_u16(uint32_t *words, uint32_t num_words)
{
	// 16bpp texture/palette data: u16 pixel values.
	// Same fixup as vertex s16 data.
	for (uint32_t i = 0; i < num_words; i++)
	{
		words[i] = rotate16(words[i]);
	}
}

static void apply_fixups(void *data, size_t file_size,
                         const std::vector<FixupRegion> &regions)
{
	uint8_t *bytes = static_cast<uint8_t *>(data);

	for (const auto &region : regions)
	{
		if (region.byte_offset + region.byte_size > file_size)
			continue;

		uint32_t *region_words = reinterpret_cast<uint32_t *>(
			bytes + region.byte_offset);
		uint32_t num_words = region.byte_size / 4;

		switch (region.type)
		{
		case FIXUP_VERTEX:
			apply_fixup_vertex(region_words, num_words);
			break;
		case FIXUP_TEX_BYTES:
			apply_fixup_tex_bytes(region_words, num_words);
			break;
		case FIXUP_TEX_U16:
			apply_fixup_tex_u16(region_words, num_words);
			break;
		}
	}
}

// ============================================================
//  Public API
// ============================================================

extern "C" void portRelocByteSwapBlob(void *data, size_t size)
{
	if (data == nullptr || size < 4)
		return;

	// Pass 1: blanket u32 swap
	pass1_swap_u32(data, size);

	// Pass 2: DL-guided fixup
	size_t num_words = size / 4;
	const uint32_t *words = static_cast<const uint32_t *>(data);

	std::vector<FixupRegion> regions;
	scan_display_lists(words, num_words, size, regions);

	if (!regions.empty())
	{
		apply_fixups(data, size, regions);
	}
}
