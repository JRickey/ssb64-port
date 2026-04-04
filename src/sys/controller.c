#include "controller.h"

#include <sys/main.h>
#include <sys/scheduler.h>
#include <missing_libultra.h>
#include <PR/os.h>

// 0x800450F0
OSMesgQueue sSYControllerInitMesgQueue; // Queue for OS controller Init, Status, and Read

// 0x80045108
OSMesg sSYControllerInitMesg[1]; // Message buffer for OS controller Init, Status, and Read

// 0x80045110
SYClient sSYControllerClient;

// 0x80045118
OSMesg sSYControllerEventMesgs[7]; // used in MqList sSYControllerClient

// 0x80045138
OSMesgQueue sSYControllerEventMesgQueue; // queue for ControllerEvent callbacks

// 0x80045150
OSMesg sSYControllerDeviceMesgs[MAXCONTROLLERS];

// 0x80045160
OSMesgQueue sSYControllerDeviceMesgQueue; // controller mesgqueue? for waiting for 0 to 1+ controllers?

// 0x80045178
OSContStatus sSYControllerDeviceStatuses[MAXCONTROLLERS];

// 0x80045188
OSContPad sSYControllerData[MAXCONTROLLERS];

// 0x800451A0
u32 gSYControllerConnectedNum;

// 0x800451A4
s8 gSYControllerDeviceStatuses[MAXCONTROLLERS];

// 0x800451A8
ControllerInfo sSYControllerDescs[MAXCONTROLLERS];

// 0x80045228
SYController gSYControllerDevices[MAXCONTROLLERS];

// 0x80045250
sb32 sSYControllerIsUpdateData;

// 0x80045254
ControllerEvent *sSYControllerWaitUpdate;

// 0x80045258
sb32 sSYControllerIsReadDeviceData; // If TRUE, always update controller data when a controller loop runs

// 0x8004525C - Number of events (or tics?) to wait until perfomring OSContStatus update
s32 sSYControllerStatusUpdateWait;

// 0x80045260 - Remaining count of controller events (or tics) until OSContStatus update
s32 sSYControllerEventsRemainNum;

// 0x80045268
Unk80045268 D_80045268[MAXCONTROLLERS];

// 0x800452C8
OSPfs sSYControllerMotorPfs[MAXCONTROLLERS];

// 0x80003C00
void syControllerUpdateDeviceIndexes(void)
{
    s32 controllers = 0;
    s32 i;

    for (i = 0; i < MAXCONTROLLERS; i++)
    {
        if (sSYControllerDescs[i].unk1C == 0)
        {
            gSYControllerDeviceStatuses[controllers++] = i;
        }
    }
    gSYControllerConnectedNum = controllers;

    for (i = controllers; i < MAXCONTROLLERS; i++)
    {
        gSYControllerDeviceStatuses[i] = -1;
    }
}

// 0x80003CC4
void syControllerUpdateDeviceStatuses(void)
{
    s32 i;

    osContStartQuery(&sSYControllerInitMesgQueue);
    osRecvMesg(&sSYControllerInitMesgQueue, NULL, OS_MESG_BLOCK);
    osContGetQuery(sSYControllerDeviceStatuses);

    for (i = 0; i != MAXCONTROLLERS; i++)
    {
        if ((sSYControllerDeviceStatuses[i].errno == 0) && (sSYControllerDeviceStatuses[i].status & CONT_CARD_ON))
        {
            if (!(sSYControllerDescs[i].unk1D & CONT_CARD_ON) || (sSYControllerDescs[i].unk1C != 0))
            {
                osMotorInit(&sSYControllerInitMesgQueue, &sSYControllerMotorPfs[i], i);
            }
        }
        sSYControllerDescs[i].unk1C = sSYControllerDeviceStatuses[i].errno;
        sSYControllerDescs[i].unk1D = sSYControllerDeviceStatuses[i].status;
    }
}

