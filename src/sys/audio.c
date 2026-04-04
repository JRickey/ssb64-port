#include <sys/audio.h>
#include <sys/scheduler.h>
#include <sys/dma.h>
#include <sys/main.h>
#include <PR/rcp.h>
#include <PR/gu.h>

extern SYAudioSettings dSYAudioPublicSettings2, dSYAudioPublicSettings3;

// only temporarily here...
typedef struct alSoundEffect
{
	void* unk_0x0;
	void* unk_0x4;
	void* unk_0x8;
	void* unk_0xC;
	u16 unk_0x10;
	u16 unk_0x12;
	u16 unk_0x14;
	u16 unk_0x16;
	u16 unk_0x18;
	u16 unk_0x1A;
	u16 unk_0x1C;
	u8 unk_0x1E;
    u8 unk_0x1F;
	u16 unk_0x20;
	u16 unk_0x22;
	u16 unk_0x24;
	u16 sfx_id;
	u16 sfx_max; // End of SFX index list
	u8 filler_0x2A[0x2F - 0x2A];
	u8 balance;

} alSoundEffect;

extern uintptr_t S1_music_sbk_ROM_START;
extern uintptr_t S1_music_sbk_ROM_END;
extern uintptr_t B1_sounds1_ctl_ROM_START;
extern uintptr_t B1_sounds1_ctl_ROM_END;
extern uintptr_t B1_sounds2_ctl_ROM_START;
extern uintptr_t B1_sounds2_ctl_ROM_END;
extern uintptr_t fgm_unk_ROM_START;
extern uintptr_t fgm_unk_ROM_END;
extern uintptr_t fgm_tbl_ROM_START;
extern uintptr_t fgm_tbl_ROM_END;
extern uintptr_t fgm_ucd_ROM_START;
extern uintptr_t fgm_ucd_ROM_END;

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8003C950
s32 dSYAudioCustomFXParams[/* */] =
{ 
      14, 19200,     0,   640,  9830, -9830,     0,     0,    0,     0,   960, 1280,  3276, -3276,  8191,     0,    0,     0,  1920, 
    2560,  3276, -3276,  5631,     0,     0,     0,  2560, 3520,  3276, -3276, 4351,     0,     0,     0,  3200, 4800,  3276, -3276,
    4351,     0,     0,     0,  3840,  6400,  3276, -3276, 4095,     0,     0,    0,  5760,  7680,  3276, -3276, 4095,     0,     0,
       0,  8000,  9600,  3276, -3276,  2559,     0,     0,    0,  9600, 11200, 3276, -3276,  2559,     0,     0,    0, 11200, 12800,
    3276, -3276,  1535,     0,     0,     0, 12800, 14400, 3276, -3276,  1535,    0,     0,     0, 14400, 16000, 3276, -3276,   255,
       0,     0,     0, 16000, 17600,  3276, -3276,   255,    0,     0,     0,    0,  9600,  5000,     0,     0,    0,     0, 20480
};

// 0x8003CB18
s32 *dSYAudioCustomFXTables[/* */] = { dSYAudioCustomFXParams };

// 0x8003CB1C
sb32 dSYAudioIsSettingsUpdated = FALSE;

// 0x8003CB20
s32 dSYAudioNextDma = 0;

// 0x8003CB24
sb32 dSYAudioSoundQuality = 1;

// 0x8003CB28
s32 dSYAudioCurrentFxType = 0;

// 0x8003CB2C
s32 dSYAudioIsRestarting = FALSE;

// 0x8003CB30
s32 dSYAudioCurrentTic = 1;

// 0x8003CB34
s16 dSYAudioSampleCounts[/* */] = { 0, 0, 0, 0 };

// 0x8003CB3C - HAL did some dumb $h17 and now we have 3 of these just to make one function match, hypers
SYAudioSettings dSYAudioPublicSettings =
{
    gSYAudioHeapBuffer,             // Heap base
    ARRAY_COUNT(gSYAudioHeapBuffer),// Heap size
    32000,                          // Output rate
    16,                             // Max pvoices
    24,                             // Max vvoices
    128,                            // Max updates
    64,                             // Max events
    24,                             // Max sounds
    { 24, 0 },                      // Max voices
    0,                              // ???
    50,                             // ???
    20,                             // Priority
    &B1_sounds2_ctl_ROM_START,      // ROM address
    &B1_sounds2_ctl_ROM_END,        // ROM address
    &B1_sounds2_ctl_ROM_END,        // ROM address
    &B1_sounds1_ctl_ROM_START,      // ROM address
    &B1_sounds1_ctl_ROM_END,        // ROM address
    &B1_sounds1_ctl_ROM_END,        // ROM address
    &S1_music_sbk_ROM_START,        // ROM address
    AL_FX_NONE,                     // FX type
    48,                             // ???
    24,                             // ???
    24,                             // Max concurrent SFX?
    0,                              // ???
    0,                              // ???
    0,                              // ???
    NULL,                           // ucode data
    NULL,                           // table data
    NULL,                           // 0x44
    0,                              // ucode count
    0,                              // table count
    0,                              // 0x4C
    &fgm_unk_ROM_START,             // ROM address
    &fgm_unk_ROM_END,               // ROM address
    &fgm_tbl_ROM_START,             // ROM address
    &fgm_tbl_ROM_END,               // ROM address
    &fgm_ucd_ROM_START,             // ROM address
    &fgm_ucd_ROM_END                // ROM address
};

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               // 
// // // // // // // // // // // //

// 0x800472D0
#if defined(REGION_US)
u8 gSYAudioHeapBuffer[0x56000];
#else
u8 gSYAudioHeapBuffer[0x53000];
#endif

// 0x8009D2D0
u32 gSYAudioThreadTimeDelta;

// 0x8009D2D8
ALHeap sSYAudioHeap;

// 0x8009D2E8
void *sSYAudioHeapBase;

// 0x8009D2EC
s32 sSYAudioHeapSize;

// 0x8009D2F0
SYClient sSYAudioClient;

// 0x8009D2F8
OSMesgQueue sSYAudioTicMesgQueue;

// 0x8009D310
OSMesgQueue sSYAudioSPTaskMesgQueue;

// 0x8009D328
OSMesgQueue sSYAudioDmaMesgQueue;

// 0x8009D340
OSMesg sSYAudioTicMesgs[1];

// 0x8009D344
OSMesg sSYAudioSPTaskMesgs[1];

// 0x8009D348
OSMesg sSYAudioDmaMesgs[50];

// 0x8009D410
OSIoMesg audDMAIOMesgBuf[MAX_BUFFERS];

// 0x8009D8C0
N_ALGlobals sSYAudioGlobals;

// 0x8009D91C
s32 sSYAudioFrequency;

// 0x8009D920
s32 D_8009D920_96D20;

// 0x8009D928
s16 *sSYAudioDataBuffers[3];

// 0x8009D934
Acmd *sSYAudioCurrentAcmdListBuffer;

// 0x8009D938
Acmd *sSYAudioAcmdListBuffers[2];

// 0x8009D940 - sSYAudioCurrentTask
SYTaskAudio *sSYAudioCurrentTask;

// 0x8009D948
SYTaskAudio *sSYAudioSchedulerTasks[2];

// 0x8009D950
ALBank *sSYAudioSequenceBank1;

