/**
 * audio_bridge.cpp — Port-side audio asset loader
 *
 * Loads audio BLOBs from .o2r archive and parses the big-endian N64 binary
 * format into native C structs with correct 64-bit pointer width.
 *
 * N64 audio banks (ALBankFile) are nested structures where every pointer
 * field is a 4-byte offset in the ROM binary.  On PC, pointer fields are
 * 8 bytes, so we can't memcpy the binary into the C structs.  Instead we
 * parse each struct field-by-field and allocate native structs from the
 * audio heap.
 *
 * TBL (sample table) data is raw ADPCM bytes — loaded as-is, no parsing.
 * Sequence data inside the SBK is also raw bytes, just needs header parsing.
 */

#include <ship/Context.h>
#include <ship/resource/Resource.h>
#include <ship/resource/ResourceManager.h>
#include <ship/resource/type/Blob.h>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <cstring>
#include <unordered_map>
#include <vector>

// We include the port bridge header with C linkage
extern "C" {
#include "bridge/audio_bridge.h"
}

// Forward-declare decomp types and functions we need.
// We define minimal compatible layouts here to avoid pulling in the full
// decomp include/ directory which conflicts with C++ standard headers.

// From PR/ultratypes.h
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;

// From PR/abi.h
typedef short ADPCM_STATE[16];

// From PR/libaudio.h — must match decomp layouts exactly
typedef s32 ALMicroTime;
typedef u8  ALPan;

typedef struct {
    s32 order;
    s32 npredictors;
    s16 book[1]; /* variable length: order * npredictors * 8 entries */
} ALADPCMBook;

typedef struct {
    u32 start;
    u32 end;
    u32 count;
    ADPCM_STATE state;
} ALADPCMloop;

typedef struct {
    u32 start;
    u32 end;
    u32 count;
} ALRawLoop;

typedef struct {
    ALMicroTime attackTime;
    ALMicroTime decayTime;
    ALMicroTime releaseTime;
    u8 attackVolume;
    u8 decayVolume;
} ALEnvelope;

typedef struct {
    u8 velocityMin;
    u8 velocityMax;
    u8 keyMin;
    u8 keyMax;
    u8 keyBase;
    s8 detune;
} ALKeyMap;

typedef struct {
    ALADPCMloop* loop;
    ALADPCMBook* book;
} ALADPCMWaveInfo;

typedef struct {
    ALRawLoop* loop;
} ALRAWWaveInfo;

#define AL_ADPCM_WAVE 0
#define AL_RAW16_WAVE 1

typedef struct ALWaveTable_s {
    u8* base;
    s32 len;
    u8  type;
    u8  flags;
    union {
        ALADPCMWaveInfo adpcmWave;
        ALRAWWaveInfo   rawWave;
    } waveInfo;
} ALWaveTable;

typedef struct ALSound_s {
    ALEnvelope*  envelope;
    ALKeyMap*    keyMap;
    ALWaveTable* wavetable;
    ALPan        samplePan;
    u8           sampleVolume;
    u8           flags;
} ALSound;

typedef struct {
    u8          volume;
    ALPan       pan;
    u8          priority;
    u8          flags;
    u8          tremType;
    u8          tremRate;
    u8          tremDepth;
    u8          tremDelay;
    u8          vibType;
    u8          vibRate;
    u8          vibDepth;
    u8          vibDelay;
    s16         bendRange;
    s16         soundCount;
    ALSound*    soundArray[1];
} ALInstrument;

typedef struct ALBank_s {
    s16           instCount;
    u8            flags;
    u8            pad;
    s32           sampleRate;
    ALInstrument* percussion;
    ALInstrument* instArray[1];
} ALBank;

typedef struct {
    s16      revision;
    s16      bankCount;
    ALBank*  bankArray[1];
} ALBankFile;

typedef struct {
    u8* offset;
    s32 len;
} ALSeqData;

