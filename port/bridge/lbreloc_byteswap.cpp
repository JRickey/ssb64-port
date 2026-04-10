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
#include "resource/RelocPointerTable.h"

#include <ship/utils/binarytools/endianness.h>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <cstring>
#include <unordered_set>
#include <vector>

extern "C" void port_log(const char *fmt, ...);

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
			size_t offset_sz = static_cast<size_t>(offset);
			size_t vtx_bytes = static_cast<size_t>(num_vtx) * 16;

			if (seg == FILE_SEGMENT_ID && num_vtx > 0 &&
			    offset_sz <= file_size && vtx_bytes <= (file_size - offset_sz))
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

			size_t pending_offset_sz = static_cast<size_t>(pending_tex_offset);
			if (pending_offset_sz > file_size)
			{
				has_pending_tex = false;
				break;
			}
			if (static_cast<size_t>(tex_bytes) > (file_size - pending_offset_sz))
				tex_bytes = static_cast<uint32_t>(file_size - pending_offset_sz);

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

			size_t pending_offset_sz = static_cast<size_t>(pending_tex_offset);
			if (pending_offset_sz > file_size)
			{
				has_pending_tex = false;
				break;
			}
			if (static_cast<size_t>(palette_bytes) > (file_size - pending_offset_sz))
				palette_bytes = static_cast<uint32_t>(file_size - pending_offset_sz);

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

static void apply_fixup_vertex(uint32_t *words, size_t num_words)
{
	// Vtx is 4 u32 words (16 bytes):
	//   Word 0: s16 ob[0] | s16 ob[1]   -> rotate16
	//   Word 1: s16 ob[2] | u16 flag     -> rotate16
	//   Word 2: s16 tc[0] | s16 tc[1]    -> rotate16
	//   Word 3: u8 cn[0-3]               -> bswap32 (restore byte order)
	for (size_t i = 0; i + 3 < num_words; i += 4)
	{
		words[i + 0] = rotate16(words[i + 0]);
		words[i + 1] = rotate16(words[i + 1]);
		words[i + 2] = rotate16(words[i + 2]);
		words[i + 3] = BSWAP32(words[i + 3]);
	}
}

static void apply_fixup_tex_bytes(uint32_t *words, size_t num_words)
{
	// 4bpp/8bpp texture data: byte-granular pixels.
	// u32 swap reversed byte order within each 4-byte group.
	// Undo by swapping back to original byte order.
	for (size_t i = 0; i < num_words; i++)
	{
		words[i] = BSWAP32(words[i]);
	}
}

static void apply_fixup_tex_u16(uint32_t *words, size_t num_words)
{
	// 16bpp texture/palette data: u16 pixel values stored in N64 BE byte order.
	// Pass1 BSWAP32 reversed the byte order within each u32; Fast3D's
	// ImportTextureRgba16 reads texels as `(addr[0] << 8) | addr[1]` (BE u16),
	// so we must restore the original BE byte layout.  BSWAP32 undoes pass1.
	//
	// Earlier this used rotate16 which only swapped the two 16-bit halves —
	// that's correct for `[s16][s16]` vertex pair fields but wrong for a stream
	// of BE-read u16 texels.  Most non-sprite RGBA16 textures (room walls, etc.)
	// surfaced as all-near-black pixels until this was switched to BSWAP32.
	for (size_t i = 0; i < num_words; i++)
	{
		words[i] = BSWAP32(words[i]);
	}
}

