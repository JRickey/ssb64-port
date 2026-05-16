#include "HiResHook.h"

#include "HiResPack.h"
#include "../port_log.h"

#include <libultraship/libultraship.h>
#include <fast/interpreter.h>

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>

namespace {

// Diagnostic: when gHiResTextures.DumpMissRgba is enabled, each unique
// cache miss (deduped by decoded-RGBA8 CRC) gets written to disk for
// visual inspection. Each file is named with the runtime's computed
// native key (`miss#<CRC32-IEEE>#<fmt>#<siz>_<w>x<h>`) — the exact
// `<hash>#<fmt>#<siz>` the pack scanner looks up, so a Reloaded PNG
// renamed to that key (any prefix, optional `_suffix`) drops straight
// into mods/ and binds with zero Rice/mupen involvement. Used to cover
// the residual textures whose Rice CRC the GLideN64 algorithm can't
// reach (tiny UI/HUD glyphs: VS records digits, etc.).
//
// The cap is a CVar (gHiResTextures.MissDumpCap, default 8192 — enough
// to capture everything reachable in one playthrough) so you can run
// straight through to the VS-records screen and collect every miss.
// A 32-bit BMP is written alongside the raw .rgba so the vanilla glyph
// can be eyeballed against the pack PNG to find its pair.
constexpr size_t kDefaultMissDumpCap = 8192;
std::unordered_set<uint32_t> gMissDumped;
std::string gMissDumpDir;
bool gMissDumpDirReady = false;

uint32_t MissDumpCrc32(const uint8_t* p, size_t n) {
    static uint32_t tbl[256];
    static bool init = false;
    if (!init) {
        for (uint32_t i = 0; i < 256; i++) {
            uint32_t c = i;
            for (int j = 0; j < 8; j++) c = (c >> 1) ^ (0xEDB88320u & -(c & 1u));
            tbl[i] = c;
        }
        init = true;
    }
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < n; i++) crc = tbl[(crc ^ p[i]) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFFu;
}

// Minimal self-contained 32-bit BI_RGB BMP (bottom-up, BGRA). No deps.
// Viewers show it opaque (BI_RGB ignores alpha) which is exactly what we
// want for identifying which glyph this is. Returns false on any I/O error.
bool WriteBmp32(const std::string& path, const uint8_t* rgba8, uint32_t w, uint32_t h) {
    const uint32_t pixels = w * h;
    const uint32_t imgBytes = pixels * 4u;
    const uint32_t fileBytes = 54u + imgBytes;
    uint8_t hdr[54] = {0};
    hdr[0] = 'B'; hdr[1] = 'M';
    auto put32 = [](uint8_t* p, uint32_t v) {
        p[0] = v & 0xFF; p[1] = (v >> 8) & 0xFF; p[2] = (v >> 16) & 0xFF; p[3] = (v >> 24) & 0xFF;
    };
    put32(hdr + 2, fileBytes);
    put32(hdr + 10, 54);          // pixel-data offset
    put32(hdr + 14, 40);          // BITMAPINFOHEADER size
    put32(hdr + 18, w);
    put32(hdr + 22, h);           // positive = bottom-up
    hdr[26] = 1;                  // planes
    hdr[28] = 32;                 // bpp
    put32(hdr + 34, imgBytes);
    FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) return false;
    std::fwrite(hdr, 1, 54, f);
    // BMP rows are bottom-up; convert RGBA -> BGRA per pixel.
    std::vector<uint8_t> row(w * 4u);
    for (int32_t y = (int32_t)h - 1; y >= 0; --y) {
        const uint8_t* src = rgba8 + (size_t)y * w * 4u;
        for (uint32_t x = 0; x < w; ++x) {
            row[x * 4 + 0] = src[x * 4 + 2]; // B
            row[x * 4 + 1] = src[x * 4 + 1]; // G
            row[x * 4 + 2] = src[x * 4 + 0]; // R
            row[x * 4 + 3] = src[x * 4 + 3]; // A
        }
        std::fwrite(row.data(), 1, row.size(), f);
    }
    std::fclose(f);
    return true;
}

void MaybeDumpMiss(uint8_t fmt, uint8_t siz, const uint8_t* rgba8, uint16_t w, uint16_t h) {
    if (CVarGetInteger("gHiResTextures.DumpMissRgba", 0) == 0) return;
    const size_t cap = (size_t)CVarGetInteger("gHiResTextures.MissDumpCap",
                                              (int32_t)kDefaultMissDumpCap);
    if (gMissDumped.size() >= cap) return;

    size_t bytes = (size_t)w * h * 4;
    uint32_t crc = MissDumpCrc32(rgba8, bytes);
    if (!gMissDumped.insert(crc).second) return;

    if (!gMissDumpDirReady) {
        try { gMissDumpDir = Ship::Context::GetPathRelativeToAppDirectory("hires_miss_dump"); }
        catch (...) { gMissDumpDir = "hires_miss_dump"; }
        std::error_code ec;
        std::filesystem::create_directories(gMissDumpDir, ec);
        gMissDumpDirReady = true;
        port_log("HiResPack: miss-dump active -> %s (cap=%zu)\n", gMissDumpDir.c_str(), cap);
    }

    // Stem encodes the port's native lookup key verbatim:
    //   miss#<CRC32-IEEE>#<fmt>#<siz>_<w>x<h>
    // Rename the matching Reloaded hi-res PNG to "<anything>#<crc>#<fmt>#<siz>_all.png"
    // (the prefix and _suffix are free-form) and drop it in mods/ to bind it.
    char stem[96];
    std::snprintf(stem, sizeof(stem),
                  "miss#%08X#%X#%X_%ux%u", crc, fmt, siz, (unsigned)w, (unsigned)h);
    std::string base = gMissDumpDir + "/" + stem;

    if (FILE* f = std::fopen((base + ".rgba").c_str(), "wb")) {
        std::fwrite(rgba8, 1, bytes, f);
        std::fclose(f);
    }
    WriteBmp32(base + ".bmp", rgba8, w, h);
}

bool HiResHook(uint8_t fmt, uint8_t siz,
               const uint8_t* rgba8, uint16_t width, uint16_t height,
               const uint8_t** outBuf, uint16_t* outW, uint16_t* outH) {
    // Master enable lives in a CVar so the menu toggle takes effect
    // immediately (no relaunch). Defaults to on — Init() already logged
    // whether the mods/ folder exists, so a flat "no PNGs to substitute"
    // setup is silently a no-op.
    if (CVarGetInteger("gHiResTextures.Enabled", 1) == 0) {
        return false;
    }

    const ssb64::hires::DecodedTexture* dec =
        ssb64::hires::HiResPack::Get().Lookup(fmt, siz, rgba8, width, height);
    if (dec == nullptr) {
        MaybeDumpMiss(fmt, siz, rgba8, width, height);
        return false;
    }

    *outBuf = dec->rgba.data();
    *outW = dec->w;
    *outH = dec->h;
    return true;
}

} // namespace

extern "C" void ssb64_hires_register(void) {
    gfx_register_hires_hook(&HiResHook);
    port_log("HiResPack: hook registered with libultraship Fast3D\n");
}
