#include "HiResPack.h"

#include "../port_log.h"

#include <libultraship/libultraship.h>
#include <ship/utils/filesystemtools/Directory.h>

// stb_image's implementation lives in libultraship's stb_impl.c (see
// libultraship/cmake/dependencies/common.cmake) — only include the header
// here to pick up the function declarations.
#include <stb_image.h>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <filesystem>
#include <list>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace ssb64::hires {

namespace {

struct HashKeyHasher {
    size_t operator()(const HashKey& k) const noexcept {
        // FNV-1a over the four payload bytes of rgba8Crc is plenty — the
        // CRC32 itself already approximates uniform distribution; folding
        // (fmt|siz) avoids degenerate collisions when packs duplicate the
        // same RGBA8 image across multiple N64 formats.
        uint64_t h = 1469598103934665603ULL;
        for (int i = 0; i < 4; i++) {
            h ^= (k.rgba8Crc >> (i * 8)) & 0xFFu;
            h *= 1099511628211ULL;
        }
        h ^= (uint64_t)k.fmt << 32;
        h ^= (uint64_t)k.siz << 40;
        h *= 1099511628211ULL;
        return (size_t)h;
    }
};

// Index of all parsed pack PNGs. Phase 3 reads it from Lookup().
std::unordered_map<HashKey, std::string, HashKeyHasher> gIndex;

std::string gModsRoot;

// Per-process dedup so dump-mode writes each unique key at most once per
// run. Protects the disk from being hammered when the same texture cycles
// through the texture cache repeatedly.
std::unordered_set<uint64_t> gDumpedKeys;
std::string gDumpDir;
bool gDumpDirReady = false;

constexpr uint32_t kDumpMagic   = 0x44524853u; // 'SHRD' little-endian
constexpr uint32_t kDumpVersion = 1u;
struct DumpHeader {
    uint32_t magic;       // 'SHRD'
    uint32_t version;     // 1
    uint32_t width;       // post-mask/clamp pixel width
    uint32_t height;      // post-mask/clamp pixel height
    uint32_t bpl;         // source row stride in bytes
    uint32_t texelBytes;  // = bytesPerLine * height
    uint32_t paletteBytes;
    uint8_t  fmt;         // G_IM_FMT_*
    uint8_t  siz;         // G_IM_SIZ_*
    uint8_t  pad[6];
};
static_assert(sizeof(DumpHeader) == 36, "DumpHeader must be 36 bytes");

uint64_t SourceDumpDedupId(uint8_t fmt, uint8_t siz, uint32_t texelCrc, uint32_t palCrc) noexcept {
    return ((uint64_t)texelCrc) | ((uint64_t)palCrc << 32) ^ ((uint64_t)fmt << 8) ^ ((uint64_t)siz << 16);
}

// CRC32-IEEE 802.3 (poly 0xEDB88320) — public-domain table-driven impl.
// Not Rice CRC32: that algorithm originated in GPL N64 plugins and is
// license-incompatible with this codebase. Pack PNGs intended for this
// port are named with this hash instead, produced offline by a separate
// (unbundled, GPL) conversion tool that decodes each source-byte dump to
// RGBA8 and computes the same plain CRC32-IEEE over the decoded pixels.
struct Crc32Table {
    uint32_t v[256];
    constexpr Crc32Table() : v{} {
        for (uint32_t i = 0; i < 256; i++) {
            uint32_t c = i;
            for (int j = 0; j < 8; j++) c = (c >> 1) ^ (0xEDB88320u & -(c & 1u));
            v[i] = c;
        }
    }
};
const Crc32Table gCrcTbl;

// Plain CRC32-IEEE over a flat byte run. Used for the decoded-RGBA8 hash
// (post-decode, no row stride) and for hashing source bytes / palette
// bytes in dump-source mode.
uint32_t Crc32Bytes(const uint8_t* src, size_t bytes) noexcept {
    if (src == nullptr || bytes == 0) return 0;
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < bytes; i++) {
        crc = gCrcTbl.v[(crc ^ src[i]) & 0xFFu] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFu;
}

// CRC32-IEEE over a row-strided source-byte image — kept for the
// dump-source helper, which still records source bytes so the offline
// conversion tool can pair Reloaded packs to our decoded-RGBA8 names.
uint32_t SourceTexelCrc(const uint8_t* src, int width, int height,
                        int size, int rowStride) noexcept {
    if (src == nullptr || width <= 0 || height <= 0 || rowStride <= 0) return 0;
    const int bytesPerLine = (width << size) >> 1;
    if (bytesPerLine <= 0) return 0;

    uint32_t crc = 0xFFFFFFFFu;
    for (int y = 0; y < height; y++) {
        const uint8_t* row = src + (size_t)y * rowStride;
        for (int x = 0; x < bytesPerLine; x++) {
            crc = gCrcTbl.v[(crc ^ row[x]) & 0xFFu] ^ (crc >> 8);
        }
    }
    return crc ^ 0xFFFFFFFFu;
}

// Decoded-RGBA LRU. Insert always succeeds (the just-inserted entry is at
// the tail and is exempt from eviction); eviction only fires on subsequent
// Insert() calls, never during a single Lookup() — so the pointer returned
// from Lookup() is guaranteed valid through the immediately-following
// UploadTexture call.
class LruCache {
public:
    using Node = std::pair<HashKey, DecodedTexture>;

