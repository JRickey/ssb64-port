#pragma once

/* Hi-res texture pack registry.
 *
 * Scans <app-data>/mods/ recursively at startup for PNG files whose names
 * follow the grammar:
 *
 *   <anything>#<rgba8CRC32>#<fmt>#<siz>[_<anything>].png
 *
 * The hash key is a CRC32-IEEE over the *decoded* RGBA8 image — the same
 * tightly-packed pixel buffer the GPU receives at upload time. This makes
 * the key independent of N64 source byte layout (Sprite/Bitmap/raw/CI all
 * decode to the same RGBA8 and hash the same), so the runtime hook can
 * sit downstream of every format-specific decoder and the offline pack-
 * conversion tool can derive the same key by decoding each pack PNG.
 *
 * Each parsed entry is stored in an in-memory map keyed by
 * (rgba8CRC, fmt, siz). PNG decoding is deferred to first lookup; Init()
 * only builds the index.
 *
 * Pack folders work additively — drop multiple subfolders under mods/ and
 * the union of their PNGs is indexed. If two PNGs hash-collide the later
 * scan wins (alphabetical order).
 */

#include <cstdint>
#include <cstddef>
#include <vector>

namespace ssb64::hires {

struct DecodedTexture {
    std::vector<uint8_t> rgba; // tightly packed RGBA8, w * h * 4 bytes
    uint16_t w = 0;
    uint16_t h = 0;
};

struct HashKey {
    uint32_t rgba8Crc;
    uint8_t fmt;
    uint8_t siz;
    uint8_t pad0 = 0, pad1 = 0;

    bool operator==(const HashKey&) const noexcept = default;
};

struct PackStats {
    size_t scannedFiles;     // every file under mods/, .png or not
    size_t indexedTextures;  // PNGs whose names parsed successfully
    size_t skippedFilenames; // PNGs that did not match the grammar
    size_t collisions;       // duplicate hash keys (later scan wins)
};

struct LookupStats {
    uint64_t lookups;     // every Lookup() call (one per cache miss)
    uint64_t hits;        // resolved to a decoded RGBA8 buffer
    uint64_t decodeFails; // index hit but stbi_load failed (entry now removed)
};

class HiResPack {
public:
    static HiResPack& Get();

    /* Resolves <app-data>/mods/, walks every file recursively, parses each
     * .png filename, and builds the in-memory index. Safe to call before
     * Window/ResourceManager are up — only depends on Ship::Context being
     * alive. Returns true if at least one mods/ subdirectory existed; the
     * pack itself is allowed to be empty. */
    bool Init();

    /* Diagnostic snapshot of the last Init() pass. */
    const PackStats& Stats() const noexcept { return mStats; }

    /* Resolved absolute path to the scanned mods/ root, for logging /
     * "Open mods folder" UI button. Empty before Init(). */
    const char* ModsRoot() const noexcept;

    /* Hash the decoded RGBA8 image (CRC32-IEEE over `width*height*4` bytes)
     * and return a substitute decoded RGBA8 buffer if the pack contains a
     * matching PNG. Decode of the pack PNG is lazy + memoized; a small LRU
     * caps total decoded RAM at ~256 MB. The returned pointer is owned by
     * the LRU and stays valid through the calling frame's UploadTexture
     * (eviction only fires on subsequent Lookup() calls that miss the
     * cache, never during this call). Returns nullptr on miss / decode
     * failure.
     *
     * Format-independent and byte-layout-independent: the input buffer is
     * the exact tightly-packed RGBA8 image the GPU is about to receive, so
     * Sprite-fixup / Bitmap-fixup / raw / CI-with-palette paths all collapse
     * to the same key for the same visible texture content.
     */
    const DecodedTexture* Lookup(uint8_t fmt, uint8_t siz,
                                 const uint8_t* rgba8,
                                 uint16_t width, uint16_t height);

    /* Live counters since process start (or last ResetLookupStats). */
    LookupStats LookupStatsSnapshot() const noexcept { return mLookupStats; }
    void ResetLookupStats() noexcept { mLookupStats = {}; }

    /* When enabled (gHiResTextures.DumpSource CVar), dumps the source N64
     * byte run + dimensions + palette to a .bin file under
     * <app-data>/hires_dump/. Used by the offline GPL pack conversion tool
     * (see tools/hires-pack-convert/) to translate Reloaded Rice-CRC PNG
     * names into our decoded-RGBA8-CRC names.
     *
     * NOTE: As of the post-decode hook refactor this function is currently
     * unreachable from the runtime path (Lookup() runs after the decode and
     * no longer has source bytes). The dump call site needs to be re-wired
     * into a pre-decode hook in libultraship; until then dump-mode is
     * dormant. The implementation is kept intact so that re-wire is a
     * one-line change. */
    void MaybeDumpSource(uint8_t fmt, uint8_t siz,
                         const uint8_t* texels, uint16_t width, uint16_t height, uint32_t bpl,
                         const uint8_t* palette, uint32_t paletteBytes);

private:
    HiResPack() = default;
    HiResPack(const HiResPack&) = delete;
    HiResPack& operator=(const HiResPack&) = delete;

    PackStats mStats{};
    LookupStats mLookupStats{};
};

} // namespace ssb64::hires
