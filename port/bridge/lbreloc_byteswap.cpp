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

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_set>
#include <vector>

extern "C" void port_log(const char *fmt, ...);
extern "C" int  portRelocFindContainingFile(const void *ptr, uintptr_t *out_base, size_t *out_size);
extern "C" int  portRelocFindFileIdAndBase(const void *ptr, uintptr_t *out_base);

// ============================================================
//  Texture-fixup diagnostic log (SSB64_TEX_FIXUP_LOG=1)
// ============================================================
//
// One-line entry per byteswap-side texture fixup invocation. See
// portRelocTexFixupLog() in lbreloc_byteswap.h for the user-facing knobs.
// All entries are interleaved into debug_traces/tex_fixup.log so chain /
// pass2 / runtime / heap / sprite paths can be diffed and grepped together.
//
// Initialization is lazy and once-only. The log is opened on the first call
// after the env var has been read.

static int  sTexLogState = 0;        // 0=uninit, 1=enabled, -1=disabled
static FILE *sTexLogFile = nullptr;
static int  sTexLogFileFilter = -1;  // -1 = no filter

static void tex_log_init_once()
{
	if (sTexLogState != 0) return;
	const char *env = std::getenv("SSB64_TEX_FIXUP_LOG");
	if (env == nullptr || env[0] == '0' || env[0] == '\0') {
		sTexLogState = -1;
		return;
	}
	sTexLogFile = std::fopen("debug_traces/tex_fixup.log", "w");
	if (sTexLogFile == nullptr) {
		// Fall back to stderr so we never silently swallow the request.
		sTexLogFile = stderr;
	}
	const char *fenv = std::getenv("SSB64_TEX_FIXUP_LOG_FILE_ID");
	if (fenv != nullptr && fenv[0] != '\0') {
		sTexLogFileFilter = (int)std::strtol(fenv, nullptr, 0);
	}
	sTexLogState = 1;
	std::fprintf(sTexLogFile,
		"# tex_fixup.log — SSB64 texture byteswap-fixup trace\n"
		"# columns: [path] file=<id> off=0x<file_off> size=<bytes> "
		"fmt=<n> siz=<n> bpp=<n> first16=<hex> note=<...>\n");
	std::fflush(sTexLogFile);
}

static inline bool tex_log_enabled()
{
	if (sTexLogState == 0) tex_log_init_once();
	return sTexLogState == 1;
}

static void tex_log_first16(char *out, size_t out_sz, const void *data, size_t bytes)
{
	if (data == nullptr || bytes == 0 || out_sz < 3) {
		if (out_sz > 0) out[0] = '-';
		if (out_sz > 1) out[1] = '\0';
		return;
	}
	size_t n = bytes < 16 ? bytes : 16;
	const uint8_t *p = static_cast<const uint8_t *>(data);
	size_t pos = 0;
	for (size_t i = 0; i < n && pos + 3 < out_sz; i++) {
		std::snprintf(out + pos, out_sz - pos, "%02X", p[i]);
		pos += 2;
	}
	if (pos < out_sz) out[pos] = '\0';
}

// Centralized emit. Caller has already pre-formatted everything except the
// optional first16 hex (which we never want to compute when the log is off).
static void tex_log_emit(const char *path, int file_id,
                         uint32_t file_off, uint32_t size_bytes,
                         int fmt, int siz, int bpp,
                         const void *first_bytes,
                         const char *note)
{
	if (!tex_log_enabled()) return;
	if (sTexLogFileFilter >= 0 && file_id != sTexLogFileFilter) return;

	char hex[40];
	tex_log_first16(hex, sizeof(hex), first_bytes, size_bytes);

	char fmt_buf[16] = "?";
	if (fmt >= 0) std::snprintf(fmt_buf, sizeof(fmt_buf), "%d", fmt);
	char siz_buf[16] = "?";
	if (siz >= 0) std::snprintf(siz_buf, sizeof(siz_buf), "%d", siz);
	char bpp_buf[16] = "?";
	if (bpp >= 0) std::snprintf(bpp_buf, sizeof(bpp_buf), "%d", bpp);
	char file_buf[24];
	if (file_id < 0) std::snprintf(file_buf, sizeof(file_buf), "heap");
	else             std::snprintf(file_buf, sizeof(file_buf), "%d", file_id);

	std::fprintf(sTexLogFile,
		"[%-12s] file=%-6s off=0x%06X size=%-6u fmt=%s siz=%s bpp=%s first16=%s%s%s\n",
		path, file_buf, file_off, size_bytes,
		fmt_buf, siz_buf, bpp_buf, hex,
		note ? " note=" : "", note ? note : "");
	std::fflush(sTexLogFile);
}