typedef struct {
    s16       revision;
    s16       seqCount;
    ALSeqData seqArray[1];
} ALSeqFile;

// From sys/audio.h
typedef struct {
    u8*       heap_base;
    size_t    heap_size;
    u16       output_rate;
    u8        pvoices_num_max;
    u8        vvoices_num_max;
    u8        updates_num_max;
    u8        events_num_max;
    u8        sounds_num_max;
    u8        voices_num_max[2];
    u8        unk11;
    u8        unk12;
    u8        priority;
    uintptr_t bank1_start;
    uintptr_t bank1_end;
    void*     table1_start;
    uintptr_t bank2_start;
    uintptr_t bank2_end;
    void*     table2_start;
    uintptr_t sbk_start;
    u8        fx_type;
    u8        unk31;
    u8        unk32;
    u8        sndplayers_num;
    u16       unk34;
    u16       unk36;
    s32       unk38;
    uintptr_t* fgm_ucode_data;
    uintptr_t* fgm_table_data;
    uintptr_t* unk44;
    u16       fgm_ucode_count;
    u16       fgm_table_count;
    u16       unk4C;
    uintptr_t unk50;
    uintptr_t unk54;
    uintptr_t fgm_table_start;
    uintptr_t fgm_table_end;
    uintptr_t fgm_ucode_start;
    uintptr_t fgm_ucode_end;
} SYAudioSettings;

typedef struct SYAudioPackage {
    s32       count;
    uintptr_t data[1];
} SYAudioPackage;

typedef struct {
    u8* base;
    u8* cur;
    s32 len;
    s32 count;
} ALHeap;

// Decomp externs we call
extern "C" {
    extern SYAudioSettings dSYAudioPublicSettings;
    extern SYAudioSettings dSYAudioPublicSettings2;
    extern SYAudioSettings dSYAudioPublicSettings3;
    extern SYAudioSettings sSYAudioCurrentSettings;
    extern ALBank*    sSYAudioSequenceBank1;
    extern ALBank*    sSYAudioSequenceBank2;
    extern ALSeqFile* sSYAudioSeqFile;
    extern u8*        sSYAudioBGMSequenceDatas[];
    extern void*      sSYAudioAcmdListBuffers[];
    extern void*      sSYAudioSchedulerTasks[];
    extern s16*       sSYAudioDataBuffers[];
    extern ALHeap     sSYAudioHeap;
    extern u8         gSYAudioHeapBuffer[];

    void  alHeapInit(ALHeap* hp, u8* base, s32 len);
    void* alHeapDBAlloc(u8* file, s32 line, ALHeap* hp, s32 num, s32 size);
}

#define alHeapAlloc(hp, elem, size) alHeapDBAlloc(0, 0, (hp), (elem), (size))

/* ========================================================================= */
/*  Big-endian readers                                                       */
/* ========================================================================= */

static inline u16 readBE16(const u8* p) { return (u16)((p[0] << 8) | p[1]); }
static inline s16 readBE16s(const u8* p) { return (s16)readBE16(p); }
static inline u32 readBE32(const u8* p) { return (u32)((p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3]); }
static inline s32 readBE32s(const u8* p) { return (s32)readBE32(p); }

/* ========================================================================= */
/*  BLOB loader                                                              */
/* ========================================================================= */

struct AudioBlob {
    const uint8_t* data;
    size_t         size;
    std::shared_ptr<Ship::IResource> resource; // prevent release
};

// File-scope so portAudioShutdownAssets() can reach them.  These must be
// released *before* Ship::Context is torn down: if they outlive it, their
// shared_ptr<Ship::IResource> destructors run during __cxa_finalize_ranges
// and the SPDLOG_TRACE call in IResource::~IResource() lands on a
// shut-down spdlog and crashes.
static AudioBlob sounds1_ctl, sounds1_tbl, sounds2_ctl, sounds2_tbl;
static AudioBlob music_sbk;
static AudioBlob fgm_unk_blob, fgm_tbl_blob, fgm_ucd_blob;

