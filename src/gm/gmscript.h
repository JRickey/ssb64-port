#ifndef _GMSCRIPT_H_
#define _GMSCRIPT_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <ft/ftdef.h>

typedef struct GMColEventDefault
{
	u32 opcode : 6;
	u32 value1 : 26;

} GMColEventDefault;

typedef struct GMColEventGoto1
{
	u32 opocode : 6;

} GMColEventGoto1;

typedef struct GMColEventGoto2
{
	void* p_goto;

} GMColEventGoto2;

typedef struct GMColEventGoto
{
	GMColEventGoto1 s1;
	GMColEventGoto2 s2;

} GMColEventGoto;

typedef struct GMColEventLoopBegin
{
	u32 opcode : 6;
	u32 loop_count : 26;

} GMColEventLoopBegin;

typedef struct GMColEventSubroutine1
{
	u32 opcode : 6;

} GMColEventSubroutine1;

typedef struct GMColEventSubroutine2
{
	void* p_subroutine;

} GMColEventSubroutine2;

typedef struct GMColEventSubroutine
{
	GMColEventSubroutine1 s1;
	GMColEventSubroutine2 s2;

} GMColEventSubroutine;

typedef struct GMColEventParallel1
{
	u32 opcode : 6;

} GMColEventParallel1;

typedef struct GMColEventParallel2
{
	void* p_script;

} GMColEventParallel2;

typedef struct GMColEventParallel
{
	GMColEventParallel1 s1;
	GMColEventParallel2 s2;

} GMColEventParallel;

typedef struct GMColEventSetRGBA1
{
	u32 opcode : 6;

} GMColEventSetRGBA1;

typedef struct GMColEventSetRGBA2
{
	u32 r : 8;
	u32 g : 8;
	u32 b : 8;
	u32 a : 8;

} GMColEventSetRGBA2;

typedef struct GMColEventSetRGBA
{
	GMColEventSetRGBA1 s1;
	GMColEventSetRGBA2 s2;

} GMColEventSetRGBA;

typedef struct GMColEventBlendRGBA1
{
	u32 opcode : 6;
	u32 blend_frames : 26;

} GMColEventBlendRGBA1;

typedef struct GMColEventBlendRGBA2
{
	u32 r : 8;
	u32 g : 8;
	u32 b : 8;
	u32 a : 8;

} GMColEventBlendRGBA2;

typedef struct GMColEventBlendRGBA
{
	GMColEventBlendRGBA1 s1;
	GMColEventBlendRGBA2 s2;

} GMColEventBlendRGBA;

typedef struct GMColEventMakeEffect1
{
	u32 opcode : 6;
	s32 joint_id : 7;
	u32 effect_id : 9;
	u32 flag : 10;

} GMColEventMakeEffect1;

typedef struct GMColEventMakeEffect2
{
	s32 off_x : 16;
	s32 off_y : 16;

} GMColEventMakeEffect2;

typedef struct GMColEventMakeEffect3
{
	s32 off_z : 16;
	s32 rng_x : 16;

} GMColEventMakeEffect3;

typedef struct GMColEventMakeEffect4
{
	s32 rng_y : 16;
	s32 rng_z : 16;

} GMColEventMakeEffect4;

typedef struct GMColEventMakeEffect
{
	GMColEventMakeEffect1 s1;
	GMColEventMakeEffect2 s2;
	GMColEventMakeEffect3 s3;
	GMColEventMakeEffect4 s4;

} GMColEventMakeEffect;

typedef struct GMColEventSetLight
{
	u32 opcode : 6;
	s32 light1 : 13;
	s32 light2 : 13;

} GMColEventSetLight;

typedef struct GMColEventPlayFGM
{
	u32 opcode : 6;
	u32 sfx_id : 26;

} GMColEventPlayFGM;

typedef union GMColEventAll
{
	GMColEventDefault ca_base;
	GMColEventGoto1 ca_goto1;
	GMColEventGoto2 ca_goto2;
	GMColEventLoopBegin ca_loopstart;
	GMColEventSubroutine1 ca_sub1;
	GMColEventSubroutine2 ca_sub2;
	GMColEventParallel1 ca_par1;
	GMColEventParallel2 ca_par2;
	GMColEventSetRGBA1 ca_rgba1;
	GMColEventSetRGBA2 ca_rgba2;
	GMColEventBlendRGBA1 ca_blend1;
	GMColEventBlendRGBA2 ca_blend2;
	GMColEventMakeEffect1 ca_effect1;
	GMColEventMakeEffect2 ca_effect2;
	GMColEventMakeEffect3 ca_effect3;
	GMColEventMakeEffect4 ca_effect4;
	GMColEventSetLight ca_light;
	GMColEventPlayFGM ca_sfx;

} GMColEventAll;