// 0x8009D954
alSoundEffect **sSYAudioSoundPlayers;

// 0x8009D958
ALBank *sSYAudioSequenceBank2;

// 0x8009D95C
ALSeqFile *sSYAudioSeqFile;

// 0x8009D960
ALCSPlayer *gSYAudioCSPlayers[SYAUDIO_BGMPLAYERS_NUM];

// 0x8009D964
ALCSeq *sSYAudioCSeqs[SYAUDIO_BGMPLAYERS_NUM];

// 0x8009D968
u8 gSYAudioGlobalBGMPriority;

// 0x8009D96C
u8 *sSYAudioBGMSequenceDatas[SYAUDIO_BGMPLAYERS_NUM];

// 0x8009D970
u8 *sSYAudioCSPlayerStatuses;

// 0x8009D974
s32 *sSYAudioBGMPlayingIDs;

// 0x8009D978
s32 sSYAudioBGMVolumeTimers[SYAUDIO_BGMPLAYERS_NUM];

// 0x8009D97C
f32 sSYAudioBGMVolumes[SYAUDIO_BGMPLAYERS_NUM];

// 0x8009D980
f32 sSYAudioBGMVolumeRates[SYAUDIO_BGMPLAYERS_NUM];

// 0x8009D988
SYAudioSettings sSYAudioCurrentSettings;

// 0x8009D9F0
OSTime sSYAudioTimeStamp;

// 0x8009D9F8
SYAudioOsc *sSYAudioOscStatesAllocFree;

extern void func_80026104_26D04(void*, u8);
extern void func_80026094_26C94(void*, u8);
extern void* func_800269C0_275C0(u16);
extern void func_80026070_26C70(u8);
extern void func_80026174_26D74(void*, u8);

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x8001E5C0
void alHeapInit(ALHeap *hp, u8 *base, s32 len)
{
    s32 align = (AL_CACHE_ALIGN + 1) - ((intptr_t) base & AL_CACHE_ALIGN);

    if (align != AL_CACHE_ALIGN + 1)
    {
        hp->base = base + align;
    }
    else hp->base = base;

    hp->len = len;
    hp->cur = hp->base;
    hp->count = 0;
}

// 0x8001E5F4
void* alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size)
{
    size_t bytes = (num * size + AL_CACHE_ALIGN) & ~AL_CACHE_ALIGN;
    u8 *ptr = NULL;

    if ((hp->cur + bytes) <= (hp->base + hp->len))
    {
        ptr = hp->cur;
        hp->cur += bytes;
    }
    return ptr;
}

// 0x8001E648
void alSeqFileNew(ALSeqFile *file, u8 *base)
{
    uintptr_t base_ptr = (uintptr_t) base;
    s32 i;

    /*
     * patch the file so that offsets are pointers
     */
    for (i = 0; i < file->seqCount; i++)
    {
        file->seqArray[i].offset = (u8*) ((uintptr_t)file->seqArray[i].offset + base_ptr);
    }
}

// 0x8001E688
void alBnkfNew(ALBankFile *file, u8 *table)
{
    uintptr_t file_base = (uintptr_t) file;
    uintptr_t table_base = (uintptr_t) table;
    s32 i;

    /*
     * patch the file so that offsets are pointers
     */
    for (i = 0; i < file->bankCount; i++)
    {
        file->bankArray[i] = (ALBank*) ((uintptr_t)file->bankArray[i] + file_base);

        if (file->bankArray[i] != NULL)
        {
            syAudioBnkfPatchBank(file->bankArray[i], file_base, table_base);
        }
    }
}

// 0x8001E708
void syAudioBnkfPatchBank(ALBank *bank, uintptr_t offset, uintptr_t table)
{
    s32 i;

    if (bank->flags)
    {
        return;
    }
    bank->flags = 1;

    if (bank->percussion != NULL)
    {
        bank->percussion = (ALInstrument*) ((uintptr_t)bank->percussion + offset);
        syAudioBnkfPatchInst(bank->percussion, offset, table);
    }
    for (i = 0; i < bank->instCount; i++)
    {
        bank->instArray[i] = (ALInstrument*) ((uintptr_t)bank->instArray[i] + offset);

        if (bank->instArray[i] != NULL)
        {
            syAudioBnkfPatchInst(bank->instArray[i], offset, table);
        }
    }
}

// 0x8001E7C0
void syAudioBnkfPatchInst(ALInstrument *inst, uintptr_t offset, uintptr_t table)
{
    s32 i;

    if (inst->flags)
    {
        return;
    }
    inst->flags = 1;

    for (i = 0; i < inst->soundCount; i++)
    {
        inst->soundArray[i] = (ALSound*) ((uintptr_t)inst->soundArray[i] + offset);
        syAudioBnkfPatchSound(inst->soundArray[i], offset, table);
    }
}

// 0x8001E858
void syAudioBnkfPatchSound(ALSound *sound, uintptr_t offset, uintptr_t table)
{
    if (sound->flags)
    {
        return;
    }
    sound->flags = 1;

    sound->envelope = (ALEnvelope*) ((uintptr_t)sound->envelope + offset);
    sound->keyMap = (ALKeyMap*) ((uintptr_t)sound->keyMap + offset);

    sound->wavetable = (ALWaveTable*) ((uintptr_t)sound->wavetable + offset);
    syAudioBnkfPatchWaveTable(sound->wavetable, offset, table);
}

// 0x8001E8B0
void syAudioBnkfPatchWaveTable(ALWaveTable *wav, uintptr_t offset, uintptr_t table)
{
    if (wav->flags)
    {
        return;
    }
    wav->flags = 1;

    wav->base += table;

    if (wav->type == AL_ADPCM_WAVE)
    {
        wav->waveInfo.adpcmWave.book = (ALADPCMBook*) ((uintptr_t)wav->waveInfo.adpcmWave.book + offset);

        if (wav->waveInfo.adpcmWave.loop != NULL)
        {
            wav->waveInfo.adpcmWave.loop = (ALADPCMloop*) ((uintptr_t)wav->waveInfo.adpcmWave.loop + offset);
        }

    }
    else if (wav->type == AL_RAW16_WAVE)
    {
        if (wav->waveInfo.rawWave.loop != NULL)
        {
            wav->waveInfo.rawWave.loop = (ALRawLoop*) ((uintptr_t)wav->waveInfo.rawWave.loop + offset);
        }
    }
}

// 0x8001E91C
void syAudioReadRom(uintptr_t rom, void *vram, size_t size)
{
    OSIoMesg dmaIoMesg;

    osInvalDCache(vram, (s32) size);

    dmaIoMesg.hdr.pri = OS_MESG_PRI_NORMAL;
    dmaIoMesg.hdr.retQueue = &sSYAudioDmaMesgQueue;
    dmaIoMesg.dramAddr = vram;
    dmaIoMesg.devAddr = rom;
    dmaIoMesg.size = size;

    osEPiStartDma(gSYDmaRomPiHandle, &dmaIoMesg, OS_READ);
    osRecvMesg(&sSYAudioDmaMesgQueue, NULL, OS_MESG_BLOCK);
}