// Public C-callable variadic logger declared in lbreloc_byteswap.h.
// Currently unused by external code but exposed so future hooks (e.g. an
// interpreter-side LoadBlock context line) can interleave entries into the
// same log without re-implementing the gating.
extern "C" void portRelocTexFixupLog(const char *fmt, ...)
{
	if (!tex_log_enabled()) return;
	std::va_list ap;
	va_start(ap, fmt);
	std::vfprintf(sTexLogFile, fmt, ap);
	va_end(ap);
	std::fflush(sTexLogFile);
}

// ============================================================
//  Texture PNG dump (SSB64_TEX_DUMP=1)
// ============================================================
//
// One-shot per texture, fired from chain_fixup_settimg and
// portFixupSpriteBitmapData immediately after the byteswap fix is applied.
// The data passed in is in N64 big-endian byte order — i.e. the same layout
// Fast3D's ImportTexture* readers consume — so what we decode here is what
// the renderer will eventually upload as RGBA8.
//
// The chain path doesn't know the texture's actual width/height (LOADBLOCK
// only encodes the total texel count), so for chain-path dumps we emit one
// TGA per plausible width in {8,16,32,64,128} that divides the pixel count
// evenly.  Sprite-path dumps have real (width_img × actualHeight) so they
// emit one image at the correct dimensions.
//
// Filename: tex_dump/f<file>_o<offset>_fmt<f>siz<s>_w<w>_h<h>.tga
//
// TGA format: uncompressed 32-bit BGRA, top-down.  Header is 18 bytes;
// supported by every image viewer.

static int sTexDumpState = 0;        // 0=uninit, 1=enabled, -1=disabled

static void tex_dump_init_once()
{
	if (sTexDumpState != 0) return;
	const char *env = std::getenv("SSB64_TEX_DUMP");
	if (env == nullptr || env[0] == '0' || env[0] == '\0') {
		sTexDumpState = -1;
		return;
	}
	// Best-effort directory create.  On Windows the binary's CWD is
	// build/Debug, so this lands at build/Debug/tex_dump/.
#if defined(_WIN32)
	std::system("if not exist tex_dump mkdir tex_dump");
#else
	std::system("mkdir -p tex_dump");
#endif
	sTexDumpState = 1;
}

static inline bool tex_dump_enabled()
{
	if (sTexDumpState == 0) tex_dump_init_once();
	return sTexDumpState == 1;
}

// Write one 32-bit RGBA texel buffer (already in r,g,b,a byte order) as a
// TGA image.  TGA uses BGRA byte order so we swizzle on the way out.
static void dump_tga_rgba(const char *path,
                          const uint8_t *rgba, uint32_t w, uint32_t h)
{
	FILE *f = std::fopen(path, "wb");
	if (f == nullptr) return;
	uint8_t hdr[18] = {0};
	hdr[2]  = 2;                  // uncompressed truecolor
	hdr[12] = (uint8_t)(w & 0xFF);
	hdr[13] = (uint8_t)((w >> 8) & 0xFF);
	hdr[14] = (uint8_t)(h & 0xFF);
	hdr[15] = (uint8_t)((h >> 8) & 0xFF);
	hdr[16] = 32;                 // 32 bpp
	hdr[17] = 0x28;               // top-down, 8 alpha bits
	std::fwrite(hdr, 1, 18, f);
	std::vector<uint8_t> bgra(static_cast<size_t>(w) * h * 4);
	for (size_t i = 0; i < (size_t)w * h; i++) {
		bgra[i * 4 + 0] = rgba[i * 4 + 2]; // B
		bgra[i * 4 + 1] = rgba[i * 4 + 1]; // G
		bgra[i * 4 + 2] = rgba[i * 4 + 0]; // R
		bgra[i * 4 + 3] = rgba[i * 4 + 3]; // A
	}
	std::fwrite(bgra.data(), 1, bgra.size(), f);
	std::fclose(f);
}

