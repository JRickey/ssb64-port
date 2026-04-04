#include "common.h"
#include "scheduler.h"

#include <sys/debug.h>
#include <sys/main.h>
#include <sys/video.h>
#include <sys/controller.h>

#include <config.h>
#include <macros.h>
#include <missing_libultra.h>
#include <ssb_types.h>

#include <PR/os.h>
#include <PR/rcp.h>
#include <PR/viint.h>
#include <PR/sptask.h>
#include <PR/ultratypes.h>

/*
    28 00 u32	type;
    2C 04 u32	flags;
    30 08 u64	*ucode_boot;
    34 0c u32	ucode_boot_size;

    38 10 u64	*ucode;
    3c 14 u32	ucode_size;
    40 18 u64	*ucode_data;
    44 1c u32	ucode_data_size;

    48 20 u64	*dram_stack;
    4c 24 u32	dram_stack_size;
    50 28 u64	*output_buffer;
    54 2c u64	*output_buffer_size;

    58 30 u64	*data_ptr;
    5c 34 u32	data_size;
    60 38 u64	*yield_data_ptr;
    64 3c u32	yield_data_size;
*/

union CheckedPtr {
    void *ptr;
    intptr_t tag;
};

typedef struct SYImageSetup
{
    ub32 antialias    : 1; // b0 0 80 => unknown game control (flags & 0x1) [aa & resamp enabled?]
    ub32 serrate      : 1; // b1 0 40 => serrate enabled (bool)
    ub32 colordepth32 : 1; // b2  20 => type_32 enabled
    ub32 gamma        : 1; // b3   10 => gamma on
    ub32 blackout     : 1; // b4   08 => unknown game control (flags & 0x100) [blackout ?]
    ub32 unk_b04      : 1; // b5   04 => unknown game control (flags & 0x400) [custom resolution?]
    ub32 gammadither  : 1; // b6   02 => gamma dither on
    ub32 ditherfilter : 1; // b7   01 => dither filter
    ub32 divot        : 1; // b8 1 80 => divot on

} SYImageSetup;

// bss
SYClient *sSYSchedulerClients;
SYTaskInfo *sSYSchedulerMainQueueHead; // largest priority/unk04?
SYTaskInfo *sSYSchedulerMainQueueTail; // smallest priority/unk04?
SYTaskGfx *sSYSchedulerCurrentTaskGfx;  // actually a pointer to SYTaskGfx?
SYTaskAudio *sSYSchedulerCurrentTaskAudio;  // largest priority queue 2
SYTaskGfx *sSYSchedulerPausedQueueHead;  // smallest priority queue 2
SYTaskGfx *sSYSchedulerPausedQueueTail;  // largest priority queue 3
SYTaskGfx *scQueue3Head;  // smallest priority queue 3
SYTaskGfx *D_80044EE0_406F0;  // standard linked list head
SYTaskGfx *scCurrentQueue3Task;  // standard linked list tail
OSViMode sSYSchedulerPendingViMode;
OSViMode sSYSchedulerCurrentViMode;
sb32 sSYSchedulerIsViModePending;
void *sSYSchedulerFramebuffers[3];
void *gSYSchedulerNextFramebuffer;
void *sSYSchedulerCustomFramebuffer;
sb32 gSYSchedulerIsCustomFramebuffer;
void *gSYSchedulerCurrentFramebuffer;
u32 gSYSchedulerFramebufferSetTimestamp;
u32 gSYSchedulerAudioStartTimestamp;

// 0x80044FB4 - Time taken to process frame
u32 gSYSchedulerFrameTime;

// 0x80044FB8 - Time taken to process audio
u32 gSYSchedulerAudioTime;

SYImageSetup sSYSchedulerImageSetup;

u64 sSYSchedulerRdpCache;

s32 sSYSchedulerRdpOutputBufferID;
u64 *sSYSchedulerRdpOutputBuffer;
size_t sSYSchedulerRdpOutputBufferSize; // size of sSYSchedulerRdpOutputBuffer
OSMesg sSYSchedulerTaskMesgs[8];
OSMesgQueue gSYSchedulerTaskMesgQueue;
sb32 scUseCustomSwapBufferFunc;
OSMesgQueue *scCustomSwapBufferQueue;
void (*sSYSchedulerFuncReset)(void);
u32 sSYSchedulerTicCount;
ub32 gSYSchedulerIsSoftReset; // gSoftReseting ..?
s32 sSYSchedulerAfterPreNMI; // return of osAfterPreNMI
void *sSYSchedulerReadyBuffers[3];
u8 sSYSchedulerReadyBufferID;
u8 sSYSchedulerSwapBufferID;

// 0x80000920
void sySchedulerSetTicCount(u32 tics)
{
    sSYSchedulerTicCount = tics;
}

// 0x0000092C
u32 sySchedulerGetTicCount(void)
{
    return sSYSchedulerTicCount;
}

void unref_80000938(void)
{
    do
    {
        continue;
    }
    while ((sSYSchedulerCurrentTaskGfx != NULL) || (scCurrentQueue3Task != NULL) || (scQueue3Head != NULL));
}

void func_80000970(SYTaskInfo *task) {
    OSMesg msgs[1];
    OSMesgQueue mq;

    osCreateMesgQueue(&mq, msgs, ARRAY_COUNT(msgs));
    task->fnCheck  = NULL;
    task->retVal = 1;
    task->mq = &mq;
    osSendMesg(&gSYSchedulerTaskMesgQueue, (OSMesg)task, OS_MESG_NOBLOCK);
    osRecvMesg(&mq, NULL, OS_MESG_BLOCK);
}

void sySchedulerAddClient(SYClient *client, OSMesgQueue *mq, OSMesg *msg, u32 count)
{
    SYTaskAddClient t;

    osCreateMesgQueue(mq, msg, count);

    client->mq = mq;
    t.info.type = nSYTaskTypeAddClient;
    t.info.priority = 100;
    t.client = client;

    func_80000970(&t.info);
}