// 0x8001E99C
s32 syAudioDma(s32 addr, s32 len, void *state)
{
    void *freeBuffer;
    AMDMAState *dState = state;
    s32 delta = 0;
    u32 bStartAddr;
    u32 bEndAddr;
    AMDMABuffer *dBuff = &dState->buffers[dState->currentBuffer];
    OSMesg dummyMesg;

    /*
     * Is it in the last buffer
     */
    bStartAddr = (u32) dBuff->addr;
    bEndAddr = (u32) bStartAddr + dBuff->len;

    if ((addr >= bStartAddr) && (addr + len <= bEndAddr))
    {
        freeBuffer = dBuff->ptr + addr - dBuff->addr;
    }
    else
    {
        if (++dState->currentBuffer >= dState->nBuffers)
        {
            dState->currentBuffer = 0;
        }
        dBuff = &dState->buffers[dState->currentBuffer];
        freeBuffer = dBuff->ptr;
        delta = addr & 0x1;
        addr -= delta;

        dBuff->addr = addr;
        dBuff->len = dBuff->size;

        audDMAIOMesgBuf[dSYAudioNextDma].hdr.pri = OS_MESG_PRI_HIGH;
        audDMAIOMesgBuf[dSYAudioNextDma].hdr.retQueue = &sSYAudioDmaMesgQueue;
        audDMAIOMesgBuf[dSYAudioNextDma].dramAddr = freeBuffer;
        audDMAIOMesgBuf[dSYAudioNextDma].devAddr = (u32) addr;
        audDMAIOMesgBuf[dSYAudioNextDma].size = dBuff->size;

        osEPiStartDma(gSYDmaRomPiHandle, &audDMAIOMesgBuf[dSYAudioNextDma++], OS_READ);
    }
    return (s32) osVirtualToPhysical(freeBuffer) + delta;
}

// 0x8001EAC8
ALDMAproc syAudioDmaNew(AMDMAState **state)
{
    s32 i;
    AMDMAState* dState;

    dState = (AMDMAState*) alHeapAlloc(&sSYAudioHeap, 1, sizeof(AMDMAState));
    dState->currentBuffer = 0;
    dState->nBuffers = NUM_DMA_BUFFERS;

    for (i = 0; i < NUM_DMA_BUFFERS; i++)
    {
        dState->buffers[i].ptr = alHeapAlloc(&sSYAudioHeap, 1, MAX_BUFFER_LENGTH);
        dState->buffers[i].addr = 0;
        dState->buffers[i].len = 0;
        dState->buffers[i].size = MAX_BUFFER_LENGTH;
    }
    *state = (AMDMAState*) dState;

    return syAudioDma;
}

// 0x8001EB98
f32 syAudioDepth2Cents(u8 depth)
{
    f32 x = 1.03099303F;
    f32 cents = 1.0F;

    while (depth)
    {
        if (depth & 1)
        {
            cents *= x;
        }
        x *= x;
        depth >>= 1;
    }
    return cents;
}

// 0x8001EBE4
ALMicroTime syAudioInitOsc(void **oscState, f32 *initVal, u8 oscType, u8 oscRate, u8 oscDepth, u8 oscDelay)
{
    SYAudioOsc *statePtr;
    ALMicroTime deltaTime = 0;

    if (sSYAudioOscStatesAllocFree != NULL) /* yes there are oscStates available */
    {
        statePtr = sSYAudioOscStatesAllocFree;
        sSYAudioOscStatesAllocFree = sSYAudioOscStatesAllocFree->next;
        statePtr->type = oscType;
        *oscState = statePtr;

        /*
         * Convert delay into usec's, In this example, multiply by
         * 0x4000, but could easily use another conversion method.
         */
        deltaTime = oscDelay * 0x4000;

        switch (oscType) /* set the initVal */
        {
            case TREMELO_SIN:
                statePtr->curCount = 0;
                statePtr->maxCount = 259 - oscRate; /* gives values 4-259 */
                statePtr->data.tsin.halfdepth = oscDepth >> 1;
                statePtr->data.tsin.baseVol = AL_VOL_FULL - statePtr->data.tsin.halfdepth;
                *initVal = (f32) statePtr->data.tsin.baseVol;
                break;

            case TREMELO_SQR:
                statePtr->maxCount = 256 - oscRate; /* values from 1-256 */
                statePtr->curCount = statePtr->maxCount;
                statePtr->stateFlags = OSC_HIGH;
                statePtr->data.tsqr.loVal = AL_VOL_FULL - oscDepth;
                statePtr->data.tsqr.hiVal = AL_VOL_FULL;
                statePtr->data.tsqr.curVal = AL_VOL_FULL;
                *initVal = (f32) AL_VOL_FULL;
                break;

            case TREMELO_DSC_SAW:
                statePtr->maxCount = 256 - oscRate;
                statePtr->curCount = 0;
                statePtr->data.tsaw.depth = oscDepth;
                statePtr->data.tsaw.baseVol = AL_VOL_FULL;
                *initVal = (f32) statePtr->data.tsaw.baseVol;
                break;

            case TREMELO_ASC_SAW:
                statePtr->maxCount = 256 - oscRate;
                statePtr->curCount = 0;
                statePtr->data.tsaw.depth = oscDepth;
                statePtr->data.tsaw.baseVol = AL_VOL_FULL - oscDepth;
                *initVal = (f32) statePtr->data.tsaw.baseVol;
                break;

            case VIBRATO_SIN:
                statePtr->data.vsin.depthcents = syAudioDepth2Cents(oscDepth);
                statePtr->curCount = 0;
                statePtr->maxCount = 259 - oscRate; /* gives values 4-259 */
                *initVal = 1.0f;                    /* start at unity pitch */
                break;

            case VIBRATO_SQR:
            {
                s32 cents;
                statePtr->maxCount = 256 - oscRate; /* values from 1-256 */
                statePtr->curCount = statePtr->maxCount;
                statePtr->stateFlags = OSC_HIGH;
                cents = syAudioDepth2Cents(oscDepth);
                statePtr->data.vsqr.loRatio = alCents2Ratio(-cents);
                statePtr->data.vsqr.hiRatio = alCents2Ratio(cents);
                *initVal = statePtr->data.vsqr.hiRatio;
            }
            break;

            case VIBRATO_DSC_SAW:
            {
                s32 cents;
                statePtr->maxCount = 256 - oscRate; /* values from 1-256 */
                statePtr->curCount = statePtr->maxCount;
                cents = syAudioDepth2Cents(oscDepth);
                statePtr->data.vdsaw.hicents = cents;
                statePtr->data.vdsaw.centsrange = 2 * cents;
                *initVal = alCents2Ratio(statePtr->data.vdsaw.hicents);
            }
            break;

            case VIBRATO_ASC_SAW:
            {
                s32 cents;
                statePtr->maxCount = 256 - oscRate; /* values from 1-256 */
                statePtr->curCount = statePtr->maxCount;
                cents = syAudioDepth2Cents(oscDepth);
                statePtr->data.vasaw.locents = -cents;
                statePtr->data.vasaw.centsrange = 2 * cents;
                *initVal = alCents2Ratio(statePtr->data.vasaw.locents);
            }
            break;
        }
    }
    return deltaTime; /* if there are no oscStates, return zero, but if oscState was available, return delay in usecs */
}

