#include <ft/fttypes.h>
#include <sc/scsubsys/scsubsys.h>
#include <reloc_data.h>

s32 D_ovl1_80392240[] =
{
	0xAC000006,
	ftMotionCommandWait(2),
	0xAC000000,
	ftMotionCommandReturn()
};

s32 D_ovl1_80392250[] =
{
	ftMotionCommandSubroutine(D_ovl1_80392240),
	ftMotionCommandWait(20),
	ftMotionCommandSubroutine(D_ovl1_80392240),
	ftMotionCommandWait(90),
	ftMotionCommandSubroutine(D_ovl1_80392240),
	ftMotionCommandWait(16),
	ftMotionCommandSubroutine(D_ovl1_80392240),
	ftMotionCommandWait(30),
	ftMotionCommandSubroutine(D_ovl1_80392240),
	ftMotionCommandWait(100),
	ftMotionCommandGoto(D_ovl1_80392250)
};

s32 D_ovl1_80392294[] =
{
	ftMotionCommandEnd()
};

s32 D_ovl1_80392298[] =
{
	ftMotionCommandEnd()
};

s32 D_ovl1_8039229C[] =
{
	ftMotionPlayFGM(nSYAudioVoicePikachuFuraSleep),
	0xAC000006,
	0x00000000,
	0x00000000
};

s32 D_ovl1_803922AC[] =
{
	0xAC100006,
	ftMotionCommandWaitAsync(20),
	ftMotionCommandSubroutine(D_ovl1_80392240),
	ftMotionCommandWaitAsync(45),
	0xAC100000,
	ftMotionCommandWaitAsync(80),
	ftMotionCommandSubroutine(D_ovl1_80392240),
	ftMotionCommandWaitAsync(95),
	0xAC100006,
	ftMotionCommandWaitAsync(125),
	ftMotionCommandSubroutine(D_ovl1_80392240),
	ftMotionCommandWaitAsync(145),
	0xAC100000,
	ftMotionCommandEnd()
};

FTMotionDesc dFTPikachuSubMotionDescs[] =
{
	&ll_1957_FileID, D_ovl1_80392250, 0x00000000,
	&ll_476_FileID, D_ovl1_80392294, 0x80000000,
	&ll_477_FileID, D_ovl1_80392298, 0x00000000,
	&ll_478_FileID, D_ovl1_8039229C, 0x00000000,
	&ll_478_FileID, D_ovl1_8039229C, 0x00000000,
	&ll_479_FileID, 0x80000000, 0x00000000,
	&ll_1963_FileID, 0x80000000, 0x00000000,
	0x00000000, 0x80000000, 0x00000000,
	&ll_480_FileID, 0x80000000, 0x80000000,
	&ll_481_FileID, 0x80000000, 0x40000000,
	&ll_482_FileID, 0x80000000, 0x40000000,
	&ll_484_FileID, 0x80000000, 0x00000000,
	&ll_485_FileID, 0x80000000, 0x00000000,
	&ll_486_FileID, 0x80000000, 0x00000000,
	&ll_487_FileID, 0x80000000, 0x00000000,
	&ll_483_FileID, D_ovl1_803922AC, 0x00000000
};

s32 dFTPikachuSubMotionDescsCount = sizeof(dFTPikachuSubMotionDescs)/sizeof(FTMotionDesc); // 0x00000010