// 0x80000A34 - Returns true if task can be executed now
s32 sySchedulerCheckReadyFramebuffer(SYTaskGfx *t)
{
    s32 unused;
    s32 i;
    void *next, *curr, *free;

    if (gSYSchedulerNextFramebuffer != NULL)
    {
        return TRUE;
    }
    else if (sSYSchedulerCustomFramebuffer != NULL)
    {
        return FALSE;
    }
    next = osViGetNextFramebuffer();
    curr = osViGetCurrentFramebuffer();

    // Set framebuffer for drawing
    if (t->framebuffer_id != -1)
    {
        free = sSYSchedulerFramebuffers[t->framebuffer_id];

        if ((free != NULL) && (curr != free) && (next != free))
        {
            sSYSchedulerCustomFramebuffer = gSYSchedulerNextFramebuffer = free;
            sSYSchedulerRdpOutputBufferID = 0;
            gSYSchedulerFramebufferSetTimestamp = osGetCount();
            return TRUE;
        }
    }
    // Set any available framebuffer
    for (i = 0; i < ARRAY_COUNT(sSYSchedulerFramebuffers); i++)
    {
        free = sSYSchedulerFramebuffers[i];

        if ((free != NULL) && (curr != free) && (next != free))
        {
            gSYSchedulerNextFramebuffer = free;
            sSYSchedulerRdpOutputBufferID = 0;
            gSYSchedulerFramebufferSetTimestamp = osGetCount();
            return TRUE;
        }
    }
    return FALSE;
}

s32 func_80000B54(UNUSED SYTaskInfo *t)
{
    SYTaskInfo *curr;

    if ((sSYSchedulerCurrentTaskGfx != NULL) && (sSYSchedulerCurrentTaskGfx->info.type == nSYTaskTypeGfx))
    {
        return FALSE;
    }
    curr = &sSYSchedulerPausedQueueHead->info;

    while (curr != NULL)
    {
        if (curr->type == nSYTaskTypeGfx)
        {
            return FALSE;
        }
        curr = curr->next;
    }
    curr = sSYSchedulerMainQueueHead;

    while (curr != NULL)
    {
        if (curr->type == nSYTaskTypeGfx)
        {
            return FALSE;
        }
        curr = curr->next;
    }
    if ((scCurrentQueue3Task != NULL) && (scCurrentQueue3Task->info.type == nSYTaskTypeGfx))
    {
        return FALSE;
    }
    curr = &scQueue3Head->info;

    while (curr != NULL)
    {
        if (curr->type == nSYTaskTypeGfx)
        {
            return FALSE;
        }
        curr = curr->next;
    }
    if (sSYSchedulerReadyBufferID != sSYSchedulerSwapBufferID)
    {
        return FALSE;
    }
    else return TRUE;
}

// Insert into task priority queue
void sySchedulerAddMainQueue(SYTaskInfo *this_info)
{
    // Find task with priority higher than this_info
    SYTaskInfo *curr_info = sSYSchedulerMainQueueTail;

    while ((curr_info != NULL) && (curr_info->priority < this_info->priority))
    {
        curr_info = curr_info->prev;
    }
    // Insert new task into queue
    this_info->prev = curr_info;

    if (curr_info != NULL)
    {
        this_info->next = curr_info->next;
        curr_info->next = this_info;
    }
    else
    {
        this_info->next = sSYSchedulerMainQueueHead;
        sSYSchedulerMainQueueHead = this_info;
    }
    curr_info = this_info->next;

    if (curr_info != NULL)
    {
        curr_info->prev = this_info;
    }
    else sSYSchedulerMainQueueTail = this_info;
}

// 0x80000CF4
void sySchedulerRemoveMainQueue(SYTaskInfo *task)
{
    if (task->prev != NULL)
    {
        task->prev->next = task->next;
    }
    else sSYSchedulerMainQueueHead = task->next;
    
    if (task->next != NULL)
    {
        task->next->prev = task->prev;
    }
    else sSYSchedulerMainQueueTail = task->prev;
}

// 0x80000D44 - Add to sSYSchedulerPausedQueueHead/sSYSchedulerPausedQueueTail priority queue
void sySchedulerAddPausedQueue(SYTaskInfo *this_info)
{
    SYTaskInfo *tail_info = &sSYSchedulerPausedQueueTail->info;

    while ((tail_info != NULL) && (tail_info->priority < this_info->priority))
    {
        tail_info = tail_info->prev;
    }
    this_info->prev = tail_info;

    if (tail_info != NULL)
    {
        this_info->next = tail_info->next;
        tail_info->next = this_info;
    }
    else
    {
        this_info->next = &sSYSchedulerPausedQueueHead->info;
        sSYSchedulerPausedQueueHead = this_info;
    }
    tail_info = this_info->next;

    if (tail_info != NULL)
    {
        tail_info->prev = this_info;
    }
    else sSYSchedulerPausedQueueTail = this_info;
}

// remove from sSYSchedulerPausedQueueHead/sSYSchedulerPausedQueueTail queue
void sySchedulerRemovePausedQueue(SYTaskInfo *this_info)
{
    if (this_info->prev != NULL)
    {
        this_info->prev->next = this_info->next;
    }
    else sSYSchedulerPausedQueueHead = this_info->next;
    
    if (this_info->next != NULL)
    {
        this_info->next->prev = this_info->prev;
    }
    else sSYSchedulerPausedQueueTail = this_info->prev;
}

// scQueue3Add
// append to head of scQueue3Head/D_80044EE0_406F0 queue
void func_80000E24(SYTaskGfx *task) {
    task->info.next = NULL;
    task->info.prev = &D_80044EE0_406F0->info;
    if (D_80044EE0_406F0 != NULL) {
        D_80044EE0_406F0->info.next = &task->info;
    } else {
        scQueue3Head = task;
    }
    D_80044EE0_406F0 = task;
}

// remove from scQueue3Head/D_80044EE0_406F0 queue
void func_80000E5C(SYTaskGfx *task) {
    if (task->info.prev != NULL) {
        task->info.prev->next = task->info.next;
    } else {
        scQueue3Head = (void *)task->info.next;
    }

    if (task->info.next != NULL) {
        task->info.next->prev = task->info.prev;
    } else {
        D_80044EE0_406F0 = (void *)task->info.prev;
    }
}

void sySchedulerApplyViMode(void)
{
    sSYSchedulerPendingViMode = sSYSchedulerCurrentViMode;
    osViSetMode(&sSYSchedulerPendingViMode);
    osViBlack(sSYSchedulerImageSetup.blackout);
    sSYSchedulerIsViModePending = FALSE;
}