// 0x8001EEB8
ALMicroTime syAudioUpdateOsc(void *oscState, f32 *updateVal)
{
    f32 tmpFlt;
    SYAudioOsc *statePtr = (SYAudioOsc*) oscState;
    ALMicroTime deltaTime = AL_USEC_PER_FRAME; /* in this example callback every */
                                               /* frame, but could be at any interval */

    switch (statePtr->type) /* perform update calculations */
    {
        case TREMELO_SIN:
            statePtr->curCount++;

            if (statePtr->curCount >= statePtr->maxCount)
            {
                statePtr->curCount = 0;
            }
            tmpFlt = (f32) statePtr->curCount / (f32) statePtr->maxCount;
            tmpFlt = sinf(tmpFlt * F_CST_DTOR32(360.0F));
            tmpFlt = tmpFlt * (f32) statePtr->data.tsin.halfdepth;
            *updateVal = (f32) statePtr->data.tsin.baseVol + tmpFlt;
            break;

        case TREMELO_SQR:
            if (statePtr->stateFlags == OSC_HIGH)
            {
                *updateVal = (f32) statePtr->data.tsqr.loVal;
                statePtr->stateFlags = OSC_LOW;
            }
            else
            {
                *updateVal = (f32) statePtr->data.tsqr.hiVal;
                statePtr->stateFlags = OSC_HIGH;
            }
            deltaTime *= statePtr->maxCount;
            break;

        case TREMELO_DSC_SAW:
            statePtr->curCount++;

            if (statePtr->curCount > statePtr->maxCount)
            {
                statePtr->curCount = 0;
            }
            tmpFlt = (f32) statePtr->curCount / (f32) statePtr->maxCount;
            tmpFlt *= (f32) statePtr->data.tsaw.depth;
            *updateVal = (f32) statePtr->data.tsaw.baseVol - tmpFlt;
            break;

        case TREMELO_ASC_SAW:
            statePtr->curCount++;

            if (statePtr->curCount > statePtr->maxCount)
            {
                statePtr->curCount = 0;
            }
            tmpFlt = (f32) statePtr->curCount / (f32) statePtr->maxCount;
            tmpFlt *= (f32) statePtr->data.tsaw.depth;
            *updateVal = (f32) statePtr->data.tsaw.baseVol + tmpFlt;
            break;

        case VIBRATO_SIN:
            /* calculate a sin value (from -1 to 1) and multiply it by depthcents.
               Then convert cents to ratio. */
            statePtr->curCount++;

            if (statePtr->curCount >= statePtr->maxCount)
            {
                statePtr->curCount = 0;
            }
            tmpFlt = (f32) statePtr->curCount / (f32) statePtr->maxCount;
            tmpFlt = sinf(tmpFlt * F_CST_DTOR32(360.0F)) * statePtr->data.vsin.depthcents;
            *updateVal = alCents2Ratio((s32) tmpFlt);
            break;

        case VIBRATO_SQR:
            if (statePtr->stateFlags == OSC_HIGH)
            {
                statePtr->stateFlags = OSC_LOW;
                *updateVal = statePtr->data.vsqr.loRatio;
            }
            else
            {
                statePtr->stateFlags = OSC_HIGH;
                *updateVal = statePtr->data.vsqr.hiRatio;
            }
            deltaTime *= statePtr->maxCount;
            break;

        case VIBRATO_DSC_SAW:
            statePtr->curCount++;

            if (statePtr->curCount > statePtr->maxCount)
            {
                statePtr->curCount = 0;
            }
            tmpFlt = (f32) statePtr->curCount / (f32) statePtr->maxCount;
            tmpFlt *= (f32) statePtr->data.vdsaw.centsrange;
            tmpFlt = (f32) statePtr->data.vdsaw.hicents - tmpFlt;
            *updateVal = alCents2Ratio((s32) tmpFlt);
            break;

        case VIBRATO_ASC_SAW:
            statePtr->curCount++;

            if (statePtr->curCount > statePtr->maxCount)
            {
                statePtr->curCount = 0;
            }
            tmpFlt = (f32) statePtr->curCount / (f32) statePtr->maxCount;
            tmpFlt *= (f32) statePtr->data.vasaw.centsrange;
            tmpFlt += (f32) statePtr->data.vasaw.locents;
            *updateVal = alCents2Ratio((s32) tmpFlt);
            break;
    }
    return deltaTime;
}


// 0x8001F42C
void syAudioStopOsc(void *oscState)
{
    ((SYAudioOsc*)oscState)->next = sSYAudioOscStatesAllocFree;
    sSYAudioOscStatesAllocFree = (SYAudioOsc*) oscState;
}

// 0x8001F444
void syAudioInit(void)
{
    sySchedulerAddClient(&sSYAudioClient, &sSYAudioTicMesgQueue, sSYAudioTicMesgs, ARRAY_COUNT(sSYAudioTicMesgs));
    osCreateMesgQueue(&sSYAudioDmaMesgQueue, sSYAudioDmaMesgs, ARRAY_COUNT(sSYAudioDmaMesgs));
    osCreateMesgQueue(&sSYAudioSPTaskMesgQueue, sSYAudioSPTaskMesgs, ARRAY_COUNT(sSYAudioSPTaskMesgs));
    osSendMesg(&sSYAudioSPTaskMesgQueue, (OSMesg)NULL, OS_MESG_BLOCK);
}