// Decode N64 texel data (in BE byte order, post fixup) into a flat RGBA8
// buffer.  Returns true if (fmt,siz) is supported and the byte count makes
// sense for the requested dimensions.  CI formats are decoded with a
// fabricated greyscale palette since we have no TLUT context here.
static bool decode_to_rgba8(const uint8_t *src, size_t src_bytes,
                            int fmt, int siz,
                            uint32_t width, uint32_t height,
                            std::vector<uint8_t> &out_rgba)
{
	size_t need_pixels = (size_t)width * height;
	out_rgba.assign(need_pixels * 4, 0);

	auto scale_5_8 = [](uint8_t v) -> uint8_t {
		return (uint8_t)((v << 3) | (v >> 2));
	};
	auto scale_4_8 = [](uint8_t v) -> uint8_t {
		return (uint8_t)((v << 4) | v);
	};
	auto scale_3_8 = [](uint8_t v) -> uint8_t {
		return (uint8_t)((v << 5) | (v << 2) | (v >> 1));
	};

	// siz: 0=4b, 1=8b, 2=16b, 3=32b
	if (siz == 3) {
		// RGBA32: 4 bytes per pixel, byte order R G B A
		size_t need = need_pixels * 4;
		if (need > src_bytes) return false;
		for (size_t i = 0; i < need_pixels; i++) {
			out_rgba[i * 4 + 0] = src[i * 4 + 0];
			out_rgba[i * 4 + 1] = src[i * 4 + 1];
			out_rgba[i * 4 + 2] = src[i * 4 + 2];
			out_rgba[i * 4 + 3] = src[i * 4 + 3];
		}
		return true;
	}
	if (siz == 2) {
		// 16-bit per pixel, BE.  fmt=0:RGBA5551, fmt=3:IA88, fmt=4:I16, others:RGBA5551
		size_t need = need_pixels * 2;
		if (need > src_bytes) return false;
		for (size_t i = 0; i < need_pixels; i++) {
			uint16_t c = ((uint16_t)src[i * 2] << 8) | src[i * 2 + 1];
			if (fmt == 3) {
				// IA88 (16b): hi=intensity, lo=alpha
				uint8_t I = (uint8_t)(c >> 8);
				uint8_t A = (uint8_t)(c & 0xFF);
				out_rgba[i * 4 + 0] = I;
				out_rgba[i * 4 + 1] = I;
				out_rgba[i * 4 + 2] = I;
				out_rgba[i * 4 + 3] = A;
			} else if (fmt == 4) {
				// I16 isn't a real RDP format — treat as IA88 fallback
				uint8_t I = (uint8_t)(c >> 8);
				out_rgba[i * 4 + 0] = I;
				out_rgba[i * 4 + 1] = I;
				out_rgba[i * 4 + 2] = I;
				out_rgba[i * 4 + 3] = 255;
			} else {
				// RGBA5551 (and CI16 falls back here)
				uint8_t r = (c >> 11) & 0x1F;
				uint8_t g = (c >>  6) & 0x1F;
				uint8_t b = (c >>  1) & 0x1F;
				uint8_t a = c & 1;
				out_rgba[i * 4 + 0] = scale_5_8(r);
				out_rgba[i * 4 + 1] = scale_5_8(g);
				out_rgba[i * 4 + 2] = scale_5_8(b);
				out_rgba[i * 4 + 3] = a ? 255 : 0;
			}
		}
		return true;
	}
	if (siz == 1) {
		// 8b per pixel.  fmt=2:CI8 (no palette here → grayscale),
		// fmt=3:IA44, fmt=4:I8
		if (need_pixels > src_bytes) return false;
		for (size_t i = 0; i < need_pixels; i++) {
			uint8_t v = src[i];
			if (fmt == 3) {
				uint8_t I = (v >> 4) & 0xF;
				uint8_t A = v & 0xF;
				out_rgba[i * 4 + 0] = scale_4_8(I);
				out_rgba[i * 4 + 1] = scale_4_8(I);
				out_rgba[i * 4 + 2] = scale_4_8(I);
				out_rgba[i * 4 + 3] = scale_4_8(A);
			} else {
				out_rgba[i * 4 + 0] = v;
				out_rgba[i * 4 + 1] = v;
				out_rgba[i * 4 + 2] = v;
				out_rgba[i * 4 + 3] = 255;
			}
		}
		return true;
	}
	if (siz == 0) {
		// 4b per pixel.  fmt=2:CI4 (no palette → grayscale),
		// fmt=3:IA31, fmt=4:I4
		if ((need_pixels + 1) / 2 > src_bytes) return false;
		for (size_t i = 0; i < need_pixels; i++) {
			uint8_t byte = src[i / 2];
			uint8_t nib = (i & 1) ? (byte & 0x0F) : (byte >> 4);
			if (fmt == 3) {
				uint8_t I = (nib >> 1) & 0x7;
				uint8_t A = nib & 1;
				out_rgba[i * 4 + 0] = scale_3_8(I);
				out_rgba[i * 4 + 1] = scale_3_8(I);
				out_rgba[i * 4 + 2] = scale_3_8(I);
				out_rgba[i * 4 + 3] = A ? 255 : 0;
			} else {
				uint8_t v = scale_4_8(nib);
				out_rgba[i * 4 + 0] = v;
				out_rgba[i * 4 + 1] = v;
				out_rgba[i * 4 + 2] = v;
				out_rgba[i * 4 + 3] = 255;
			}
		}
		return true;
	}
	return false;
}