static bool loadBlob(const char* name, AudioBlob& out) {
    auto ctx = Ship::Context::GetInstance();
    if (!ctx) { spdlog::error("audio_bridge: no Ship::Context"); return false; }

    std::string path = std::string("__OTR__") + name;
    auto res = ctx->GetResourceManager()->LoadResource(path);
    if (!res) {
        spdlog::error("audio_bridge: failed to load '{}'", path);
        return false;
    }

    auto blob = std::dynamic_pointer_cast<Ship::Blob>(res);
    if (!blob) {
        spdlog::error("audio_bridge: '{}' is not a Blob", path);
        return false;
    }

    out.data = blob->Data.data();
    out.size = blob->Data.size();
    out.resource = res;
    spdlog::info("audio_bridge: loaded '{}' ({} bytes)", name, out.size);
    return true;
}

/* ========================================================================= */
/*  ALBankFile parser — big-endian N64 binary → native structs               */
/* ========================================================================= */

// N64 ROM binary field sizes (all big-endian):
// ALBankFile:    2(rev) + 2(cnt) + 4*cnt(offsets)
// ALBank:        2(instCnt) + 1(flags) + 1(pad) + 4(rate) + 4(perc) + 4*instCnt(offsets) = 12+4*N
// ALInstrument:  1+1+1+1+1+1+1+1+1+1+1+1+2+2 + 4*soundCount = 16+4*N
// ALSound:       4(env) + 4(key) + 4(wav) + 1(pan) + 1(vol) + 1(flags) + 1(pad) = 16
// ALEnvelope:    4+4+4+1+1 = 14 (padded to 16 in ROM? Actually N64 struct is 14 bytes)
// ALKeyMap:      1+1+1+1+1+1 = 6
// ALWaveTable:   4(base) + 4(len) + 1(type) + 1(flags) + 2(pad) + 4(loop) + 4(book) = 20
// ALADPCMBook:   4(order) + 4(npred) + 2*order*npred*8(book)
// ALADPCMloop:   4+4+4+32(state) = 44
// ALRawLoop:     4+4+4 = 12

class BankParser {
    const u8*  ctl;
    size_t     ctlSize;
    u8*        tbl;
    ALHeap*    heap;

    // Cache: ROM offset in CTL → already-parsed PC pointer
    std::unordered_map<u32, void*> cache;

    template<typename T>
    T* cached(u32 off, T* ptr) { cache[off] = ptr; return ptr; }

    void* lookup(u32 off) {
        auto it = cache.find(off);
        return it != cache.end() ? it->second : nullptr;
    }

public:
    BankParser(const u8* c, size_t cs, u8* t, ALHeap* h)
        : ctl(c), ctlSize(cs), tbl(t), heap(h) {}

    ALEnvelope* parseEnvelope(u32 off) {
        if (auto* p = (ALEnvelope*)lookup(off)) return p;
        const u8* r = ctl + off;
        auto* e = (ALEnvelope*)alHeapAlloc(heap, 1, sizeof(ALEnvelope));
        e->attackTime   = readBE32s(r + 0);
        e->decayTime    = readBE32s(r + 4);
        e->releaseTime  = readBE32s(r + 8);
        e->attackVolume  = r[12];
        e->decayVolume   = r[13];
        return cached(off, e);
    }

    ALKeyMap* parseKeyMap(u32 off) {
        if (auto* p = (ALKeyMap*)lookup(off)) return p;
        const u8* r = ctl + off;
        auto* k = (ALKeyMap*)alHeapAlloc(heap, 1, sizeof(ALKeyMap));
        k->velocityMin = r[0];
        k->velocityMax = r[1];
        k->keyMin      = r[2];
        k->keyMax      = r[3];
        k->keyBase     = r[4];
        k->detune      = (s8)r[5];
        return cached(off, k);
    }