// 0x80000F30
void sySchedulerUpdateViMode(u32 width, u32 height, u32 flags, s16 off_left, s16 off_right, s16 off_top, s16 off_bottom)
{
    sb32 not_phi_v1;
    sb32 phi_v1;
    sb32 not_res_in_bounds;
    sb32 is_res_in_bounds;
    s32 vertical;
    s32 pos1;
    s32 pos2;

    is_res_in_bounds = ((width > GS_SCREEN_WIDTH_DEFAULT) || (height > GS_SCREEN_HEIGHT_DEFAULT)) ? FALSE : TRUE;

    if (flags & SYVIDEO_FLAG_SERRATE)
    {
        sSYSchedulerImageSetup.serrate = TRUE;
        sSYSchedulerCurrentViMode.comRegs.ctrl |= VI_CTRL_SERRATE_ON;
    }
    if (flags & SYVIDEO_FLAG_NOSERRATE)
    {
        sSYSchedulerImageSetup.serrate = FALSE;
        sSYSchedulerCurrentViMode.comRegs.ctrl &= ~VI_CTRL_SERRATE_ON;
    }
    if (flags & SYVIDEO_FLAG_COLORDEPTH16)
    {
        sSYSchedulerImageSetup.colordepth32 = FALSE;
        sSYSchedulerCurrentViMode.comRegs.ctrl &= ~(VI_CTRL_TYPE_32 | VI_CTRL_TYPE_16);
        sSYSchedulerCurrentViMode.comRegs.ctrl |= VI_CTRL_TYPE_16;
    }
    if (flags & SYVIDEO_FLAG_COLORDEPTH32)
    {
        sSYSchedulerImageSetup.colordepth32 = TRUE;
        sSYSchedulerCurrentViMode.comRegs.ctrl &= ~(VI_CTRL_TYPE_32 | VI_CTRL_TYPE_16);
        sSYSchedulerCurrentViMode.comRegs.ctrl |= VI_CTRL_TYPE_32;
    }
    if (flags & SYVIDEO_FLAG_GAMMA)
    {
        sSYSchedulerImageSetup.gamma = TRUE;
        sSYSchedulerCurrentViMode.comRegs.ctrl |= VI_CTRL_GAMMA_ON;
    }
    if (flags & SYVIDEO_FLAG_NOGAMMA)
    {
        sSYSchedulerImageSetup.gamma = FALSE;
        sSYSchedulerCurrentViMode.comRegs.ctrl &= ~VI_CTRL_GAMMA_ON;
    }
    if (flags & SYVIDEO_FLAG_GAMMADITHER)
    {
        sSYSchedulerImageSetup.gammadither = TRUE;
        sSYSchedulerCurrentViMode.comRegs.ctrl |= VI_CTRL_GAMMA_DITHER_ON;
    }
    if (flags & SYVIDEO_FLAG_NOGAMMADITHER)
    {
        sSYSchedulerImageSetup.gammadither = FALSE;
        sSYSchedulerCurrentViMode.comRegs.ctrl &= ~VI_CTRL_GAMMA_DITHER_ON;
    }
    if (flags & SYVIDEO_FLAG_DITHERFILTER)
    {
        sSYSchedulerImageSetup.ditherfilter = TRUE;
        sSYSchedulerCurrentViMode.comRegs.ctrl |= VI_CTRL_DITHER_FILTER_ON;
    }
    if (flags & SYVIDEO_FLAG_NODITHERFILTER)
    {
        sSYSchedulerImageSetup.ditherfilter = FALSE;
        sSYSchedulerCurrentViMode.comRegs.ctrl &= ~VI_CTRL_DITHER_FILTER_ON;
    }
    if (flags & SYVIDEO_FLAG_DIVOT)
    {
        sSYSchedulerImageSetup.divot = TRUE;
        sSYSchedulerCurrentViMode.comRegs.ctrl |= VI_CTRL_DIVOT_ON;
    }
    if (flags & SYVIDEO_FLAG_NODIVOT)
    {
        sSYSchedulerImageSetup.divot = FALSE;
        sSYSchedulerCurrentViMode.comRegs.ctrl &= ~VI_CTRL_DIVOT_ON;
    }
    if (flags & SYVIDEO_FLAG_BLACKOUT)
    {
        sSYSchedulerImageSetup.blackout = TRUE;
    }
    if (flags & SYVIDEO_FLAG_NOBLACKOUT)
    {
        sSYSchedulerImageSetup.blackout = FALSE;
    }
    if (flags & 0x400)
    {
        sSYSchedulerImageSetup.unk_b04 = TRUE;
    }
    if (flags & 0x800)
    {
        sSYSchedulerImageSetup.unk_b04 = FALSE;
    }
    if (flags & SYVIDEO_FLAG_ANTIALIAS)
    {
        sSYSchedulerImageSetup.antialias = TRUE;
    }
    if (flags & SYVIDEO_FLAG_NOANTIALIAS)
    {
        sSYSchedulerImageSetup.antialias = FALSE;
    }
    sSYSchedulerCurrentViMode.comRegs.ctrl &= ~VI_CTRL_ANTIALIAS_MASK;

    if (sSYSchedulerImageSetup.antialias)
    {
        sSYSchedulerCurrentViMode.comRegs.ctrl |= ((sSYSchedulerImageSetup.ditherfilter) ? 0 : VI_CTRL_ANTIALIAS_MODE_1);
    }
    else if (!(sSYSchedulerImageSetup.unk_b04) && (sSYSchedulerImageSetup.colordepth32 == TRUE))
    {
        sSYSchedulerCurrentViMode.comRegs.ctrl |= VI_CTRL_ANTIALIAS_MODE_3; // neither (replicate pixels, no interpolate)
    }
    else sSYSchedulerCurrentViMode.comRegs.ctrl |= VI_CTRL_ANTIALIAS_MODE_2; // resamp only (treat as all fully covered)
    
    if (is_res_in_bounds != FALSE)
    {
        phi_v1 = (sSYSchedulerImageSetup.serrate) ? FALSE : TRUE;
    }
    else phi_v1 = (sSYSchedulerImageSetup.unk_b04) ? FALSE : TRUE;

    off_top &= ~1;
    off_bottom &= ~1;

    not_phi_v1 = !phi_v1;
    not_res_in_bounds = !is_res_in_bounds;

    vertical = (((not_res_in_bounds != FALSE) && (not_phi_v1 != FALSE)) ? 2 : 1) * 
        (((height * 2048) / ((off_bottom - off_top) + 480)) / ((is_res_in_bounds != FALSE) ? 1 : 2));
    
    sSYSchedulerCurrentViMode.comRegs.width = (((not_res_in_bounds != FALSE) && (phi_v1 != FALSE)) ? 2 : 1) * width;

    if (TRUE)
    {
#if defined(REGION_US)
        if (osTvType == OS_TV_NTSC)
        {
            sSYSchedulerCurrentViMode.comRegs.burst     = 0x3E52239;
            sSYSchedulerCurrentViMode.comRegs.vSync     = 0x20C;
            sSYSchedulerCurrentViMode.comRegs.hSync     = 0xC15;
            sSYSchedulerCurrentViMode.comRegs.leap      = 0xC150C15;
            sSYSchedulerCurrentViMode.comRegs.hStart    = 0x6C02EC;
            sSYSchedulerCurrentViMode.fldRegs[0].vStart = 0x2501FF;
            sSYSchedulerCurrentViMode.fldRegs[0].vBurst = 0xE0204;
        }
        if (osTvType == OS_TV_MPAL)
        {
            sSYSchedulerCurrentViMode.comRegs.burst     = 0x4651E39;
            sSYSchedulerCurrentViMode.comRegs.vSync     = 0x20C;
            sSYSchedulerCurrentViMode.comRegs.hSync     = 0xC10;
            sSYSchedulerCurrentViMode.comRegs.leap      = 0xC1C0C1C;
            sSYSchedulerCurrentViMode.comRegs.hStart    = 0x6C02EC;
            sSYSchedulerCurrentViMode.fldRegs[0].vStart = 0x2501FF;
            sSYSchedulerCurrentViMode.fldRegs[0].vBurst = 0xE0204;
        }
#else
        sSYSchedulerCurrentViMode.comRegs.burst     = 0x3E52239;
        sSYSchedulerCurrentViMode.comRegs.vSync     = 0x20C;
        sSYSchedulerCurrentViMode.comRegs.hSync     = 0xC15;
        sSYSchedulerCurrentViMode.comRegs.leap      = 0xC150C15;
        sSYSchedulerCurrentViMode.comRegs.hStart    = 0x6C02EC;
        sSYSchedulerCurrentViMode.fldRegs[0].vStart = 0x2501FF;
        sSYSchedulerCurrentViMode.fldRegs[0].vBurst = 0xE0204;
#endif
    }
    
    sSYSchedulerCurrentViMode.fldRegs[1].vStart = sSYSchedulerCurrentViMode.fldRegs[0].vStart;
    
    pos1 = sSYSchedulerCurrentViMode.comRegs.hStart >> 16;
    pos2 = sSYSchedulerCurrentViMode.comRegs.hStart & 0xFFFF;

    pos1 += off_left;

    if (pos1 < 0)
    {
        pos1 = 0;
    }
    pos2 += off_right;

    if (pos2 < 0)
    {
        pos2 = 0;
    }
    sSYSchedulerCurrentViMode.comRegs.hStart = (pos1 << 16) | pos2;

    pos1 = sSYSchedulerCurrentViMode.fldRegs[0].vStart >> 16;
    pos2 = sSYSchedulerCurrentViMode.fldRegs[0].vStart & 0xFFFF;

    pos1 += off_top;

    if (pos1 < 0)
    {
        pos1 = 0;
    }
    pos2 += off_bottom;

    if (pos2 < 0)
    {
        pos2 = 0;
    }
    sSYSchedulerCurrentViMode.fldRegs[0].vStart = (pos1 << 16) | pos2;
    
    pos1 = sSYSchedulerCurrentViMode.fldRegs[1].vStart >> 16;
    pos2 = sSYSchedulerCurrentViMode.fldRegs[1].vStart & 0xFFFF;

    pos1 += off_top;

    if (pos1 < 0)
    {
        pos1 = 0;
    }
    pos2 += off_bottom;

    if (pos2 < 0)
    {
        pos2 = 0;
    }
    sSYSchedulerCurrentViMode.fldRegs[1].vStart = (pos1 << 16) | pos2;
    
    sSYSchedulerCurrentViMode.fldRegs[1].vBurst = sSYSchedulerCurrentViMode.fldRegs[0].vBurst;

    if ((is_res_in_bounds != FALSE) && (phi_v1 != FALSE))
    {
        sSYSchedulerCurrentViMode.comRegs.vSync += 1;
        if (1);
        
#if defined(REGION_US)
        if (osTvType == OS_TV_MPAL)
        {
            sSYSchedulerCurrentViMode.comRegs.hSync += 0x40001;
            
        }
        if (osTvType == OS_TV_MPAL)
        {
            sSYSchedulerCurrentViMode.comRegs.leap += 0xFFFCFFFE;
        }
#endif
    }
    else
    {
        sSYSchedulerCurrentViMode.fldRegs[0].vStart += 0xFFFDFFFE;
        if (1);

#if defined(REGION_US)
        if (osTvType == OS_TV_MPAL)
        {
            sSYSchedulerCurrentViMode.fldRegs[0].vBurst += 0xFFFCFFFE;
        }
        if (osTvType == OS_TV_PAL) 
        {
            sSYSchedulerCurrentViMode.fldRegs[1].vBurst += 0x2FFFE;
        }
#endif
    }
    
    sSYSchedulerCurrentViMode.comRegs.vCurrent  = 0;
    sSYSchedulerCurrentViMode.comRegs.xScale    = (width * 1024) / ((off_right - off_left) + 640);
    sSYSchedulerCurrentViMode.fldRegs[0].origin = (!(sSYSchedulerImageSetup.colordepth32) ? 1 : 2) * width * 2;
    phi_v1 = sSYSchedulerImageSetup.colordepth32;
    sSYSchedulerCurrentViMode.fldRegs[1].origin = (!(phi_v1) ? 1 : 2) * (((is_res_in_bounds != FALSE) ? 1 : 2) * width * 2);
    sSYSchedulerCurrentViMode.fldRegs[0].yScale = vertical;
    sSYSchedulerCurrentViMode.fldRegs[1].yScale = vertical;

    if (sSYSchedulerImageSetup.unk_b04)
    {
        if (1);
        if (height < 360)
        {
            sSYSchedulerCurrentViMode.fldRegs[0].yScale += 0x3000000;
            sSYSchedulerCurrentViMode.fldRegs[1].yScale += 0x1000000;
        }
        else
        {
            sSYSchedulerCurrentViMode.fldRegs[0].yScale += 0x2000000;
            sSYSchedulerCurrentViMode.fldRegs[1].yScale += 0x2000000;
        }
    }
    sSYSchedulerCurrentViMode.fldRegs[0].vIntr = 2;
    sSYSchedulerCurrentViMode.fldRegs[1].vIntr = 2;
    sSYSchedulerIsViModePending = 1;
}

