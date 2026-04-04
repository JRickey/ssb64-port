#include <ft/fighter.h>

typedef struct scSubsysUnkACUnk8
{
    void **unk_subsysAC_8_0x0[4];

} scSubsysUnkACUnk8;

typedef struct scSubsysUnkAC
{
    u8 filler_0x0[0x8];
    scSubsysUnkACUnk8 *unk_subsysAC_0x8;

} scSubsysUnkAC;

typedef struct scSubsysUnk10
{
    u8 filler_0x0[0x18];
    DObj *unk_subsys10_0x18;

} scSubsysUnk10;

typedef struct scSubsysUnkUD
{
    u8 filler_0x0[0x8];
    u8 unk_subsysud_0x8;
    u8 unk_subsysud_0x9;
    s32 unk_subsysud_0xC;
    DObj *unk_subsysud_0x10[7];
    u8 filler_0x2C[0xAC - 0x2C];
    scSubsysUnkAC *unk_subsysud_0xAC;

} scSubsysUnkUD;

// This function isn't even called, ever. Stinky.
void func_ovl1_803903E0(GObj *gobj, s32 arg1)
{
    s32 i;
    DObj *temp_s0;
    scSubsysUnkUD *user_data;
    DObj **var_s4;
    void **var_s1;
    void **var_s2;

    user_data = gobj->user_data.p;
    var_s1 = user_data->unk_subsysud_0xAC->unk_subsysAC_0x8[user_data->unk_subsysud_0x8 - 1].unk_subsysAC_8_0x0[1];
    var_s2 = user_data->unk_subsysud_0xAC->unk_subsysAC_0x8[user_data->unk_subsysud_0x8 - 1].unk_subsysAC_8_0x0[2];
    user_data->unk_subsysud_0x9 = arg1;
    
    for (i = 4; i < 37; i++)
    {
        temp_s0 = user_data->unk_subsysud_0x10[i + 2];
        
        if (temp_s0 != NULL)
        {
            gcRemoveMObjAll(temp_s0);

            lbCommonAddMObjForFighterPartsDObj(temp_s0, (var_s1 != NULL) ? *var_s1 : NULL, (var_s2 != NULL) ? *var_s2 : NULL, NULL, user_data->unk_subsysud_0x9);
        }
        if (var_s1 != NULL)
        {
            var_s1++;
        }
        if (var_s2 != NULL) 
        {
            var_s2++;
        }
    }
}