// 0x80003DD4
void syControllerReadDeviceData(void)
{
    s32 i;

    osContStartReadData(&sSYControllerInitMesgQueue);
    osRecvMesg(&sSYControllerInitMesgQueue, NULL, OS_MESG_BLOCK);
    osContGetReadData(sSYControllerData);

    for (i = 0; i != MAXCONTROLLERS; i++)
    {
        if (!sSYControllerData[i].errno && (sSYControllerDeviceStatuses[i].status & CONT_CARD_ON) && sSYControllerDescs[i].unk1C)
        {
            osMotorInit(&sSYControllerInitMesgQueue, &sSYControllerMotorPfs[i], i);
        }
        sSYControllerDescs[i].unk1C = sSYControllerData[i].errno;

        if (sSYControllerDescs[i].unk1C == 0)
        {
            sSYControllerDescs[i].unk02 = (sSYControllerData[i].button ^ sSYControllerDescs[i].unk00) & sSYControllerData[i].button;
            sSYControllerDescs[i].unk0A = (sSYControllerData[i].button ^ sSYControllerDescs[i].unk00) & sSYControllerDescs[i].unk00;

            if (sSYControllerData[i].button ^ sSYControllerDescs[i].unk00)
            {
                // there is a new press
                sSYControllerDescs[i].unk06 = sSYControllerDescs[i].unk02;
                sSYControllerDescs[i].unk18 = sSYControllerDescs[i].unk10;
            }
            else
            {
                // no new button presses
                sSYControllerDescs[i].unk18--;

                if (sSYControllerDescs[i].unk18 > 0)
                {
                    sSYControllerDescs[i].unk06 = 0;
                }
                else
                {
                    sSYControllerDescs[i].unk06 = sSYControllerData[i].button;
                    sSYControllerDescs[i].unk18 = sSYControllerDescs[i].unk14;
                }
            }
            sSYControllerDescs[i].unk00 = sSYControllerData[i].button;
            sSYControllerDescs[i].unk0E = sSYControllerData[i].stick_x;
            sSYControllerDescs[i].unk0F = sSYControllerData[i].stick_y;
            sSYControllerDescs[i].unk04 |= sSYControllerDescs[i].unk02;
            sSYControllerDescs[i].unk0C |= sSYControllerDescs[i].unk0A;
            sSYControllerDescs[i].unk08 |= sSYControllerDescs[i].unk06;
        }
    }
    sSYControllerIsUpdateData = TRUE;
}

// 0x80003F98
void syControllerUpdateGlobalData(void)
{
    s32 i;
    SYController *players, *sys = &gSYControllerMain;

    for (i = 0; i < (ARRAY_COUNT(sSYControllerDescs) + ARRAY_COUNT(gSYControllerDevices)) / 2; i++)
    {
        if (sSYControllerDescs[i].unk1C == FALSE)
        {
            gSYControllerDevices[i].button_hold = sSYControllerDescs[i].unk00;
            gSYControllerDevices[i].button_tap = sSYControllerDescs[i].unk04;
            gSYControllerDevices[i].button_release = sSYControllerDescs[i].unk0C;
            gSYControllerDevices[i].button_update = sSYControllerDescs[i].unk08;
            gSYControllerDevices[i].stick_range.x = sSYControllerDescs[i].unk0E;
            gSYControllerDevices[i].stick_range.y = sSYControllerDescs[i].unk0F;

            sSYControllerDescs[i].unk04 = sSYControllerDescs[i].unk08 = sSYControllerDescs[i].unk0C = 0;
        }
    }
    syControllerUpdateDeviceIndexes();

    players = &gSYControllerDevices[gSYControllerDeviceStatuses[0]];

    sys->button_tap = players->button_tap;
    sys->button_hold = players->button_hold;
    sys->button_update = players->button_update;
    sys->button_release = players->button_release;
    sys->stick_range.x = players->stick_range.x;
    sys->stick_range.y = players->stick_range.y;

    sSYControllerIsUpdateData = FALSE;
}