void sySchedulerSwapBuffer(void)
{
    void *curr, *next;

    if (sSYSchedulerSwapBufferID != sSYSchedulerReadyBufferID)
    {
        next = osViGetNextFramebuffer(), curr = osViGetCurrentFramebuffer();

        if (next == curr)
        {
            osViSwapBuffer(sSYSchedulerReadyBuffers[sSYSchedulerSwapBufferID]);

            if (sSYSchedulerSwapBufferID == (ARRAY_COUNT(sSYSchedulerReadyBuffers) - 1))
            {
                sSYSchedulerSwapBufferID = 0;
            }
            else sSYSchedulerSwapBufferID += 1;
        }
    }
}

void sySchedulerReadyBuffer(void *framebuffer)
{
    sSYSchedulerReadyBuffers[sSYSchedulerReadyBufferID] = framebuffer;

    if (sSYSchedulerReadyBufferID == (ARRAY_COUNT(sSYSchedulerReadyBuffers) - 1))
    {
        sSYSchedulerReadyBufferID = 0;
    }
    else sSYSchedulerReadyBufferID += 1;
    
    sySchedulerSwapBuffer();
}

// 0x800017B8
void sySchedulerSetNextFramebuffer(void *framebuffer)
{
    if (sSYSchedulerIsViModePending != FALSE)
    {
        if (gSYSchedulerIsSoftReset == FALSE)
        {
            sySchedulerApplyViMode();
        }
    }
    if (scUseCustomSwapBufferFunc != 0)
    {
        osSendMesg(scCustomSwapBufferQueue, (OSMesg)1, OS_MESG_NOBLOCK);

        if (framebuffer == SYSCHEDULER_BUFFER_NULL)
        {
            gSYSchedulerCurrentFramebuffer = gSYSchedulerNextFramebuffer;
            gSYSchedulerNextFramebuffer = NULL;
        }
        else gSYSchedulerCurrentFramebuffer = framebuffer;
    }
    else
    {
        if (framebuffer == SYSCHEDULER_BUFFER_NULL)
        {
            sySchedulerReadyBuffer(gSYSchedulerNextFramebuffer);

            if (sSYSchedulerCustomFramebuffer == gSYSchedulerNextFramebuffer)
            { 
                gSYSchedulerIsCustomFramebuffer = TRUE;
            }
            gSYSchedulerCurrentFramebuffer = gSYSchedulerNextFramebuffer;
            gSYSchedulerNextFramebuffer = NULL;
        }
        else
        {
            sySchedulerReadyBuffer(framebuffer);
            gSYSchedulerCurrentFramebuffer = framebuffer;
        }
    }
    gSYSchedulerFrameTime = (osGetCount() - gSYSchedulerFramebufferSetTimestamp) / 2971;
}