static void apply_fixups(void *data, size_t file_size,
                         const std::vector<FixupRegion> &regions)
{
	uint8_t *bytes = static_cast<uint8_t *>(data);

	for (const auto &region : regions)
	{
		size_t start = static_cast<size_t>(region.byte_offset);
		size_t len = static_cast<size_t>(region.byte_size);

		// Bounds check using size_t math to prevent 32-bit overflow.
		if (start > file_size || len > (file_size - start))
			continue;

		// We reinterpret as u32 words, so both start and length must be word-aligned.
		if ((start & 3) != 0)
			continue;
		len &= ~static_cast<size_t>(3);
		if (len == 0)
			continue;

		uint32_t *region_words = reinterpret_cast<uint32_t *>(
			bytes + start);
		size_t num_words = len / 4;

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

// ============================================================
//  Struct u16 fixup — rotate16 for u16 fields in ROM structs
// ============================================================

// Tracks which (base + offset) regions have been fixed to prevent
// double-fixup when the same struct pointer is loaded multiple times
// from a cached file blob.
static std::unordered_set<uintptr_t> sStructU16Fixups;

extern "C" void portFixupStructU16(void *base, unsigned int byte_offset, unsigned int num_words)
{
	uintptr_t key = reinterpret_cast<uintptr_t>(base) + byte_offset;
	if (sStructU16Fixups.count(key))
		return;
	sStructU16Fixups.insert(key);

	uint32_t *words = reinterpret_cast<uint32_t *>(
		static_cast<uint8_t *>(base) + byte_offset);
	for (unsigned int i = 0; i < num_words; i++)
	{
		words[i] = (words[i] << 16) | (words[i] >> 16);
	}
}

extern "C" void portResetStructFixups(void)
{
	sStructU16Fixups.clear();
}

// ============================================================
//  Chain-walk fixup (textures and vertices)
// ============================================================
//
// Pass2's seg==0x0E filter only catches a tiny minority of in-file textures
// and vertices.  SSB64 model DLs reference these via real-pointer slots that
// the reloc chain rewrites at load time.  Pass2 sees those slots BEFORE
// rewrite — they contain chain encoding (random-looking high bytes), not
// seg=0x0E references, so they get missed.
//
// Workaround: during the chain walk, for each chain entry whose preceding
// cmd is a G_SETTIMG or G_VTX, apply the appropriate fixup using the chain
// encoding's words_num field as the in-file target offset.  The chain knows
// the truth because it's an explicit list of "slots that need fixup" —
// no false positives possible.
//
// Args:
//   file_base       — start of the file in PC memory (post-pass1)
//   file_size       — size of the file blob
//   slot_byte_off   — byte offset of the cmd's w1 slot (the slot the
//                     chain entry refers to)
//   target_byte_off — byte offset of the target data within the same file
//                     (computed from chain entry's words_num * 4)
//
// Returns 1 if the slot was at a G_SETTIMG/G_VTX and a fixup was applied;
// 0 otherwise.

static int chain_fixup_settimg(void *file_base, size_t file_size,
                                unsigned int slot_byte_off,
                                unsigned int target_byte_off,
                                uint32_t w0)
{
	const uint8_t *file_bytes = static_cast<const uint8_t *>(file_base);

	uint32_t fmt = (w0 >> 21) & 0x07;
	uint32_t siz = (w0 >> 19) & 0x03;
	(void)fmt;

	// Walk forward up to 8 cmds (64 bytes) to find G_LOADBLOCK or G_LOADTLUT.
	uint32_t loadblock_w1 = 0;
	uint32_t loadtlut_w1  = 0;
	int      found_load   = 0;
	for (int step = 1; step <= 8; step++)
	{
		size_t walk_off = (size_t)slot_byte_off - 4 + (size_t)(step * 8);
		if (walk_off + 8 > file_size) break;
		uint32_t walk_w0 = *(const uint32_t *)(file_bytes + walk_off);
		uint32_t walk_w1 = *(const uint32_t *)(file_bytes + walk_off + 4);
		uint8_t  walk_op = (uint8_t)(walk_w0 >> 24);
		if (walk_op == GBI_LOADBLOCK)
		{
			loadblock_w1 = walk_w1;
			found_load = 1;
			break;
		}
		if (walk_op == GBI_LOADTLUT)
		{
			loadtlut_w1 = walk_w1;
			found_load = 2;
			break;
		}
		if (walk_op == GBI_SETTIMG) return 0;
	}
	if (!found_load) return 0;

	uint32_t tex_bytes = 0;
	if (found_load == 1)
	{
		uint32_t lrs = (loadblock_w1 >> 12) & 0xFFF;
		uint32_t num_texels = lrs + 1;
		uint32_t bpp = (siz == G_IM_SIZ_4b)  ? 4
		             : (siz == G_IM_SIZ_8b)  ? 8
		             : (siz == G_IM_SIZ_16b) ? 16
		             : (siz == G_IM_SIZ_32b) ? 32
		             : 0;
		if (bpp == 0) return 0;
		tex_bytes = (num_texels * bpp + 7) / 8;
	}
	else
	{
		uint32_t count = ((loadtlut_w1 >> 14) & 0x3FF) + 1;
		tex_bytes = count * 2;
	}
	tex_bytes = (tex_bytes + 3) & ~3u;
	if (tex_bytes == 0) return 0;
	if ((size_t)target_byte_off + tex_bytes > file_size)
	{
		tex_bytes = (uint32_t)(file_size - target_byte_off);
		tex_bytes &= ~3u;
		if (tex_bytes == 0) return 0;
	}

	uintptr_t fixup_key = reinterpret_cast<uintptr_t>(file_base) + (uintptr_t)target_byte_off;
	if (sStructU16Fixups.count(fixup_key)) return 1;
	sStructU16Fixups.insert(fixup_key);

	uint32_t *region = (uint32_t *)((uint8_t *)file_base + target_byte_off);
	size_t num_words = tex_bytes / 4;

	switch (siz)
	{
	case G_IM_SIZ_4b:
	case G_IM_SIZ_8b:
	case G_IM_SIZ_16b:
		// Per-byte / per-u16 BE data: pass1 BSWAP32 reversed it; another
		// BSWAP32 restores the original BE byte order Fast3D expects.
		for (size_t i = 0; i < num_words; i++) region[i] = BSWAP32(region[i]);
		break;
	case G_IM_SIZ_32b:
		// 32bpp: pass1 BSWAP32 was correct (per-channel-byte read).  No fixup.
		break;
	}

	return 1;
}

static int chain_fixup_vertex(void *file_base, size_t file_size,
                               unsigned int slot_byte_off,
                               unsigned int target_byte_off,
                               uint32_t w0)
{
	(void)slot_byte_off;
	// G_VTX layout in F3DEX2 (matches gbi_decoder.c decoding):
	//   w0[31:24] = 0x01  (G_VTX opcode)
	//   w0[23:20] = 0     (reserved)
	//   w0[19:12] = num_vtx (n), 8 bits, practically 1..32
	//   w0[11:1]  = (v0 + n) << 1, end vertex slot index in TMEM
	//   w0[0]     = 0     (must be even because of << 1)
	//   w1        = vertex array address
	//
	// Each Vtx is 16 bytes:
	//   word 0: s16 ob[0]  | s16 ob[1]   → rotate16
	//   word 1: s16 ob[2]  | u16 flag    → rotate16
	//   word 2: s16 tc[0]  | s16 tc[1]   → rotate16
	//   word 3: u8  cn[0..3]              → bswap32 (restore byte order)
	//
	// STRICT VALIDATION: opcode 0x01 is too common to trust by itself.
	// Many non-cmd u32 words start with 0x01.  Reject anything where the
	// reserved/structural bits don't match the spec.

	// bits [23:20] must be zero (reserved)
	if ((w0 & 0x00F00000) != 0) return 0;
	// bit [0] must be zero (the count is multiplied by 2)
	if ((w0 & 0x1) != 0) return 0;

	uint32_t num_vtx = (w0 >> 12) & 0xFF;
	uint32_t end_idx = (w0 & 0xFFE) >> 1;

	// Sanity-check num_vtx and the destination range.  Real game DLs use
	// 1..32 vertices per G_VTX (Vtx buffer is 32 entries on N64).
	if (num_vtx == 0 || num_vtx > 32) return 0;
	if (end_idx > 32 || end_idx < num_vtx) return 0;

	size_t total_bytes = (size_t)num_vtx * 16;
	if ((size_t)target_byte_off + total_bytes > file_size) return 0;

	uint32_t *region = (uint32_t *)((uint8_t *)file_base + target_byte_off);

	// Content-based sanity check on the FIRST vertex.
	// Post-pass1 word 0 (high 16 bits) = original BE ob[0] (s16),
	// (low 16 bits) = original BE ob[1] (s16). Word 1 likewise has
	// ob[2] and flag. Real game vertex coords observed up to ±4200,
	// flag is always 0. Reject anything with much larger coords or
	// nonzero flag — those are false-positive struct-field chain
	// entries whose preceding 4 bytes happen to look like a G_VTX cmd.
	{
		int16_t ob0 = (int16_t)((region[0] >> 16) & 0xFFFF);
		int16_t ob1 = (int16_t)(region[0] & 0xFFFF);
		int16_t ob2 = (int16_t)((region[1] >> 16) & 0xFFFF);
		uint16_t flag = (uint16_t)(region[1] & 0xFFFF);
		const int kMaxCoord = 16384;
		if (ob0 < -kMaxCoord || ob0 > kMaxCoord) return 0;
		if (ob1 < -kMaxCoord || ob1 > kMaxCoord) return 0;
		if (ob2 < -kMaxCoord || ob2 > kMaxCoord) return 0;
		if (flag != 0) return 0;
	}

	uintptr_t fixup_key = reinterpret_cast<uintptr_t>(file_base) + (uintptr_t)target_byte_off;
	if (sStructU16Fixups.count(fixup_key)) return 1;
	sStructU16Fixups.insert(fixup_key);

	for (uint32_t i = 0; i < num_vtx; i++)
	{
		region[i * 4 + 0] = (region[i * 4 + 0] << 16) | (region[i * 4 + 0] >> 16);
		region[i * 4 + 1] = (region[i * 4 + 1] << 16) | (region[i * 4 + 1] >> 16);
		region[i * 4 + 2] = (region[i * 4 + 2] << 16) | (region[i * 4 + 2] >> 16);
		region[i * 4 + 3] = BSWAP32(region[i * 4 + 3]);
	}
	return 1;
}

extern "C" int portRelocFixupTextureFromChain(void *file_base, size_t file_size,
                                              unsigned int slot_byte_off,
                                              unsigned int target_byte_off)
{
	if (file_base == nullptr || slot_byte_off < 4 ||
	    (size_t)(slot_byte_off + 4) > file_size ||
	    (size_t)target_byte_off >= file_size)
	{
		return 0;
	}

	const uint8_t *file_bytes = static_cast<const uint8_t *>(file_base);

	// The cmd's w0 is at slot_byte_off - 4, w1 is at slot_byte_off.
	uint32_t w0 = *(const uint32_t *)(file_bytes + slot_byte_off - 4);
	uint8_t opcode = (uint8_t)(w0 >> 24);

	if (opcode == GBI_SETTIMG)
	{
		return chain_fixup_settimg(file_base, file_size, slot_byte_off,
		                           target_byte_off, w0);
	}
	if (opcode == GBI_VTX)
	{
		// Filter: real packed cmd w1 must be at 8*N+4 alignment.
		// Reject struct-field chain entries whose preceding 4 bytes
		// happen to look like a G_VTX cmd by coincidence.  Verified via
		// runtime diagnostic that all legitimate vertex fixups satisfy this.
		if ((slot_byte_off & 0x7) != 4) return 0;
		return chain_fixup_vertex(file_base, file_size, slot_byte_off, target_byte_off, w0);
	}
	return 0;
}

// ============================================================
//  Sprite / Bitmap / MObjSub — struct-level byte-swap fixups
// ============================================================
//
// After the blanket u32 swap, u32 and f32 fields are correct but
// u16-pair words and u8-quad words are garbled.
//
// rotate16: fixes two u16 values packed in one u32 word
// bswap32:  undoes the blanket swap for a u8-quad word
//
// Each function is idempotent: tracked by the same sStructU16Fixups set.

static void fixup_rotate16(uint32_t *word)
{
	*word = (*word << 16) | (*word >> 16);
}

static void fixup_bswap32(uint32_t *word)
{
	*word = BSWAP32(*word);
}

// Fixup for a u32 word laid out as [u16 a][u8 b][u8 c] in original BE memory.
// Pass1's blanket BSWAP32 leaves the bytes as [c, b, a_lo, a_hi] which makes
// a_lo/a_hi appear in the wrong slots: reading `u16 a` from offset 0 yields
// (b << 8) | c, and reading `u8 b`/`u8 c` from offsets 2/3 yields a_lo/a_hi.
// Neither rotate16 nor a second bswap32 produces the correct LE layout for
// all three fields simultaneously, so we permute the bytes directly.
static void fixup_u16_u8u8(uint32_t *word)
{
	uint8_t *p = reinterpret_cast<uint8_t *>(word);
	uint8_t b0 = p[0];
	uint8_t b1 = p[1];
	uint8_t b2 = p[2];
	uint8_t b3 = p[3];
	// Have (post-pass1): [c, b, a_lo, a_hi] = [b0, b1, b2, b3]
	// Want (LE struct):  [a_lo, a_hi, b, c] = [b2, b3, b1, b0]
	p[0] = b2;
	p[1] = b3;
	p[2] = b1;
	p[3] = b0;
}

extern "C" void portFixupSprite(void *sprite)
{
	if (sprite == NULL)
		return;

	uintptr_t key = reinterpret_cast<uintptr_t>(sprite);
	if (sStructU16Fixups.count(key))
		return;
	sStructU16Fixups.insert(key);

	uint32_t *w = static_cast<uint32_t *>(sprite);

	// Sprite layout (17 words = 68 bytes):
	//  Word  Offset  Fields                   Fixup
	//  0     0x00    s16 x, s16 y             rotate16
	//  1     0x04    s16 width, s16 height    rotate16
	//  2     0x08    f32 scalex               (ok)
	//  3     0x0C    f32 scaley               (ok)
	//  4     0x10    s16 expx, s16 expy       rotate16
	//  5     0x14    u16 attr, s16 zdepth     rotate16
	//  6     0x18    u8 r,g,b,a               bswap32
	//  7     0x1C    s16 startTLUT, s16 nTLUT rotate16
	//  8     0x20    u32 LUT (token)          (ok)
	//  9     0x24    s16 istart, s16 istep    rotate16
	//  10    0x28    s16 nbitmaps, s16 ndisplist rotate16
	//  11    0x2C    s16 bmheight, s16 bmHreal rotate16
	//  12    0x30    u8 bmfmt, u8 bmsiz, pad  bswap32
	//  13    0x34    u32 bitmap (token)        (ok)
	//  14    0x38    u32 rsp_dl (token)        (ok)
	//  15    0x3C    u32 rsp_dl_next (token)   (ok)
	//  16    0x40    s16 frac_s, s16 frac_t   rotate16

	fixup_rotate16(&w[0]);   // x, y
	fixup_rotate16(&w[1]);   // width, height
	// w[2], w[3]: f32 scalex, scaley — ok
	fixup_rotate16(&w[4]);   // expx, expy
	fixup_rotate16(&w[5]);   // attr, zdepth
	fixup_bswap32(&w[6]);    // rgba
	fixup_rotate16(&w[7]);   // startTLUT, nTLUT
	// w[8]: u32 LUT — ok
	fixup_rotate16(&w[9]);   // istart, istep
	fixup_rotate16(&w[10]);  // nbitmaps, ndisplist
	fixup_rotate16(&w[11]);  // bmheight, bmHreal
	fixup_bswap32(&w[12]);   // bmfmt, bmsiz, pad
	// w[13..15]: u32 tokens — ok
	fixup_rotate16(&w[16]);  // frac_s, frac_t
}

extern "C" void portFixupBitmap(void *bitmap)
{
	if (bitmap == NULL)
		return;

	uintptr_t key = reinterpret_cast<uintptr_t>(bitmap);
	if (sStructU16Fixups.count(key))
		return;
	sStructU16Fixups.insert(key);

	uint32_t *w = static_cast<uint32_t *>(bitmap);

	// Bitmap layout (4 words = 16 bytes):
	//  Word  Offset  Fields                        Fixup
	//  0     0x00    s16 width, s16 width_img      rotate16
	//  1     0x04    s16 s, s16 t                  rotate16
	//  2     0x08    u32 buf (token)               (ok)
	//  3     0x0C    s16 actualHeight, s16 LUToffset rotate16

	fixup_rotate16(&w[0]);
	fixup_rotate16(&w[1]);
	// w[2]: u32 buf — ok
	fixup_rotate16(&w[3]);
}

extern "C" void portFixupBitmapArray(void *bitmaps, unsigned int count)
{
	if (bitmaps == NULL || count == 0)
		return;

	uint8_t *ptr = static_cast<uint8_t *>(bitmaps);
	for (unsigned int i = 0; i < count; i++)
	{
		portFixupBitmap(ptr + i * 16);
	}
}

// Sprite layout offsets used by the texel-data fixup. We do not include
// PR/sp.h here (to keep the bridge translation unit lean), so we hard-code
// the field offsets matching the static_assert(sizeof(Sprite) == 68).
//
//   sprite[0x28]  s16 nbitmaps
//   sprite[0x32]  u8  bmsiz                (within the u8 quad word at 0x30)
//   sprite[0x34]  u32 bitmap (token)
//
//   bitmap[0x00]  s16 width        (unused for size — see width_img)
//   bitmap[0x02]  s16 width_img
//   bitmap[0x08]  u32 buf (token)
//   bitmap[0x0C]  s16 actualHeight
//
// Sizes match decomp Sprite/Bitmap structs after portFixupSprite/portFixupBitmap
// have run (i.e. fields readable as native LE).
extern "C" void portFixupSpriteBitmapData(void *sprite_v, void *bitmaps_v)
{
	if (sprite_v == NULL || bitmaps_v == NULL)
		return;

	uint8_t *sprite = static_cast<uint8_t *>(sprite_v);
	uint8_t *bitmaps = static_cast<uint8_t *>(bitmaps_v);

	int16_t nbitmaps = *reinterpret_cast<int16_t *>(sprite + 0x28);
	uint8_t bmsiz    = *reinterpret_cast<uint8_t  *>(sprite + 0x31);

	if (nbitmaps <= 0)
		return;

	// G_IM_SIZ_ values: 4b=0, 8b=1, 16b=2, 32b=3, 4c=4 (compressed-4b)
	//
	// Fast3D's ImportTextureRgba16 (and the other texel readers) read texture
	// bytes in N64 big-endian order, e.g. RGBA16: `(addr[0] << 8) | addr[1]`.
	// So texel data must be left in the original BE byte order from the file.
	// Pass1 BSWAP32 destroys that order; another BSWAP32 restores it.
	int bpp;
	switch (bmsiz)
	{
	case 0: bpp = 4;  break;   // 4b
	case 1: bpp = 8;  break;   // 8b
	case 2: bpp = 16; break;   // 16b
	case 3: return;            // 32b — no fixup (pass1 byteswap is already correct: Fast3D's RGBA32 reader is per-channel-byte)
	case 4: bpp = 4;  break;   // 4c (compressed-4b: byte-granular like 4b)
	default:
		return;
	}

	for (int i = 0; i < nbitmaps; i++)
	{
		uint8_t *bm = bitmaps + (i * 16);
		int16_t  width_img    = *reinterpret_cast<int16_t *>(bm + 0x02);
		uint32_t buf_token    = *reinterpret_cast<uint32_t *>(bm + 0x08);
		int16_t  actualHeight = *reinterpret_cast<int16_t *>(bm + 0x0C);

		void *buf = portRelocResolvePointer(buf_token);
		if (buf == NULL || width_img <= 0 || actualHeight <= 0)
			continue;

		// Idempotency: skip if this buf was already fixed up.
		uintptr_t key = reinterpret_cast<uintptr_t>(buf);
		if (sStructU16Fixups.count(key))
			continue;
		sStructU16Fixups.insert(key);

		size_t num_texels = static_cast<size_t>(width_img) *
		                    static_cast<size_t>(actualHeight);
		size_t tex_bytes = (num_texels * bpp + 7) / 8;
		// Word-align so we can iterate as u32
		tex_bytes = (tex_bytes + 3) & ~size_t{3};

		uint32_t *words = static_cast<uint32_t *>(buf);
		size_t num_words = tex_bytes / 4;

		// All non-32bpp formats: undo pass1 BSWAP32 to restore N64 BE byte order.
		for (size_t j = 0; j < num_words; j++)
			words[j] = BSWAP32(words[j]);

		// N64 RDP TMEM line swizzle: textures stored in DRAM are pre-swizzled
		// to avoid TMEM bank conflicts when sampled. The hardware applies an
		// XOR to the byte address based on the row parity (bit 0 of t):
		//
		//   16bpp / IA / CI:    odd rows XOR address with 0x4 (swap the two
		//                       4-byte halves within each 8-byte qword)
		//
		// LOAD_BLOCK with dxt=0 loads the data into TMEM as-is (still swizzled).
		// On real hardware, the sampler reads with the inverse XOR and gets the
		// correct linear pixel order. Fast3D doesn't emulate TMEM addressing,
		// so it would read the swizzled data linearly and produce a corrupted
		// (zigzag/sheared) image. Pre-unswizzle here so Fast3D sees a normal
		// linear texture.
		//
		// Each Bitmap is loaded as one independent LOAD_BLOCK in
		// lbCommonDrawSObjBitmap, so the swizzle row index is strip-local.
		if (bpp == 16 && width_img > 0 && (width_img % 4) == 0)
		{
			size_t row_bytes = static_cast<size_t>(width_img) * 2;
			uint8_t *bytes = static_cast<uint8_t *>(buf);
			for (int row = 1; row < actualHeight; row += 2)
			{
				uint8_t *row_p = bytes + row * row_bytes;
				for (size_t qw = 0; qw + 8 <= row_bytes; qw += 8)
				{
					uint8_t tmp[4];
					std::memcpy(tmp, row_p + qw, 4);
					std::memcpy(row_p + qw, row_p + qw + 4, 4);
					std::memcpy(row_p + qw + 4, tmp, 4);
				}
			}
		}
	}
}

extern "C" void portFixupMObjSub(void *mobjsub)
{
	if (mobjsub == NULL)
		return;

	uintptr_t key = reinterpret_cast<uintptr_t>(mobjsub);
	if (sStructU16Fixups.count(key))
		return;
	sStructU16Fixups.insert(key);

	uint32_t *w = static_cast<uint32_t *>(mobjsub);

	// MObjSub layout (30 words = 0x78 bytes):
	//  Word  Offset  Fields                           Fixup
	//  0     0x00    u16 pad00, u8 fmt, u8 siz        bswap32 (mixed u16+u8)
	//  1     0x04    u32 sprites (token)               (ok)
	//  2     0x08    u16 unk08, u16 unk0A              rotate16
	//  3     0x0C    u16 unk0C, u16 unk0E              rotate16
	//  4     0x10    s32 unk10                          (ok)
	//  5-10  0x14    f32 trau..unk28                    (ok)
	//  11    0x2C    u32 palettes (token)               (ok)
	//  12    0x30    u16 flags, u8 block_fmt, u8 block_siz  bswap32
	//  13    0x34    u16 block_dxt, u16 unk36           rotate16
	//  14    0x38    u16 unk38, u16 unk3A               rotate16
	//  15-18 0x3C    f32 scrollu..unk48                 (ok)
	//  19    0x4C    u32 unk4C                          (ok)
	//  20    0x50    SYColorPack primcolor (u8 rgba)    bswap32
	//  21    0x54    u8 prim_l, u8 prim_m, u8[2] pad   bswap32
	//  22    0x58    SYColorPack envcolor               bswap32
	//  23    0x5C    SYColorPack blendcolor             bswap32
	//  24    0x60    SYColorPack light1color            bswap32
	//  25    0x64    SYColorPack light2color            bswap32
	//  26-29 0x68    s32 unk68..unk74                   (ok)

	fixup_bswap32(&w[0]);    // pad00 + fmt + siz (pad16 unused, u8 fields ok)
	// w[1]: sprites token — ok
	fixup_rotate16(&w[2]);   // unk08, unk0A
	fixup_rotate16(&w[3]);   // unk0C, unk0E
	// w[4..11]: s32/f32/u32 — ok
	fixup_u16_u8u8(&w[12]);  // u16 flags + u8 block_fmt + u8 block_siz
	fixup_rotate16(&w[13]);  // block_dxt, unk36
	fixup_rotate16(&w[14]);  // unk38, unk3A
	// w[15..19]: f32/u32 — ok
	fixup_bswap32(&w[20]);   // primcolor
	fixup_bswap32(&w[21]);   // prim_l, prim_m, pad
	fixup_bswap32(&w[22]);   // envcolor
	fixup_bswap32(&w[23]);   // blendcolor
	fixup_bswap32(&w[24]);   // light1color
	fixup_bswap32(&w[25]);   // light2color
	// w[26..29]: s32 — ok
}

extern "C" void portFixupFTAttributes(void *attr)
{
	if (attr == NULL)
		return;

	uintptr_t key = reinterpret_cast<uintptr_t>(attr);
	if (sStructU16Fixups.count(key))
		return;
	sStructU16Fixups.insert(key);

	uint32_t *w = static_cast<uint32_t *>(attr);

	// FTAttributes layout (0x348 bytes = 210 words):
	// Words 0x00..0x2C: f32/s32 physics fields — ok
	// Words 0x27..0x2B: MPObjectColl (4 f32) + Vec2f (2 f32) — ok
	//
	//  Word  Offset  Fields                               Fixup
	//  0x2D  0x0B4   u16 dead_fgm_ids[0], [1]            rotate16
	//  0x2E  0x0B8   u16 deadup_sfx, u16 damage_sfx      rotate16
	//  0x2F  0x0BC   u16 smash_sfx[0], smash_sfx[1]      rotate16
	//  0x30  0x0C0   u16 smash_sfx[2], pad                rotate16
	// Words 0x31..0x38: FTItemPickup (8 f32) — ok
	//  0x39  0x0E4   u16 itemthrow_vel_scale, damage_scale rotate16
	//  0x3A  0x0E8   u16 heavyget_sfx, pad                rotate16
	// Word 0x3B: f32 halo_size — ok
	//  0x3C  0x0F0   SYColorRGBA shade_color[0]           bswap32
	//  0x3D  0x0F4   SYColorRGBA shade_color[1]           bswap32
	//  0x3E  0x0F8   SYColorRGBA shade_color[2]           bswap32
	//  0x3F  0x0FC   SYColorRGBA fog_color                bswap32
	// Words 0x40..end: bitfields, DamageCollDescs (s32/f32), pointers (u32 tokens) — ok

	fixup_rotate16(&w[0x2D]);  // dead_fgm_ids[0], [1]
	fixup_rotate16(&w[0x2E]);  // deadup_sfx, damage_sfx
	fixup_rotate16(&w[0x2F]);  // smash_sfx[0], smash_sfx[1]
	fixup_rotate16(&w[0x30]);  // smash_sfx[2], pad
	fixup_rotate16(&w[0x39]);  // itemthrow_vel_scale, itemthrow_damage_scale
	fixup_rotate16(&w[0x3A]);  // heavyget_sfx, pad
	fixup_bswap32(&w[0x3C]);   // shade_color[0] rgba
	fixup_bswap32(&w[0x3D]);   // shade_color[1] rgba
	fixup_bswap32(&w[0x3E]);   // shade_color[2] rgba
	fixup_bswap32(&w[0x3F]);   // fog_color rgba
}
