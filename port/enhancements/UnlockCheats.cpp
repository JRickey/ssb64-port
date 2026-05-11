#include "enhancements.h"

#include <libultraship/bridge/consolevariablebridge.h>

extern "C" {
    int port_cheat_unlock_all()        { return CVarGetInteger("gCheats.UnlockAll", 0); }
    int port_cheat_unlock_luigi()      { return CVarGetInteger("gCheats.UnlockLuigi", 0); }
    int port_cheat_unlock_ness()       { return CVarGetInteger("gCheats.UnlockNess", 0); }
    int port_cheat_unlock_captain()    { return CVarGetInteger("gCheats.UnlockCaptain", 0); }
    int port_cheat_unlock_purin()      { return CVarGetInteger("gCheats.UnlockPurin", 0); }
    int port_cheat_unlock_inishie()    { return CVarGetInteger("gCheats.UnlockInishie", 0); }
    int port_cheat_unlock_soundtest()  { return CVarGetInteger("gCheats.UnlockSoundTest", 0); }
    int port_cheat_unlock_itemswitch() { return CVarGetInteger("gCheats.UnlockItemSwitch", 0); }
}