void syAudioLoadAssets(void)
{
    SYAudioSettings *psettings = &dSYAudioPublicSettings;
    ALBankFile *bnkf;
    s32 i;
    s32 len;
    SYAudioPackage *pkgf3, *pkgf2, *pkgf1;

    bzero(sSYAudioCurrentSettings.heap_base, sSYAudioCurrentSettings.heap_size);
    alHeapInit(&sSYAudioHeap, sSYAudioCurrentSettings.heap_base, sSYAudioCurrentSettings.heap_size);

    // load sfx bank
    if (sSYAudioCurrentSettings.bank2_start >= 0x80000000)
    {
        sSYAudioSequenceBank2 = (ALBank*) sSYAudioCurrentSettings.bank2_start;
    }
    else
    {
        len = sSYAudioCurrentSettings.bank2_end - sSYAudioCurrentSettings.bank2_start;
        bnkf = alHeapAlloc(&sSYAudioHeap, 1, len);
        syAudioReadRom(sSYAudioCurrentSettings.bank2_start, bnkf, len);
        alBnkfNew(bnkf, sSYAudioCurrentSettings.table2_start);
        sSYAudioSequenceBank2 = bnkf->bankArray[0];
    }
    if (sSYAudioCurrentSettings.bank1_start >= 0x80000000)
    {
        sSYAudioSequenceBank1 = (ALBank*) sSYAudioCurrentSettings.bank1_start;
    }
    else
    {
        len = sSYAudioCurrentSettings.bank1_end - sSYAudioCurrentSettings.bank1_start;
        bnkf = alHeapAlloc(&sSYAudioHeap, 1, len);
        syAudioReadRom(sSYAudioCurrentSettings.bank1_start, bnkf, len);
        alBnkfNew(bnkf, sSYAudioCurrentSettings.table1_start);
        sSYAudioSequenceBank1 = bnkf->bankArray[0];
    }
    // load sequnces
    if (sSYAudioCurrentSettings.sbk_start >= 0x80000000)
    {
        sSYAudioSeqFile = (ALSeqFile*) sSYAudioCurrentSettings.sbk_start;
    }
    else
    {
        sSYAudioSeqFile = alHeapAlloc(&sSYAudioHeap, 1, 4);
        syAudioReadRom(sSYAudioCurrentSettings.sbk_start, sSYAudioSeqFile, 4);
         
        len = sSYAudioSeqFile->seqCount * sizeof(ALSeqData) + 4;
        sSYAudioSeqFile = alHeapAlloc(&sSYAudioHeap, 1, sSYAudioSeqFile->seqCount * sizeof(ALSeqData) + 4);
        syAudioReadRom(sSYAudioCurrentSettings.sbk_start, sSYAudioSeqFile, len); 
        alSeqFileNew(sSYAudioSeqFile, (uintptr_t)sSYAudioCurrentSettings.sbk_start);
    }
    // get maximal seq length
    for (i = 0, len = 0; i < sSYAudioSeqFile->seqCount; i++)
    {
        sSYAudioSeqFile->seqArray[i].len += (sSYAudioSeqFile->seqArray[i].len & 1);
        
        if (len < sSYAudioSeqFile->seqArray[i].len)
        {
            len = sSYAudioSeqFile->seqArray[i].len;
        }
    }
    for (i = 0; i < ARRAY_COUNT(sSYAudioBGMSequenceDatas); i++)
    {
        sSYAudioBGMSequenceDatas[i] = alHeapAlloc(&sSYAudioHeap, 1, len);
    }
    sSYAudioAcmdListBuffers[0] = alHeapAlloc(&sSYAudioHeap, 1, 0x8000);
    sSYAudioAcmdListBuffers[1] = alHeapAlloc(&sSYAudioHeap, 1, 0x8000);

    sSYAudioSchedulerTasks[0] = alHeapAlloc(&sSYAudioHeap, 1, sizeof(SYTaskAudio));
    sSYAudioSchedulerTasks[1] = alHeapAlloc(&sSYAudioHeap, 1, sizeof(SYTaskAudio));

    sSYAudioDataBuffers[0] = alHeapAlloc(&sSYAudioHeap, 1, 0xE60);
    sSYAudioDataBuffers[1] = alHeapAlloc(&sSYAudioHeap, 1, 0xE60);
    sSYAudioDataBuffers[2] = alHeapAlloc(&sSYAudioHeap, 1, 0xE60);

    if (sSYAudioCurrentSettings.unk50 < 0x80000000)
    {
        len = sSYAudioCurrentSettings.unk54 - sSYAudioCurrentSettings.unk50;
        pkgf3 = alHeapAlloc(&sSYAudioHeap, 1, len);
        syAudioReadRom(sSYAudioCurrentSettings.unk50, pkgf3, len);
        
        dSYAudioPublicSettings.unk4C = sSYAudioCurrentSettings.unk4C = pkgf3->count;
        dSYAudioPublicSettings.unk44 = sSYAudioCurrentSettings.unk44 = pkgf3->data;
    }
    if (sSYAudioCurrentSettings.fgm_table_start < 0x80000000)
    {
        len = sSYAudioCurrentSettings.fgm_table_end - sSYAudioCurrentSettings.fgm_table_start;
        pkgf2 = alHeapAlloc(&sSYAudioHeap, 1, len);
        syAudioReadRom(sSYAudioCurrentSettings.fgm_table_start, pkgf2, len);
        
        // Have to use the symbol of dSYAudioPublicSettings2 (same address as dSYAudioPublicSettings) for this to match...
        dSYAudioPublicSettings2.fgm_table_count = sSYAudioCurrentSettings.fgm_table_count = pkgf2->count;
        dSYAudioPublicSettings.fgm_table_data = sSYAudioCurrentSettings.fgm_table_data = pkgf2->data;

        for (i = 0; i < sSYAudioCurrentSettings.fgm_table_count; i++)
        {
            sSYAudioCurrentSettings.fgm_table_data[i] += (uintptr_t)pkgf2;
        }
    }
    if (sSYAudioCurrentSettings.fgm_ucode_start < 0x80000000)
    {
        len = sSYAudioCurrentSettings.fgm_ucode_end - sSYAudioCurrentSettings.fgm_ucode_start;
        pkgf1 = alHeapAlloc(&sSYAudioHeap, 1, len);
        syAudioReadRom(sSYAudioCurrentSettings.fgm_ucode_start, pkgf1, len);
        
        // ...and dSYAudioPublicSettings3 for this. Why!?
        dSYAudioPublicSettings3.fgm_ucode_count = sSYAudioCurrentSettings.fgm_ucode_count = pkgf1->count;
        dSYAudioPublicSettings3.fgm_ucode_data = sSYAudioCurrentSettings.fgm_ucode_data = pkgf1->data;

        for (i = 0; i < sSYAudioCurrentSettings.fgm_ucode_count; i++)
        {
            sSYAudioCurrentSettings.fgm_ucode_data[i] += (uintptr_t)pkgf1;
        }
    }
}