// 0x800018E0
void sySchedulerExecuteTaskGfx(SYTaskGfx *t)
{
    if (sSYSchedulerCurrentTaskGfx != NULL)
    {
        osSpTaskYield();
        sSYSchedulerCurrentTaskGfx->info.state = nSYSchedulerStatusTaskSuspending;
        sySchedulerAddPausedQueue(sSYSchedulerCurrentTaskGfx);
        t->info.state = nSYSchedulerStatusTaskPending;
    }
    else
    {
        osSpTaskStart(&t->task);
        t->info.state = nSYSchedulerStatusTaskRunning;
    }
    sSYSchedulerCurrentTaskGfx = t;
}

// 0x80001968
void sySchedulerExecuteTaskAudio(SYTaskAudio *t)
{
    gSYSchedulerAudioStartTimestamp = osGetCount();

    if ((sSYSchedulerCurrentTaskGfx != NULL) && (sSYSchedulerCurrentTaskGfx->info.state == nSYSchedulerStatusTaskRunning))
    {
        osSpTaskYield();
        sSYSchedulerCurrentTaskGfx->info.state = nSYSchedulerStatusTaskSuspending;
        t->info.state = nSYSchedulerStatusTaskPending;
    }
    else
    {
        osSpTaskStart(&t->task);
        t->info.state = nSYSchedulerStatusTaskRunning;
    }
    sSYSchedulerCurrentTaskAudio = t;
}

