#include <ft/fttypes.h>
#include <sc/scsubsys/scsubsys.h>
#include <reloc_data.h>

s32 D_ovl1_803914F0[] =
{
	ftMotionCommandWaitAsync(19),
	0x98807C00,
	0x003C0000,
	0x00000000,
	0x00000000,
	ftMotionPlayFGM(nSYAudioFGMLightSwingS),
	ftMotionCommandWaitAsync(46),
	0x98807C00,
	0x003C0000,
	0x00000000,
	0x00000000,
	ftMotionPlayFGM(nSYAudioFGMLightSwingS),
	ftMotionCommandWaitAsync(79),
	ftMotionPlayFGM(nSYAudioFGMLightSwingS),
	0x80000003,
	0x98807000,
	0x012C0000,
	0x00000000,
	0x00000000,
	ftMotionPlayFGM(nSYAudioFGMFireShoot1),
	ftMotionCommandWait(16),
	0x84000000,
	ftMotionCommandEnd()
};

s32 D_ovl1_8039154C[] =
{
	ftMotionPlayFGM(nSYAudioFGMLightSwingS),
	ftMotionCommandWaitAsync(75),
	0x98807C00,
	0x003C0000,
	0x00000000,
	0x00000000,
	ftMotionPlayFGM(nSYAudioVoiceSamusSmash1),
	ftMotionCommandEnd()
};

s32 D_ovl1_8039156C[] =
{
	ftMotionCommandWaitAsync(100),
	ftMotionPlayFGM(nSYAudioFGMBladeDraw),
	ftMotionCommandEnd()
};

s32 D_ovl1_80391578[] =
{
	0xA0500001,
	ftMotionCommandEnd()
};

FTMotionDesc dFTSamusSubMotionDescs[] =
{
    &ll_953_FileID, 0x80000000, 0x00000000,
    &ll_393_FileID, D_ovl1_803914F0, 0x00000000,
    &ll_394_FileID, D_ovl1_8039154C, 0x00000000,
    &ll_395_FileID, D_ovl1_8039156C, 0x00000000,
    &ll_395_FileID, D_ovl1_8039156C, 0x00000000,
    &ll_396_FileID, D_ovl1_80391578, 0x00000000,
    &ll_959_FileID, 0x80000000, 0x00000000,
    0x00000000,   0x80000000, 0x00000000,
    &ll_397_FileID, 0x80000000, 0x80000000,
    &ll_398_FileID, 0x80000000, 0x40000000,
    &ll_399_FileID, 0x80000000, 0x40000000,
    &ll_400_FileID, 0x80000000, 0x00000000,
    &ll_401_FileID, 0x80000000, 0x00000000,
    &ll_402_FileID, 0x80000000, 0x00000000,
    &ll_403_FileID, 0x80000000, 0x00000000
};
s32 dFTSamusSubMotionDescsCount = sizeof(dFTSamusSubMotionDescs)/sizeof(FTMotionDesc); // 0x0000000F
