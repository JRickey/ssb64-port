#include <ssb_types.h>

void unref_80023840(u8 *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4)
{
    arg0[0x18] = arg1;
    arg0[0x19] = arg2;
    arg0[0x1A] = arg3;
    arg0[0x1B] = arg4;
}