void syControllerInitDevices(void)
{
    s32 i;
    u8 pattern;

    osCreateMesgQueue(&sSYControllerInitMesgQueue, sSYControllerInitMesg, ARRAY_COUNT(sSYControllerInitMesg));
    osSetEventMesg(OS_EVENT_SI, &sSYControllerInitMesgQueue, (OSMesg)1);
    osContInit(&sSYControllerInitMesgQueue, &pattern, sSYControllerDeviceStatuses);

    for (i = 0; i < (ARRAY_COUNT(sSYControllerDeviceStatuses) + ARRAY_COUNT(sSYControllerMotorPfs)) / 2; i++)
    {
        if (sSYControllerDeviceStatuses[i].status & CONT_CARD_ON)
        {
            osMotorInit(&sSYControllerInitMesgQueue, &sSYControllerMotorPfs[i], i);
        }
    }
    osCreateMesgQueue(&sSYControllerDeviceMesgQueue, sSYControllerDeviceMesgs, ARRAY_COUNT(sSYControllerDeviceMesgs));

    for (i = 0; i < MAXCONTROLLERS; i++)
    {
        D_80045268[i].unk08 = i;
        D_80045268[i].unk00 = 0;
        D_80045268[i].unk04 = 5; // OS_EVENT_SI?
        D_80045268[i].unk0C = &sSYControllerDeviceMesgQueue;
    }
    for (i = 0; i < (ARRAY_COUNT(gSYControllerDevices) + ARRAY_COUNT(sSYControllerDescs) + ARRAY_COUNT(sSYControllerData)) / 3; i++)
    {
        sSYControllerData[i].button = 0;

        sSYControllerDescs[i].unk06 = 0;
        sSYControllerDescs[i].unk02 = 0;
        sSYControllerDescs[i].unk00 = 0;
        sSYControllerDescs[i].unk0F = 0;
        sSYControllerDescs[i].unk18 = 30;
        sSYControllerDescs[i].unk10 = 30;
        sSYControllerDescs[i].unk14 = 5;

        sSYControllerDescs[i].unk0E = sSYControllerDescs[i].unk0F;
        sSYControllerDescs[i].unk1C = sSYControllerDeviceStatuses[i].errno;
        sSYControllerDescs[i].unk1D = sSYControllerDeviceStatuses[i].status;

        gSYControllerDevices[i].button_hold = gSYControllerDevices[i].button_tap = gSYControllerDevices[i].button_update = 0;
        gSYControllerDevices[i].stick_range.x = gSYControllerDevices[i].stick_range.y = 0;
    }
    syControllerUpdateDeviceIndexes();
    sSYControllerIsUpdateData    = FALSE;
    sSYControllerWaitUpdate = NULL;
    sSYControllerIsReadDeviceData      = TRUE;
    sSYControllerEventsRemainNum = sSYControllerStatusUpdateWait = 1;
}

// 0x80004284
void syControllerDispatchEvent(ControllerEvent *evt)
{
    OSMesg mesg[1];
    OSMesgQueue queue;

    osCreateMesgQueue(&queue, mesg, ARRAY_COUNT(mesg));
    evt->mesg    = (OSMesg)1;
    evt->cbQueue = &queue;

    osSendMesg(&sSYControllerEventMesgQueue, (OSMesg)evt, OS_MESG_NOBLOCK);
    osRecvMesg(&queue, NULL, OS_MESG_BLOCK);
}

void syControllerScheduleRead(void)
{
    ControllerEvent evt;

    evt.type = CONT_EVENT_READ_CONT_DATA;
    syControllerDispatchEvent(&evt);
}

void syControllerFuncRead(void)
{
    ControllerEvent evt;

    evt.type = CONT_EVENT_UPDATE_GLOBAL_DATA;
    syControllerDispatchEvent(&evt);
}

void unref_80004338(s32 arg0, s32 arg1)
{
    ContEvtType3 data;

    data.evt.type = CONT_EVENT_UNK_3;
    data.unk0C    = arg0;
    data.unk10    = arg1;
    syControllerDispatchEvent(&data.evt);
}

// 0x80004368
void syControllerSetAutoRead(sb32 is_scheduled)
{
    ContSchedReadEvt data;

    data.evt.type     = CONT_EVENT_SCHEDULE_READ_CONT_DATA;
    data.scheduleRead = is_scheduled;
    syControllerDispatchEvent(&data.evt);
}

// 0x80004394
void syControllerSetStatusDelay(s32 delay)
{
    ContStatusDelayEvt data;

    data.evt.type = CONT_EVENT_SET_STATUS_DELAY;
    data.delay    = delay;
    syControllerDispatchEvent(&data.evt);
}

// 0x800043C0
void syControllerUpdateRumbleEvent(s32 port, s32 ev_kind)
{
    s32 i;

    for (i = 0; i < MAXCONTROLLERS; i++)
    {
        if (D_80045268[i].unk00 == 0)
        {
            break;
        }
    }
    if (i == MAXCONTROLLERS)
    {
        osRecvMesg(&sSYControllerDeviceMesgQueue, (OSMesg*)&i, OS_MESG_BLOCK);
    }
    else D_80045268[i].unk00 = 1;
    
    D_80045268[i].unk10 = port;
    D_80045268[i].unk14 = ev_kind;
    osSendMesg(&sSYControllerEventMesgQueue, (OSMesg)&D_80045268[i].unk04, OS_MESG_NOBLOCK);
}

// 0x80004474
void syControllerStartRumble(s32 port)
{
    syControllerUpdateRumbleEvent(port, 1);
}

