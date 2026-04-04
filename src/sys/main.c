#include "main.h"

// #include "scenemgr/scene_manager.h"
#include <sys/debug.h>
#include <sys/dma.h>
#include <sys/taskman.h>
#include <sys/audio.h>
#include <sys/scheduler.h>
#include <sys/controller.h>

// #include <linkersegs.h>
#include <macros.h>
#include <ssb_types.h>

#include <PR/R4300.h>
#include <PR/os.h>
#include <PR/rcp.h>
#include <PR/ultratypes.h>

// libultra internal
#if defined(REGION_US)
void __osSetWatchLo(u32); // Only define this for US region as it breaks the JP region
#endif

#define STACK_CANARY_OFFSET 7 //this is weird but kirby also does this
#define STACK_CANARY 0xFEDCBA98

// size of stack in double words (u64, 8 bytes)
#define THREAD0_STACK_SIZE 0x200 / sizeof(u64)
#define THREAD1_STACK_SIZE 0x100 / sizeof(u64)
#define THREAD3_STACK_SIZE 0x400 / sizeof(u64)
#define THREAD4_STACK_SIZE 0x600 / sizeof(u64)
#if defined(REGION_US)
#define THREAD5_STACK_SIZE 0x3400 / sizeof(u64)
#else
#define THREAD5_STACK_SIZE 0x3000 / sizeof(u64)
#endif
#define THREAD6_STACK_SIZE 0x800 / sizeof(u64)

// Thread Scheduler Priorities
#define THREAD3_PRI 120
#define THREAD4_PRI 110
#define THREAD5_PRI 50
#define THREAD6_PRI 115

// // // // // // // // // // // //
//                               //
//       EXTERNAL VARIABLES      //
//                               //
// // // // // // // // // // // //

extern uintptr_t scmanager_ROM_START;
extern uintptr_t scmanager_ROM_END;
extern uintptr_t scmanager_VRAM;
extern uintptr_t scmanager_TEXT_START;
extern uintptr_t scmanager_TEXT_END;
extern uintptr_t scmanager_DATA_START;
extern uintptr_t scmanager_RODATA_END;
extern uintptr_t scmanager_BSS_START;
extern uintptr_t scmanager_BSS_END;

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

SYOverlay dSYMainSceneManagerOverlay =
{
    (uintptr_t)&scmanager_ROM_START,
    (uintptr_t)&scmanager_ROM_END,
    (uintptr_t)&scmanager_VRAM,
    (uintptr_t)&scmanager_TEXT_START,
    (uintptr_t)&scmanager_TEXT_END,
    (uintptr_t)&scmanager_DATA_START,
    (uintptr_t)&scmanager_RODATA_END,
    (uintptr_t)&scmanager_BSS_START,
    (uintptr_t)&scmanager_BSS_END
};

sb32 dSYMainNoThread5 = FALSE;

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

u64 gSYMainThread0Stack[THREAD0_STACK_SIZE];
OSThread sSYMainThread1;
u64 sSYMainThread1Stack[THREAD1_STACK_SIZE];
OSThread sSYMainThread3;
u64 sSYMainThread3Stack[THREAD3_STACK_SIZE];
OSThread sSYMainThread4;
u64 sSYMainThread4Stack[THREAD4_STACK_SIZE];
OSThread gSYMainThread5;
u64 sSYMainThread5Stack[THREAD5_STACK_SIZE];
OSThread gSYMainThread6;
u64 sSYMainThread6Stack[THREAD6_STACK_SIZE];
u64 gSYMainRspBootCode[0x20]; // IP3 font?
sb8 gSYMainImemOK;
sb8 gSYMainDmemOK;
OSMesg sSYMainBlockMesg[1];
OSMesgQueue gSYMainThreadingMesgQueue;
OSMesg sSYMainPiCmdMesg[50];
OSMesgQueue sSYMainPiCmdQueue;
u8 sSYMainThreadArgBuf[0x80];

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

u64* syMainGetThread4StackStart(void) 
{
    return sSYMainThread4Stack + ARRAY_COUNT(sSYMainThread4Stack);
}

u64* unref_8000046C(void)
{
    return &sSYMainThread5Stack[0];
}

void* unref_80000478(void) 
{
    #if defined(REGION_US)
    return (void*)(0x00003400);
    #else
    return (void*)(0x00003000);
    #endif
}

void syMainSetImemStatus(void)
{
    if (IO_READ(SP_IMEM_START) == 6103)
    {
        gSYMainImemOK = TRUE;
    }
    else gSYMainImemOK = FALSE;
}

