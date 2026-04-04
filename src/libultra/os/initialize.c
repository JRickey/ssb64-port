#include <PR/os_internal.h>
#include <PR/rcp.h>
#include <PR/os.h>
#include <piint.h>

typedef struct {
    /* 0x0 */ unsigned int inst1;
    /* 0x4 */ unsigned int inst2;
    /* 0x8 */ unsigned int inst3;
    /* 0xC */ unsigned int inst4;
} __osExceptionVector;
extern __osExceptionVector __osExceptionPreamble[];

extern OSPiHandle __Dom1SpeedParam;
extern OSPiHandle __Dom2SpeedParam;

OSTime osClockRate = OS_CLOCK_RATE;
s32 osViClock = VI_NTSC_CLOCK;
u32 __osShutdown = 0;
u32 __OSGlobalIntMask = OS_IM_ALL;
u32 __osFinalrom;

#define INITIALIZE_FUNC osInitialize
#define SPEED_PARAM_FUNC createSpeedParam
#if defined(REGION_US)
static void SPEED_PARAM_FUNC(void);
#endif
extern __osExceptionVector __ptExceptionPreamble[];

void INITIALIZE_FUNC() {
    u32 pifdata;
    u32 clock = 0;

    __osFinalrom = TRUE;

    __osSetSR(__osGetSR() | SR_CU1);    // enable fpu
    __osSetFpcCsr(FPCSR_FS | FPCSR_EV | FPCSR_RM_RN); // flush denorm to zero, enable invalid operation

    while (__osSiRawReadIo(PIF_RAM_END - 3, &pifdata)) { // last byte of joychannel ram
        ;
    }
    while (__osSiRawWriteIo(PIF_RAM_END - 3, pifdata | 8)) {
        ; // todo: magic contant
    }
    *(__osExceptionVector*)UT_VEC = *__osExceptionPreamble;
    *(__osExceptionVector*)XUT_VEC = *__osExceptionPreamble;
    *(__osExceptionVector*)ECC_VEC = *__osExceptionPreamble;
    *(__osExceptionVector*)E_VEC = *__osExceptionPreamble;
    osWritebackDCache((void*)UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
    osInvalICache((void*)UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
#if defined(REGION_US)
    SPEED_PARAM_FUNC();
    osUnmapTLBAll();
#endif
    osMapTLBRdb();
    __osPiRawReadIo(4, &clock); // Read clock rate from the ROM header
    clock &= ~0xf;
    if (clock != 0)
    {
        osClockRate = clock;
    }
    osClockRate = osClockRate * 3 / 4;

    if (osResetType == 0) { // cold reset
        bzero(osAppNMIBuffer, OS_APP_NMI_BUFSIZE);
    }

    if (osTvType == OS_TV_PAL) {
        osViClock = VI_PAL_CLOCK;
    } else if (osTvType == OS_TV_MPAL) {
        osViClock = VI_MPAL_CLOCK;
    } else {
        osViClock = VI_NTSC_CLOCK;
    }
#if defined(REGION_US)
    IO_WRITE(AI_CONTROL_REG, AI_CONTROL_DMA_ON);
    IO_WRITE(AI_DACRATE_REG, AI_MAX_DAC_RATE - 1);
    IO_WRITE(AI_BITRATE_REG, AI_MAX_BIT_RATE - 1);
#endif
}

#if defined(REGION_US)
static void SPEED_PARAM_FUNC(void) {
    __Dom1SpeedParam.type = DEVICE_TYPE_INIT;
    __Dom1SpeedParam.latency = IO_READ(PI_BSD_DOM1_LAT_REG);
    __Dom1SpeedParam.pulse = IO_READ(PI_BSD_DOM1_PWD_REG);
    __Dom1SpeedParam.pageSize = IO_READ(PI_BSD_DOM1_PGS_REG);
    __Dom1SpeedParam.relDuration = IO_READ(PI_BSD_DOM1_RLS_REG);

    __Dom2SpeedParam.type = DEVICE_TYPE_INIT;
    __Dom2SpeedParam.latency = IO_READ(PI_BSD_DOM2_LAT_REG);
    __Dom2SpeedParam.pulse = IO_READ(PI_BSD_DOM2_PWD_REG);
    __Dom2SpeedParam.pageSize = IO_READ(PI_BSD_DOM2_PGS_REG);
    __Dom2SpeedParam.relDuration = IO_READ(PI_BSD_DOM2_RLS_REG);
}
#endif
