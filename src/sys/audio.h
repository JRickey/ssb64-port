#ifndef _SYAUDIO_H_
#define _SYAUDIO_H_

#include <ssb_types.h>
#include <n_audio/n_libaudio.h>
#include <n_audio/synthInternals.h>

#define SYAUDIO_BGMPLAYERS_NUM 1
#define NUM_DMA_BUFFERS 4
#define MAX_BUFFERS 50
#define MAX_BUFFER_LENGTH 1024
#define AUDIO_EXTRA_SAMPLES 80

#define TREMELO_SIN 1
#define TREMELO_SQR 2
#define TREMELO_DSC_SAW 3
#define TREMELO_ASC_SAW 4
#define VIBRATO_SIN 128
#define VIBRATO_SQR 129
#define VIBRATO_DSC_SAW 130
#define VIBRATO_ASC_SAW 131
#define OSC_HIGH 0
#define OSC_LOW 1
#define SYAUDIO_OSC_STATES_NUM 32

typedef struct {
    u32 addr;
    u32 len;
    u32 size;
    char* ptr;
} AMDMABuffer;

typedef struct {
    u32 nBuffers;
    u32 currentBuffer;
    AMDMABuffer buffers[4];
} AMDMAState;

typedef struct SYAudioPackage
{
    s32 count;
    uintptr_t data[1];

} SYAudioPackage;

typedef struct SYAudioSettings
{
    u8 *heap_base;
    size_t heap_size;
    u16 output_rate;
    u8 pvoices_num_max;
    u8 vvoices_num_max;
    u8 updates_num_max;
    u8 events_num_max;
    u8 sounds_num_max;
    u8 voices_num_max[2];
    u8 unk11;
    u8 unk12;
    u8 priority;
    uintptr_t bank1_start;
    uintptr_t bank1_end;
    void *table1_start;
    uintptr_t bank2_start;
    uintptr_t bank2_end;
    void *table2_start;
    uintptr_t sbk_start;
    u8 fx_type;
    u8 unk31;
    u8 unk32;
    u8 sndplayers_num;
    u16 unk34;
    u16 unk36;
    s32 unk38;
    uintptr_t *fgm_ucode_data;
    uintptr_t *fgm_table_data;
    uintptr_t *unk44;
    u16 fgm_ucode_count;
    u16 fgm_table_count;
    u16 unk4C;
    uintptr_t unk50;
    uintptr_t unk54;
    uintptr_t fgm_table_start;
    uintptr_t fgm_table_end;
    uintptr_t fgm_ucode_start;
    uintptr_t fgm_ucode_end;

} SYAudioSettings;

typedef struct SYAudioOsc_s
{
    struct SYAudioOsc_s *next;
    u8 type;
    u8 stateFlags;
    u16 maxCount;
    u16 curCount;

    union SYOscDataUnion
    {
        struct SYOscDef
        {
            u8 rate;
            u8 depth;
            u8 oscCount;
        } def;

        struct SYOscTremSin
        {
            u8 halfdepth;
            u8 baseVol;
        } tsin;

        struct SYOscTremSqr
        {
            u8 curVal;
            u8 hiVal;
            u8 loVal;
        } tsqr;

        struct SYOscTremSaw
        {
            u8 baseVol;
            u8 depth;
        } tsaw;

        struct SYOscVibSin
        {  
            f32 depthcents;
        } vsin;

        struct SYOscVibSqr
        {
            f32 loRatio;
            f32 hiRatio;
        } vsqr;

        struct SYOscVibDSaw
        {
            s32 hicents;
            s32 centsrange;
        } vdsaw;

        struct SYOscVibASaw
        {
            s32 locents;
            s32 centsrange;
        } vasaw;

    } data;

} SYAudioOsc;