void syAudioMakeBGMPlayers(void)
{
    ALSynConfig syn_config;
    ALSeqpConfig seqp_config;
    SYAudioConfig audio_config;
    s32 i, j;
    s32 unused;

    // 8003CB00 expected
    // 8003CB18 current

    sSYAudioHeapBase = sSYAudioHeap.cur;
    sSYAudioHeapSize = sSYAudioHeap.count;
    
    syn_config.maxVVoices = sSYAudioCurrentSettings.vvoices_num_max;
    syn_config.maxPVoices = sSYAudioCurrentSettings.pvoices_num_max;
    syn_config.maxUpdates = sSYAudioCurrentSettings.updates_num_max;
    syn_config.dmaproc = syAudioDmaNew;
    syn_config.outputRate = osAiSetFrequency(sSYAudioCurrentSettings.output_rate);
    syn_config.heap = &sSYAudioHeap;

    dSYAudioCurrentFxType = sSYAudioCurrentSettings.fx_type;

    if (dSYAudioCurrentFxType < AL_FX_CUSTOM)
    {
        syn_config.fxType = dSYAudioCurrentFxType;
    }
    else
    {
        syn_config.fxType = AL_FX_CUSTOM;
        syn_config.params = dSYAudioCustomFXTables[dSYAudioCurrentFxType - AL_FX_CUSTOM];
    }
    n_alInit(&sSYAudioGlobals, &syn_config);
    
    sSYAudioFrequency = (syn_config.outputRate / 60.0F);
    sSYAudioFrequency = ((sSYAudioFrequency / 184) * 184) + 184;
    D_8009D920_96D20 = sSYAudioFrequency - 184;
    
    audio_config.unk_80026204_0x0 = sSYAudioCurrentSettings.unk31;
    audio_config.unk_80026204_0x2 = sSYAudioCurrentSettings.unk32;
    audio_config.unk_80026204_0x4 = sSYAudioCurrentSettings.sndplayers_num;
    
    if (sSYAudioCurrentSettings.unk34 != 0)
    {
        audio_config.inst_sound_count = sSYAudioCurrentSettings.unk34;
        audio_config.inst_sound_array = sSYAudioCurrentSettings.unk38;
    }
    else
    {
        audio_config.inst_sound_count = sSYAudioSequenceBank1->instArray[0]->soundCount;
        audio_config.inst_sound_array = sSYAudioSequenceBank1->instArray[0]->soundArray;
    }
    audio_config.fgm_ucode_data = sSYAudioCurrentSettings.fgm_ucode_data;
    audio_config.fgm_table_data = sSYAudioCurrentSettings.fgm_table_data;
    audio_config.unk_80026204_0x1C = sSYAudioCurrentSettings.unk44;
    audio_config.fgm_ucode_count = sSYAudioCurrentSettings.fgm_ucode_count;
    audio_config.fgm_table_count = sSYAudioCurrentSettings.fgm_table_count;
    audio_config.unk_80026204_0xC = sSYAudioCurrentSettings.unk4C;
    audio_config.heap = &sSYAudioHeap;
    audio_config.unk_80026204_0x24 = sSYAudioCurrentSettings.unk12;
    audio_config.unk_80026204_0x26 = 10;
    audio_config.unk_80026204_0x28 = 20;
    audio_config.unk_80026204_0x2A = 30;
    audio_config.unk_80026204_0x2C = 40;
    audio_config.unk_80026204_0x2E = 50;
    audio_config.unk_80026204_0x30 = 60;
    
    func_80026204_26E04(&audio_config);

    sSYAudioSoundPlayers = alHeapAlloc(&sSYAudioHeap, 1, sSYAudioCurrentSettings.sndplayers_num * sizeof(*sSYAudioSoundPlayers));
    
    for (i = 0; i < sSYAudioCurrentSettings.sndplayers_num; i++)
    {
        sSYAudioSoundPlayers[i] = NULL;
    }
    sSYAudioCSPlayerStatuses = alHeapAlloc(&sSYAudioHeap, 1, sizeof(*sSYAudioCSPlayerStatuses));
    sSYAudioBGMPlayingIDs = alHeapAlloc(&sSYAudioHeap, 1, sizeof(*sSYAudioBGMPlayingIDs));
    gSYAudioGlobalBGMPriority = sSYAudioCurrentSettings.priority;
    
    sSYAudioOscStatesAllocFree = alHeapAlloc(&sSYAudioHeap, sizeof(*sSYAudioOscStatesAllocFree), SYAUDIO_OSC_STATES_NUM);

    for (j = 0; j < SYAUDIO_OSC_STATES_NUM - 1; j++)
    {
        sSYAudioOscStatesAllocFree[j].next = &sSYAudioOscStatesAllocFree[j + 1];
    }
    sSYAudioOscStatesAllocFree[j].next = NULL;
    
    for (i = 0; i < SYAUDIO_BGMPLAYERS_NUM; i++)
    {
        seqp_config.maxVoices = sSYAudioCurrentSettings.voices_num_max[i];
        seqp_config.maxEvents = sSYAudioCurrentSettings.events_num_max;
        seqp_config.maxChannels = AL_MAX_CHANNELS;
        seqp_config.heap = &sSYAudioHeap;
        seqp_config.initOsc = syAudioInitOsc;
        seqp_config.updateOsc = syAudioUpdateOsc;
        seqp_config.stopOsc = syAudioStopOsc;

        gSYAudioCSPlayers[i] = alHeapAlloc(&sSYAudioHeap, 1, sizeof(*gSYAudioCSPlayers[i]));
        n_alCSPNew(gSYAudioCSPlayers[i], &seqp_config);
        n_alCSPSetBank(gSYAudioCSPlayers[i], sSYAudioSequenceBank2);
        sSYAudioCSeqs[i] = alHeapAlloc(&sSYAudioHeap, 1, sizeof(*sSYAudioCSeqs[i]));
        
        sSYAudioCSPlayerStatuses[i] = 0;
        sSYAudioBGMPlayingIDs[i] = -1;
        sSYAudioBGMVolumeTimers[i] = 0;
        sSYAudioBGMVolumes[i] = 30720.0F;
        sSYAudioBGMVolumeRates[i] = 0.0F;
    }
}

