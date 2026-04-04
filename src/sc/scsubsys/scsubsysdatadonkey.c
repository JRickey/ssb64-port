#include <ft/fttypes.h>
#include <sc/scsubsys/scsubsys.h>
#include <reloc_data.h>

s32 D_ovl1_80391340[] =
{
	ftMotionPlayFGM(nSYAudioVoiceDonkeySmash1),
	ftMotionCommandEnd()
};

s32 D_ovl1_80391348[] =
{
	ftMotionCommandEnd()
};

s32 D_ovl1_8039134C[] =
{
	ftMotionCommandEnd()
};

s32 D_ovl1_80391350[] =
{
	0xA0800001,
	0xA0500001,
	ftMotionCommandEnd()
};

FTMotionDesc dFTDonkeySubMotionDescs[] =
{
    &ll_800_FileID, 0x80000000, 0x00000000,
    &ll_381_FileID, D_ovl1_80391340, 0x00000000,
    &ll_382_FileID, D_ovl1_80391348, 0x00000000,
    &ll_383_FileID, 0x80000000, 0x00000000,
    &ll_384_FileID, D_ovl1_8039134C, 0x00000000,
    &ll_385_FileID, D_ovl1_80391350, 0x00000000,
    &ll_806_FileID, 0x80000000, 0x00000000,
    0x00000000,   0x80000000, 0x00000000,
    &ll_386_FileID, 0x80000000, 0x80000000,
    &ll_387_FileID, 0x80000000, 0x40000000,
    &ll_388_FileID, 0x80000000, 0x40000000,
    &ll_389_FileID, 0x80000000, 0x80000000,
    &ll_390_FileID, 0x80000000, 0x00000000,
    &ll_391_FileID, 0x80000000, 0x00000000,
    &ll_392_FileID, 0x80000000, 0x00000000
};

s32 dFTDonkeySubMotionDescsCount = sizeof(dFTDonkeySubMotionDescs)/sizeof(FTMotionDesc); // 0x0000000F
