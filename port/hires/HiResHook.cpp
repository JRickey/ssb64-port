#include "HiResHook.h"

#include "HiResPack.h"
#include "../port_log.h"

#include <libultraship/libultraship.h>
#include <fast/interpreter.h>

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <unordered_set>

namespace {

// Diagnostic: when gHiResTextures.DumpMissRgba is enabled, the first
// ~64 unique cache misses get their decoded RGBA8 written to disk for
// visual inspection. Each file is named with the runtime's computed
// hash + format + dims, so they can be diffed against the GLideN64
// dump corpus to identify what category of texture isn't matching
// (e.g., 4-bit formats with subtle decoder differences, sub-tile
// loads where dimensions don't agree, etc.).
constexpr size_t kMaxMissDumps = 64;
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

void MaybeDumpMiss(uint8_t fmt, uint8_t siz, const uint8_t* rgba8, uint16_t w, uint16_t h) {
    if (CVarGetInteger("gHiResTextures.DumpMissRgba", 0) == 0) return;
    if (gMissDumped.size() >= kMaxMissDumps) return;

    size_t bytes = (size_t)w * h * 4;
    uint32_t crc = MissDumpCrc32(rgba8, bytes);
    if (!gMissDumped.insert(crc).second) return;

    if (!gMissDumpDirReady) {
        try { gMissDumpDir = Ship::Context::GetPathRelativeToAppDirectory("hires_miss_dump"); }
        catch (...) { gMissDumpDir = "hires_miss_dump"; }
        std::error_code ec;
        std::filesystem::create_directories(gMissDumpDir, ec);
        gMissDumpDirReady = true;
        port_log("HiResPack: miss-dump active -> %s (cap=%zu)\n", gMissDumpDir.c_str(), kMaxMissDumps);
    }

    char namebuf[96];
    std::snprintf(namebuf, sizeof(namebuf),
                  "miss#%08X#%X#%X_%ux%u.rgba", crc, fmt, siz, (unsigned)w, (unsigned)h);
    std::string path = gMissDumpDir + "/" + namebuf;
    if (FILE* f = std::fopen(path.c_str(), "wb")) {
        std::fwrite(rgba8, 1, bytes, f);
        std::fclose(f);
    }
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
