#include <ft/fttypes.h>
#include <sc/scsubsys/scsubsys.h>
#include <reloc_data.h>

s32 D_ovl1_80391110[] =
{
	0xA0880000,
	ftMotionCommandWaitAsync(85),
	0xA08FFFFF,
	0xA0800001,
	ftMotionCommandEnd()
};

s32 D_ovl1_80391124[] =
{
	0xA0880000,
	ftMotionCommandWaitAsync(110),
	ftMotionPlayFGM(nSYAudioVoiceFoxWin),
	ftMotionCommandEnd()
};

s32 D_ovl1_80391134[] =
{
	ftMotionCommandWaitAsync(90),
	ftMotionPlayFGM(nSYAudioVoiceFoxSelected),
	ftMotionCommandEnd()
};

s32 D_ovl1_80391140[] =
{
	0xA0800001,
	0xA0500001,
	ftMotionCommandEnd()
};

s32 D_ovl1_8039114C[] =
{
	0xA0880000,
	ftMotionCommandEnd()
};

FTMotionDesc dFTFoxSubMotionDescs[] =
{
	&ll_642_FileID, 0x80000000, 0x00000000,
	&ll_370_FileID, D_ovl1_80391110, 0x00000000,
	&ll_371_FileID, D_ovl1_80391124, 0x00000000,
	&ll_372_FileID, D_ovl1_80391134, 0x00000000,
	&ll_372_FileID, D_ovl1_80391134, 0x00000000,
	&ll_373_FileID, D_ovl1_80391140, 0x00000000,
	&ll_648_FileID, 0x80000000, 0x00000000,
	0x00000000, 0x80000000, 0x00000000,
	&ll_374_FileID, 0x80000000, 0x80000000,
	&ll_375_FileID, 0x80000000, 0x40000000,
	&ll_376_FileID, 0x80000000, 0x40000000,
	&ll_377_FileID, 0x80000000, 0x00000000,
	&ll_378_FileID, 0x80000000, 0x00000000,
	&ll_379_FileID, D_ovl1_8039114C, 0x00000000,
	&ll_380_FileID, D_ovl1_8039114C, 0x00000000,
	&ll_781_FileID, 0x80000000, 0x00000000,
	&ll_782_FileID, 0x80000000, 0x00000000,
	&ll_783_FileID, 0x80000000, 0x00000000,
	&ll_784_FileID, 0x80000000, 0x00000000,
	&ll_779_FileID, 0x80000000, 0x00000000,
	&ll_790_FileID, 0x80000000, 0x00000000,
	&ll_791_FileID, 0x80000000, 0x00000000,
	&ll_792_FileID, 0x80000000, 0x00000000,
	&ll_793_FileID, 0x80000000, 0x00000000
};
s32 dFTFoxSubMotionDescsCount = sizeof(dFTFoxSubMotionDescs)/sizeof(FTMotionDesc); // 0x00000018
