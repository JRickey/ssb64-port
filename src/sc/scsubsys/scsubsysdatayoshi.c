#include <ft/fttypes.h>
#include <sc/scsubsys/scsubsys.h>
#include <reloc_data.h>

s32 D_ovl1_80391B90[] =
{
	0xAC000004,
	0xAC100004,
	ftMotionCommandWait(2),
	0xAC000005,
	0xAC100005,
	ftMotionCommandWait(4),
	0xAC000004,
	0xAC100004,
	ftMotionCommandWait(3),
	0xAC000000,
	0xAC100000,
	ftMotionCommandWait(1),
	ftMotionCommandReturn()
};

s32 D_ovl1_80391BC4[] =
{
	ftMotionCommandSubroutine(D_ovl1_80391B90),
	ftMotionCommandWait(80),
	ftMotionCommandSubroutine(D_ovl1_80391B90),
	ftMotionCommandWait(20),
	ftMotionCommandSubroutine(D_ovl1_80391B90),
	ftMotionCommandWait(80),
	ftMotionCommandGoto(D_ovl1_80391BC4)
};

s32 D_ovl1_80391BF0[] =
{
	ftMotionCommandWaitAsync(40),
	0xAC00000A,
	0xAC10000A,
	ftMotionCommandWaitAsync(100),
	ftMotionPlayFGM(nSYAudioVoiceYoshiAppeal),
	ftMotionCommandEnd()
};

s32 D_ovl1_80391C08[] =
{
	0xAC000009,
	0xAC100009,
	ftMotionCommandWaitAsync(75),
	0xAC00000A,
	0xAC10000A,
	ftMotionCommandWaitAsync(120),
	ftMotionPlayFGM(nSYAudioVoiceYoshiAppeal),
	ftMotionCommandEnd()
};

s32 D_ovl1_80391C28[] =
{
	0xAC000006,
	0xAC100006,
	ftMotionCommandWaitAsync(15),
	ftMotionPlayFGM(nSYAudioFGMLightSwingS),
	ftMotionPlayFGM(nSYAudioVoiceYoshiSmash1),
	ftMotionCommandWaitAsync(25),
	ftMotionPlayFGM(nSYAudioFGMLightSwingS),
	ftMotionPlayFGM(nSYAudioVoiceYoshiSmash1),
	ftMotionCommandWaitAsync(55),
	ftMotionPlayFGM(nSYAudioFGMLightSwingS),
	ftMotionPlayFGM(nSYAudioVoiceYoshiSmash2),
	ftMotionCommandWaitAsync(110),
	ftMotionPlayFGM(nSYAudioVoiceYoshiSmash1),
	ftMotionCommandEnd()
};

s32 D_ovl1_80391C60[] =
{
	ftMotionCommandEnd()
};

s32 D_ovl1_80391C64[] =
{
	0xAC000005,
	0xAC100005,
	ftMotionCommandWaitAsync(50),
	0xAC000004,
	0xAC100004,
	ftMotionCommandWaitAsync(90),
	0xAC000005,
	0xAC100005,
	ftMotionCommandWaitAsync(95),
	0xAC000004,
	0xAC100004,
	ftMotionCommandWaitAsync(98),
	0xAC000000,
	0xAC100000,
	ftMotionCommandEnd()
};

FTMotionDesc dFTYoshiSubMotionDescs[] =
{
    &ll_1815_FileID, D_ovl1_80391BC4, 0x00000000,
    &ll_443_FileID,  D_ovl1_80391BF0, 0x00000000,
    &ll_444_FileID,  D_ovl1_80391C08, 0x00000000,
    &ll_445_FileID,  D_ovl1_80391C28, 0x00000000,
    &ll_445_FileID,  D_ovl1_80391C28, 0x00000000,
    &ll_446_FileID,  D_ovl1_80391C60, 0x00000000,
    &ll_1821_FileID, 0x80000000,      0x00000000,
    0x00000000,      0x80000000,      0x00000000,
    &ll_447_FileID,  0x80000000,      0x80000000,
    &ll_448_FileID,  0x80000000,      0x40000000,
    &ll_449_FileID,  0x80000000,      0x40000000,
    &ll_454_FileID,  0x80000000,      0x00000000,
    &ll_455_FileID,  0x80000000,      0x00000000,
    &ll_456_FileID,  0x80000000,      0x00000000,
    &ll_457_FileID,  0x80000000,      0x00000000,
    &ll_450_FileID,  D_ovl1_80391C64, 0x00000000,
    &ll_451_FileID,  0x80000000,      0x00000000,
    &ll_452_FileID,  0x80000000,      0x00000000,
    &ll_453_FileID,  0x80000000,      0x00000000
};

s32 dFTYoshiSubMotionDescsCount = sizeof(dFTYoshiSubMotionDescs)/sizeof(FTMotionDesc); // 0x00000013
