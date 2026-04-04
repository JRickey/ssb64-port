#ifndef SYS_THREAD_6_H
#define SYS_THREAD_6_H

#include <PR/os.h>
#include <PR/ultratypes.h>
#include <ssb_types.h>

typedef struct ControllerInput
{
    /* 0x00 */ u16 unk00; // button
    /* 0x02 */ u16 unk02; // new button presses?
    /* 0x04 */ u16 unk04; // buttons to handle?
    /* 0x06 */ u16 unk06; // released buttons?
    /* 0x08 */ s8 unk08;  // stick_x
    /* 0x09 */ s8 unk09;  // stick_y

} ControllerInput; // size = 0x0A

typedef struct SYController
{
    u16 button_hold; // button
    u16 button_tap; // new button presses?
    u16 button_update; // buttons to handle?
    u16 button_release; // released buttons?
    Vec2b stick_range;

} SYController; // size = 0x0A

typedef struct ControllerInfo
{
    /* 0x00 */ u16 unk00; // contpad button
    /* 0x02 */ u16 unk02; // new button presses
    /* 0x04 */ u16 unk04; // pressed buttons ( |= .02)
    /* 0x06 */ u16 unk06; // moved new presses? buttons to handle?
    /* 0x08 */ u16 unk08; // all buttons? ( |= .06)
    /* 0x0A */ u16 unk0A; // button releases
    /* 0x0C */ u16 unk0C; // released buttons ( |= .0A)
    /* 0x0E */ s8 unk0E;  // stick x
    /* 0x0F */ s8 unk0F;  // stick y
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18; // some sort of countdown
    /* 0x1C */ u8 unk1C;  // cont status errno
    /* 0x1D */ u8 unk1D;  // cont status status

} ControllerInfo; // size = 0x20

typedef enum ContEventType
{
    CONT_EVENT_READ_CONT_DATA = 1,
    CONT_EVENT_UPDATE_GLOBAL_DATA,
    CONT_EVENT_UNK_3,
    CONT_EVENT_SCHEDULE_READ_CONT_DATA,
    CONT_EVENT_MOTOR,
    CONT_EVENT_SET_STATUS_DELAY

} ContEventType;

typedef struct ControllerEvent
{
    /* 0x00 */ ContEventType type;
    /* 0x04 */ OSMesg mesg;
    /* 0x08 */ OSMesgQueue *cbQueue;

} ControllerEvent; // size = 0x0C

typedef struct ContEvtType3
{
    /* 0x00 */ ControllerEvent evt;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;

} ContEvtType3; // size = 0x14

typedef struct ContSchedReadEvt
{
    /* 0x00 */ ControllerEvent evt;
    /* 0x0C */ s32 scheduleRead;

} ContSchedReadEvt; // size = 0x10

typedef enum MotorCmd
{
    MOTOR_CMD_INIT,
    MOTOR_CMD_START,
    MOTOR_CMD_STOP

} MotorCmd;

typedef struct ContMotorEvt
{
    /* 0x00 */ ControllerEvent evt;
    /* 0x0C */ s32 contID;
    /* 0x10 */ MotorCmd cmd;

} ContMotorEvt; // size = 0x14

typedef struct ContStatusDelayEvt
{
    /* 0x00 */ ControllerEvent evt;
    /* 0x0C */ s32 delay;

} ContStatusDelayEvt; // size = 0x10

typedef struct Unk80045268
{
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04; // event?
    /* 0x08 */ s32 unk08; // controller number
    /* 0x0C */ OSMesgQueue *unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;

} Unk80045268; // size = 0x18

extern u32 gSYControllerConnectedNum;
extern sb32 sSYControllerIsUpdateData;
extern s8 gSYControllerDeviceStatuses[MAXCONTROLLERS];
extern SYController gSYControllerDevices[MAXCONTROLLERS];
extern SYController gSYControllerMain;

extern void syControllerScheduleRead(void);
extern void syControllerFuncRead(void);
extern void syControllerSetAutoRead(s32 shouldSchedule);
extern void syControllerSetStatusDelay(s32 delay);
extern void syControllerStartRumble(s32 arg0);
extern void syControllerStopRumble(s32 arg0);
extern void syControllerInitRumble(s32 arg0);
extern void syControllerThreadMain(void *arg);

#endif /* SYS_THREAD_6_H */