    ALADPCMBook* parseBook(u32 off) {
        if (auto* p = (ALADPCMBook*)lookup(off)) return p;
        const u8* r = ctl + off;
        s32 order = readBE32s(r + 0);
        s32 npred = readBE32s(r + 4);
        s32 bookLen = order * npred * 16; // 8 entries per predictor-order pair, each s16
        size_t totalSize = 8 + bookLen * (s32)sizeof(s16); // 8 for order+npred header
        auto* b = (ALADPCMBook*)alHeapAlloc(heap, 1, (s32)totalSize);
        b->order = order;
        b->npredictors = npred;
        for (s32 i = 0; i < order * npred * 8; i++) {
            b->book[i] = readBE16s(r + 8 + i * 2);
        }
        return cached(off, b);
    }

    ALADPCMloop* parseADPCMLoop(u32 off) {
        if (auto* p = (ALADPCMloop*)lookup(off)) return p;
        const u8* r = ctl + off;
        auto* l = (ALADPCMloop*)alHeapAlloc(heap, 1, sizeof(ALADPCMloop));
        l->start = readBE32(r + 0);
        l->end   = readBE32(r + 4);
        l->count = readBE32(r + 8);
        // ADPCM_STATE is 16 shorts at offset 12
        for (int i = 0; i < 16; i++) {
            l->state[i] = readBE16s(r + 12 + i * 2);
        }
        return cached(off, l);
    }

    ALRawLoop* parseRawLoop(u32 off) {
        if (auto* p = (ALRawLoop*)lookup(off)) return p;
        const u8* r = ctl + off;
        auto* l = (ALRawLoop*)alHeapAlloc(heap, 1, sizeof(ALRawLoop));
        l->start = readBE32(r + 0);
        l->end   = readBE32(r + 4);
        l->count = readBE32(r + 8);
        return cached(off, l);
    }

    ALWaveTable* parseWaveTable(u32 off) {
        if (auto* p = (ALWaveTable*)lookup(off)) return p;
        const u8* r = ctl + off;
        auto* w = (ALWaveTable*)alHeapAlloc(heap, 1, sizeof(ALWaveTable));
        u32 baseOff = readBE32(r + 0);
        w->base  = tbl + baseOff; // Point directly into TBL data
        w->len   = readBE32s(r + 4);
        w->type  = r[8];
        w->flags = 1; // Mark as "already patched"
        // Union starts at offset 12 in ROM (after 2 pad bytes at 10-11)
        if (w->type == AL_ADPCM_WAVE) {
            u32 loopOff = readBE32(r + 12);
            u32 bookOff = readBE32(r + 16);
            w->waveInfo.adpcmWave.loop = loopOff ? parseADPCMLoop(loopOff) : NULL;
            w->waveInfo.adpcmWave.book = bookOff ? parseBook(bookOff) : NULL;
        } else {
            u32 loopOff = readBE32(r + 12);
            w->waveInfo.rawWave.loop = loopOff ? parseRawLoop(loopOff) : NULL;
        }
        return cached(off, w);
    }

    ALSound* parseSound(u32 off) {
        if (auto* p = (ALSound*)lookup(off)) return p;
        const u8* r = ctl + off;
        auto* s = (ALSound*)alHeapAlloc(heap, 1, sizeof(ALSound));
        u32 envOff = readBE32(r + 0);
        u32 keyOff = readBE32(r + 4);
        u32 wavOff = readBE32(r + 8);
        s->envelope  = envOff ? parseEnvelope(envOff) : NULL;
        s->keyMap    = keyOff ? parseKeyMap(keyOff) : NULL;
        s->wavetable = wavOff ? parseWaveTable(wavOff) : NULL;
        s->samplePan    = r[12];
        s->sampleVolume = r[13];
        s->flags        = 1; // already patched
        return cached(off, s);
    }