// 0x80001A00
s32 sySchedulerExecuteTask(SYTaskInfo *task)
{
    s32 ret = 0;
    s32 unused[4]; // required to match
    SYTaskInfo* sp34[2];

    switch (task->type) {
        case nSYTaskTypeGfx: {
            SYTaskGfx* t = (void*) task;

            if (t->unk68 != NULL) {
                *t->unk68 |= (uintptr_t) gSYSchedulerNextFramebuffer;
                osWritebackDCache(t->unk68, sizeof(t->unk68));
            }
            if (t->task.t.output_buffer == SYSCHEDULER_BUFFER_NULL)
            {
                t->task.t.output_buffer = (u64*) ((uintptr_t) sSYSchedulerRdpOutputBuffer + sSYSchedulerRdpOutputBufferID);
                osWritebackDCache(&t->task.t.output_buffer, sizeof(t->task.t.output_buffer));
            }
            if (t->unk74 == 1) {
                osInvalDCache(&sSYSchedulerRdpCache, sizeof(sSYSchedulerRdpCache));
            }
            sySchedulerExecuteTaskGfx(t);
            ret = 1;
            break;
        }
        case nSYTaskTypeAudio: {
            SYTaskAudio* t = (void*) task;

            osWritebackDCacheAll();
            sySchedulerExecuteTaskAudio(t);
            ret = 1;
            break;
        }
        case nSYTaskTypeAddClient: {
            SYTaskAddClient* t = (void*) task;
            SYClient* temp;

            temp = t->client;
            temp->next = sSYSchedulerClients;
            sSYSchedulerClients = temp;

            if (t->info.mq != NULL) {
                osSendMesg(t->info.mq, (OSMesg) t->info.retVal, OS_MESG_NOBLOCK);
            }
            break;
        }
        case nSYTaskTypeVi: {
            SYTaskVi* t = (void*) task;

            sySchedulerUpdateViMode(t->width, t->height, t->flags, t->edgeOffsetLeft, t->edgeOffsetRight, t->edgeOffsetTop, t->edgeOffsetBottom);

            if (t->info.mq != NULL) {
                osSendMesg(t->info.mq, (OSMesg) t->info.retVal, OS_MESG_NOBLOCK);
            }
            break;
        }
        case nSYTaskTypeFramebuffers:
        {
            SYTaskFramebuffer* t = (void*) task;
            s32 i;

            for (i = 0; i < ARRAY_COUNT(sSYSchedulerFramebuffers); i++)
            {
                sSYSchedulerFramebuffers[i] = t->framebuffers[i];
            }

            if (t->info.mq != NULL) {
                osSendMesg(t->info.mq, (OSMesg) t->info.retVal, OS_MESG_NOBLOCK);
            }
            break;
        }
        case nSYTaskTypeGfxEnd: {
            SYTaskGfxEnd* t = (void*) task;
            SYTaskGfx* v1 = NULL;
            SYTaskInfo* v0;

            if (sSYSchedulerCurrentTaskGfx != NULL && sSYSchedulerCurrentTaskGfx->info.type == nSYTaskTypeGfx && sSYSchedulerCurrentTaskGfx->task_id == t->task_id) {
                v1 = sSYSchedulerCurrentTaskGfx;
            }

            v0 = &sSYSchedulerPausedQueueHead->info;
            while (v0 != NULL) {
                if (v0->type == nSYTaskTypeGfx) {
                    if (((SYTaskGfx*) v0)->task_id == t->task_id) {
                        v1 = (void*) v0;
                    }
                }
                v0 = v0->next;
            }

            v0 = sSYSchedulerMainQueueHead;
            while (v0 != NULL) {
                if (v0->type == nSYTaskTypeGfx) {
                    if (((SYTaskGfx*) v0)->task_id == t->task_id) {
                        v1 = (void*) v0;
                    }
                }

                v0 = v0->next;
            }

            v0 = &scCurrentQueue3Task->info;
            if (v0 != NULL) {
                if (v0->type == nSYTaskTypeGfx) {
                    if (sSYSchedulerCurrentTaskGfx->task_id == t->task_id) {
                        v1 = (void*) v0;
                    }
                }
            }

            v0 = &scQueue3Head->info;
            while (v0 != NULL) {
                if (v0->type == nSYTaskTypeGfx) {
                    if (((SYTaskGfx*) v0)->task_id == t->task_id) {
                        v1 = (void*) v0;
                    }
                }
                v0 = v0->next;
            }

            if (v1 != NULL) {
                v1->info.retVal = t->info.retVal;
                v1->info.mq = t->info.mq;
                v1->fb = t->fb;
            } else {
                if (t->fb != NULL) {
                    sySchedulerSetNextFramebuffer(t->fb);
                }

                if (t->info.mq != NULL) {
                    osSendMesg(t->info.mq, (OSMesg) t->info.retVal, OS_MESG_NOBLOCK);
                }
            }
            break;
        }
        case nSYTaskTypeNoOp:
            if (task->mq != NULL) {
                osSendMesg(task->mq, (OSMesg) task->retVal, OS_MESG_NOBLOCK);
            }
            break;
        case nSYTaskTypeRdpBuffer: {
            SYTaskRdpBuffer* t = (void*) task;

            sSYSchedulerRdpOutputBuffer = t->buffer;
            sSYSchedulerRdpOutputBufferSize = t->size;
            if (t->info.mq != NULL) {
                osSendMesg(t->info.mq, (OSMesg) t->info.retVal, OS_MESG_NOBLOCK);
            }
            break;
        }
        case nSYTaskTypeCustomBuffer: {
            SYTaskType9* t = (void*) task;

            scUseCustomSwapBufferFunc = TRUE;
            scCustomSwapBufferQueue = t->unk24;

            if (t->info.mq != NULL)
            {
                osSendMesg(t->info.mq, (OSMesg) t->info.retVal, OS_MESG_NOBLOCK);
            }
            break;
        }
        case nSYTaskTypeDefaultBuffer:
            scUseCustomSwapBufferFunc = FALSE;
            if (task->mq != NULL) {
                osSendMesg(task->mq, (OSMesg) task->retVal, OS_MESG_NOBLOCK);
            }
            break;
        case SC_TASK_TYPE_11: {
            SYTaskInfo* a0 = sSYSchedulerMainQueueHead;
            while (a0 != NULL) {
                if (a0->type == nSYTaskTypeGfx || a0->type == nSYTaskTypeVi) {
                    sp34[0] = a0->next;
                    sySchedulerRemoveMainQueue(a0);
                    a0 = sp34[0];
                } else {
                    a0 = a0->next;
                }
            }

            sSYSchedulerCustomFramebuffer = NULL;
            if (task->mq != NULL) {
                osSendMesg(task->mq, (OSMesg) task->retVal, OS_MESG_NOBLOCK);
            }
            break;
        }
    }
    return ret;
}

// 0x80001E64
void sySchedulerExecuteTasksAll(void)
{
    s32 yield_priority;
    s32 curr_priority; // cur "priority"
    sb32 is_task_ready;
    SYTaskInfo *curr, *next;
    sb32 is_task_started = FALSE;
    s32 paused_priority; // "priority" of sSYSchedulerPausedQueueHead
    s32 av_priority; // "priority" of sSYSchedulerCurrentTaskGfx or sSYSchedulerCurrentTaskAudio

    av_priority = (sSYSchedulerCurrentTaskGfx != NULL) ? sSYSchedulerCurrentTaskGfx->info.priority : -1;

    if (sSYSchedulerCurrentTaskAudio != NULL)
    {
        av_priority = sSYSchedulerCurrentTaskAudio->info.priority;
    }
    paused_priority = (sSYSchedulerPausedQueueHead != NULL) ? sSYSchedulerPausedQueueHead->info.priority : -1;

    curr = sSYSchedulerMainQueueHead;

    while (is_task_started == FALSE)
    {
        curr_priority = (curr != NULL) ? curr->priority : -1;

        if (paused_priority >= curr_priority)
        {
            is_task_ready = FALSE;
            yield_priority = paused_priority;
        }
        else
        {
            is_task_ready = TRUE;
            yield_priority = curr_priority;
        }
        if (av_priority >= yield_priority)
        {
            is_task_started = TRUE;
        }
        else switch (is_task_ready)
        {
        case FALSE:
            osSpTaskStart(&sSYSchedulerPausedQueueHead->task);
            is_task_started = TRUE;
            sSYSchedulerPausedQueueHead->info.state = nSYSchedulerStatusTaskRunning;
            sSYSchedulerCurrentTaskGfx = sSYSchedulerPausedQueueHead;
            sySchedulerRemovePausedQueue(sSYSchedulerPausedQueueHead);
            break;
                
        case TRUE:
            if ((curr->fnCheck == NULL) || (curr->fnCheck(curr) != FALSE))
            {
                is_task_started = sySchedulerExecuteTask(curr);
                next = curr->next;
                sySchedulerRemoveMainQueue(curr);
                curr = next;
            }
            else curr = curr->next;
            break;
        }
    }
}