// Dump a texture for which we know the width and height (sprite path).
static void tex_dump_known_dims(int file_id, uint32_t file_off,
                                int fmt, int siz, int bpp,
                                const uint8_t *src, size_t src_bytes,
                                uint32_t w, uint32_t h)
{
	if (!tex_dump_enabled()) return;
	std::vector<uint8_t> rgba;
	if (!decode_to_rgba8(src, src_bytes, fmt, siz, w, h, rgba)) return;
	char path[256];
	std::snprintf(path, sizeof(path),
	              "tex_dump/sprite_f%d_o0x%06X_fmt%dsiz%dbpp%d_w%u_h%u.tga",
	              file_id, file_off, fmt, siz, bpp, w, h);
	dump_tga_rgba(path, rgba.data(), w, h);
}

// Dump a chain-path texture across plausible widths.
static void tex_dump_chain(int file_id, uint32_t file_off,
                           int fmt, int siz, uint32_t bpp,
                           const uint8_t *src, size_t src_bytes)
{
	if (!tex_dump_enabled()) return;
	if (bpp == 0 || src_bytes == 0) return;

	size_t total_bits = src_bytes * 8;
	if (total_bits % bpp != 0) return;
	size_t pixels = total_bits / bpp;
	if (pixels == 0 || pixels > 256 * 256) return;

	static const uint32_t kCandidateWidths[] = {8, 16, 32, 64, 128, 256};
	int written = 0;
	for (uint32_t w : kCandidateWidths) {
		if ((size_t)w > pixels) break;
		if (pixels % w != 0) continue;
		uint32_t h = (uint32_t)(pixels / w);
		if (h > 256) continue;

		std::vector<uint8_t> rgba;
		if (!decode_to_rgba8(src, src_bytes, fmt, siz, w, h, rgba)) continue;
		char path[256];
		std::snprintf(path, sizeof(path),
		              "tex_dump/chain_f%d_o0x%06X_fmt%dsiz%dbpp%u_w%u_h%u.tga",
		              file_id, file_off, fmt, siz, bpp, w, h);
		dump_tga_rgba(path, rgba.data(), w, h);
		if (++written >= 4) break; // cap to keep file count manageable
	}
}