    ALInstrument* parseInstrument(u32 off) {
        if (auto* p = (ALInstrument*)lookup(off)) return p;
        const u8* r = ctl + off;
        s16 soundCount = readBE16s(r + 14);
        // Variable-length: base struct + soundCount pointer slots
        size_t allocSize = sizeof(ALInstrument) + (soundCount > 1 ? (soundCount - 1) * sizeof(ALSound*) : 0);
        auto* inst = (ALInstrument*)alHeapAlloc(heap, 1, (s32)allocSize);
        inst->volume    = r[0];
        inst->pan       = r[1];
        inst->priority  = r[2];
        inst->flags     = 1; // already patched
        inst->tremType  = r[4];
        inst->tremRate  = r[5];
        inst->tremDepth = r[6];
        inst->tremDelay = r[7];
        inst->vibType   = r[8];
        inst->vibRate   = r[9];
        inst->vibDepth  = r[10];
        inst->vibDelay  = r[11];
        inst->bendRange = readBE16s(r + 12);
        inst->soundCount = soundCount;
        for (s16 i = 0; i < soundCount; i++) {
            u32 sndOff = readBE32(r + 16 + i * 4);
            inst->soundArray[i] = sndOff ? parseSound(sndOff) : NULL;
        }
        return cached(off, inst);
    }

    ALBank* parseBank(u32 off) {
        if (auto* p = (ALBank*)lookup(off)) return p;
        const u8* r = ctl + off;
        s16 instCount = readBE16s(r + 0);
        size_t allocSize = sizeof(ALBank) + (instCount > 1 ? (instCount - 1) * sizeof(ALInstrument*) : 0);
        auto* bank = (ALBank*)alHeapAlloc(heap, 1, (s32)allocSize);
        bank->instCount  = instCount;
        bank->flags      = 1; // already patched
        bank->pad        = r[3];
        bank->sampleRate = readBE32s(r + 4);
        u32 percOff = readBE32(r + 8);
        bank->percussion = percOff ? parseInstrument(percOff) : NULL;
        for (s16 i = 0; i < instCount; i++) {
            u32 instOff = readBE32(r + 12 + i * 4);
            bank->instArray[i] = instOff ? parseInstrument(instOff) : NULL;
        }
        return cached(off, bank);
    }

    ALBankFile* parseBankFile() {
        s16 bankCount = readBE16s(ctl + 2);
        size_t allocSize = sizeof(ALBankFile) + (bankCount > 1 ? (bankCount - 1) * sizeof(ALBank*) : 0);
        auto* bf = (ALBankFile*)alHeapAlloc(heap, 1, (s32)allocSize);
        bf->revision  = readBE16s(ctl + 0);
        bf->bankCount = bankCount;
        for (s16 i = 0; i < bankCount; i++) {
            u32 bankOff = readBE32(ctl + 4 + i * 4);
            bf->bankArray[i] = parseBank(bankOff);
        }
        return bf;
    }
};

/* ========================================================================= */
/*  ALSeqFile parser                                                         */
/* ========================================================================= */

// N64 binary: 2(rev) + 2(cnt) + cnt * (4(offset) + 4(len))
// offset is relative to start of SBK data
static ALSeqFile* parseSeqFile(const u8* sbk, size_t sbkSize, ALHeap* heap) {
    s16 seqCount = readBE16s(sbk + 2);
    size_t allocSize = sizeof(ALSeqFile) + (seqCount > 1 ? (seqCount - 1) * sizeof(ALSeqData) : 0);
    auto* sf = (ALSeqFile*)alHeapAlloc(heap, 1, (s32)allocSize);
    sf->revision = readBE16s(sbk + 0);
    sf->seqCount = seqCount;

    // Copy the raw SBK data into heap so sequence pointers remain valid
    u8* sbkCopy = (u8*)alHeapAlloc(heap, 1, (s32)sbkSize);
    memcpy(sbkCopy, sbk, sbkSize);

    for (s16 i = 0; i < seqCount; i++) {
        u32 off = readBE32(sbk + 4 + i * 8);
        s32 len = readBE32s(sbk + 4 + i * 8 + 4);
        sf->seqArray[i].offset = sbkCopy + off; // Point into heap copy
        sf->seqArray[i].len    = len;
    }
    return sf;
}