void func_80001FF4(void) {
    if ((scCurrentQueue3Task == NULL) && (scQueue3Head != NULL))
    {
        scCurrentQueue3Task = scQueue3Head;
        func_80000E5C(scQueue3Head);
        scCurrentQueue3Task->info.state = 2;
        osDpSetNextBuffer(scCurrentQueue3Task->task.t.output_buffer, scCurrentQueue3Task->rdp_buffer_size);
    }
}

void sySchedulerVRetrace(void)
{
    SYClient *curr, *temp;

    sSYSchedulerTicCount++;

    curr = sSYSchedulerClients;

    while (curr != NULL)
    {
        temp = curr;
        osSendMesg(curr->mq, (OSMesg)1, 0);
        curr = curr->next;

        if (temp);
    }
    sySchedulerSwapBuffer();
    sySchedulerExecuteTasksAll();
}

// 0x800020D0
void sySchedulerSpTaskDone(void)
{
    if ((sSYSchedulerCurrentTaskAudio != NULL) && (sSYSchedulerCurrentTaskAudio->info.state == nSYSchedulerStatusTaskRunning))
    {
        osSendMesg(sSYSchedulerCurrentTaskAudio->info.mq, (OSMesg)NULL, OS_MESG_NOBLOCK);
        sSYSchedulerCurrentTaskAudio = NULL;
        sySchedulerExecuteTasksAll();
        gSYSchedulerAudioTime = (osGetCount() - gSYSchedulerAudioStartTimestamp) / 2971;
    }
    else
    {
        if (sSYSchedulerCurrentTaskGfx != NULL && sSYSchedulerCurrentTaskGfx->info.state == nSYSchedulerStatusTaskSuspending)
        {
            if (osSpTaskYielded(&sSYSchedulerCurrentTaskGfx->task) == OS_TASK_YIELDED)
            {
                sSYSchedulerCurrentTaskGfx->info.state = nSYSchedulerStatusTaskSuspended;
                sySchedulerAddPausedQueue(sSYSchedulerCurrentTaskGfx);
                sSYSchedulerCurrentTaskGfx = NULL;
            }
            else sSYSchedulerCurrentTaskGfx->info.state = nSYSchedulerStatusTaskStopped;
            
            osSpTaskStart(&sSYSchedulerCurrentTaskAudio->task);
            sSYSchedulerCurrentTaskAudio->info.state = nSYSchedulerStatusTaskRunning;
        }
        if ((sSYSchedulerCurrentTaskGfx != NULL) && (sSYSchedulerCurrentTaskGfx->info.unk18 == 1) && (sSYSchedulerCurrentTaskGfx->info.state != nSYSchedulerStatusTaskSuspended))
        {
            if ((sSYSchedulerCurrentTaskGfx->info.type == nSYTaskTypeGfx) && (sSYSchedulerCurrentTaskGfx->unk74 == 1))
            {
                osInvalDCache(&sSYSchedulerRdpCache, sizeof(sSYSchedulerRdpCache));
                sSYSchedulerCurrentTaskGfx->rdp_buffer_size = sSYSchedulerRdpCache;
                sSYSchedulerRdpOutputBufferID += (s32) sSYSchedulerRdpCache;
                sSYSchedulerRdpOutputBufferID = OS_DCACHE_ROUNDUP_SIZE(sSYSchedulerRdpOutputBufferID);
    
                if (sSYSchedulerRdpOutputBufferID < sSYSchedulerRdpCache)
                {
                    syDebugPrintf("rdp_output_buff over !! size = %d\n byte", sSYSchedulerRdpOutputBufferID);
                    while (TRUE);
                }
                sSYSchedulerCurrentTaskGfx->info.state = nSYSchedulerStatusTaskQueued;
                func_80000E24(sSYSchedulerCurrentTaskGfx);
                func_80001FF4();
            }
            sSYSchedulerCurrentTaskGfx = NULL;
            sySchedulerExecuteTasksAll();
        }
        else if ((sSYSchedulerCurrentTaskGfx != NULL) && (sSYSchedulerCurrentTaskGfx->info.unk18 == 2) && (sSYSchedulerCurrentTaskGfx->info.type == nSYTaskTypeGfx))
        {
            sSYSchedulerCurrentTaskGfx->info.state = nSYSchedulerStatusTaskStopped;
    
            if (!(sSYSchedulerCurrentTaskGfx->unk7C & 2))
            {
                sSYSchedulerCurrentTaskGfx->unk7C |= 1;
            }
        }
    }
}

void sySchedulerDpFullSync(void)
{
    if ((sSYSchedulerCurrentTaskGfx != NULL) && (sSYSchedulerCurrentTaskGfx->info.unk18 == 2))
    {
        if (sSYSchedulerCurrentTaskGfx->info.type == nSYTaskTypeGfx)
        {
            if (sSYSchedulerCurrentTaskGfx->fb != NULL)
            {
                sySchedulerSetNextFramebuffer(sSYSchedulerCurrentTaskGfx->fb);
            }
            if (sSYSchedulerCurrentTaskGfx->info.mq != NULL)
            {
                osSendMesg(sSYSchedulerCurrentTaskGfx->info.mq, (OSMesg)sSYSchedulerCurrentTaskGfx->info.retVal, OS_MESG_NOBLOCK);
            }
            if (sSYSchedulerCurrentTaskGfx->info.state == nSYSchedulerStatusTaskSuspending)
            {
                osSpTaskStart(&sSYSchedulerCurrentTaskAudio->task);
                sSYSchedulerCurrentTaskAudio->info.state = nSYSchedulerStatusTaskRunning;
            }
        }
        sSYSchedulerCurrentTaskGfx = NULL;
        sySchedulerExecuteTasksAll();
    }
    else if (scCurrentQueue3Task != NULL)
    {
        if (scCurrentQueue3Task->fb != NULL)
        {
            sySchedulerSetNextFramebuffer(scCurrentQueue3Task->fb);
        }
        if (scCurrentQueue3Task->info.mq != NULL)
        {
            osSendMesg(scCurrentQueue3Task->info.mq, (OSMesg)scCurrentQueue3Task->info.retVal, OS_MESG_NOBLOCK);
        }
        scCurrentQueue3Task = NULL;
        func_80001FF4();
    }
    else if ((sSYSchedulerPausedQueueHead != NULL) && (sSYSchedulerPausedQueueHead->info.unk18 == 2))
    {
        if (sSYSchedulerPausedQueueHead->info.type == nSYTaskTypeGfx)
        {
            if (sSYSchedulerPausedQueueHead->fb != NULL)
            {
                sySchedulerSetNextFramebuffer(sSYSchedulerPausedQueueHead->fb);
            }
            if (sSYSchedulerPausedQueueHead->info.mq != NULL)
            {
                osSendMesg(sSYSchedulerPausedQueueHead->info.mq, (OSMesg)sSYSchedulerPausedQueueHead->info.retVal, OS_MESG_NOBLOCK);
            }
            sySchedulerRemovePausedQueue(sSYSchedulerPausedQueueHead);
        }
        sySchedulerExecuteTasksAll();
    }
}