void syAudioThreadMain(void *arg)
{
    s32 unused;
    s32 sp80;
    s32 id_mod3;
    s32 ai_len;
    u32 count_start;
    u8 sp73;
    s32 i;

    ai_len = 0;
    sp73 = 2;

    syAudioInit();
    
    sSYAudioCurrentSettings = dSYAudioPublicSettings;
    
    syAudioLoadAssets();
    syAudioMakeBGMPlayers();
    
    dSYAudioPublicSettings = sSYAudioCurrentSettings;
    
    osSendMesg(&gSYMainThreadingMesgQueue, (OSMesg)1, OS_MESG_NOBLOCK);

    while (TRUE)
    {
        count_start = osGetCount();
        i = dSYAudioCurrentTic & 1;
        sSYAudioCurrentTask = sSYAudioSchedulerTasks[i];
        sSYAudioCurrentAcmdListBuffer = sSYAudioAcmdListBuffers[i];
        id_mod3 = dSYAudioCurrentTic % 3;

        if (((ai_len > 368) && (sp73 != 2)) || ((ai_len > 184) && (sp73 == 0)))
        {
            dSYAudioSampleCounts[id_mod3] = D_8009D920_96D20;
            sp73 = 2;
        }
        else
        {
            dSYAudioSampleCounts[id_mod3] = sSYAudioFrequency;
        
            if (sp73 != 0)
            {
                sp73--;
            }
        }
        sSYAudioTimeStamp = osGetTime();
        
        sSYAudioCurrentAcmdListBuffer = n_alAudioFrame(sSYAudioCurrentAcmdListBuffer, &sp80, osVirtualToPhysical(sSYAudioDataBuffers[id_mod3]), dSYAudioSampleCounts[id_mod3]);

        sSYAudioCurrentTask->info.type = nSYTaskTypeAudio;
        sSYAudioCurrentTask->info.priority = 80;
        sSYAudioCurrentTask->info.fnCheck = NULL;
        sSYAudioCurrentTask->info.unk18 = 1;
        sSYAudioCurrentTask->info.retVal = 0;
        sSYAudioCurrentTask->info.mq = &sSYAudioSPTaskMesgQueue;
        sSYAudioCurrentTask->task.t.type = nSYTaskTypeAudio;
        sSYAudioCurrentTask->task.t.flags = 0;
        sSYAudioCurrentTask->task.t.data_ptr = (u64*) sSYAudioAcmdListBuffers[i];
        sSYAudioCurrentTask->task.t.data_size = ((sSYAudioCurrentAcmdListBuffer - sSYAudioAcmdListBuffers[i]) * 8);
        sSYAudioCurrentTask->task.t.ucode_boot = (u64*) gSYMainRspBootCode;
        sSYAudioCurrentTask->task.t.ucode_boot_size = 0x100;
        sSYAudioCurrentTask->task.t.ucode = (u64*) n_aspMainTextStart;
        sSYAudioCurrentTask->task.t.ucode_size = 0x1000;
        sSYAudioCurrentTask->task.t.ucode_data = (u64*) n_aspMainDataStart;
        sSYAudioCurrentTask->task.t.ucode_data_size = 0x800;
        sSYAudioCurrentTask->task.t.dram_stack = NULL;
        sSYAudioCurrentTask->task.t.dram_stack_size = 0;
        sSYAudioCurrentTask->task.t.yield_data_ptr = NULL;
        sSYAudioCurrentTask->task.t.yield_data_size = 0;
        sSYAudioCurrentTask->task.t.output_buffer = NULL;
        sSYAudioCurrentTask->task.t.output_buffer_size = 0;
        
        gSYAudioThreadTimeDelta = (osGetCount() - count_start) / 2971;
        osRecvMesg(&sSYAudioTicMesgQueue, NULL, OS_MESG_BLOCK);
        ai_len = IO_READ(AI_LEN_REG) >> 2;
        osRecvMesg(&sSYAudioSPTaskMesgQueue, NULL, OS_MESG_BLOCK);

        id_mod3 = (dSYAudioCurrentTic - 1) % 3;
        
        if (dSYAudioSoundQuality == 0)
        {
            for (i = 0; i < dSYAudioSampleCounts[id_mod3] * 2; i += 2)
            {
                sp80 = (sSYAudioDataBuffers[id_mod3][i] + sSYAudioDataBuffers[id_mod3][i + 1]) / 2;
                sSYAudioDataBuffers[id_mod3][i] = sp80;
                sSYAudioDataBuffers[id_mod3][i + 1] = sp80;
            }
        }
        osWritebackDCacheAll();
        osAiSetNextBuffer(sSYAudioDataBuffers[id_mod3], dSYAudioSampleCounts[id_mod3] * 4);
    
        for (i = 0; i < dSYAudioNextDma; i++)
        {
            osRecvMesg(&sSYAudioDmaMesgQueue, NULL, OS_MESG_NOBLOCK);
        }
        osWritebackDCacheAll();
        osSendMesg(&gSYSchedulerTaskMesgQueue, (OSMesg)sSYAudioCurrentTask, OS_MESG_NOBLOCK);
        
        dSYAudioCurrentTic++;
        dSYAudioNextDma = 0;
    
        for (i = 0; i < sSYAudioCurrentSettings.sndplayers_num; i++)
        {
            if ((sSYAudioSoundPlayers[i] != NULL) && (sSYAudioSoundPlayers[i]->unk_0x10 == 0))
            {
                sSYAudioSoundPlayers[i] = NULL;
            }
        }    
        for (i = 0; i < SYAUDIO_BGMPLAYERS_NUM; i++)
        {
            switch (sSYAudioCSPlayerStatuses[i])
            {
            case 1:
                if (gSYAudioCSPlayers[i]->state != AL_STOPPED)
                {
                    n_alCSPStop(gSYAudioCSPlayers[i]);
                    continue;
                }
                else if (sSYAudioBGMPlayingIDs[i] < 0)
                {
                    sSYAudioCSPlayerStatuses[i]--;
                    continue;
                }
                else
                {
                    syAudioReadRom(sSYAudioSeqFile->seqArray[sSYAudioBGMPlayingIDs[i]].offset, sSYAudioBGMSequenceDatas[i], sSYAudioSeqFile->seqArray[sSYAudioBGMPlayingIDs[i]].len);
                    sSYAudioCSPlayerStatuses[i]++;
                    continue;
                }
                break;
                
            case 2:
                n_alCSeqNew(sSYAudioCSeqs[i], sSYAudioBGMSequenceDatas[i]);
                n_alCSPSetSeq(gSYAudioCSPlayers[i], sSYAudioCSeqs[i]);
                n_alCSPPlay(gSYAudioCSPlayers[i]);

                for (sp80 = 0; sp80 < AL_MAX_CHANNELS; sp80++)
                {
                    n_alCSPSetChlPriority(gSYAudioCSPlayers[i], sp80, gSYAudioGlobalBGMPriority);
                }
                sSYAudioCSPlayerStatuses[i]++;
                break;
                
            case 3:
                if (gSYAudioCSPlayers[i]->state == AL_STOPPED)
                {
                    sSYAudioCSPlayerStatuses[i] = AL_STOPPED;
                    sSYAudioBGMPlayingIDs[i] = -1;
                }
                break;
            }
        }
        for (i = 0; i < ARRAY_COUNT(sSYAudioBGMVolumeTimers); i++)
        {
            if (sSYAudioBGMVolumeTimers[i] != 0)
            {
                sSYAudioBGMVolumeTimers[i]--;

                sSYAudioBGMVolumes[i] += sSYAudioBGMVolumeRates[i];

                if (sSYAudioBGMVolumes[i] < 0.0F)
                {
                    sSYAudioBGMVolumes[i] = 0.0F;
                }
                else if (sSYAudioBGMVolumes[i] > 30720.0F)
                {
                    sSYAudioBGMVolumes[i] = 30720.0F;
                }
                n_alCSPSetVol(gSYAudioCSPlayers[i], sSYAudioBGMVolumes[i]);
            }
        }
        if (dSYAudioIsSettingsUpdated != FALSE)
        {
            sp80 = sSYAudioCurrentSettings.sndplayers_num + 1;
            
            for (i = 0; i < sSYAudioCurrentSettings.sndplayers_num; i++)
            {
                if (sSYAudioSoundPlayers[i] == NULL)
                {
                    sp80--;
                }
            }
            for (i = 0; i < SYAUDIO_BGMPLAYERS_NUM; i++)
            {
                if (sSYAudioCSPlayerStatuses[i] == AL_STOPPED)
                {
                    sp80--;
                }
            }
            if (sp80 == 0)
            {
                osRecvMesg(&sSYAudioSPTaskMesgQueue, NULL, OS_MESG_BLOCK);
                osSendMesg(&sSYAudioSPTaskMesgQueue, NULL, OS_MESG_BLOCK);
                n_alClose(&sSYAudioGlobals);
            
                sSYAudioCurrentSettings = dSYAudioPublicSettings;
            
                syAudioLoadAssets();
                syAudioMakeBGMPlayers();
            
                dSYAudioPublicSettings = sSYAudioCurrentSettings;
            
                dSYAudioIsSettingsUpdated = FALSE;
                ai_len = 0;
                osSendMesg(&gSYMainThreadingMesgQueue, (OSMesg*)1, OS_MESG_NOBLOCK);
            }
            else
            {
                syAudioStopBGMAll();
                func_80020E28();
            }
        }
        if (dSYAudioIsRestarting != FALSE)
        {
            sp80 = sSYAudioCurrentSettings.sndplayers_num + 1;
            
            for (i = 0; i < sSYAudioCurrentSettings.sndplayers_num; i++)
            {
                if (sSYAudioSoundPlayers[i] == NULL)
                {
                    sp80--;
                }
            }
            for (i = 0; i < SYAUDIO_BGMPLAYERS_NUM; i++)
            {
                if (sSYAudioCSPlayerStatuses[i] == AL_STOPPED)
                {
                    sp80--;
                }
            }
            if (sp80 == 0)
            {
                osRecvMesg(&sSYAudioSPTaskMesgQueue, NULL, OS_MESG_BLOCK);
                osSendMesg(&sSYAudioSPTaskMesgQueue, NULL, OS_MESG_BLOCK);
                n_alClose(&sSYAudioGlobals);
                sSYAudioHeap.cur = sSYAudioHeapBase;
                sSYAudioHeap.count = sSYAudioHeapSize;
                syAudioMakeBGMPlayers();
                dSYAudioPublicSettings = sSYAudioCurrentSettings;
                dSYAudioIsRestarting = FALSE;
                ai_len = IO_READ(AI_LEN_REG) >> 2;
            }
            else
            {
                syAudioStopBGMAll();
                func_80020E28();
            }
        }
    }
}

