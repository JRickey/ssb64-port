#include <ft/fighter.h>
#ifdef PORT
extern void port_log(const char *fmt, ...);
#endif

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80115B10
void ftKeyProcessKeyEvents(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTComputerInput *cp = &fp->input.cp;
    FTKey *key = &fp->key;

    if (key->script != NULL)
    {
        if (key->input_wait != 0)
        {
            key->input_wait--;
        }
        while (TRUE)
        {
            if ((key->script == NULL) || (key->input_wait > 0))
            {
                break;
            }
            else key->input_wait = key->script->command.param;

            switch (key->script->command.opcode)
            {
            case nFTKeyEventEnd:
                key->script = NULL;
                break;

            case nFTKeyEventButton:
                key->script++;

                cp->button_inputs = ftKeyGetButtons(key->script);
#ifdef PORT
                port_log("SSB64: ftKey fkind=%d BUTTON=0x%04X wait=%d\n",
                    fp->fkind, cp->button_inputs, key->input_wait);
#endif

                key->script++;
                break;

            case nFTKeyEventStick:
                key->script++;

                cp->stick_range.x = ftKeyGetStickRange(key->script)->x;
                cp->stick_range.y = ftKeyGetStickRange(key->script)->y;
#ifdef PORT
                port_log("SSB64: ftKey fkind=%d STICK x=%d y=%d wait=%d raw=0x%04X\n",
                    fp->fkind, cp->stick_range.x, cp->stick_range.y,
                    key->input_wait, key->script->halfword);
#endif

                key->script++;
                break;
            }
        }
    }
}