void sySchedulerPrepTask(SYTaskInfo *task)
{
    task->state = nSYSchedulerStatusTaskQueued;

    sySchedulerAddMainQueue(task);
    sySchedulerExecuteTasksAll();
}

// forward dec
void sySchedulerSoftReset(void);

#define INTR_VRETRACE     1
#define INTR_SP_TASK_DONE 2
#define INTR_DP_FULL_SYNC 3
#define INTR_SOFT_RESET   99

// 0x80002518
// jp: 0x8000241C 
void sySchedulerThreadMain(void *arg)
{
    s32 mesg;

    // the wonders of matching
    sSYSchedulerClients = NULL;

    sSYSchedulerMainQueueHead =
    sSYSchedulerMainQueueTail =
    sSYSchedulerCurrentTaskGfx =
    sSYSchedulerCurrentTaskAudio =
    sSYSchedulerPausedQueueHead =
    sSYSchedulerPausedQueueTail = NULL;
    scCurrentQueue3Task = scQueue3Head = D_80044EE0_406F0 = NULL;

    sSYSchedulerIsViModePending = FALSE;
    gSYSchedulerCurrentFramebuffer = gSYSchedulerNextFramebuffer = sSYSchedulerCustomFramebuffer = NULL;
    scUseCustomSwapBufferFunc = FALSE;
    sSYSchedulerFuncReset = sySchedulerSoftReset;
    gSYSchedulerIsSoftReset = FALSE;
#if defined(REGION_US)
    sSYSchedulerAfterPreNMI = -1;
#endif
    sSYSchedulerTicCount = 0;
    sSYSchedulerReadyBufferID = sSYSchedulerSwapBufferID = 0;

#if defined(REGION_US)
    switch (osTvType)
    {
    case OS_TV_NTSC:
        sSYSchedulerCurrentViMode = sSYSchedulerPendingViMode = osViModeNtscLan1;
        break;
        
    case OS_TV_PAL:
        while (TRUE);
        break;
        
    case OS_TV_MPAL:
        sSYSchedulerCurrentViMode = sSYSchedulerPendingViMode = osViModeMpalLan1;
        break;
    }
#else
    sSYSchedulerCurrentViMode = sSYSchedulerPendingViMode = osViModeNtscLan1;
#endif

    sSYSchedulerPendingViMode.comRegs.ctrl = VI_CTRL_TYPE_16 | VI_CTRL_GAMMA_DITHER_ON | VI_CTRL_DIVOT_ON | VI_CTRL_DITHER_FILTER_ON;
    sSYSchedulerCurrentViMode.comRegs.ctrl = VI_CTRL_TYPE_16 | VI_CTRL_GAMMA_DITHER_ON | VI_CTRL_DIVOT_ON | VI_CTRL_DITHER_FILTER_ON;
    osViSetMode(&sSYSchedulerPendingViMode);
    osViBlack(TRUE);

    sSYSchedulerImageSetup.antialias    = TRUE;
    sSYSchedulerImageSetup.serrate      = FALSE;
    sSYSchedulerImageSetup.colordepth32  = FALSE;
    sSYSchedulerImageSetup.gamma        = FALSE;
    sSYSchedulerImageSetup.blackout     = TRUE;
    sSYSchedulerImageSetup.unk_b04      = FALSE;
    sSYSchedulerImageSetup.gammadither  = TRUE;
    sSYSchedulerImageSetup.ditherfilter = TRUE;
    sSYSchedulerImageSetup.divot        = TRUE;

    osCreateMesgQueue(&gSYSchedulerTaskMesgQueue, sSYSchedulerTaskMesgs, ARRAY_COUNT(sSYSchedulerTaskMesgs));
    osViSetEvent(&gSYSchedulerTaskMesgQueue, (OSMesg)INTR_VRETRACE, 1);
    osSetEventMesg(OS_EVENT_SP, &gSYSchedulerTaskMesgQueue, (OSMesg)INTR_SP_TASK_DONE);
    osSetEventMesg(OS_EVENT_DP, &gSYSchedulerTaskMesgQueue, (OSMesg)INTR_DP_FULL_SYNC);
    osSetEventMesg(OS_EVENT_PRENMI, &gSYSchedulerTaskMesgQueue, (OSMesg)INTR_SOFT_RESET);

    osSendMesg(&gSYMainThreadingMesgQueue, (OSMesg)1, OS_MESG_NOBLOCK);

    while (TRUE)
    {
        osRecvMesg(&gSYSchedulerTaskMesgQueue, (OSMesg)&mesg, OS_MESG_BLOCK);

        switch (mesg)
        {
        case INTR_VRETRACE:
            sySchedulerVRetrace();
            break;
        
        case INTR_SP_TASK_DONE:
            sySchedulerSpTaskDone();

#if defined(REGION_US)
            if ((gSYSchedulerIsSoftReset == 1) && (sSYSchedulerAfterPreNMI == -1))
            {
                sSYSchedulerAfterPreNMI = osAfterPreNMI();
            }
#endif
            break;

        case INTR_DP_FULL_SYNC:
            sySchedulerDpFullSync();
            break;

        case INTR_SOFT_RESET:
            if (sSYSchedulerFuncReset != NULL)
            {
                sSYSchedulerFuncReset();
            }
            break;

        default:
            if (gSYSchedulerIsSoftReset == FALSE)
            {
                sySchedulerPrepTask(mesg);
            }
        }
    }
}

void sySchedulerSoftReset(void)
{
    s32 i;

    gSYSchedulerIsSoftReset = TRUE;

    osViSetYScale(1.0);
    osViBlack(TRUE);

    for (i = 0; i < MAXCONTROLLERS; i++)
    {
        syControllerInitRumble(i);
        syControllerStopRumble(i);
    }
    #if defined(REGION_US)
    sSYSchedulerAfterPreNMI = osAfterPreNMI();
    #endif
}

void unref_80002A50(void (*func)(void))
{
    sSYSchedulerFuncReset = func;
}