typedef struct SYAudioConfig
{
    u16 unk_80026204_0x0;
    u16 unk_80026204_0x2;
    u16 unk_80026204_0x4;
    u16 inst_sound_count;
    u16 fgm_ucode_count;
    u16 fgm_table_count;
    u16 unk_80026204_0xC;
    void *inst_sound_array;
    void *fgm_ucode_data;
    void *fgm_table_data;
    s32 unk_80026204_0x1C;
    ALHeap *heap;
    u8 unk_80026204_0x24;
    u16 unk_80026204_0x26;
    u16 unk_80026204_0x28;
    u16 unk_80026204_0x2A;
    u16 unk_80026204_0x2C;
    u16 unk_80026204_0x2E;
    u16 unk_80026204_0x30;

} SYAudioConfig;

extern SYAudioSettings dSYAudioPublicSettings;
#if defined(REGION_US)
extern u8 gSYAudioHeapBuffer[0x56000];
#else
extern u8 gSYAudioHeapBuffer[0x53000];
#endif
extern u32 gSYAudioThreadTimeDelta;
extern ALCSPlayer *gSYAudioCSPlayers[/* */];

extern void alHeapInit(ALHeap *hp, u8 *base, s32 len);
extern void* alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
extern void alSeqFileNew(ALSeqFile *file, u8 *base);
extern void alBnkfNew(ALBankFile *file, u8 *table);
extern void syAudioBnkfPatchBank(ALBank *bank, uintptr_t offset, uintptr_t table);
extern void syAudioBnkfPatchInst(ALInstrument *inst, uintptr_t offset, uintptr_t table);
extern void syAudioBnkfPatchSound(ALSound *sound, uintptr_t offset, uintptr_t table);
extern void syAudioBnkfPatchWaveTable(ALWaveTable *wav, uintptr_t offset, uintptr_t table);
extern void syAudioReadRom(uintptr_t rom, void *vram, size_t size);
extern s32 syAudioDma(s32 addr, s32 len, void *state);
extern ALDMAproc syAudioDmaNew(AMDMAState **state);
extern f32 syAudioDepth2Cents(u8 depth);
extern ALMicroTime syAudioInitOsc(void **oscState, f32 *initVal, u8 oscType, u8 oscRate, u8 oscDepth, u8 oscDelay);
extern ALMicroTime syAudioUpdateOsc(void *oscState, f32 *updateVal);
extern void syAudioStopOsc(void *oscState);
extern void syAudioInit(void);
extern void syAudioLoadAssets(void);
extern void syAudioMakeBGMPlayers(void);
extern void syAudioThreadMain(void *arg);
extern void syAudioSetStereo(void);
extern void syAudioSetMono(void);
extern void syAudioSetQuality(s32 quality);
extern void syAudioSetFXType(s32 fx_type);
extern void syAudioStopBGMAll(void);
extern s32 syAudioPlayBGM(s32 sngplayer, u32 bgm);
extern void syAudioStopBGM(s32 sngplayer);
extern void syAudioSetBGMVolume(s32 sngplayer, u32 vol);
extern void syAudioSetBGMVolumeFade(s32 sngplayer, u32 vol, u32 time);
extern void syAudioSetBGMReverb(s32 sngplayer, u32 reverb);
extern void syAudioSetBGMPriority(s32 sngplayer, u8 priority);
extern s32 syAudioCheckBGMPlaying(s32 sngplayer);
extern s32 syAudioPlayFGM(u32 fgm);
extern s32 func_80020E10(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_80020E28(void);
extern void func_80020E64(u32 volume);
extern void func_80020EA0(s32 sndplayer, u32 arg1);
extern void func_80020EF8(s32 sndplayer, s32 arg1);
extern void func_80020F4C(s32 sndplayer, s32 arg1);
extern void syAudioStopFGM(s32 sndplayer);
extern void func_80020FFC(s32 sndplayer, u8 arg1);
extern void syAudioSetSettingsUpdated(void);
extern s32 syAudioGetSettingsUpdated(void);
extern s32 syAudioGetRestarting(void);
extern sb32 syAudioGetStatus(void);
extern void syAudioUpdateMesgQueue(void);

#endif
