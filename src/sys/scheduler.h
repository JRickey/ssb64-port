#ifndef _SYSCHEDULER_H_
#define _SYSCHEDULER_H_

#include <macros.h>

#include <PR/os.h>
#include <PR/sptask.h>
#include <PR/ultratypes.h>

#define SYSCHEDULER_BUFFER_NULL ((void*)-1)

enum SYTaskType
{
    nSYTaskTypeNone,
    nSYTaskTypeGfx,
    nSYTaskTypeAudio,
    nSYTaskTypeAddClient,
    nSYTaskTypeVi,
    nSYTaskTypeFramebuffers,
    nSYTaskTypeGfxEnd,
    nSYTaskTypeNoOp,
    nSYTaskTypeRdpBuffer,
    nSYTaskTypeCustomBuffer,
    nSYTaskTypeDefaultBuffer,
    SC_TASK_TYPE_11
};

enum SYTaskState
{
    nSYSchedulerStatusDefault,
    nSYSchedulerStatusTaskQueued,
    nSYSchedulerStatusTaskRunning,
    nSYSchedulerStatusTaskPending,
    nSYSchedulerStatusTaskSuspending,
    nSYSchedulerStatusTaskSuspended,
    nSYSchedulerStatusTaskStopped
};

typedef struct SYClient
{
    /* 0x00 */ struct SYClient* next;
    /* 0x04 */ OSMesgQueue* mq;
} SYClient; // size = 0x8

struct SYTaskInfo;
struct SYTaskGfx;

typedef sb32 (*SYTaskCallback)(struct SYTaskInfo*);
typedef sb32 (*SYTaskGfxCallback)(struct SYTaskGfx*);

// This may be the real form of `struct SpMqInfo`, but I'll have to double check
// scheduler.c to see if I can replace all forms...
typedef struct SYTaskInfo {
    /* 0x00 */ s32 type; // SYTaskType
    /* 0x04 */ s32 priority;
    /* 0x08 */ s32 state; // SYTaskState
    /* 0x0C */ struct SYTaskInfo *next;
    /* 0x10 */ struct SYTaskInfo *prev;
    /* 0x14 */ SYTaskCallback fnCheck; // if set, the task is not executed until this function returns TRUE
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 retVal;
    /* 0x20 */ OSMesgQueue *mq;
} SYTaskInfo; // size == 0x24

// Task SubTypes:
// 1  - OSTask (struct SYTaskGfx )
// 2  - Also something with an OSTask (using struct SYTaskGfx for now)
// 3  - Something with Mesq queueing?
// 4  - OsViMode changing?
// 5  - frame buffer pointers?
// 6  - SYTaskGfxEnd
// 7  - nop?
// 8  - ?
// 9  - passed an OSMesgQueue
// 10 - Sets D_80045010..? no special fields
// 11 - Remove all Type1 and Type4 from D_80044EC4 queue

typedef struct SYTaskGfx
{
    /* 0x00 */ SYTaskInfo info;
    /* 0x28 */ OSTask task;
    /* 0x68 */ u32 *unk68;
    /* 0x6C */ s32 *fb; // checked type? (-1 is meaningful)
    /* 0x70 */ s32 framebuffer_id; // frame buffer idx for D_80044F90
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 rdp_buffer_size;
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ u32 task_id;
    /* 0x84 */ s32 unk_84;

} SYTaskGfx; // size = 0x84

typedef struct
{
    /* 0x00 */ SYTaskInfo info;
    /* 0x28 */ OSTask task;
} SYTaskAudio; // size = 0x68

typedef struct
{
    /* 0x00 */ SYTaskInfo info;
    /* 0x24 */ s32 width;
    /* 0x28 */ s32 height;
    /* 0x2C */ s32 flags;
    /* 0x30 */ s16 edgeOffsetLeft;
    /* 0x32 */ s16 edgeOffsetRight;
    /* 0x34 */ s16 edgeOffsetTop;
    /* 0x36 */ s16 edgeOffsetBottom;
} SYTaskVi; // size == 0x38

typedef struct
{
    /* 0x00 */ SYTaskInfo info;
    /* 0x24 */ void *framebuffers[3]; // frame buffer pointers
} SYTaskFramebuffer;                    // size == 0x30

typedef struct
{
    /* 0x00 */ SYTaskInfo info;
    /* 0x24 */ void* buffer;
    /* 0x28 */ s32 size;
} SYTaskRdpBuffer; // size >= 0x2C

typedef struct
{
    /* 0x00 */ SYTaskInfo info;
    /* 0x24 */ SYClient* client;
} SYTaskAddClient; // size == 0x28

typedef struct SYTaskGfxEnd
{
    /* 0x00 */ SYTaskInfo info;
    /* 0x24 */ void* fb;
    /* 0x28 */ u32 task_id;

} SYTaskGfxEnd; /* size == 0x2C */

typedef struct SYTaskType9
{
    /* 0x00 */ SYTaskInfo info;
    /* 0x24 */ OSMesgQueue *unk24;

} SYTaskType9; // size >= 0x28

extern OSMesgQueue gSYSchedulerTaskMesgQueue;
extern sb32 gSYSchedulerIsCustomFramebuffer;
extern u32 gSYSchedulerFrameTime;
extern u32 gSYSchedulerAudioTime;
extern u64 sSYSchedulerRdpCache;
extern ub32 gSYSchedulerIsSoftReset;
extern void *gSYSchedulerNextFramebuffer;
extern void *gSYSchedulerCurrentFramebuffer;
extern u32 gSYSchedulerFramebufferSetTimestamp;

extern void func_80000970(SYTaskInfo *arg0);
extern void sySchedulerAddClient(SYClient *arg0, OSMesgQueue *mq, OSMesg *msg, u32 count);
extern s32 sySchedulerCheckReadyFramebuffer(SYTaskGfx *t);
extern void sySchedulerThreadMain(void *arg);
extern s32 func_80000B54(UNUSED SYTaskInfo *t);

#endif /* SYS_THREAD_3_H */