// 0x80020A18
void syAudioSetStereo(void)
{
    dSYAudioSoundQuality = 1;
}

// 0x80020A28
void syAudioSetMono(void)
{
    dSYAudioSoundQuality = 0;
}

// 0x80020A34
void syAudioSetQuality(s32 quality)
{
    // 0 = mono, 1 = stereo
    dSYAudioSoundQuality = quality;
}

// 0x80020A40
void syAudioSetFXType(s32 fx_type)
{
    if (fx_type != dSYAudioCurrentFxType)
    {
        dSYAudioIsRestarting++;
        sSYAudioCurrentSettings.fx_type = fx_type;
    }
}

// 0x80020A74
void syAudioStopBGMAll(void)
{
    s32 i;

    for (i = 0; i < SYAUDIO_BGMPLAYERS_NUM; i++)
    {
        syAudioStopBGM(i);
    }
}

// 0x80020AB4
s32 syAudioPlayBGM(s32 sngplayer, u32 bgm)
{
    if (bgm < sSYAudioSeqFile->seqCount)
    {
        sSYAudioCSPlayerStatuses[sngplayer] = AL_PLAYING;
        sSYAudioBGMPlayingIDs[sngplayer] = bgm;

        return bgm;
    }
    else return -1;
}

// 0x80020B08
void syAudioStopBGM(s32 sngplayer)
{
    sSYAudioCSPlayerStatuses[sngplayer] = AL_PLAYING;
    sSYAudioBGMPlayingIDs[sngplayer] = -1;
}

// 0x80020B38
void syAudioSetBGMVolume(s32 sngplayer, u32 vol)
{
    if (vol > 30720)
    {
        vol = 30720;
    }
    n_alCSPSetVol(gSYAudioCSPlayers[sngplayer], vol);
    sSYAudioBGMVolumes[sngplayer] = vol;
    sSYAudioBGMVolumeTimers[sngplayer] = 0;
}

// 0x80020BC0
void syAudioSetBGMVolumeFade(s32 sngplayer, u32 vol, u32 time)
{
    if (vol > 30720)
    {
        vol = 30720;
    }
    if (time != 0)
    {
        sSYAudioBGMVolumeTimers[sngplayer] = time;
        sSYAudioBGMVolumeRates[sngplayer] = (vol - sSYAudioBGMVolumes[sngplayer]) / time;
    }
    else syAudioSetBGMVolume(sngplayer, vol);
}

// 0x80020C5C
void syAudioSetBGMReverb(s32 sngplayer, u32 reverb)
{
    s32 i;

    if (reverb > 127)
    {
        reverb = 127;
    }
    for (i = 0; i < AL_MAX_CHANNELS; i++)
    {
        n_alCSPSetChlFXMix(gSYAudioCSPlayers[sngplayer], i, reverb);
    }
}

// 0x80020CD4
void syAudioSetBGMPriority(s32 sngplayer, u8 priority)
{
    s32 i;

    if (priority > AL_MAX_PRIORITY)
    {
        priority = AL_MAX_PRIORITY;
    }
    gSYAudioGlobalBGMPriority = priority;

    for (i = 0; i < AL_MAX_CHANNELS; i++)
    {
        n_alCSPSetChlPriority(gSYAudioCSPlayers[sngplayer], i, priority);
    }
}

// 0x80020D58
s32 syAudioCheckBGMPlaying(s32 sngplayer)
{
    if (gSYAudioCSPlayers[sngplayer]->state == AL_STOPPED)
    {
        return FALSE;
    }
    else return TRUE;
}

s32 syAudioPlayFGM(u32 fgm)
{
    s32 i;
    
    for (i = 0; i < sSYAudioCurrentSettings.sndplayers_num; i++)
    {
        if (sSYAudioSoundPlayers[i] == NULL)
        {
            sSYAudioSoundPlayers[i] = func_800269C0_275C0(fgm);
            return i;
        }
    }
    return -1;
}

s32 func_80020E10(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    return -1;
}

void func_80020E28(void)
{
    s32 i;

    func_800266A0_272A0();

    // ??????
    for (i = 0; i < sSYAudioCurrentSettings.sndplayers_num; i++)
    {
        continue;
    }
}

void func_80020E64(u32 volume)
{
    u8 vol = (volume > 30720) ? AL_VOL_FULL : volume >> 8;

    func_80026070_26C70(vol);
}

void func_80020EA0(s32 sndplayer, u32 arg1)
{
    if (arg1 > 32767)
    {
        arg1 = 32767;
    }
    if (sSYAudioSoundPlayers[sndplayer] != NULL)
    {
        func_80026174_26D74(sSYAudioSoundPlayers[sndplayer], arg1 >> 8);
    }
}

void func_80020EF8(s32 sndplayer, s32 arg1)
{
    u8 var = arg1;

    if (var > 127)
    {
        var = 127;
    }
    if (sSYAudioSoundPlayers[sndplayer] != NULL)
    {
        func_80026104_26D04(sSYAudioSoundPlayers[sndplayer], var);
    }
}

void func_80020F4C(s32 sndplayer, s32 arg1)
{
    u8 var = arg1;

    if (var > 127)
    {
        var = 127;
    }
    if (sSYAudioSoundPlayers[sndplayer] != NULL)
    {
        func_80026094_26C94(sSYAudioSoundPlayers[sndplayer], var);
    }
}

// 0x80020FA0
void func_80020FA0_21BA0(s32 sndplayer, s32 arg1)
{
    return;
}

// 0x80020FAC
void syAudioStopFGM(s32 sndplayer)
{
    if (sSYAudioSoundPlayers[sndplayer] != NULL)
    {
        func_80026738_27338(sSYAudioSoundPlayers[sndplayer]);
        sSYAudioSoundPlayers[sndplayer] = NULL;
    }
}

// 0x80020FFC
void func_80020FFC(s32 sndplayer, u8 arg1)
{
    if (sSYAudioSoundPlayers[sndplayer] != NULL)
    {
        sSYAudioSoundPlayers[sndplayer]->unk_0x1F = arg1;
    }
}

// 0x8002102C
void syAudioSetSettingsUpdated(void)
{
    dSYAudioIsSettingsUpdated = TRUE;
}

// 0x8002103C
sb32 syAudioGetSettingsUpdated(void)
{
    return dSYAudioIsSettingsUpdated;
}

// 0x80021048
sb32 syAudioGetRestarting(void)
{
    return dSYAudioIsRestarting;
}

// 0x80021054 - Returns (dSYAudioIsRestarting | dSYAudioIsSettingsUpdated)
sb32 syAudioGetStatus(void)
{
    return dSYAudioIsRestarting | dSYAudioIsSettingsUpdated;
}

// 0x8002106C - unreferenced?
void syAudioUpdateMesgQueue(void)
{
    osRecvMesg(&gSYMainThreadingMesgQueue, NULL, OS_MESG_NOBLOCK);
    dSYAudioIsSettingsUpdated = TRUE;
    osRecvMesg(&gSYMainThreadingMesgQueue, NULL, OS_MESG_BLOCK);
}