/* ========================================================================= */
/*  SYAudioPackage parser                                                    */
/* ========================================================================= */

// N64 binary: 4(count) + count * 4(offset)
// Offsets are relative to start of package data (after the count field)
static SYAudioPackage* parsePackage(const u8* data, size_t size, ALHeap* heap) {
    s32 count = readBE32s(data);
    // Allocate: s32 count + count * uintptr_t (8 bytes each on PC)
    size_t allocSize = sizeof(SYAudioPackage) + (count > 1 ? (count - 1) * sizeof(uintptr_t) : 0);
    auto* pkg = (SYAudioPackage*)alHeapAlloc(heap, 1, (s32)allocSize);
    pkg->count = count;

    // Copy raw package data into heap so offset pointers remain valid
    u8* pkgData = (u8*)alHeapAlloc(heap, 1, (s32)size);
    memcpy(pkgData, data, size);

    // Each entry is a 4-byte BE offset relative to pkgData
    for (s32 i = 0; i < count; i++) {
        u32 off = readBE32(data + 4 + i * 4);
        pkg->data[i] = (uintptr_t)(pkgData + off);
    }
    return pkg;
}

/* ========================================================================= */
/*  Main loader                                                              */
/* ========================================================================= */

extern "C" void portAudioLoadAssets(void)
{
    spdlog::info("audio_bridge: loading audio assets from .o2r");

    // Initialize heap
    memset(sSYAudioCurrentSettings.heap_base, 0, sSYAudioCurrentSettings.heap_size);
    alHeapInit(&sSYAudioHeap, sSYAudioCurrentSettings.heap_base,
               (s32)sSYAudioCurrentSettings.heap_size);

    // BLOBs live at file scope so portAudioShutdownAssets() can release
    // them before Ship::Context tears down spdlog.  They keep data alive
    // for the entire session (wavetable base pointers reference TBL data
    // directly, FGM packages reference their data, etc.)
    bool ok = true;
    ok = ok && loadBlob("audio/B1_sounds1_ctl", sounds1_ctl);
    ok = ok && loadBlob("audio/B1_sounds1_tbl", sounds1_tbl);
    ok = ok && loadBlob("audio/B1_sounds2_ctl", sounds2_ctl);
    ok = ok && loadBlob("audio/B1_sounds2_tbl", sounds2_tbl);
    ok = ok && loadBlob("audio/S1_music_sbk",   music_sbk);

    if (!ok) {
        spdlog::error("audio_bridge: failed to load core audio assets, audio disabled");
        return;
    }

    // TBL data is raw ADPCM samples — too large for the N64-sized audio heap
    // (~350KB).  Instead of copying, point wavetables directly into the Blob
    // data (kept alive by the static AudioBlob shared_ptrs above).
    u8* tbl1 = const_cast<u8*>(sounds1_tbl.data);
    u8* tbl2 = const_cast<u8*>(sounds2_tbl.data);

    // Bank → slot mapping must match the original game's SYAudioSettings:
    //   bank1_start = B1_sounds2_ctl → sSYAudioSequenceBank1 (SFX direct-index)
    //   bank2_start = B1_sounds1_ctl → sSYAudioSequenceBank2 (music CSP players)
    // sys/audio.c then does n_alCSPSetBank(player, sSYAudioSequenceBank2) and
    // reads SFX sounds from sSYAudioSequenceBank1->instArray[0]->soundArray.
    // Loading the swapped pair (which the bridge previously did) feeds the
    // music CSP a single-instrument 322-sound table with degenerate keymaps,
    // and every NoteOn misses in __n_lookupSoundQuick → silent BGM.
    {
        BankParser parser(sounds1_ctl.data, sounds1_ctl.size, tbl1, &sSYAudioHeap);
        ALBankFile* bf = parser.parseBankFile();
        sSYAudioSequenceBank2 = bf->bankArray[0];
        spdlog::info("audio_bridge: parsed sounds1_ctl → sSYAudioSequenceBank2 (music): {} instruments, rate={}",
                     sSYAudioSequenceBank2->instCount, sSYAudioSequenceBank2->sampleRate);
    }
    {
        BankParser parser(sounds2_ctl.data, sounds2_ctl.size, tbl2, &sSYAudioHeap);
        ALBankFile* bf = parser.parseBankFile();
        sSYAudioSequenceBank1 = bf->bankArray[0];
        spdlog::info("audio_bridge: parsed sounds2_ctl → sSYAudioSequenceBank1 (SFX): {} instruments, rate={}",
                     sSYAudioSequenceBank1->instCount, sSYAudioSequenceBank1->sampleRate);
    }

    // Parse sequence file
    sSYAudioSeqFile = parseSeqFile(music_sbk.data, music_sbk.size, &sSYAudioHeap);
    spdlog::info("audio_bridge: parsed seq file: {} sequences", sSYAudioSeqFile->seqCount);

    // Find max sequence length and allocate sequence data buffers
    s32 maxSeqLen = 0;
    for (s16 i = 0; i < sSYAudioSeqFile->seqCount; i++) {
        sSYAudioSeqFile->seqArray[i].len += (sSYAudioSeqFile->seqArray[i].len & 1);
        if (maxSeqLen < sSYAudioSeqFile->seqArray[i].len) {
            maxSeqLen = sSYAudioSeqFile->seqArray[i].len;
        }
    }
    // Allocate BGM sequence data buffers (1 per BGMPLAYER)
    sSYAudioBGMSequenceDatas[0] = (u8*)alHeapAlloc(&sSYAudioHeap, 1, maxSeqLen);

    // Allocate Acmd list double buffers
    sSYAudioAcmdListBuffers[0] = alHeapAlloc(&sSYAudioHeap, 1, 0x8000);
    sSYAudioAcmdListBuffers[1] = alHeapAlloc(&sSYAudioHeap, 1, 0x8000);

    // Allocate scheduler task buffers (sizeof SYTaskAudio ≈ 136 bytes, use 256 to be safe)
    sSYAudioSchedulerTasks[0] = alHeapAlloc(&sSYAudioHeap, 1, 256);
    sSYAudioSchedulerTasks[1] = alHeapAlloc(&sSYAudioHeap, 1, 256);

    // Allocate audio output data buffers (triple buffered, 0xE60 bytes each)
    sSYAudioDataBuffers[0] = (s16*)alHeapAlloc(&sSYAudioHeap, 1, 0xE60);
    sSYAudioDataBuffers[1] = (s16*)alHeapAlloc(&sSYAudioHeap, 1, 0xE60);
    sSYAudioDataBuffers[2] = (s16*)alHeapAlloc(&sSYAudioHeap, 1, 0xE60);

    // Load FGM packages (optional — game still works without SFX)
    if (loadBlob("audio/fgm_unk", fgm_unk_blob)) {
        /* fgm_unk holds a flat array of 16-byte envelope-modulator structs
         * (ALWhatever8009EE0C_3): u8 unk0..unk3 then f32 unk4/unk8/unkC.
         * The consumer (`unk_alsound_0x24[i]`) indexes with 16-byte stride,
         * so `unk44` must point at the FIRST struct (not at parsePackage's
         * pointer table). The f32 fields are stored BE in the ROM data and
         * need per-word bswap32 on LE; the leading 4 u8s stay as-is.
         *
         * The pre-fix path (parsePackage + unk44 = pkg->data) treated this
         * blob as a pointer table, so flat indexing landed inside the
         * pointer table for the first ~50 entries and then drifted into
         * adjacent heap memory. The float fields were also unswapped, so
         * every envelope read produced f4=f8=fC≈0 → no envelope shaping →
         * SFX voices played at the constant amplitude they were started at
         * for the full duration of the bytecode. The Star-KO chime's
         * 150-frame sustain made the bug audible as a high-pitched ring.
         *
         * Actual file layout (verified by diagnostic dump 2026-04-30):
         *   [BE u32 count][16-byte struct[count]]
         * No offset table — structs start at file offset 4 with 16-byte
         * stride. (An earlier draft of this fix assumed an interleaved
         * offset-table layout, which read struct[0]'s u8 fields as BE u32
         * offsets and faulted with SIGBUS on the first iteration.) */
        const u8 *raw = fgm_unk_blob.data;
        size_t blob_size = fgm_unk_blob.size;
        s32 count = readBE32s(raw);
        s32 totalStructBytes = count * 16;
        size_t neededBytes = 4 + (size_t)totalStructBytes;
        if (count <= 0 || neededBytes > blob_size) {
            spdlog::error("audio_bridge: fgm_unk parse rejected — count={} blob_size={} needed={}",
                          (int)count, (size_t)blob_size, (size_t)neededBytes);
        } else {
            u8 *flat = (u8*)alHeapAlloc(&sSYAudioHeap, 1, totalStructBytes);
            for (s32 i = 0; i < count; i++) {
                const u8 *src = raw + 4 + i * 16;
                u8 *dst = flat + i * 16;
                /* u8 fields: copy as-is */
                dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3];
                /* f32 fields: bswap32 each word from BE → LE */
                for (s32 w = 1; w < 4; w++) {
                    dst[w*4 + 0] = src[w*4 + 3];
                    dst[w*4 + 1] = src[w*4 + 2];
                    dst[w*4 + 2] = src[w*4 + 1];
                    dst[w*4 + 3] = src[w*4 + 0];
                }
            }
            dSYAudioPublicSettings.unk4C = sSYAudioCurrentSettings.unk4C = (u16)count;
            dSYAudioPublicSettings.unk44 = sSYAudioCurrentSettings.unk44 = (uintptr_t*)flat;
            spdlog::info("audio_bridge: fgm_unk parsed as flat envelope array ({} entries, {} bytes)",
                         (int)count, (int)totalStructBytes);
        }
    }

    if (loadBlob("audio/fgm_tbl", fgm_tbl_blob)) {
        SYAudioPackage* pkg = parsePackage(fgm_tbl_blob.data, fgm_tbl_blob.size, &sSYAudioHeap);
        dSYAudioPublicSettings2.fgm_table_count = sSYAudioCurrentSettings.fgm_table_count = (u16)pkg->count;
        dSYAudioPublicSettings.fgm_table_data = sSYAudioCurrentSettings.fgm_table_data = pkg->data;
    }

    if (loadBlob("audio/fgm_ucd", fgm_ucd_blob)) {
        SYAudioPackage* pkg = parsePackage(fgm_ucd_blob.data, fgm_ucd_blob.size, &sSYAudioHeap);
        dSYAudioPublicSettings3.fgm_ucode_count = sSYAudioCurrentSettings.fgm_ucode_count = (u16)pkg->count;
        dSYAudioPublicSettings3.fgm_ucode_data = sSYAudioCurrentSettings.fgm_ucode_data = pkg->data;
    }

    spdlog::info("audio_bridge: all audio assets loaded successfully");
}

extern "C" void portAudioShutdownAssets(void)
{
    sounds1_ctl = AudioBlob{};
    sounds1_tbl = AudioBlob{};
    sounds2_ctl = AudioBlob{};
    sounds2_tbl = AudioBlob{};
    music_sbk   = AudioBlob{};
    fgm_unk_blob = AudioBlob{};
    fgm_tbl_blob = AudioBlob{};
    fgm_ucd_blob = AudioBlob{};
}