// 0x80004494
void syControllerStopRumble(s32 port)
{
    syControllerUpdateRumbleEvent(port, 2);
}

// 0x800044B4
void syControllerInitRumble(s32 port)
{
    syControllerUpdateRumbleEvent(port, 0);
}

// 0x800044D4
void syControllerParseEvent(ControllerEvent *evt)
{
    switch (evt->type)
    {
        case CONT_EVENT_READ_CONT_DATA:
        {
            syControllerReadDeviceData();
            syControllerUpdateGlobalData();

            if (evt->cbQueue != NULL)
            {
                osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
            }
            break;
        }
        case CONT_EVENT_UPDATE_GLOBAL_DATA:
        {
            if (sSYControllerIsUpdateData != FALSE)
            {
                syControllerUpdateGlobalData();
                if (evt->cbQueue != NULL)
                {
                    osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
                }
            }
            else sSYControllerWaitUpdate = evt;
            
            break;
        }
        case CONT_EVENT_UNK_3:
        {
            s32 i;
            for (i = 0; i < MAXCONTROLLERS; i++)
            {
                sSYControllerDescs[i].unk10 = ((ContEvtType3*)evt)->unk0C;
                sSYControllerDescs[i].unk14 = ((ContEvtType3*)evt)->unk10;
            }
            if (evt->cbQueue != NULL)
            {
                osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
            }
            break;
        }
        case CONT_EVENT_SCHEDULE_READ_CONT_DATA:
        {
            sSYControllerIsReadDeviceData = ((ContSchedReadEvt*)evt)->scheduleRead;
            if (evt->cbQueue != NULL)
            {
                osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
            }
            break;
        }
        case CONT_EVENT_SET_STATUS_DELAY:
        {
            sSYControllerStatusUpdateWait = ((ContStatusDelayEvt*)evt)->delay;
            if (evt->cbQueue != NULL)
            {
                osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
            }
            break;
        }
        case CONT_EVENT_MOTOR:
        {
            // why
            if
            (
                !(sSYControllerDescs[((ContMotorEvt *)evt)->contID].unk1C) &&
                (sSYControllerDescs[((ContMotorEvt *)evt)->contID].unk1D & CONT_CARD_ON)
            )
            {
                s32 id     = ((ContMotorEvt *)evt)->contID;
                OSPfs *pfs = &sSYControllerMotorPfs[id];

                switch (((ContMotorEvt *)evt)->cmd)
                {
                case MOTOR_CMD_INIT: osMotorInit(&sSYControllerInitMesgQueue, pfs, id);
                    break;

                case MOTOR_CMD_START:
                    if (!gSYSchedulerIsSoftReset)
                    {
                         osMotorStart(pfs);
                    }
                    break;

                case MOTOR_CMD_STOP: osMotorStop(pfs);
                    break;
                }
            }
            if (evt->cbQueue != NULL)
            {
                osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
            }
            break;
        }
    }
}

void syControllerThreadMain(void *unused)
{
    OSMesg mesg;

    syControllerInitDevices();
    sySchedulerAddClient(&sSYControllerClient, &sSYControllerEventMesgQueue, sSYControllerEventMesgs, ARRAY_COUNT(sSYControllerEventMesgs));
    osSendMesg(&gSYMainThreadingMesgQueue, (OSMesg)1, OS_MESG_NOBLOCK);

    while (TRUE)
    {
        osRecvMesg(&sSYControllerEventMesgQueue, &mesg, OS_MESG_BLOCK);

        if ((intptr_t)mesg == 1)
        {
            if (sSYControllerEventsRemainNum != 0)
            {
                sSYControllerEventsRemainNum--;
            }
            if (sSYControllerEventsRemainNum == 0)
            {
                syControllerUpdateDeviceStatuses();
                sSYControllerEventsRemainNum = sSYControllerStatusUpdateWait;
            }
            if (sSYControllerIsReadDeviceData == FALSE)
            {
                continue;
            }
            else syControllerReadDeviceData();

            if (sSYControllerWaitUpdate == NULL)
            {
                continue;
            }
            else syControllerUpdateGlobalData();

            if (sSYControllerWaitUpdate->cbQueue != NULL)
            {
                osSendMesg(sSYControllerWaitUpdate->cbQueue, sSYControllerWaitUpdate->mesg, OS_MESG_NOBLOCK);
            }
            sSYControllerWaitUpdate = NULL;
        }
        else syControllerParseEvent((ControllerEvent*)mesg);
    }
}