// Tracks which (base + offset) regions have been fixed to prevent
// double-fixup. Used by per-struct fixups (Sprite, Bitmap, MObjSub, etc.),
// pass2 vertex fixups, the chain-walk texture fixup, and the runtime
// lazy vertex fixup. All paths share this set so they don't undo each
// other's work. Cleared at scene change via portResetStructFixups.
static std::unordered_set<uintptr_t> sStructU16Fixups;

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
                         const std::vector<FixupRegion> &regions,
                         unsigned int file_id)
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
			// Per-vertex registration so the runtime lazy fixup
			// (portRelocFixupVertexAtRuntime) skips each Vtx
			// individually — handles overlapping sub-loads.
			{
				uintptr_t base = reinterpret_cast<uintptr_t>(region_words);
				size_t n_vtx = num_words / 4;  // 4 u32 words per Vtx
				for (size_t v = 0; v < n_vtx; v++) {
					sStructU16Fixups.insert(base + v * 16);
				}
			}
			break;
		case FIXUP_TEX_BYTES:
			apply_fixup_tex_bytes(region_words, num_words);
			tex_log_emit("pass2.bytes", (int)file_id,
			             (uint32_t)start, (uint32_t)len,
			             -1, -1, -1, region_words, "4b/8b");
			break;
		case FIXUP_TEX_U16:
			apply_fixup_tex_u16(region_words, num_words);
			tex_log_emit("pass2.u16", (int)file_id,
			             (uint32_t)start, (uint32_t)len,
			             -1, -1, 16, region_words, "16b/tlut");
			break;
		}
	}
}

// ============================================================
//  Public API
// ============================================================

extern "C" void portRelocByteSwapBlob(void *data, size_t size, unsigned int file_id)
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
		apply_fixups(data, size, regions, file_id);
	}
}

// ============================================================
//  Struct u16 fixup — rotate16 for u16 fields in ROM structs
// ============================================================

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

	// All formats need BSWAP32 to restore N64 BE byte order:
	//  - 4b/8b: per-byte data; Fast3D reads bytes in their original order
	//  - 16b: pixels are BE u16; Fast3D reads `(addr[0] << 8) | addr[1]`
	//  - 32b: pixels are BE [R,G,B,A]; Fast3D's ImportTextureRgba32 reads
	//         addr[0]=R, addr[1]=G, addr[2]=B, addr[3]=A
	// Pass1 BSWAP32 reversed the byte order within each u32, so a second
	// BSWAP32 restores the original layout for ALL of these formats.
	for (size_t i = 0; i < num_words; i++) region[i] = BSWAP32(region[i]);

	// Diagnostic: record what was just fixed (chain-walk path).
	if (tex_log_enabled() || tex_dump_enabled()) {
		uintptr_t fb_addr = reinterpret_cast<uintptr_t>(file_base);
		int chain_file_id = portRelocFindFileIdAndBase(
			reinterpret_cast<const void *>(fb_addr), nullptr);
		uint32_t bpp_val = (siz == G_IM_SIZ_4b)  ? 4u
		                 : (siz == G_IM_SIZ_8b)  ? 8u
		                 : (siz == G_IM_SIZ_16b) ? 16u
		                 : (siz == G_IM_SIZ_32b) ? 32u
		                 : 0u;
		int fmt_val = (int)((w0 >> 21) & 0x07);
		const char *note = (found_load == 2) ? "tlut" : "loadblock";
		tex_log_emit("chain.settimg", chain_file_id,
		             target_byte_off, tex_bytes,
		             fmt_val, (int)siz, (int)bpp_val,
		             region, note);

		// Dump the texture as one or more TGA images so we can identify
		// it visually.  Skips palettes (loadtlut) since they're 1-D LUTs
		// not viewable as images.
		if (found_load == 1) {
			tex_dump_chain(chain_file_id, target_byte_off,
			               fmt_val, (int)siz, bpp_val,
			               reinterpret_cast<const uint8_t *>(region),
			               (size_t)tex_bytes);
		}
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

	// Per-vertex idempotency (matches portRelocFixupVertexAtRuntime).
	uintptr_t target_addr = reinterpret_cast<uintptr_t>(file_base) + (uintptr_t)target_byte_off;
	for (uint32_t i = 0; i < num_vtx; i++)
	{
		uintptr_t vtx_key = target_addr + (uintptr_t)i * 16;
		if (sStructU16Fixups.count(vtx_key)) continue;
		sStructU16Fixups.insert(vtx_key);

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
		if ((slot_byte_off & 0x7) != 4) return 0;
		return chain_fixup_vertex(file_base, file_size, slot_byte_off, target_byte_off, w0);
	}
	return 0;
}