// // ColAnim Script 0 is NULL

// // 0x8012CA90 - ColAnim Script 1
// GMColEventSetRGBA     GMColEvents_Col1_Ev0 = {
// nGMColEventSetColor1, 0xFF, 0xFF, 0xFF, 0x30 }; GMColEventDefault
// GMColEvents_Col1_Ev1 = { nGMColEventWait, 65535 }; GMColEventGoto
// GMColEvents_Col1_Ev2 = { nGMColEventGoto, &GMColEvents_Col1_Ev0 };

// // 0x8012CAA4 - ColAnim Script 2
// GMColEventDefault     GMColEvents_Col2_Ev0 = {
// nGMColEventClearColorAll }; GMColEventDefault
// GMColEvents_Col2_Ev1 = { nGMColEventEnd };

// // 0x8012CAAC - ColAnim Script 3
// GMColEventSetRGBA     GMColEvents_Col3_Ev0 = {
// nGMColEventSetColor1, 0xFF, 0xFF, 0xFF, 0x82 }; GMColEventBlendRGBA
// GMColEvents_Col3_Ev1 = { nGMColEventBlendColor1, 3, 0xFF, 0xFF,
// 0xFF, 0x32 }; GMColEventDefault     GMColEvents_Col3_Ev2 = {
// nGMColEventWait, 3 }; GMColEventDefault     GMColEvents_Col3_Ev3
// = { nGMColEventClearColorAll }; GMColEventDefault
// GMColEvents_Col3_Ev4 = { nGMColEventWait, 1 }; GMColEventGoto
// GMColEvents_Col3_Ev5 = { nGMColEventGoto, &GMColEvents_Col3_Ev0
// };

// // 0x8012CAD0 - ColAnim Script 4
// GMColEventSetRGBA     GMColEvents_Col4_Ev0 = {
// nGMColEventSetColor1, 0x80, 0xFF, 0x80, 0x50 }; GMColEventBlendRGBA
// GMColEvents_Col4_Ev1 = { nGMColEventBlendColor1, 3, 0x80, 0xFF,
// 0x80, 0x14 }; GMColEventDefault     GMColEvents_Col4_Ev2 = {
// nGMColEventWait, 3 }; GMColEventDefault     GMColEvents_Col4_Ev3
// = { nGMColEventClearColorAll }; GMColEventDefault
// GMColEvents_Col4_Ev4 = { nGMColEventWait, 1 }; GMColEventGoto
// GMColEvents_Col4_Ev5 = { nGMColEventGoto, &GMColEvents_Col4_Ev0
// };

// // 0x8012CAF4 - ColAnim Script 5
// GMColEventSetLight    GMColEvents_Col5_Ev0 = {
// nGMColEventSetLight, 90, 0 }; GMColEventDefault
// GMColEvents_Col5_Ev1 = { nGMColEventClearColorAll };
// GMColEventDefault     GMColEvents_Col5_Ev2 = { nGMColEventWait, 1
// }; GMColEventSetRGBA     GMColEvents_Col5_Ev3 = {
// nGMColEventSetColor1, 0xFF, 0xFF, 0xFF, 0xE6 }; GMColEventDefault
// GMColEvents_Col5_Ev4 = { nGMColEventWait, 1 }; GMColEventBlendRGBA
// GMColEvents_Col5_Ev5 = { nGMColEventBlendColor1, 6, 0xFF, 0xFF,
// 0xFF, 0x1E }; GMColEventDefault     GMColEvents_Col5_Ev6 = {
// nGMColEventWait, 6 }; GMColEventDefault     GMColEvents_Col5_Ev7
// = { nGMColEventClearColorAll }; GMColEventDefault
// GMColEvents_Col5_Ev8 = { nGMColEventEnd };

// // Next: 8012CB20


typedef struct GMRumbleEventDefault
{
	u16 opcode : 3;
	u16 param : 13;

} GMRumbleEventDefault;

typedef struct gmRumbleEvent
{
	u8 rumble_id;
	ub8 is_rumble_active;
	u16 rumble_status;
	u16 loop_count;
	s32 rumble_timer;
	void* p_goto;
	GMRumbleEventDefault* p_script;

} gmRumbleEvent;

typedef struct GMRumbleLink
{
	gmRumbleEvent* p_event;
	struct GMRumbleLink* rnext;
	struct GMRumbleLink* rprev;

} GMRumbleLink;

typedef struct GMRumblePlayer
{
	ub8 is_active;
	GMRumbleLink* rlink;

} GMRumblePlayer;

#endif