    explicit LruCache(size_t budgetBytes) : mBudget(budgetBytes) {}

    const DecodedTexture* Get(const HashKey& k) {
        auto it = mIndex.find(k);
        if (it == mIndex.end()) return nullptr;
        // Move the hit to MRU end.
        mList.splice(mList.end(), mList, it->second);
        return &it->second->second;
    }

    void Insert(const HashKey& k, DecodedTexture&& tex) {
        // If the same key already lives in the cache, drop the older entry.
        if (auto it = mIndex.find(k); it != mIndex.end()) {
            mBytes -= it->second->second.rgba.size();
            mList.erase(it->second);
            mIndex.erase(it);
        }
        size_t addBytes = tex.rgba.size();
        mList.emplace_back(k, std::move(tex));
        mIndex[k] = std::prev(mList.end());
        mBytes += addBytes;
        // Evict from the LRU end (front) until back under budget. Never
        // evict the tail — that's what we just inserted.
        while (mBytes > mBudget && mList.size() > 1) {
            auto& front = mList.front();
            mBytes -= front.second.rgba.size();
            mIndex.erase(front.first);
            mList.pop_front();
        }
    }

    size_t Bytes() const noexcept { return mBytes; }

private:
    std::list<Node> mList;
    std::unordered_map<HashKey, std::list<Node>::iterator, HashKeyHasher> mIndex;
    size_t mBytes = 0;
    size_t mBudget;
};

constexpr size_t kDefaultLruBudget = 512u * 1024u * 1024u; // 512 MB
LruCache gLru{ kDefaultLruBudget };

bool IsHexDigit(char c) noexcept {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

bool ParseHex32(std::string_view s, uint32_t* out) noexcept {
    if (s.size() != 8) return false;
    uint32_t v = 0;
    for (char c : s) {
        if (!IsHexDigit(c)) return false;
        uint32_t d = (c <= '9') ? (c - '0') : ((c | 0x20) - 'a' + 10);
        v = (v << 4) | d;
    }
    *out = v;
    return true;
}

bool ParseSingleDigit(std::string_view s, uint8_t* out) noexcept {
    if (s.size() != 1 || s[0] < '0' || s[0] > '9') return false;
    *out = (uint8_t)(s[0] - '0');
    return true;
}

/* Parse a hires-pack filename into a HashKey.
 *
 *   <prefix>#<rgba8CRC8>#<fmt>#<siz>[_<anything>].png
 *
 * Returns nullopt on grammar mismatch. We accept any prefix (including
 * none) — the prefix is decorative.
 *
 * Examples that parse successfully:
 *   "SMASH BROTHERS#A1B2C3D4#0#2_all.png"   RGBA16 (decoded RGBA8 hash)
 *   "anything#cafe1234#3#0.png"             IA4 (no fmtTag suffix)
 *   "anything#cafe1234#3#0_anything.png"    IA4 with arbitrary fmtTag
 *
 * The fmt+siz fields are kept in the filename as a sanity check / human-
 * readable hint, but they're folded into the HashKey so two textures whose
 * decoded RGBA8 happens to collide across formats stay distinct.
 */
std::optional<HashKey> ParseFilename(std::string_view filename) {
    // Strip trailing ".png" (case-insensitive).
    if (filename.size() < 5) return std::nullopt;
    std::string_view ext = filename.substr(filename.size() - 4);
    bool isPng = (ext.size() == 4)
        && (ext[0] == '.')
        && ((ext[1] | 0x20) == 'p')
        && ((ext[2] | 0x20) == 'n')
        && ((ext[3] | 0x20) == 'g');
    if (!isPng) return std::nullopt;
    std::string_view stem = filename.substr(0, filename.size() - 4);

    // Optional trailing _<fmtTag>: strip everything from the rightmost '_'
    // forward IF it sits after the last '#' (so we don't eat underscores
    // inside the prefix).
    size_t lastHash = stem.rfind('#');
    size_t lastUnder = stem.rfind('_');
    if (lastUnder != std::string_view::npos && (lastHash == std::string_view::npos || lastUnder > lastHash)) {
        stem = stem.substr(0, lastUnder);
    }

    // Split the body on '#' from the right: <prefix>#<crc>#<fmt>#<siz>.
    size_t h3 = stem.rfind('#');
    if (h3 == std::string_view::npos) return std::nullopt;
    size_t h2 = stem.rfind('#', h3 - 1);
    if (h2 == std::string_view::npos) return std::nullopt;
    size_t h1 = stem.rfind('#', h2 - 1);
    if (h1 == std::string_view::npos) return std::nullopt;

    std::string_view crcField = stem.substr(h1 + 1, h2 - h1 - 1);
    std::string_view fmtField = stem.substr(h2 + 1, h3 - h2 - 1);
    std::string_view sizField = stem.substr(h3 + 1);

    HashKey key{};
    if (!ParseHex32(crcField, &key.rgba8Crc)) return std::nullopt;
    if (!ParseSingleDigit(fmtField, &key.fmt)) return std::nullopt;
    if (!ParseSingleDigit(sizField, &key.siz)) return std::nullopt;

    return key;
}

} // namespace

HiResPack& HiResPack::Get() {
    static HiResPack inst;
    return inst;
}

const char* HiResPack::ModsRoot() const noexcept {
    return gModsRoot.c_str();
}

bool HiResPack::Init() {
    mStats = {};
    gIndex.clear();

    // Resolve <app-data>/mods alongside BattleShip.o2r and ssb64_save.bin.
    // Same convention as port_save.cpp.
    try {
        gModsRoot = Ship::Context::GetPathRelativeToAppDirectory("mods");
    } catch (...) {
        gModsRoot.clear();
        port_log("HiResPack: Ship::Context not ready; mods/ disabled\n");
        return false;
    }

    if (!Directory::Exists(gModsRoot)) {
        port_log("HiResPack: %s does not exist; create it and drop a pack inside to enable\n",
                 gModsRoot.c_str());
        return false;
    }

    auto files = Directory::ListFiles(gModsRoot); // recursive
    std::sort(files.begin(), files.end()); // deterministic collision-winner

    for (const std::string& path : files) {
        mStats.scannedFiles++;

        // Extract the basename for parsing.
        size_t slash = path.find_last_of("/\\");
        std::string_view name = (slash == std::string::npos)
            ? std::string_view(path)
            : std::string_view(path).substr(slash + 1);

        // Cheap pre-filter: must end in .png.
        if (name.size() < 5) continue;
        std::string_view ext = name.substr(name.size() - 4);
        if (ext.size() != 4 || ext[0] != '.'
            || (ext[1] | 0x20) != 'p' || (ext[2] | 0x20) != 'n' || (ext[3] | 0x20) != 'g') {
            continue;
        }

        auto key = ParseFilename(name);
        if (!key) {
            mStats.skippedFilenames++;
            continue;
        }

        auto [it, inserted] = gIndex.try_emplace(*key, path);
        if (!inserted) {
            mStats.collisions++;
            it->second = path; // last-scanned (alphabetically last) wins
        } else {
            mStats.indexedTextures++;
        }
    }

    port_log("HiResPack: scanned %s — %zu files, %zu indexed, %zu unparsed PNGs, %zu hash collisions\n",
             gModsRoot.c_str(),
             mStats.scannedFiles, mStats.indexedTextures,
             mStats.skippedFilenames, mStats.collisions);
    return true;
}

const DecodedTexture* HiResPack::Lookup(uint8_t fmt, uint8_t siz,
                                         const uint8_t* rgba8,
                                         uint16_t width, uint16_t height) {
    if (gIndex.empty() || rgba8 == nullptr || width == 0 || height == 0) return nullptr;

    mLookupStats.lookups++;

    HashKey key{};
    key.fmt = fmt;
    key.siz = siz;
    key.rgba8Crc = Crc32Bytes(rgba8, (size_t)width * height * 4);

    // Per-call diagnostic + hit/miss flag for the first 8 lookups — lets us
    // confirm the hook is firing on real boots without spamming the log
    // during normal play.
    bool will_hit = gIndex.find(key) != gIndex.end();
    if (mLookupStats.lookups <= 8) {
        port_log("HiResPack: lookup #%llu fmt=%u siz=%u w=%u h=%u "
                 "key=%08X#%X#%X %s\n",
                 (unsigned long long)mLookupStats.lookups,
                 fmt, siz, width, height,
                 key.rgba8Crc, key.fmt, key.siz,
                 will_hit ? "HIT" : "miss");
    }
    // Periodic log so the user can see coverage drift while playing — every
    // 64 cache misses (a few seconds of typical gameplay).
    if ((mLookupStats.lookups & 0x3F) == 0) {
        double rate = mLookupStats.lookups
            ? (100.0 * (double)mLookupStats.hits / (double)mLookupStats.lookups)
            : 0.0;
        port_log("HiResPack: %llu lookups, %llu hits (%.1f%%), %llu decode-fails, LRU=%zu MB\n",
                 (unsigned long long)mLookupStats.lookups,
                 (unsigned long long)mLookupStats.hits,
                 rate,
                 (unsigned long long)mLookupStats.decodeFails,
                 gLru.Bytes() / (1024u * 1024u));
    }

    if (const DecodedTexture* hit = gLru.Get(key)) {
        mLookupStats.hits++;
        return hit;
    }

    auto it = gIndex.find(key);
    if (it == gIndex.end()) {
        return nullptr;
    }

    int w = 0, h = 0, ch = 0;
    uint8_t* raw = stbi_load(it->second.c_str(), &w, &h, &ch, 4);
    if (raw == nullptr || w <= 0 || h <= 0 || w > 65535 || h > 65535) {
        if (raw) stbi_image_free(raw);
        port_log("HiResPack: stbi_load failed for %s (%s)\n",
                 it->second.c_str(), stbi_failure_reason() ? stbi_failure_reason() : "?");
        // Drop the entry so we don't keep retrying every cache miss.
        gIndex.erase(it);
        mLookupStats.decodeFails++;
        return nullptr;
    }

    DecodedTexture tex;
    tex.w = (uint16_t)w;
    tex.h = (uint16_t)h;
    tex.rgba.assign(raw, raw + (size_t)w * h * 4);
    stbi_image_free(raw);

    gLru.Insert(key, std::move(tex));
    mLookupStats.hits++;
    return gLru.Get(key);
}

void HiResPack::MaybeDumpSource(uint8_t fmt, uint8_t siz,
                                 const uint8_t* texels, uint16_t width, uint16_t height, uint32_t bpl,
                                 const uint8_t* palette, uint32_t paletteBytes) {
    // Cheap CVar check up front. The hook layer reads the same CVar for the
    // master enable; reading it here keeps dump-on toggling responsive.
    if (CVarGetInteger("gHiResTextures.DumpSource", 0) == 0) return;
    if (texels == nullptr || width == 0 || height == 0 || bpl == 0) return;

    const uint32_t texelCrc = SourceTexelCrc(texels, width, height, siz, (int)bpl);
    const uint32_t palCrc = (palette != nullptr && paletteBytes > 0)
                                ? Crc32Bytes(palette, paletteBytes)
                                : 0;

    uint64_t dedup = SourceDumpDedupId(fmt, siz, texelCrc, palCrc);
    if (!gDumpedKeys.insert(dedup).second) return; // already dumped this run

    if (!gDumpDirReady) {
        try {
            gDumpDir = Ship::Context::GetPathRelativeToAppDirectory("hires_dump");
        } catch (...) { gDumpDir = "hires_dump"; }
        std::error_code ec;
        std::filesystem::create_directories(gDumpDir, ec);
        gDumpDirReady = true;
        port_log("HiResPack: dump-source mode active, writing to %s\n", gDumpDir.c_str());
    }

    // Filename includes source-byte CRC + (optionally) palette CRC so the
    // offline conversion tool can pair each .bin with its Reloaded PNG by
    // looking up Rice CRC of the dumped texel bytes.
    char namebuf[96];
    if (palCrc != 0 || paletteBytes > 0) {
        std::snprintf(namebuf, sizeof(namebuf),
                      "ssb64src#%08X#%X#%X#%08X.bin",
                      texelCrc, fmt, siz, palCrc);
    } else {
        std::snprintf(namebuf, sizeof(namebuf),
                      "ssb64src#%08X#%X#%X.bin",
                      texelCrc, fmt, siz);
    }

    std::string path = gDumpDir + "/" + namebuf;
    FILE* f = std::fopen(path.c_str(), "wb");
    if (f == nullptr) {
        port_log("HiResPack: dump-source open failed: %s\n", path.c_str());
        return;
    }

    DumpHeader hdr{};
    hdr.magic = kDumpMagic;
    hdr.version = kDumpVersion;
    hdr.width = width;
    hdr.height = height;
    hdr.bpl = bpl;
    const uint32_t bytesPerLine = (uint32_t)((width << siz) >> 1);
    hdr.texelBytes = bytesPerLine * height;
    hdr.paletteBytes = paletteBytes;
    hdr.fmt = fmt;
    hdr.siz = siz;

    std::fwrite(&hdr, sizeof(hdr), 1, f);
    // Texel bytes — write only the active rows (bytesPerLine each, row-by-
    // row at bpl stride). The conversion tool reads them as a contiguous
    // bytesPerLine*height blob, no stride.
    for (uint32_t y = 0; y < height; y++) {
        std::fwrite(texels + (size_t)y * bpl, 1, bytesPerLine, f);
    }
    if (paletteBytes > 0 && palette != nullptr) {
        std::fwrite(palette, 1, paletteBytes, f);
    }
    std::fclose(f);
}

} // namespace ssb64::hires