// ============================================================
//  Lazy runtime fixup (Option A)
// ============================================================
//
// Called from the interpreter's gfx_vtx_handler_f3dex2 with the resolved
// vertex array address and the cmd's num_vtx.  If the address is inside a
// reloc file (i.e., not heap-built data), apply the per-Vtx byte permutation
// idempotently.
//
// Why this is correct: only data the running interpreter actually treats as
// vertices reaches this function — there's no guessing.  Each target's "type"
// is unambiguously vertex because a real G_VTX cmd just dispatched it.

// Lazy texture byte-order fixup at G_LOADBLOCK / G_LOADTLUT execute time.
// Called from libultraship's GfxDpLoadBlock and GfxDpLoadTLUT once both
// the texture address (set by G_SETTIMG) and the texel count are known.
//
// This catches textures referenced by RUNTIME-BUILT DLs that pass2 doesn't
// see (no in-file SETTIMG/LOADBLOCK pair) and that the chain walk also
// doesn't see (the slot was looked up via an MObjSub field, not a chain
// entry whose preceding cmd is SETTIMG).  Fighter material textures are
// the most common example: lbCommonAddMObjForFighterPartsDObj walks the
// fighter's MObjSub array at draw time and builds the SETTIMG/LOADBLOCK
// pairs in heap, never embedding them in any file DL.
//
// Fast3D's texel readers expect bytes in N64 BE byte order:
//   4b/8b: per-byte data, addr[0] is the first pixel
//   16b: pixels are BE u16, read via `(addr[0] << 8) | addr[1]`
//   32b: pixels are BE [R,G,B,A], read addr[0]=R, addr[1]=G, ...
// Pass1 BSWAP32 reversed the byte order within each u32 word, so we apply
// BSWAP32 again to restore the original layout.
//
// Idempotent via sStructU16Fixups, keyed on the texture's base address
// (one entry per first-load).
extern "C" void portRelocFixupTextureAtRuntime(const void *addr, unsigned int num_bytes)
{
	if (addr == nullptr || num_bytes == 0) return;

	// Only fix data that lives inside a reloc file blob.  Heap-built textures
	// (procedurally generated, copied from elsewhere, etc.) are already in
	// correct host byte order.
	uintptr_t fileBase = 0;
	size_t    fileSize = 0;
	if (!portRelocFindContainingFile(addr, &fileBase, &fileSize))
	{
		// Log heap textures so we can spot any geometry texture that
		// somehow lives outside any reloc file (the "third path" case).
		if (tex_log_enabled()) {
			char heap_note[40];
			std::snprintf(heap_note, sizeof(heap_note),
			              "addr=0x%016llX",
			              (unsigned long long)reinterpret_cast<uintptr_t>(addr));
			tex_log_emit("runtime.heap", -1,
			             0u, num_bytes, -1, -1, -1, addr, heap_note);
		}
		return;
	}

	uintptr_t target = reinterpret_cast<uintptr_t>(addr);
	size_t    target_offset = target - fileBase;
	if (target_offset + num_bytes > fileSize)
	{
		num_bytes = (unsigned int)(fileSize - target_offset);
	}
	num_bytes &= ~3u;
	if (num_bytes == 0) return;

	uintptr_t fixup_key = target;
	bool already_fixed = sStructU16Fixups.count(fixup_key) != 0;
	if (already_fixed) {
		// Still record that the runtime path SAW this texture, even if some
		// earlier path already fixed it.  Distinguishes "wrong fmt fixup
		// applied earlier" from "never reached this path at all".
		if (tex_log_enabled()) {
			int rt_file_id = portRelocFindFileIdAndBase(addr, nullptr);
			tex_log_emit("runtime.skip", rt_file_id,
			             (uint32_t)target_offset, num_bytes,
			             -1, -1, -1, addr, "already-fixed");
		}
		return;
	}
	sStructU16Fixups.insert(fixup_key);

	uint32_t *region = reinterpret_cast<uint32_t *>(target);
	size_t num_words = num_bytes / 4;
	for (size_t i = 0; i < num_words; i++)
	{
		region[i] = BSWAP32(region[i]);
	}

	if (tex_log_enabled()) {
		int rt_file_id = portRelocFindFileIdAndBase(addr, nullptr);
		tex_log_emit("runtime.fix", rt_file_id,
		             (uint32_t)target_offset, num_bytes,
		             -1, -1, -1, region, "loadblock/loadtile/loadtlut");
	}
}

