#include <ft/fttypes.h>
#include <sc/scsubsys/scsubsys.h>
#include <reloc_data.h>

s32 D_ovl1_80392480[] =
{
	0xAC000004,
	0xAC100004,
	ftMotionCommandWait(2),
	0xAC000005,
	0xAC100005,
	ftMotionCommandWait(4),
	0xAC000004,
	0xAC100004,
	ftMotionCommandWait(2),
	0xAC000000,
	0xAC100000,
	ftMotionCommandWait(1),
	ftMotionCommandReturn()
};

s32 D_ovl1_803924B4[] =
{
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandWait(90),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandWait(10),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandWait(80),
	ftMotionCommandGoto(D_ovl1_803924B4)
};

s32 D_ovl1_803924E0[] =
{
	ftMotionCommandWaitAsync(0),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandWaitAsync(30),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandWaitAsync(60),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandEnd()
};

s32 D_ovl1_80392508[] =
{
	ftMotionCommandWaitAsync(8),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandWaitAsync(32),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandWaitAsync(62),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandWaitAsync(92),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandWaitAsync(120),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandEnd()
};

s32 D_ovl1_80392548[] =
{
	0xAC000005,
	0xAC100005,
	ftMotionCommandWaitAsync(95),
	0xAC000004,
	0xAC100004,
	ftMotionCommandWaitAsync(108),
	0xAC000000,
	0xAC100000,
	ftMotionCommandWaitAsync(110),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandWaitAsync(120),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandEnd()
};

s32 D_ovl1_80392584[] =
{
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandWait(80),
	ftMotionCommandSubroutine(D_ovl1_80392480),
	ftMotionCommandWait(10),
	ftMotionCommandEnd()
};

FTMotionDesc dFTPurinSubMotionDescs[] =
{
	&ll_1492_FileID, D_ovl1_803924B4, 0x00000000,
	&ll_469_FileID, D_ovl1_803924E0, 0x00000000,
	&ll_470_FileID, D_ovl1_80392508, 0x00000000,
	&ll_471_FileID, D_ovl1_80392548, 0x00000000,
	&ll_471_FileID, D_ovl1_80392548, 0x00000000,
	&ll_472_FileID, D_ovl1_80392584, 0x00000000,
	0x00000000, 0x80000000, 0x00000000,
	0x00000000, 0x80000000, 0x00000000,
	0x00000000, 0x80000000, 0x00000000,
	&ll_473_FileID, 0x80000000, 0x40000000,
	&ll_474_FileID, 0x80000000, 0x40000000,
	0x00000000, 0x80000000, 0x00000000,
	0x00000000, 0x80000000, 0x00000000,
	&ll_475_FileID, 0x80000000, 0x00000000,
	0x00000000, 0x80000000, 0x00000000
};

s32 dFTPurinSubMotionDescsCount = sizeof(dFTPurinSubMotionDescs)/sizeof(FTMotionDesc); // 0x0000000F