void syMainSetDmemStatus(void)
{
    if (IO_READ(SP_DMEM_START) == -1)
    {
        gSYMainDmemOK = TRUE;
    } 
    else gSYMainDmemOK = FALSE;
}

void syMainThreadStackOverflow(s32 tid)
{
    syDebugPrintf("thread stack overflow  id = %d\n", tid);

    while (TRUE);
}

void syMainVerifyStackProbes(void) 
{
    if (gSYMainThread0Stack[STACK_CANARY_OFFSET] != STACK_CANARY)
    {
        syMainThreadStackOverflow(0);
    }
    if (sSYMainThread1Stack[STACK_CANARY_OFFSET] != STACK_CANARY)
    {
        syMainThreadStackOverflow(1);
    }
    if (sSYMainThread3Stack[STACK_CANARY_OFFSET] != STACK_CANARY)
    {
        syMainThreadStackOverflow(3);
    }
    if (sSYMainThread5Stack[STACK_CANARY_OFFSET] != STACK_CANARY)
    {
        syMainThreadStackOverflow(5);
    }
}

// 0x800005D8
void syMainThread5(void *arg)
{
    osCreateViManager(OS_PRIORITY_VIMGR);
    gSYDmaRomPiHandle = osCartRomInit();
    syDmaSramPiInit();
    osCreatePiManager(OS_PRIORITY_PIMGR, &sSYMainPiCmdQueue, sSYMainPiCmdMesg, ARRAY_COUNT(sSYMainPiCmdMesg));
    syDmaCreateMesgQueue();

    syDmaReadRom(PHYSICAL_TO_ROM(0xB70), gSYMainRspBootCode, sizeof(gSYMainRspBootCode));
    syMainSetImemStatus();
    syMainSetDmemStatus();
    osCreateMesgQueue(&gSYMainThreadingMesgQueue, sSYMainBlockMesg, ARRAY_COUNT(sSYMainBlockMesg));

    osCreateThread(&sSYMainThread3, 3, sySchedulerThreadMain, NULL, &sSYMainThread3Stack[THREAD3_STACK_SIZE], THREAD3_PRI);
    sSYMainThread3Stack[STACK_CANARY_OFFSET] = STACK_CANARY; osStartThread(&sSYMainThread3);
    osRecvMesg(&gSYMainThreadingMesgQueue, NULL, OS_MESG_BLOCK);

    osCreateThread(&sSYMainThread4, 4, syAudioThreadMain, NULL, &sSYMainThread4Stack[THREAD4_STACK_SIZE], THREAD4_PRI);
    sSYMainThread4Stack[STACK_CANARY_OFFSET] = STACK_CANARY; osStartThread(&sSYMainThread4);
    osRecvMesg(&gSYMainThreadingMesgQueue, NULL, OS_MESG_BLOCK);

    osCreateThread(&gSYMainThread6, 6, syControllerThreadMain, NULL, &sSYMainThread6Stack[THREAD6_STACK_SIZE], THREAD6_PRI);
    sSYMainThread6Stack[STACK_CANARY_OFFSET] = STACK_CANARY; osStartThread(&gSYMainThread6);
    osRecvMesg(&gSYMainThreadingMesgQueue, NULL, OS_MESG_BLOCK);

    func_80006B80();
    syDmaLoadOverlay(&dSYMainSceneManagerOverlay);
    scManagerRunLoop(0);
}

void syMainThread1Idle(void *arg) 
{
    syDebugStartRmonThread8();
    osCreateThread(&gSYMainThread5, 5, syMainThread5, arg, &sSYMainThread5Stack[THREAD5_STACK_SIZE], THREAD5_PRI);
    sSYMainThread5Stack[STACK_CANARY_OFFSET] = STACK_CANARY;

    if (dSYMainNoThread5 == FALSE)
    { 
        osStartThread(&gSYMainThread5);
    }
    osSetThreadPri(NULL, OS_PRIORITY_IDLE);

    while (TRUE);
}

void syMainLoop(void)
{
    gSYMainThread0Stack[STACK_CANARY_OFFSET] = STACK_CANARY;
    #if defined(REGION_US)
    __osSetWatchLo(0x04900000 & WATCHLO_ADDRMASK);
    #endif
    osInitialize();
    osCreateThread(&sSYMainThread1, 1, syMainThread1Idle, sSYMainThreadArgBuf, &sSYMainThread1Stack[THREAD1_STACK_SIZE], OS_PRIORITY_APPMAX);

    sSYMainThread1Stack[STACK_CANARY_OFFSET] = STACK_CANARY; osStartThread(&sSYMainThread1);
}