extern "C" void portRelocFixupVertexAtRuntime(const void *addr, unsigned int num_vtx)
{
	if (addr == nullptr || num_vtx == 0 || num_vtx > 32) return;

	// Only fix data that lives inside a reloc file blob.  Heap-built vertex
	// arrays (e.g., dynamic UI sprites) are constructed by game code with
	// already-correct byte order on the host LE side.
	uintptr_t fileBase = 0;
	size_t    fileSize = 0;
	if (!portRelocFindContainingFile(addr, &fileBase, &fileSize))
	{
		return;
	}

	uintptr_t target = reinterpret_cast<uintptr_t>(addr);
	size_t target_offset = target - fileBase;
	size_t total_bytes = (size_t)num_vtx * 16;
	if (target_offset + total_bytes > fileSize) return;

	// PER-VERTEX idempotency.  Game code can re-load OVERLAPPING sub-ranges
	// of the same vertex array (e.g. cmd 1 loads vertices 0..15 at addr A,
	// cmd 2 loads vertices 1..14 at addr A+16).  Address-keyed tracking
	// would treat these as distinct keys and double-fix the overlap region.
	// Instead we mark each individual 16-byte Vtx; only un-marked vertices
	// get fixed.
	uint32_t *region = reinterpret_cast<uint32_t *>(target);
	for (unsigned int i = 0; i < num_vtx; i++)
	{
		uintptr_t vtx_key = target + (uintptr_t)i * 16;
		if (sStructU16Fixups.count(vtx_key)) continue;
		sStructU16Fixups.insert(vtx_key);

		// word 0: s16 ob[0] | s16 ob[1] → rotate16
		region[i * 4 + 0] = (region[i * 4 + 0] << 16) | (region[i * 4 + 0] >> 16);
		// word 1: s16 ob[2] | u16 flag → rotate16
		region[i * 4 + 1] = (region[i * 4 + 1] << 16) | (region[i * 4 + 1] >> 16);
		// word 2: s16 tc[0] | s16 tc[1] → rotate16
		region[i * 4 + 2] = (region[i * 4 + 2] << 16) | (region[i * 4 + 2] >> 16);
		// word 3: u8 cn[0..3] → bswap32 to restore byte order
		region[i * 4 + 3] = BSWAP32(region[i * 4 + 3]);
	}
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
	uint8_t bmfmt    = *reinterpret_cast<uint8_t  *>(sprite + 0x30);
	uint8_t bmsiz    = *reinterpret_cast<uint8_t  *>(sprite + 0x31);

	if (nbitmaps <= 0)
		return;

	// G_IM_SIZ_ values: 4b=0, 8b=1, 16b=2, 32b=3, 4c=4 (compressed-4b)
	//
	// Fast3D's texel readers (ImportTextureRgba16/Rgba32/CI4/CI8/etc.) all
	// read texture bytes in N64 big-endian order — for example
	//   RGBA16: `(addr[0] << 8) | addr[1]`
	//   RGBA32: `r=addr[0], g=addr[1], b=addr[2], a=addr[3]`
	// So texel data must be left in the original BE byte order from the file.
	// Pass1 BSWAP32 destroys that order; another BSWAP32 restores it.
	//
	// 32bpp used to be skipped here on the assumption "pass1 byteswap is
	// already correct because the reader is per-channel byte-by-byte".  That
	// reasoning is wrong: pass1's per-u32 byte reverse turns each pixel's
	// `[R G B A]` into `[A B G R]` in PC memory, so the per-byte reader sees
	// channel-reversed RGBA32.  Symptom: HAL Labs dog (a 32bpp sprite) renders
	// with R↔A and G↔B swapped — brown becomes a different color entirely.
	// chain_fixup_settimg already had the same wrong assumption removed; this
	// is the matching fix on the sprite path.
	int bpp;
	switch (bmsiz)
	{
	case 0: bpp = 4;  break;   // 4b
	case 1: bpp = 8;  break;   // 8b
	case 2: bpp = 16; break;   // 16b
	case 3: bpp = 32; break;   // 32b
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

		if (tex_log_enabled() || tex_dump_enabled()) {
			uintptr_t bm_base = 0;
			int sprite_file_id = portRelocFindFileIdAndBase(buf, &bm_base);
			uint32_t off = (sprite_file_id >= 0)
				? (uint32_t)(reinterpret_cast<uintptr_t>(buf) - bm_base)
				: 0u;
			char note[64];
			std::snprintf(note, sizeof(note),
			              "sprite bm=%d w=%d h=%d", i, (int)width_img, (int)actualHeight);
			tex_log_emit("sprite.bitmap", sprite_file_id,
			             off, (uint32_t)tex_bytes,
			             -1, (int)bmsiz, bpp, words, note);
			tex_dump_known_dims(sprite_file_id, off,
			                    (int)bmfmt, (int)bmsiz, bpp,
			                    reinterpret_cast<const uint8_t *>(buf),
			                    (size_t)tex_bytes,
			                    (uint32_t)width_img,
			                    (uint32_t)actualHeight);
		}

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
		//
		// Why this also fires for 4b and 8b sprites (not just 16b):
		// lbCommonDrawSObjBitmap builds runtime LOAD_BLOCK commands using
		// G_IM_SIZ_{4b,8b,16b}_LOAD_BLOCK, all of which are #defined to
		// G_IM_SIZ_16b.  So every non-32bpp sprite is loaded into TMEM as
		// 16bpp, and the same odd-row XOR-0x4 swizzle applies to the DRAM
		// storage regardless of the sprite's logical bit depth.  The 4b/8b
		// case surfaces on fighter-intro title card text (e.g. "MARIO",
		// "YOSHI") where each letter is a 4b/8b sprite rendered via this
		// path; those letters were sheared until this fix was extended
		// beyond 16b.
		//
		// 4c (compressed) is excluded because the on-disk data is 2bpp
		// compressed source; the N64 game decodes it to 4b via
		// lbCommonDecodeSpriteBitmapsSiz4b AFTER this fixup runs, so the
		// swizzle would target the wrong byte layout.  The decoder emits
		// linear 4b output, and if any 4c sprite turns out to be swizzled
		// after decode it needs a separate post-decode fixup.
		if (bpp > 0 && bpp < 32 && bmsiz != 4 &&
		    width_img > 0 && actualHeight > 0)
		{
			// row_bytes = DRAM bytes per pixel row. For 4b this is width/2
			// (rounded up for odd widths); 8b is width; 16b is width*2.
			size_t row_bytes = ((size_t)width_img * (size_t)bpp + 7) / 8;
			// The swizzle operates on 8-byte qwords. Require the row to
			// hold at least one full qword and be qword-aligned so we
			// don't mis-swap partial trailing bytes.  Anything narrower
			// than 8 bytes (e.g. a 4b 12-wide sprite → row_bytes=6) has
			// no qword to swap, and a non-qword-aligned row would leave
			// the tail bytes in an inconsistent state.  In practice, real
			// font-style sprites are almost always power-of-2 or 8-aligned.
			if (row_bytes >= 8 && (row_bytes % 8) == 0)
			{
				uint8_t *bytes = static_cast<uint8_t *>(buf);
				for (int row = 1; row < actualHeight; row += 2)
				{
					uint8_t *row_p = bytes + (size_t)row * row_bytes;
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
