#include <sys/obj.h>
#include <sys/controller.h>

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x803929F0
s32 sSCSubsysPad0x803929F0;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80390700
sb32 scSubsysControllerCheckConnected(s32 player)
{
    s32 i;
    
    for (i = 0; i < ARRAY_COUNT(gSYControllerDeviceStatuses); i++)
    {
        if (player == gSYControllerDeviceStatuses[i])
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 0x8039076C - Get first controller pressing expected button(s)
s32 scSubsysControllerGetPlayerTapButtons(u32 buttons)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gSYControllerDevices); i++)
    {
        if ((scSubsysControllerCheckConnected(i) != FALSE) && (gSYControllerDevices[i].button_tap & buttons)) 
        {
            return i + 1;
        }
    }
    return 0;
}

// 0x80390804
s32 scSubsysControllerGetPlayerHoldButtons(u32 buttons)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gSYControllerDevices); i++)
    {
        if ((scSubsysControllerCheckConnected(i) != FALSE) && (gSYControllerDevices[i].button_hold & buttons)) 
        {
            return i + 1;
        }
    }
    return 0;
}

// 0x8039089C
s32 scSubsysControllerGetPlayerStickLR(s8 range, sb32 right_or_left)
{
    s32 i;
    
    for (i = 0; i < ARRAY_COUNT(gSYControllerDevices); i++)
    {
        if (scSubsysControllerCheckConnected(i) != FALSE)
        {
            if (right_or_left != 0)
            {
                if (range < gSYControllerDevices[i].stick_range.x)
                {
                    return gSYControllerDevices[i].stick_range.x;
                }
            }
            else if (range > gSYControllerDevices[i].stick_range.x)
            {
                return gSYControllerDevices[i].stick_range.x;
            }
        }
    }
    return 0;
}

// 0x80390950
s32 scSubsysControllerGetPlayerStickUD(s8 range, sb32 up_or_down)
{
    s32 i;
    
    for (i = 0; i < ARRAY_COUNT(gSYControllerDevices); i++)
    {
        if (scSubsysControllerCheckConnected(i) != FALSE)
        {
            if (up_or_down != 0)
            {
                if (range < gSYControllerDevices[i].stick_range.y)
                {
                    return gSYControllerDevices[i].stick_range.y;
                }
            }
            else if (range > gSYControllerDevices[i].stick_range.y)
            {
                return gSYControllerDevices[i].stick_range.y;
            }
        }
    }
    return 0;
}

// 0x80390A04
sb32 scSubsysControllerGetPlayerStickInRangeLR(s32 range_l_min, s32 range_r_min)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gSYControllerDevices); i++)
    {
        if (scSubsysControllerCheckConnected(i) != FALSE)
        {
            sb32 is_out_range;
            
            if
            (
                (gSYControllerDevices[i].stick_range.x < range_l_min) || 
                (gSYControllerDevices[i].stick_range.x > range_r_min)
            )
            {
                is_out_range = TRUE;
            }
            else is_out_range = FALSE;
            
            if (is_out_range != FALSE)
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

// 0x80390AC0
sb32 scSubsysControllerGetPlayerStickInRangeUD(s32 range_d_min, s32 range_u_min)
{
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gSYControllerDevices); i++)
    {
        if (scSubsysControllerCheckConnected(i) != FALSE)
        {
            sb32 is_out_range;
            
            if
            (
                (gSYControllerDevices[i].stick_range.y < range_d_min) || 
                (gSYControllerDevices[i].stick_range.y > range_u_min)
            )
            {
                is_out_range = TRUE;
            }
            else is_out_range = FALSE;
            
            if (is_out_range != FALSE)
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

// 0x80390B7C
sb32 scSubsysControllerCheckNoInputAll(void)
{
    if
    (
        (scSubsysControllerGetPlayerStickInRangeLR(-20, 20) != FALSE) && 
        (scSubsysControllerGetPlayerStickInRangeUD(-20, 20) != FALSE) && 
        (
            scSubsysControllerGetPlayerHoldButtons
            (
                A_BUTTON   | B_BUTTON   | START_BUTTON | 
                L_TRIG     | R_TRIG     | Z_TRIG       |
                U_CBUTTONS | L_CBUTTONS | R_CBUTTONS   | D_CBUTTONS |
                U_JPAD     | L_JPAD     | R_JPAD       | D_JPAD
            ) 
            == 
            0
        )
    )
    {
        return TRUE;
    }
    else return FALSE;
}
