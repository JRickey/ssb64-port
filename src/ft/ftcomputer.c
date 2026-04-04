#include <ft/fighter.h>
#include <wp/weapon.h>
#include <it/item.h>
#include <gr/ground.h>
#include <sc/scene.h>

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x80186660
u8 dFTComputerAttackUnknown[/* */] =
{
    0x08, 0x08, 0x0B, 0x08,
    0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x0B, 0x08,
    0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x0B, 0x08,
    0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08
};

// 0x80186684
FTComputerAttack dFTComputerAttacksMario[/* */] =
{
    // Attack11
    {
        nFTComputerInputStickNButtonA,                      // Input script index
        2,                                                  // First frame of hitbox
        6,                                                  // Last frame of hitbox
        150.0F,                                             // Horizontal range nearest
        400.0F,                                             // Horizontal range farthest
        30.0F,                                              // Vertical range nearest
        210.0F                                              // Vertical range farthest
    },

    // AttackS3
    {
        nFTComputerInputStickTiltAutoXButtonA,             // Input script index
        8,                                                  // First frame of hitbox
        17,                                                 // Last frame of hitbox
        45.0F,                                              // Horizontal range nearest
        560.0F,                                             // Horizontal range farthest
        -45.0F,                                             // Vertical range nearest
        270.0F                                              // Vertical range farthest
    },

    // AttackS4
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        16,                                                 // First frame of hitbox
        20,                                                 // Last frame of hitbox
        60.0F,                                              // Horizontal range nearest
        580.0F,                                             // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        280.0F                                              // Vertical range farthest
    },

    // AttackHi3
    {
        nFTComputerInputStickTiltHiButtonA,                // Input script index
        5,                                                  // First frame of hitbox
        16,                                                 // Last frame of hitbox
        -140.0F,                                            // Horizontal range nearest
        470.0F,                                             // Horizontal range farthest
        120.0F,                                             // Vertical range nearest
        640.0F                                              // Vertical range farthest
    },

    // AttackHi4
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        7,                                                  // First frame of hitbox
        15,                                                 // Last frame of hitbox
        -230.0F,                                            // Horizontal range nearest
        360.0F,                                             // Horizontal range farthest
        140.0F,                                             // Vertical range nearest
        630.0F                                              // Vertical range farthest
    },

    // AttackLw3
    {
        nFTComputerInputStickTiltLwButtonA,                // Input script index
        5,                                                  // First frame of hitbox
        11,                                                 // Last frame of hitbox
        130.0F,                                             // Horizontal range nearest
        720.0F,                                             // Horizontal range farthest
        -90.0F,                                             // Vertical range nearest
        170.0F                                              // Vertical range farthest
    },

    // AttackLw4
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        9,                                                  // First frame of hitbox
        30,                                                 // Last frame of hitbox
        -420.0F,                                            // Horizontal range nearest
        490.0F,                                             // Horizontal range farthest
        -130.0F,                                            // Vertical range nearest
        400.0F                                              // Vertical range farthest
    },

    // SpecialN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        10,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        400.0F,                                             // Horizontal range nearest
        1200.0F,                                            // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        280.0F                                              // Vertical range farthest
    },

    // SpecialHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        2,                                                  // First frame of hitbox
        23,                                                 // Last frame of hitbox
        -120.0F,                                            // Horizontal range nearest
        950.0F,                                             // Horizontal range farthest
        -120.0F,                                            // Vertical range nearest
        2100.0F                                             // Vertical range farthest
    },

    // SpecialLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        1,                                                  // First frame of hitbox
        44,                                                 // Last frame of hitbox
        -250.0F,                                            // Horizontal range nearest
        250.0F,                                             // Horizontal range farthest
        -70.0F,                                             // Vertical range nearest
        500.0F                                              // Vertical range farthest
    },

    // Catch
    {
        nFTComputerInputStickNButtonZButtonA,              // Input script index
        7,                                                  // First frame of hitbox
        7,                                                  // Last frame of hitbox
        50.0F,                                              // Horizontal range nearest
        340.0F,                                             // Horizontal range farthest
        65.0F,                                              // Vertical range nearest
        355.0F                                              // Vertical range farthest
    },

    // End of Ground Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // AttackAirN
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        3,                                                  // First frame of hitbox
        36,                                                 // Last frame of hitbox
        -192.0F,                                            // Horizontal range nearest
        201.0F,                                             // Horizontal range farthest
        -30.0F,                                             // Vertical range nearest
        280.0F                                              // Vertical range farthest
    },

    // AttackAirF / AttackAirB
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        10,                                                 // First frame of hitbox
        19,                                                 // Last frame of hitbox
        -443.0F,                                            // Horizontal range nearest
        147.0F,                                             // Horizontal range farthest
        -5.0F,                                              // Vertical range nearest
        381.0F                                              // Vertical range farthest
    },

    // AttackAirHi
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        2,                                                  // First frame of hitbox
        11,                                                 // Last frame of hitbox
        -273.0F,                                            // Horizontal range nearest
        459.0F,                                             // Horizontal range farthest
        -3.0F,                                              // Vertical range nearest
        634.0F                                              // Vertical range farthest
    },

    // AttackAirLw
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        11,                                                 // First frame of hitbox
        33,                                                 // Last frame of hitbox
        -60.0F,                                             // Horizontal range nearest
        438.0F,                                             // Horizontal range farthest
        -153.0F,                                            // Vertical range nearest
        404.0F                                              // Vertical range farthest
    },

    // SpecialAirN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        10,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        400.0F,                                             // Horizontal range nearest
        1200.0F,                                            // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        280.0F                                              // Vertical range farthest
    },

    // SpecialAirHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        2,                                                  // First frame of hitbox
        23,                                                 // Last frame of hitbox
        -334.0F,                                            // Horizontal range nearest
        431.0F,                                             // Horizontal range farthest
        -124.0F,                                            // Vertical range nearest
        540.0F                                              // Vertical range farthest
    },

    // SpecialAirLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        1,                                                  // First frame of hitbox
        46,                                                 // Last frame of hitbox
        -250.0F,                                            // Horizontal range nearest
        250.0F,                                             // Horizontal range farthest
        -90.0F,                                             // Vertical range nearest
        470.0F                                              // Vertical range farthest
    },

    // End of Aerial Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    }
};

// 0x801868B4
FTComputerAttack dFTComputerAttacksFox[/* */] =
{
    // Attack11
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        3,                                                  // First frame of hitbox
        4,                                                  // Last frame of hitbox
        180.0F,                                             // Horizontal range nearest
        590.0F,                                             // Horizontal range farthest
        140.0F,                                             // Vertical range nearest
        355.0F                                              // Vertical range farthest
    },

    // AttackS3
    {
        nFTComputerInputStickTiltAutoXButtonA,             // Input script index
        3,                                                  // First frame of hitbox
        8,                                                  // Last frame of hitbox
        180.0F,                                             // Horizontal range nearest
        590.0F,                                             // Horizontal range farthest
        140.0F,                                             // Vertical range nearest
        355.0F                                              // Vertical range farthest
    },

    // AttackS4
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        12,                                                 // First frame of hitbox
        24,                                                 // Last frame of hitbox
        280.0F,                                             // Horizontal range nearest
        1020.0F,                                            // Horizontal range farthest
        35.0F,                                              // Vertical range nearest
        460.0F                                              // Vertical range farthest
    },

    // AttackHi3
    {
        nFTComputerInputStickTiltHiButtonA,                // Input script index
        6,                                                  // First frame of hitbox
        13,                                                 // Last frame of hitbox
        -90.0F,                                             // Horizontal range nearest
        270.0F,                                             // Horizontal range farthest
        185.0F,                                             // Vertical range nearest
        750.0F                                              // Vertical range farthest
    },

    // AttackHi4
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        6,                                                  // First frame of hitbox
        21,                                                 // Last frame of hitbox
        -390.0F,                                            // Horizontal range nearest
        425.0F,                                             // Horizontal range farthest
        60.0F,                                              // Vertical range nearest
        790.0F                                              // Vertical range farthest
    },

    // AttackLw3
    {
        nFTComputerInputStickTiltLwButtonA,                // Input script index
        6,                                                  // First frame of hitbox
        9,                                                  // Last frame of hitbox
        135.0F,                                             // Horizontal range nearest
        750.0F,                                             // Horizontal range farthest
        -145.0F,                                            // Vertical range nearest
        155.0F                                              // Vertical range farthest
    },

    // AttackLw4
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        7,                                                  // First frame of hitbox
        11,                                                 // Last frame of hitbox
        -465.0F,                                            // Horizontal range nearest
        470.0F,                                             // Horizontal range farthest
        -100.0F,                                            // Vertical range nearest
        180.0F                                              // Vertical range farthest
    },

    // SpecialN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        5,                                                  // First frame of hitbox
        10,                                                 // Last frame of hitbox
        400.0F,                                             // Horizontal range nearest
        1200.0F,                                            // Horizontal range farthest
        35.0F,                                              // Vertical range nearest
        460.0F                                              // Vertical range farthest
    },

    // SpecialHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        43,                                                 // First frame of hitbox
        72,                                                 // Last frame of hitbox
        -40.0F,                                             // Horizontal range nearest
        140.0F,                                             // Horizontal range farthest
        350.0F,                                             // Vertical range nearest
        2500.0F                                             // Vertical range farthest
    },

    // SpecialLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        1,                                                  // First frame of hitbox
        1,                                                  // Last frame of hitbox
        -180.0F,                                            // Horizontal range nearest
        180.0F,                                             // Horizontal range farthest
        60.0F,                                              // Vertical range nearest
        420.0F                                              // Vertical range farthest
    },

    // Catch
    {
        nFTComputerInputStickNButtonZButtonA,              // Input script index
        7,                                                  // First frame of hitbox
        7,                                                  // Last frame of hitbox
        200.0F,                                             // Horizontal range nearest
        460.0F,                                             // Horizontal range farthest
        110.0F,                                             // Vertical range nearest
        370.0F                                              // Vertical range farthest
    },

    // End of Ground Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // AttackAirN
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        4,                                                  // First frame of hitbox
        28,                                                 // Last frame of hitbox
        -105.0F,                                            // Horizontal range nearest
        322.0F,                                             // Horizontal range farthest
        59.0F,                                              // Vertical range nearest
        411.0F                                              // Vertical range farthest
    },

    // AttackAirF / AttackAirB
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        4,                                                  // First frame of hitbox
        27,                                                 // Last frame of hitbox
        -466.0F,                                            // Horizontal range nearest
        299.0F,                                             // Horizontal range farthest
        53.0F,                                              // Vertical range nearest
        418.0F                                              // Vertical range farthest
    },

    // AttackAirHi
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        6,                                                  // First frame of hitbox
        13,                                                 // Last frame of hitbox
        -273.0F,                                            // Horizontal range nearest
        268.0F,                                             // Horizontal range farthest
        -158.0F,                                            // Vertical range nearest
        704.0F                                              // Vertical range farthest
    },

    // AttackAirLw
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        5,                                                  // First frame of hitbox
        24,                                                 // Last frame of hitbox
        -132.0F,                                            // Horizontal range nearest
        276.0F,                                             // Horizontal range farthest
        -90.0F,                                             // Vertical range nearest
        329.0F                                              // Vertical range farthest
    },

    // SpecialAirN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        5,                                                  // First frame of hitbox
        10,                                                 // Last frame of hitbox
        400.0F,                                             // Horizontal range nearest
        1200.0F,                                            // Horizontal range farthest
        35.0F,                                              // Vertical range nearest
        460.0F                                              // Vertical range farthest
    },

    // SpecialAirHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        43,                                                 // First frame of hitbox
        72,                                                 // Last frame of hitbox
        -144.0F,                                            // Horizontal range nearest
        43.0F,                                              // Horizontal range farthest
        234.0F,                                             // Vertical range nearest
        418.0F                                              // Vertical range farthest
    },

    // SpecialAirLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        1,                                                  // First frame of hitbox
        1,                                                  // Last frame of hitbox
        -180.0F,                                            // Horizontal range nearest
        180.0F,                                             // Horizontal range farthest
        60.0F,                                              // Vertical range nearest
        420.0F                                              // Vertical range farthest
    },

    // End of Aerial Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    }
};

// 0x80186AE4
FTComputerAttack dFTComputerAttacksDonkey[/* */] =
{
    // Attack11
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        5,                                                  // First frame of hitbox
        8,                                                  // Last frame of hitbox
        170.0F,                                             // Horizontal range nearest
        700.0F,                                             // Horizontal range farthest
        10.0F,                                              // Vertical range nearest
        450.0F                                              // Vertical range farthest
    },

    // AttackS3
    {
        nFTComputerInputStickTiltAutoXButtonA,             // Input script index
        5,                                                  // First frame of hitbox
        8,                                                  // Last frame of hitbox
        170.0F,                                             // Horizontal range nearest
        700.0F,                                             // Horizontal range farthest
        10.0F,                                              // Vertical range nearest
        450.0F                                              // Vertical range farthest
    },

    // AttackS4
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        27,                                                 // First frame of hitbox
        32,                                                 // Last frame of hitbox
        220.0F,                                             // Horizontal range nearest
        1060.0F,                                            // Horizontal range farthest
        50.0F,                                              // Vertical range nearest
        465.0F                                              // Vertical range farthest
    },

    // AttackHi3
    {
        nFTComputerInputStickTiltHiButtonA,                // Input script index
        4,                                                  // First frame of hitbox
        23,                                                 // Last frame of hitbox
        -950.0F,                                            // Horizontal range nearest
        535.0F,                                             // Horizontal range farthest
        200.0F,                                             // Vertical range nearest
        965.0F                                              // Vertical range farthest
    },

    // AttackHi4
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        16,                                                 // First frame of hitbox
        18,                                                 // Last frame of hitbox
        -275.0F,                                            // Horizontal range nearest
        255.0F,                                             // Horizontal range farthest
        485.0F,                                             // Vertical range nearest
        985.0F                                              // Vertical range farthest
    },

    // AttackLw3
    {
        nFTComputerInputStickTiltLwButtonA,                // Input script index
        11,                                                 // First frame of hitbox
        16,                                                 // Last frame of hitbox
        190.0F,                                             // Horizontal range nearest
        760.0F,                                             // Horizontal range farthest
        -70.0F,                                             // Vertical range nearest
        230.0F                                              // Vertical range farthest
    },

    // AttackLw4
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        13,                                                 // First frame of hitbox
        32,                                                 // Last frame of hitbox
        -580.0F,                                            // Horizontal range nearest
        635.0F,                                             // Horizontal range farthest
        -100.0F,                                            // Vertical range nearest
        265.0F                                              // Vertical range farthest
    },

    // SpecialN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        23,                                                 // First frame of hitbox
        45,                                                 // Last frame of hitbox
        -190.0F,                                            // Horizontal range nearest
        1440.0F,                                            // Horizontal range farthest
        10.0F,                                              // Vertical range nearest
        450.0F                                              // Vertical range farthest
    },

    // SpecialHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        3,                                                  // First frame of hitbox
        72,                                                 // Last frame of hitbox
        -515.0F,                                            // Horizontal range nearest
        575.0F,                                             // Horizontal range farthest
        40.0F,                                              // Vertical range nearest
        720.0F                                              // Vertical range farthest
    },

    // SpecialLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        19,                                                 // First frame of hitbox
        30,                                                 // Last frame of hitbox
        -825.0F,                                            // Horizontal range nearest
        1075.0F,                                            // Horizontal range farthest
        -200.0F,                                            // Vertical range nearest
        200.0F                                              // Vertical range farthest
    },

    // Catch
    {
        nFTComputerInputStickNButtonZButtonA,              // Input script index
        7,                                                  // First frame of hitbox
        7,                                                  // Last frame of hitbox
        220.0F,                                             // Horizontal range nearest
        690.0F,                                             // Horizontal range farthest
        155.0F,                                             // Vertical range nearest
        480.0F                                              // Vertical range farthest
    },

    // End of Ground Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // AttackAirN
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        4,                                                  // First frame of hitbox
        29,                                                 // Last frame of hitbox
        -619.0F,                                            // Horizontal range nearest
        438.0F,                                             // Horizontal range farthest
        76.0F,                                              // Vertical range nearest
        691.0F                                              // Vertical range farthest
    },

    // AttackAirF / AttackAirB
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        8,                                                  // First frame of hitbox
        15,                                                 // Last frame of hitbox
        -146.0F,                                            // Horizontal range nearest
        599.0F,                                             // Horizontal range farthest
        -133.0F,                                            // Vertical range nearest
        1019.0F                                             // Vertical range farthest
    },

    // AttackAirHi
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        3,                                                  // First frame of hitbox
        20,                                                 // Last frame of hitbox
        -858.0F,                                            // Horizontal range nearest
        559.0F,                                             // Horizontal range farthest
        102.0F,                                             // Vertical range nearest
        1022.0F                                             // Vertical range farthest
    },

    // AttackAirLw
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        7,                                                  // First frame of hitbox
        30,                                                 // Last frame of hitbox
        -307.0F,                                            // Horizontal range nearest
        127.0F,                                             // Horizontal range farthest
        -310.0F,                                            // Vertical range nearest
        257.0F                                              // Vertical range farthest
    },

    // SpecialAirN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        23,                                                 // First frame of hitbox
        30,                                                 // Last frame of hitbox
        -447.0F,                                            // Horizontal range nearest
        987.0F,                                             // Horizontal range farthest
        158.0F,                                             // Vertical range nearest
        906.0F                                              // Vertical range farthest
    },

    // SpecialAirHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        3,                                                  // First frame of hitbox
        72,                                                 // Last frame of hitbox
        -615.0F,                                            // Horizontal range nearest
        604.0F,                                             // Horizontal range farthest
        23.0F,                                              // Vertical range nearest
        639.0F                                              // Vertical range farthest
    },

    // SpecialAirLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // End of Aerial Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    }
};

// 0x80186D14
FTComputerAttack dFTComputerAttacksSamus[/* */] =
{
    // Attack11
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        4,                                                  // First frame of hitbox
        5,                                                  // Last frame of hitbox
        170.0F,                                             // Horizontal range nearest
        650.0F,                                             // Horizontal range farthest
        240.0F,                                             // Vertical range nearest
        450.0F                                              // Vertical range farthest
    },

    // AttackS3
    {
        nFTComputerInputStickTiltAutoXButtonA,             // Input script index
        4,                                                  // First frame of hitbox
        9,                                                  // Last frame of hitbox
        170.0F,                                             // Horizontal range nearest
        655.0F,                                             // Horizontal range farthest
        200.0F,                                             // Vertical range nearest
        445.0F                                              // Vertical range farthest
    },

    // AttackS4
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        12,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        65.0F,                                              // Horizontal range nearest
        670.0F,                                             // Horizontal range farthest
        200.0F,                                             // Vertical range nearest
        445.0F                                              // Vertical range farthest
    },

    // AttackHi3
    {
        nFTComputerInputStickTiltHiButtonA,                // Input script index
        8,                                                  // First frame of hitbox
        29,                                                 // Last frame of hitbox
        -355.0F,                                            // Horizontal range nearest
        600.0F,                                             // Horizontal range farthest
        -5.0F,                                              // Vertical range nearest
        895.0F                                              // Vertical range farthest
    },

    // AttackHi4
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        17,                                                 // First frame of hitbox
        34,                                                 // Last frame of hitbox
        -535.0F,                                            // Horizontal range nearest
        435.0F,                                             // Horizontal range farthest
        335.0F,                                             // Vertical range nearest
        950.0F                                              // Vertical range farthest
    },

    // AttackLw3
    {
        nFTComputerInputStickTiltLwButtonA,                // Input script index
        8,                                                  // First frame of hitbox
        12,                                                 // Last frame of hitbox
        160.0F,                                             // Horizontal range nearest
        655.0F,                                             // Horizontal range farthest
        -60.0F,                                             // Vertical range nearest
        195.0F                                              // Vertical range farthest
    },

    // AttackLw4
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        9,                                                  // First frame of hitbox
        23,                                                 // Last frame of hitbox
        -525.0F,                                            // Horizontal range nearest
        545.0F,                                             // Horizontal range farthest
        -100.0F,                                            // Vertical range nearest
        270.0F                                              // Vertical range farthest
    },

    // SpecialN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        10,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        500.0F,                                             // Horizontal range nearest
        1500.0F,                                            // Horizontal range farthest
        200.0F,                                             // Vertical range nearest
        445.0F                                              // Vertical range farthest
    },

    // SpecialHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        4,                                                  // First frame of hitbox
        33,                                                 // Last frame of hitbox
        -230.0F,                                            // Horizontal range nearest
        500.0F,                                             // Horizontal range farthest
        -130.0F,                                            // Vertical range nearest
        1860.0F                                             // Vertical range farthest
    },

    // SpecialLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // Catch
    {
        nFTComputerInputStickNButtonZButtonA,              // Input script index
        21,                                                 // First frame of hitbox
        39,                                                 // Last frame of hitbox
        75.0F,                                              // Horizontal range nearest
        1275.0F,                                            // Horizontal range farthest
        135.0F,                                             // Vertical range nearest
        350.0F                                              // Vertical range farthest
    },

    // End of Ground Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // AttackAirN
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        4,                                                  // First frame of hitbox
        27,                                                 // Last frame of hitbox
        -129.0F,                                            // Horizontal range nearest
        308.0F,                                             // Horizontal range farthest
        121.0F,                                             // Vertical range nearest
        488.0F                                              // Vertical range farthest
    },

    // AttackAirF / AttackAirB
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        5,                                                  // First frame of hitbox
        16,                                                 // Last frame of hitbox
        -447.0F,                                            // Horizontal range nearest
        80.0F,                                              // Horizontal range farthest
        -34.0F,                                             // Vertical range nearest
        690.0F                                              // Vertical range farthest
    },

    // AttackAirHi
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        6,                                                  // First frame of hitbox
        19,                                                 // Last frame of hitbox
        -165.0F,                                            // Horizontal range nearest
        346.0F,                                             // Horizontal range farthest
        177.0F,                                             // Vertical range nearest
        703.0F                                              // Vertical range farthest
    },

    // AttackAirLw
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        5,                                                  // First frame of hitbox
        13,                                                 // Last frame of hitbox
        -453.0F,                                            // Horizontal range nearest
        428.0F,                                             // Horizontal range farthest
        -267.0F,                                            // Vertical range nearest
        547.0F                                              // Vertical range farthest
    },

    // SpecialAirN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        10,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        500.0F,                                             // Horizontal range nearest
        1500.0F,                                            // Horizontal range farthest
        200.0F,                                             // Vertical range nearest
        445.0F                                              // Vertical range farthest
    },

    // SpecialAirHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        4,                                                  // First frame of hitbox
        31,                                                 // Last frame of hitbox
        -230.0F,                                            // Horizontal range nearest
        230.0F,                                             // Horizontal range farthest
        -70.0F,                                             // Vertical range nearest
        370.0F                                              // Vertical range farthest
    },

    // SpecialAirLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // End of Aerial Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    }
};

// 0x80186F44
FTComputerAttack dFTComputerAttacksLuigi[/* */] =
{
    // Attack11
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        2,                                                  // First frame of hitbox
        6,                                                  // Last frame of hitbox
        150.0F,                                             // Horizontal range nearest
        400.0F,                                             // Horizontal range farthest
        30.0F,                                              // Vertical range nearest
        210.0F                                              // Vertical range farthest
    },

    // AttackS3
    {
        nFTComputerInputStickTiltAutoXButtonA,             // Input script index
        8,                                                  // First frame of hitbox
        17,                                                 // Last frame of hitbox
        45.0F,                                              // Horizontal range nearest
        560.0F,                                             // Horizontal range farthest
        -45.0F,                                             // Vertical range nearest
        270.0F                                              // Vertical range farthest
    },

    // AttackS4
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        16,                                                 // First frame of hitbox
        20,                                                 // Last frame of hitbox
        60.0F,                                              // Horizontal range nearest
        580.0F,                                             // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        280.0F                                              // Vertical range farthest
    },

    // AttackHi3
    {
        nFTComputerInputStickTiltHiButtonA,                // Input script index
        5,                                                  // First frame of hitbox
        16,                                                 // Last frame of hitbox
        -140.0F,                                            // Horizontal range nearest
        470.0F,                                             // Horizontal range farthest
        120.0F,                                             // Vertical range nearest
        640.0F                                              // Vertical range farthest
    },

    // AttackHi4
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        7,                                                  // First frame of hitbox
        15,                                                 // Last frame of hitbox
        -230.0F,                                            // Horizontal range nearest
        360.0F,                                             // Horizontal range farthest
        140.0F,                                             // Vertical range nearest
        630.0F                                              // Vertical range farthest
    },

    // AttackLw3
    {
        nFTComputerInputStickTiltLwButtonA,                // Input script index
        5,                                                  // First frame of hitbox
        11,                                                 // Last frame of hitbox
        130.0F,                                             // Horizontal range nearest
        720.0F,                                             // Horizontal range farthest
        -90.0F,                                             // Vertical range nearest
        170.0F                                              // Vertical range farthest
    },

    // AttackLw4
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        9,                                                  // First frame of hitbox
        30,                                                 // Last frame of hitbox
        -420.0F,                                            // Horizontal range nearest
        490.0F,                                             // Horizontal range farthest
        -130.0F,                                            // Vertical range nearest
        400.0F                                              // Vertical range farthest
    },

    // SpecialN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        10,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        400.0F,                                             // Horizontal range nearest
        1200.0F,                                            // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        280.0F                                              // Vertical range farthest
    },

    // SpecialHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        2,                                                  // First frame of hitbox
        23,                                                 // Last frame of hitbox
        -120.0F,                                            // Horizontal range nearest
        950.0F,                                             // Horizontal range farthest
        -120.0F,                                            // Vertical range nearest
        2100.0F                                             // Vertical range farthest
    },

    // SpecialLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        1,                                                  // First frame of hitbox
        44,                                                 // Last frame of hitbox
        -250.0F,                                            // Horizontal range nearest
        250.0F,                                             // Horizontal range farthest
        -70.0F,                                             // Vertical range nearest
        500.0F                                              // Vertical range farthest
    },

    // Catch
    {
        nFTComputerInputStickNButtonZButtonA,              // Input script index
        7,                                                  // First frame of hitbox
        7,                                                  // Last frame of hitbox
        50.0F,                                              // Horizontal range nearest
        340.0F,                                             // Horizontal range farthest
        65.0F,                                              // Vertical range nearest
        355.0F                                              // Vertical range farthest
    },

    // End of Ground Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // AttackAirN
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        3,                                                  // First frame of hitbox
        36,                                                 // Last frame of hitbox
        -192.0F,                                            // Horizontal range nearest
        201.0F,                                             // Horizontal range farthest
        -30.0F,                                             // Vertical range nearest
        280.0F                                              // Vertical range farthest
    },

    // AttackAirF / AttackAirB
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        10,                                                 // First frame of hitbox
        19,                                                 // Last frame of hitbox
        -443.0F,                                            // Horizontal range nearest
        147.0F,                                             // Horizontal range farthest
        -5.0F,                                              // Vertical range nearest
        381.0F                                              // Vertical range farthest
    },

    // AttackAirHi
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        2,                                                  // First frame of hitbox
        11,                                                 // Last frame of hitbox
        -273.0F,                                            // Horizontal range nearest
        459.0F,                                             // Horizontal range farthest
        -3.0F,                                              // Vertical range nearest
        634.0F                                              // Vertical range farthest
    },

    // AttackAirLw
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        11,                                                 // First frame of hitbox
        33,                                                 // Last frame of hitbox
        -60.0F,                                             // Horizontal range nearest
        438.0F,                                             // Horizontal range farthest
        -153.0F,                                            // Vertical range nearest
        404.0F                                              // Vertical range farthest
    },

    // SpecialAirN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        10,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        400.0F,                                             // Horizontal range nearest
        1200.0F,                                            // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        280.0F                                              // Vertical range farthest
    },

    // SpecialAirHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        1,                                                  // First frame of hitbox
        23,                                                 // Last frame of hitbox
        -239.0F,                                            // Horizontal range nearest
        449.0F,                                             // Horizontal range farthest
        -109.0F,                                            // Vertical range nearest
        560.0F                                              // Vertical range farthest
    },

    // SpecialAirLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        1,                                                  // First frame of hitbox
        46,                                                 // Last frame of hitbox
        -250.0F,                                            // Horizontal range nearest
        250.0F,                                             // Horizontal range farthest
        10.0F,                                              // Vertical range nearest
        465.0F                                              // Vertical range farthest
    },

    // End of Aerial Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    }
};

// 0x80187174
FTComputerAttack dFTComputerAttacksLink[/* */] =
{
    // Attack11
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        6,                                                  // First frame of hitbox
        9,                                                  // Last frame of hitbox
        -5.0F,                                              // Horizontal range nearest
        595.0F,                                             // Horizontal range farthest
        -25.0F,                                             // Vertical range nearest
        380.0F                                              // Vertical range farthest
    },

    // AttackS3
    {
        nFTComputerInputStickTiltAutoXButtonA,             // Input script index
        15,                                                 // First frame of hitbox
        20,                                                 // Last frame of hitbox
        -340.0F,                                            // Horizontal range nearest
        630.0F,                                             // Horizontal range farthest
        -140.0F,                                            // Vertical range nearest
        755.0F                                              // Vertical range farthest
    },

    // AttackS4
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        16,                                                 // First frame of hitbox
        25,                                                 // Last frame of hitbox
        -610.0F,                                            // Horizontal range nearest
        770.0F,                                             // Horizontal range farthest
        -65.0F,                                             // Vertical range nearest
        885.0F                                              // Vertical range farthest
    },

    // AttackHi3
    {
        nFTComputerInputStickTiltHiButtonA,                // Input script index
        8,                                                  // First frame of hitbox
        16,                                                 // Last frame of hitbox
        -530.0F,                                            // Horizontal range nearest
        660.0F,                                             // Horizontal range farthest
        -140.0F,                                            // Vertical range nearest
        775.0F                                              // Vertical range farthest
    },

    // AttackHi4
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        11,                                                 // First frame of hitbox
        35,                                                 // Last frame of hitbox
        -500.0F,                                            // Horizontal range nearest
        500.0F,                                             // Horizontal range farthest
        90.0F,                                              // Vertical range nearest
        940.0F                                              // Vertical range farthest
    },

    // AttackLw3
    {
        nFTComputerInputStickTiltLwButtonA,                // Input script index
        12,                                                 // First frame of hitbox
        19,                                                 // Last frame of hitbox
        170.0F,                                             // Horizontal range nearest
        750.0F,                                             // Horizontal range farthest
        -50.0F,                                             // Vertical range nearest
        240.0F                                              // Vertical range farthest
    },

    // AttackLw4
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        10,                                                 // First frame of hitbox
        24,                                                 // Last frame of hitbox
        -655.0F,                                            // Horizontal range nearest
        740.0F,                                             // Horizontal range farthest
        -100.0F,                                            // Vertical range nearest
        380.0F                                              // Vertical range farthest
    },

    // SpecialN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        10,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        500.0F,                                             // Horizontal range nearest
        1500.0F,                                            // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        400.0F                                              // Vertical range farthest
    },

    // SpecialHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        8,                                                  // First frame of hitbox
        39,                                                 // Last frame of hitbox
        -775.0F,                                            // Horizontal range nearest
        785.0F,                                             // Horizontal range farthest
        145.0F,                                             // Vertical range nearest
        455.0F                                              // Vertical range farthest
    },

    // SpecialLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // Catch
    {
        nFTComputerInputStickNButtonZButtonA,              // Input script index
        18,                                                 // First frame of hitbox
        29,                                                 // Last frame of hitbox
        220.0F,                                             // Horizontal range nearest
        1205.0F,                                            // Horizontal range farthest
        125.0F,                                             // Vertical range nearest
        365.0F                                              // Vertical range farthest
    },

    // End of Ground Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // AttackAirN
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        4,                                                  // First frame of hitbox
        29,                                                 // Last frame of hitbox
        -195.0F,                                            // Horizontal range nearest
        341.0F,                                             // Horizontal range farthest
        18.0F,                                              // Vertical range nearest
        361.0F                                              // Vertical range farthest
    },

    // AttackAirF / AttackAirB
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        6,                                                  // First frame of hitbox
        26,                                                 // Last frame of hitbox
        -620.0F,                                            // Horizontal range nearest
        6.0F,                                               // Horizontal range farthest
        41.0F,                                              // Vertical range nearest
        439.0F                                              // Vertical range farthest
    },

    // AttackAirHi
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        5,                                                  // First frame of hitbox
        29,                                                 // Last frame of hitbox
        -211.0F,                                            // Horizontal range nearest
        201.0F,                                             // Horizontal range farthest
        366.0F,                                             // Vertical range nearest
        779.0F                                              // Vertical range farthest
    },

    // AttackAirLw
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        6,                                                  // First frame of hitbox
        29,                                                 // Last frame of hitbox
        -122.0F,                                            // Horizontal range nearest
        262.0F,                                             // Horizontal range farthest
        -116.0F,                                            // Vertical range nearest
        294.0F                                              // Vertical range farthest
    },

    // SpecialAirN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        10,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        500.0F,                                             // Horizontal range nearest
        1500.0F,                                            // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        400.0F                                              // Vertical range farthest
    },

    // SpecialAirHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        8,                                                  // First frame of hitbox
        39,                                                 // Last frame of hitbox
        -707.0F,                                            // Horizontal range nearest
        730.0F,                                             // Horizontal range farthest
        190.0F,                                             // Vertical range nearest
        516.0F                                              // Vertical range farthest
    },

    // SpecialAirLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // End of Aerial Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    }
};

// 0x801873A4
FTComputerAttack dFTComputerAttacksYoshi[/* */] =
{
    // Attack11
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        3,                                                  // First frame of hitbox
        5,                                                  // Last frame of hitbox
        15.0F,                                              // Horizontal range nearest
        630.0F,                                             // Horizontal range farthest
        -60.0F,                                             // Vertical range nearest
        445.0F                                              // Vertical range farthest
    },

    // AttackS3
    {
        nFTComputerInputStickTiltAutoXButtonA,             // Input script index
        3,                                                  // First frame of hitbox
        5,                                                  // Last frame of hitbox
        15.0F,                                              // Horizontal range nearest
        630.0F,                                             // Horizontal range farthest
        -60.0F,                                             // Vertical range nearest
        445.0F                                              // Vertical range farthest
    },

    // AttackS4
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        18,                                                 // First frame of hitbox
        24,                                                 // Last frame of hitbox
        155.0F,                                             // Horizontal range nearest
        810.0F,                                             // Horizontal range farthest
        -15.0F,                                             // Vertical range nearest
        425.0F                                              // Vertical range farthest
    },

    // AttackHi3
    {
        nFTComputerInputStickTiltHiButtonA,                // Input script index
        7,                                                  // First frame of hitbox
        10,                                                 // Last frame of hitbox
        -240.0F,                                            // Horizontal range nearest
        435.0F,                                             // Horizontal range farthest
        100.0F,                                             // Vertical range nearest
        710.0F                                              // Vertical range farthest
    },

    // AttackHi4
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        9,                                                  // First frame of hitbox
        15,                                                 // Last frame of hitbox
        -380.0F,                                            // Horizontal range nearest
        425.0F,                                             // Horizontal range farthest
        230.0F,                                             // Vertical range nearest
        910.0F                                              // Vertical range farthest
    },

    // AttackLw3
    {
        nFTComputerInputStickTiltLwButtonA,                // Input script index
        8,                                                  // First frame of hitbox
        10,                                                 // Last frame of hitbox
        130.0F,                                             // Horizontal range nearest
        645.0F,                                             // Horizontal range farthest
        -40.0F,                                             // Vertical range nearest
        245.0F                                              // Vertical range farthest
    },

    // AttackLw4
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        7,                                                  // First frame of hitbox
        23,                                                 // Last frame of hitbox
        -630.0F,                                            // Horizontal range nearest
        685.0F,                                             // Horizontal range farthest
        -100.0F,                                            // Vertical range nearest
        285.0F                                              // Vertical range farthest
    },

    // SpecialN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        18,                                                 // First frame of hitbox
        23,                                                 // Last frame of hitbox
        225.0F,                                             // Horizontal range nearest
        1065.0F,                                            // Horizontal range farthest
        105.0F,                                             // Vertical range nearest
        410.0F                                              // Vertical range farthest
    },

    // SpecialHi
    {
        nFTComputerInputYoshiSpecialHiAim,                 // Input script index
        10,                                                 // First frame of hitbox
        20,                                                 // Last frame of hitbox
        -900.0F,                                            // Horizontal range nearest
        900.0F,                                             // Horizontal range farthest
        800.0F,                                             // Vertical range nearest
        2000.0F                                             // Vertical range farthest
    },

    // SpecialLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        30,                                                 // First frame of hitbox
        41,                                                 // Last frame of hitbox
        630.0F,                                             // Horizontal range nearest
        1085.0F,                                            // Horizontal range farthest
        -25.0F,                                             // Vertical range nearest
        1755.0F                                             // Vertical range farthest
    },

    // Catch
    {
        nFTComputerInputStickNButtonZButtonA,              // Input script index
        16,                                                 // First frame of hitbox
        20,                                                 // Last frame of hitbox
        290.0F,                                             // Horizontal range nearest
        845.0F,                                             // Horizontal range farthest
        40.0F,                                              // Vertical range nearest
        260.0F                                              // Vertical range farthest
    },

    // End of Ground Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // AttackAirN
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        5,                                                  // First frame of hitbox
        30,                                                 // Last frame of hitbox
        -186.0F,                                            // Horizontal range nearest
        258.0F,                                             // Horizontal range farthest
        20.0F,                                              // Vertical range nearest
        356.0F                                              // Vertical range farthest
    },

    // AttackAirF / AttackAirB
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        10,                                                 // First frame of hitbox
        19,                                                 // Last frame of hitbox
        -580.0F,                                            // Horizontal range nearest
        148.0F,                                             // Horizontal range farthest
        34.0F,                                              // Vertical range nearest
        429.0F                                              // Vertical range farthest
    },

    // AttackAirHi
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        9,                                                  // First frame of hitbox
        10,                                                 // Last frame of hitbox
        -171.0F,                                            // Horizontal range nearest
        187.0F,                                             // Horizontal range farthest
        302.0F,                                             // Vertical range nearest
        819.0F                                              // Vertical range farthest
    },

    // AttackAirLw
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        5,                                                  // First frame of hitbox
        29,                                                 // Last frame of hitbox
        -204.0F,                                            // Horizontal range nearest
        254.0F,                                             // Horizontal range farthest
        -53.0F,                                             // Vertical range nearest
        444.0F                                              // Vertical range farthest
    },

    // SpecialAirN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        18,                                                 // First frame of hitbox
        23,                                                 // Last frame of hitbox
        224.0F,                                             // Horizontal range nearest
        1064.0F,                                            // Horizontal range farthest
        108.0F,                                             // Vertical range nearest
        411.0F                                              // Vertical range farthest
    },

    // SpecialAirHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // SpecialAirLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        24,                                                 // First frame of hitbox
        31,                                                 // Last frame of hitbox
        -230.0F,                                            // Horizontal range nearest
        230.0F,                                             // Horizontal range farthest
        -54.0F,                                             // Vertical range nearest
        406.0F                                              // Vertical range farthest
    },

    // End of Aerial Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    }
};

// 0x801875D4
FTComputerAttack dFTComputerAttacksKirby[/* */] =
{
    // Attack11
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        3,                                                  // First frame of hitbox
        4,                                                  // Last frame of hitbox
        110.0F,                                             // Horizontal range nearest
        495.0F,                                             // Horizontal range farthest
        35.0F,                                              // Vertical range nearest
        255.0F                                              // Vertical range farthest
    },

    // AttackS3
    {
        nFTComputerInputStickTiltAutoXButtonA,             // Input script index
        3,                                                  // First frame of hitbox
        4,                                                  // Last frame of hitbox
        110.0F,                                             // Horizontal range nearest
        495.0F,                                             // Horizontal range farthest
        35.0F,                                              // Vertical range nearest
        255.0F                                              // Vertical range farthest
    },

    // AttackS4
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        10,                                                 // First frame of hitbox
        23,                                                 // Last frame of hitbox
        215.0F,                                             // Horizontal range nearest
        1115.0F,                                            // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        330.0F                                              // Vertical range farthest
    },

    // AttackHi3
    {
        nFTComputerInputStickTiltHiButtonA,                // Input script index
        4,                                                  // First frame of hitbox
        11,                                                 // Last frame of hitbox
        -230.0F,                                            // Horizontal range nearest
        480.0F,                                             // Horizontal range farthest
        80.0F,                                              // Vertical range nearest
        750.0F                                              // Vertical range farthest
    },

    // AttackHi4
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        14,                                                 // First frame of hitbox
        23,                                                 // Last frame of hitbox
        -430.0F,                                            // Horizontal range nearest
        480.0F,                                             // Horizontal range farthest
        185.0F,                                             // Vertical range nearest
        880.0F                                              // Vertical range farthest
    },

    // AttackLw3
    {
        nFTComputerInputStickTiltLwButtonA,                // Input script index
        4,                                                  // First frame of hitbox
        10,                                                 // Last frame of hitbox
        105.0F,                                             // Horizontal range nearest
        745.0F,                                             // Horizontal range farthest
        -115.0F,                                            // Vertical range nearest
        245.0F                                              // Vertical range farthest
    },

    // AttackLw4
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        8,                                                  // First frame of hitbox
        27,                                                 // Last frame of hitbox
        -560.0F,                                            // Horizontal range nearest
        565.0F,                                             // Horizontal range farthest
        -140.0F,                                            // Vertical range nearest
        190.0F                                              // Vertical range farthest
    },

    // SpecialN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        25,                                                 // First frame of hitbox
        59,                                                 // Last frame of hitbox
        45.0F,                                              // Horizontal range nearest
        470.0F,                                             // Horizontal range farthest
        80.0F,                                              // Vertical range nearest
        370.0F                                              // Vertical range farthest
    },

    // SpecialHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        23,                                                 // First frame of hitbox
        56,                                                 // Last frame of hitbox
        115.0F,                                             // Horizontal range nearest
        600.0F,                                             // Horizontal range farthest
        55.0F,                                              // Vertical range nearest
        2350.0F                                             // Vertical range farthest
    },

    // SpecialLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // Catch
    {
        nFTComputerInputStickNButtonZButtonA,              // Input script index
        7,                                                  // First frame of hitbox
        7,                                                  // Last frame of hitbox
        175.0F,                                             // Horizontal range nearest
        525.0F,                                             // Horizontal range farthest
        95.0F,                                              // Vertical range nearest
        355.0F                                              // Vertical range farthest
    },

    // End of Ground Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // AttackAirN
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        3,                                                  // First frame of hitbox
        30,                                                 // Last frame of hitbox
        -110.0F,                                            // Horizontal range nearest
        425.0F,                                             // Horizontal range farthest
        -40.0F,                                             // Vertical range nearest
        238.0F                                              // Vertical range farthest
    },

    // AttackAirF / AttackAirB
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        6,                                                  // First frame of hitbox
        25,                                                 // Last frame of hitbox
        -416.0F,                                            // Horizontal range nearest
        86.0F,                                              // Horizontal range farthest
        -49.0F,                                             // Vertical range nearest
        331.0F                                              // Vertical range farthest
    },

    // AttackAirHi
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        10,                                                 // First frame of hitbox
        37,                                                 // Last frame of hitbox
        -291.0F,                                            // Horizontal range nearest
        291.0F,                                             // Horizontal range farthest
        -150.0F,                                            // Vertical range nearest
        309.0F                                              // Vertical range farthest
    },

    // AttackAirLw
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        5,                                                  // First frame of hitbox
        33,                                                 // Last frame of hitbox
        -124.0F,                                            // Horizontal range nearest
        284.0F,                                             // Horizontal range farthest
        -303.0F,                                            // Vertical range nearest
        180.0F                                              // Vertical range farthest
    },

    // SpecialAirN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        25,                                                 // First frame of hitbox
        59,                                                 // Last frame of hitbox
        43.0F,                                              // Horizontal range nearest
        474.0F,                                             // Horizontal range farthest
        81.0F,                                              // Vertical range nearest
        371.0F                                              // Vertical range farthest
    },

    // SpecialAirHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        23,                                                 // First frame of hitbox
        56,                                                 // Last frame of hitbox
        125.0F,                                             // Horizontal range nearest
        625.0F,                                             // Horizontal range farthest
        25.0F,                                              // Vertical range nearest
        525.0F                                              // Vertical range farthest
    },

    // SpecialAirLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        23,                                                 // First frame of hitbox
        30,                                                 // Last frame of hitbox
        -200.0F,                                            // Horizontal range nearest
        200.0F,                                             // Horizontal range farthest
        -200.0F,                                            // Vertical range nearest
        200.0F                                              // Vertical range farthest
    },

    // End of Aerial Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    }
};

// 0x80187804
FTComputerAttack dFTComputerAttacksPikachu[/* */] =
{
    // Attack11
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        2,                                                  // First frame of hitbox
        5,                                                  // Last frame of hitbox
        -10.0F,                                             // Horizontal range nearest
        455.0F,                                             // Horizontal range farthest
        -70.0F,                                             // Vertical range nearest
        235.0F                                              // Vertical range farthest
    },

    // AttackS3
    {
        nFTComputerInputStickTiltAutoXButtonA,             // Input script index
        5,                                                  // First frame of hitbox
        14,                                                 // Last frame of hitbox
        20.0F,                                              // Horizontal range nearest
        490.0F,                                             // Horizontal range farthest
        10.0F,                                              // Vertical range nearest
        375.0F                                              // Vertical range farthest
    },

    // AttackS4
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        22,                                                 // First frame of hitbox
        39,                                                 // Last frame of hitbox
        50.0F,                                              // Horizontal range nearest
        730.0F,                                             // Horizontal range farthest
        -65.0F,                                             // Vertical range nearest
        255.0F                                              // Vertical range farthest
    },

    // AttackHi3
    {
        nFTComputerInputStickTiltHiButtonA,                // Input script index
        5,                                                  // First frame of hitbox
        14,                                                 // Last frame of hitbox
        -360.0F,                                            // Horizontal range nearest
        370.0F,                                             // Horizontal range farthest
        95.0F,                                              // Vertical range nearest
        675.0F                                              // Vertical range farthest
    },

    // AttackHi4
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        10,                                                 // First frame of hitbox
        18,                                                 // Last frame of hitbox
        -460.0F,                                            // Horizontal range nearest
        425.0F,                                             // Horizontal range farthest
        40.0F,                                              // Vertical range nearest
        765.0F                                              // Vertical range farthest
    },

    // AttackLw3
    {
        nFTComputerInputStickTiltLwButtonA,                // Input script index
        6,                                                  // First frame of hitbox
        13,                                                 // Last frame of hitbox
        5.0F,                                               // Horizontal range nearest
        465.0F,                                             // Horizontal range farthest
        -70.0F,                                             // Vertical range nearest
        245.0F                                              // Vertical range farthest
    },

    // AttackLw4
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        11,                                                 // First frame of hitbox
        29,                                                 // Last frame of hitbox
        -475.0F,                                            // Horizontal range nearest
        435.0F,                                             // Horizontal range farthest
        -100.0F,                                            // Vertical range nearest
        360.0F                                              // Vertical range farthest
    },

    // SpecialN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        10,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        400.0F,                                             // Horizontal range nearest
        1200.0F,                                            // Horizontal range farthest
        -70.0F,                                             // Vertical range nearest
        375.0F                                              // Vertical range farthest
    },

    // SpecialHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // SpecialLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        40,                                                 // First frame of hitbox
        48,                                                 // Last frame of hitbox
        -350.0F,                                            // Horizontal range nearest
        350.0F,                                             // Horizontal range farthest
        -65.0F,                                             // Vertical range nearest
        635.0F                                              // Vertical range farthest
    },

    // Catch
    {
        nFTComputerInputStickNButtonZButtonA,              // Input script index
        7,                                                  // First frame of hitbox
        7,                                                  // Last frame of hitbox
        170.0F,                                             // Horizontal range nearest
        455.0F,                                             // Horizontal range farthest
        80.0F,                                              // Vertical range nearest
        370.0F                                              // Vertical range farthest
    },

    // End of Ground Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // AttackAirN
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        3,                                                  // First frame of hitbox
        28,                                                 // Last frame of hitbox
        -247.0F,                                            // Horizontal range nearest
        208.0F,                                             // Horizontal range farthest
        32.0F,                                              // Vertical range nearest
        373.0F                                              // Vertical range farthest
    },

    // AttackAirF / AttackAirB
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        10,                                                 // First frame of hitbox
        21,                                                 // Last frame of hitbox
        -549.0F,                                            // Horizontal range nearest
        56.0F,                                              // Horizontal range farthest
        14.0F,                                              // Vertical range nearest
        387.0F                                              // Vertical range farthest
    },

    // AttackAirHi
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        3,                                                  // First frame of hitbox
        10,                                                 // Last frame of hitbox
        -530.0F,                                            // Horizontal range nearest
        505.0F,                                             // Horizontal range farthest
        -29.0F,                                             // Vertical range nearest
        729.0F                                              // Vertical range farthest
    },

    // AttackAirLw
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        9,                                                  // First frame of hitbox
        26,                                                 // Last frame of hitbox
        -284.0F,                                            // Horizontal range nearest
        187.0F,                                             // Horizontal range farthest
        -267.0F,                                            // Vertical range nearest
        387.0F                                              // Vertical range farthest
    },

    // SpecialAirN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        10,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        400.0F,                                             // Horizontal range nearest
        1200.0F,                                            // Horizontal range farthest
        -70.0F,                                             // Vertical range nearest
        375.0F                                              // Vertical range farthest
    },

    // SpecialAirHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // SpecialAirLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        40,                                                 // First frame of hitbox
        48,                                                 // Last frame of hitbox
        -350.0F,                                            // Horizontal range nearest
        350.0F,                                             // Horizontal range farthest
        -65.0F,                                             // Vertical range nearest
        635.0F                                              // Vertical range farthest
    },

    // End of Aerial Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    }
};

// 0x80187A34
FTComputerAttack dFTComputerAttacksCaptain[/* */] =
{
    // Attack11
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        5,                                                  // First frame of hitbox
        8,                                                  // Last frame of hitbox
        25.0F,                                              // Horizontal range nearest
        495.0F,                                             // Horizontal range farthest
        280.0F,                                             // Vertical range nearest
        510.0F                                              // Vertical range farthest
    },

    // AttackS3
    {
        nFTComputerInputStickTiltAutoXButtonA,             // Input script index
        9,                                                  // First frame of hitbox
        15,                                                 // Last frame of hitbox
        20.0F,                                              // Horizontal range nearest
        570.0F,                                             // Horizontal range farthest
        270.0F,                                             // Vertical range nearest
        520.0F                                              // Vertical range farthest
    },

    // AttackS4
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        16,                                                 // First frame of hitbox
        23,                                                 // Last frame of hitbox
        250.0F,                                             // Horizontal range nearest
        1170.0F,                                            // Horizontal range farthest
        225.0F,                                             // Vertical range nearest
        590.0F                                              // Vertical range farthest
    },

    // AttackHi3
    {
        nFTComputerInputStickTiltHiButtonA,                // Input script index
        8,                                                  // First frame of hitbox
        23,                                                 // Last frame of hitbox
        -395.0F,                                            // Horizontal range nearest
        700.0F,                                             // Horizontal range farthest
        -85.0F,                                             // Vertical range nearest
        955.0F                                              // Vertical range farthest
    },

    // AttackHi4
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        4,                                                  // First frame of hitbox
        11,                                                 // Last frame of hitbox
        -115.0F,                                            // Horizontal range nearest
        460.0F,                                             // Horizontal range farthest
        80.0F,                                              // Vertical range nearest
        1000.0F                                             // Vertical range farthest
    },

    // AttackLw3
    {
        nFTComputerInputStickTiltLwButtonA,                // Input script index
        8,                                                  // First frame of hitbox
        15,                                                 // Last frame of hitbox
        -95.0F,                                             // Horizontal range nearest
        540.0F,                                             // Horizontal range farthest
        -180.0F,                                            // Vertical range nearest
        260.0F                                              // Vertical range farthest
    },

    // AttackLw4
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        9,                                                  // First frame of hitbox
        23,                                                 // Last frame of hitbox
        -630.0F,                                            // Horizontal range nearest
        670.0F,                                             // Horizontal range farthest
        -260.0F,                                            // Vertical range nearest
        300.0F                                              // Vertical range farthest
    },

    // SpecialN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        42,                                                 // First frame of hitbox
        46,                                                 // Last frame of hitbox
        200.0F,                                             // Horizontal range nearest
        1045.0F,                                            // Horizontal range farthest
        100.0F,                                             // Vertical range nearest
        440.0F                                              // Vertical range farthest
    },

    // SpecialHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        13,                                                 // First frame of hitbox
        44,                                                 // Last frame of hitbox
        90.0F,                                              // Horizontal range nearest
        565.0F,                                             // Horizontal range farthest
        125.0F,                                             // Vertical range nearest
        1985.0F                                             // Vertical range farthest
    },

    // SpecialLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        12,                                                 // First frame of hitbox
        39,                                                 // Last frame of hitbox
        -10.0F,                                             // Horizontal range nearest
        1720.0F,                                            // Horizontal range farthest
        320.0F,                                             // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // Catch
    {
        nFTComputerInputStickNButtonZButtonA,              // Input script index
        7,                                                  // First frame of hitbox
        7,                                                  // Last frame of hitbox
        170.0F,                                             // Horizontal range nearest
        470.0F,                                             // Horizontal range farthest
        140.0F,                                             // Vertical range nearest
        440.0F                                              // Vertical range farthest
    },

    // End of Ground Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // AttackAirN
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        4,                                                  // First frame of hitbox
        27,                                                 // Last frame of hitbox
        -144.0F,                                            // Horizontal range nearest
        311.0F,                                             // Horizontal range farthest
        119.0F,                                             // Vertical range nearest
        407.0F                                              // Vertical range farthest
    },

    // AttackAirF / AttackAirB
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        7,                                                  // First frame of hitbox
        18,                                                 // Last frame of hitbox
        -629.0F,                                            // Horizontal range nearest
        -81.0F,                                             // Horizontal range farthest
        118.0F,                                             // Vertical range nearest
        533.0F                                              // Vertical range farthest
    },

    // AttackAirHi
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        5,                                                  // First frame of hitbox
        13,                                                 // Last frame of hitbox
        -604.0F,                                            // Horizontal range nearest
        568.0F,                                             // Horizontal range farthest
        128.0F,                                             // Vertical range nearest
        1103.0F                                             // Vertical range farthest
    },

    // AttackAirLw
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        8,                                                  // First frame of hitbox
        25,                                                 // Last frame of hitbox
        -249.0F,                                            // Horizontal range nearest
        192.0F,                                             // Horizontal range farthest
        -150.0F,                                            // Vertical range nearest
        426.0F                                              // Vertical range farthest
    },

    // SpecialAirN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        42,                                                 // First frame of hitbox
        46,                                                 // Last frame of hitbox
        161.0F,                                             // Horizontal range nearest
        859.0F,                                             // Horizontal range farthest
        100.0F,                                             // Vertical range nearest
        442.0F                                              // Vertical range farthest
    },

    // SpecialAirHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        13,                                                 // First frame of hitbox
        44,                                                 // Last frame of hitbox
        89.0F,                                              // Horizontal range nearest
        570.0F,                                             // Horizontal range farthest
        123.0F,                                             // Vertical range nearest
        423.0F                                              // Vertical range farthest
    },

    // SpecialAirLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        12,                                                 // First frame of hitbox
        30,                                                 // Last frame of hitbox
        -247.0F,                                            // Horizontal range nearest
        265.0F,                                             // Horizontal range farthest
        -139.0F,                                            // Vertical range nearest
        516.0F                                              // Vertical range farthest
    },

    // End of Aerial Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    }
};

// 0x80187C64
FTComputerAttack dFTComputerAttacksPurin[/* */] =
{
    // Attack11
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        5,                                                  // First frame of hitbox
        6,                                                  // Last frame of hitbox
        100.0F,                                             // Horizontal range nearest
        500.0F,                                             // Horizontal range farthest
        50.0F,                                              // Vertical range nearest
        270.0F                                              // Vertical range farthest
    },

    // AttackS3
    {
        nFTComputerInputStickTiltAutoXButtonA,             // Input script index
        7,                                                  // First frame of hitbox
        10,                                                 // Last frame of hitbox
        55.0F,                                              // Horizontal range nearest
        520.0F,                                             // Horizontal range farthest
        90.0F,                                              // Vertical range nearest
        330.0F                                              // Vertical range farthest
    },

    // AttackS4
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        12,                                                 // First frame of hitbox
        19,                                                 // Last frame of hitbox
        310.0F,                                             // Horizontal range nearest
        1115.0F,                                            // Horizontal range farthest
        30.0F,                                              // Vertical range nearest
        340.0F                                              // Vertical range farthest
    },

    // AttackHi3
    {
        nFTComputerInputStickTiltHiButtonA,                // Input script index
        7,                                                  // First frame of hitbox
        14,                                                 // Last frame of hitbox
        -455.0F,                                            // Horizontal range nearest
        220.0F,                                             // Horizontal range farthest
        110.0F,                                             // Vertical range nearest
        745.0F                                              // Vertical range farthest
    },

    // AttackHi4
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        8,                                                  // First frame of hitbox
        14,                                                 // Last frame of hitbox
        -345.0F,                                            // Horizontal range nearest
        430.0F,                                             // Horizontal range farthest
        -20.0F,                                             // Vertical range nearest
        550.0F                                              // Vertical range farthest
    },

    // AttackLw3
    {
        nFTComputerInputStickTiltLwButtonA,                // Input script index
        11,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        30.0F,                                              // Horizontal range nearest
        610.0F,                                             // Horizontal range farthest
        25.0F,                                              // Vertical range nearest
        255.0F                                              // Vertical range farthest
    },

    // AttackLw4
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        8,                                                  // First frame of hitbox
        27,                                                 // Last frame of hitbox
        -435.0F,                                            // Horizontal range nearest
        435.0F,                                             // Horizontal range farthest
        -90.0F,                                             // Vertical range nearest
        220.0F                                              // Vertical range farthest
    },

    // SpecialN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        12,                                                 // First frame of hitbox
        27,                                                 // Last frame of hitbox
        -255.0F,                                            // Horizontal range nearest
        805.0F,                                             // Horizontal range farthest
        -115.0F,                                            // Vertical range nearest
        335.0F                                              // Vertical range farthest
    },

    // SpecialHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        28,                                                 // First frame of hitbox
        125,                                                // Last frame of hitbox
        -355.0F,                                            // Horizontal range nearest
        385.0F,                                             // Horizontal range farthest
        -125.0F,                                            // Vertical range nearest
        610.0F                                              // Vertical range farthest
    },

    // SpecialLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // Catch
    {
        nFTComputerInputStickNButtonZButtonA,              // Input script index
        7,                                                  // First frame of hitbox
        7,                                                  // Last frame of hitbox
        210.0F,                                             // Horizontal range nearest
        550.0F,                                             // Horizontal range farthest
        130.0F,                                             // Vertical range nearest
        370.0F                                              // Vertical range farthest
    },

    // End of Ground Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // AttackAirN
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        6,                                                  // First frame of hitbox
        33,                                                 // Last frame of hitbox
        -110.0F,                                            // Horizontal range nearest
        463.0F,                                             // Horizontal range farthest
        -86.0F,                                             // Vertical range nearest
        241.0F                                              // Vertical range farthest
    },

    // AttackAirF / AttackAirB
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        8,                                                  // First frame of hitbox
        21,                                                 // Last frame of hitbox
        -354.0F,                                            // Horizontal range nearest
        145.0F,                                             // Horizontal range farthest
        -20.0F,                                             // Vertical range nearest
        267.0F                                              // Vertical range farthest
    },

    // AttackAirHi
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        8,                                                  // First frame of hitbox
        16,                                                 // Last frame of hitbox
        -236.0F,                                            // Horizontal range nearest
        289.0F,                                             // Horizontal range farthest
        123.0F,                                             // Vertical range nearest
        508.0F                                              // Vertical range farthest
    },

    // AttackAirLw
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        5,                                                  // First frame of hitbox
        33,                                                 // Last frame of hitbox
        -117.0F,                                            // Horizontal range nearest
        285.0F,                                             // Horizontal range farthest
        -306.0F,                                            // Vertical range nearest
        180.0F                                              // Vertical range farthest
    },

    // SpecialAirN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        12,                                                 // First frame of hitbox
        27,                                                 // Last frame of hitbox
        -263.0F,                                            // Horizontal range nearest
        530.0F,                                             // Horizontal range farthest
        -117.0F,                                            // Vertical range nearest
        285.0F                                              // Vertical range farthest
    },

    // SpecialAirHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        28,                                                 // First frame of hitbox
        125,                                                // Last frame of hitbox
        -358.0F,                                            // Horizontal range nearest
        387.0F,                                             // Horizontal range farthest
        -127.0F,                                            // Vertical range nearest
        611.0F                                              // Vertical range farthest
    },

    // SpecialAirLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // End of Aerial Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    }
};

// 0x80187E94
FTComputerAttack dFTComputerAttacksNess[/* */] =
{
    // Attack11
    {
        nFTComputerInputStickNButtonA,                     // Input script index
        2,                                                  // First frame of hitbox
        3,                                                  // Last frame of hitbox
        150.0F,                                             // Horizontal range nearest
        400.0F,                                             // Horizontal range farthest
        35.0F,                                              // Vertical range nearest
        210.0F                                              // Vertical range farthest
    },

    // AttackS3
    {
        nFTComputerInputStickTiltAutoXButtonA,             // Input script index
        7,                                                  // First frame of hitbox
        11,                                                 // Last frame of hitbox
        220.0F,                                             // Horizontal range nearest
        580.0F,                                             // Horizontal range farthest
        70.0F,                                              // Vertical range nearest
        300.0F                                              // Vertical range farthest
    },

    // AttackS4
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        18,                                                 // First frame of hitbox
        21,                                                 // Last frame of hitbox
        80.0F,                                              // Horizontal range nearest
        590.0F,                                             // Horizontal range farthest
        -40.0F,                                             // Vertical range nearest
        285.0F                                              // Vertical range farthest
    },

    // AttackHi3
    {
        nFTComputerInputStickTiltHiButtonA,                // Input script index
        5,                                                  // First frame of hitbox
        19,                                                 // Last frame of hitbox
        -250.0F,                                            // Horizontal range nearest
        250.0F,                                             // Horizontal range farthest
        215.0F,                                             // Vertical range nearest
        665.0F                                              // Vertical range farthest
    },

    // AttackHi4
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        13,                                                 // First frame of hitbox
        25,                                                 // Last frame of hitbox
        -515.0F,                                            // Horizontal range nearest
        540.0F,                                             // Horizontal range farthest
        10.0F,                                              // Vertical range nearest
        730.0F                                              // Vertical range farthest
    },

    // AttackLw3
    {
        nFTComputerInputStickTiltLwButtonA,                // Input script index
        4,                                                  // First frame of hitbox
        8,                                                  // Last frame of hitbox
        -5.0F,                                              // Horizontal range nearest
        380.0F,                                             // Horizontal range farthest
        -100.0F,                                            // Vertical range nearest
        140.0F                                              // Vertical range farthest
    },

    // AttackLw4
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        14,                                                 // First frame of hitbox
        53,                                                 // Last frame of hitbox
        -680.0F,                                            // Horizontal range nearest
        870.0F,                                             // Horizontal range farthest
        -100.0F,                                            // Vertical range nearest
        290.0F                                              // Vertical range farthest
    },

    // SpecialN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        10,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        500.0F,                                             // Horizontal range nearest
        1000.0F,                                            // Horizontal range farthest
        -40.0F,                                             // Vertical range nearest
        285.0F                                              // Vertical range farthest
    },

    // SpecialHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // SpecialLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // Catch
    {
        nFTComputerInputStickNButtonZButtonA,              // Input script index
        7,                                                  // First frame of hitbox
        7,                                                  // Last frame of hitbox
        50.0F,                                              // Horizontal range nearest
        360.0F,                                             // Horizontal range farthest
        95.0F,                                              // Vertical range nearest
        405.0F                                              // Vertical range farthest
    },

    // End of Ground Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // AttackAirN
    {
        nFTComputerInputStickNButtonA,                      // Input script index
        5,                                                  // First frame of hitbox
        30,                                                 // Last frame of hitbox
        -162.0F,                                            // Horizontal range nearest
        220.0F,                                             // Horizontal range farthest
        -92.0F,                                             // Vertical range nearest
        194.0F                                              // Vertical range farthest
    },

    // AttackAirF / AttackAirB
    {
        nFTComputerInputStickSmashAutoXNYButtonA,          // Input script index
        10,                                                 // First frame of hitbox
        19,                                                 // Last frame of hitbox
        -457.0F,                                            // Horizontal range nearest
        148.0F,                                             // Horizontal range farthest
        -92.0F,                                             // Vertical range nearest
        296.0F                                              // Vertical range farthest
    },

    // AttackAirHi
    {
        nFTComputerInputStickSmashHiButtonA,               // Input script index
        8,                                                  // First frame of hitbox
        16,                                                 // Last frame of hitbox
        -335.0F,                                            // Horizontal range nearest
        377.0F,                                             // Horizontal range farthest
        -33.0F,                                             // Vertical range nearest
        522.0F                                              // Vertical range farthest
    },

    // AttackAirLw
    {
        nFTComputerInputStickSmashLwButtonA,               // Input script index
        5,                                                  // First frame of hitbox
        21,                                                 // Last frame of hitbox
        -129.0F,                                            // Horizontal range nearest
        192.0F,                                             // Horizontal range farthest
        -192.0F,                                            // Vertical range nearest
        129.0F                                              // Vertical range farthest
    },

    // SpecialAirN
    {
        nFTComputerInputStickSmashAutoXButtonB,            // Input script index
        10,                                                 // First frame of hitbox
        15,                                                 // Last frame of hitbox
        500.0F,                                             // Horizontal range nearest
        1000.0F,                                            // Horizontal range farthest
        -40.0F,                                             // Vertical range nearest
        285.0F                                              // Vertical range farthest
    },

    // SpecialAirHi
    {
        nFTComputerInputStickSmashHiButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // SpecialAirLw
    {
        nFTComputerInputStickSmashLwButtonB,               // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    },

    // End of Aerial Attacks
    {
        -1,                                                 // Input script index
        0,                                                  // First frame of hitbox
        0,                                                  // Last frame of hitbox
        0.0F,                                               // Horizontal range nearest
        0.0F,                                               // Horizontal range farthest
        0.0F,                                               // Vertical range nearest
        0.0F                                                // Vertical range farthest
    }
};

// 0x801880C4
FTComputerAttack *dFTComputerAttackList[/* */] =
{
    dFTComputerAttacksMario,            // Mario
    dFTComputerAttacksFox,              // Fox
    dFTComputerAttacksDonkey,           // Donkey Kong
    dFTComputerAttacksSamus,            // Samus
    dFTComputerAttacksLuigi,            // Luigi
    dFTComputerAttacksLink,             // Link
    dFTComputerAttacksYoshi,            // Yoshi
    dFTComputerAttacksCaptain,          // Captain Falcon
    dFTComputerAttacksKirby,            // Kirby
    dFTComputerAttacksPikachu,          // Pikachu
    dFTComputerAttacksPurin,            // Jigglypuff
    dFTComputerAttacksNess,             // Ness
    NULL,                               // Master Hand
    dFTComputerAttacksMario,            // Metal Mario
    dFTComputerAttacksMario,            // Poly Mario
    dFTComputerAttacksFox,              // Poly Fox
    dFTComputerAttacksDonkey,           // Poly Donkey Kong
    dFTComputerAttacksSamus,            // Poly Samus
    dFTComputerAttacksLuigi,            // Poly Luigi
    dFTComputerAttacksLink,             // Poly Link
    dFTComputerAttacksYoshi,            // Poly Yoshi
    dFTComputerAttacksCaptain,          // Poly Captain Falcon
    dFTComputerAttacksKirby,            // Poly Kirby
    dFTComputerAttacksPikachu,          // Poly Pikachu
    dFTComputerAttacksPurin,            // Poly Jigglypuff
    dFTComputerAttacksNess,             // Poly Ness
    dFTComputerAttacksDonkey            // Giant Donkey Kong
};

// 0x80188130
u8 dFTComputerPlayerInputScript0[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_END()
};

// 0x80188138
u8 dFTComputerPlayerInputScript1[/* */] =
{
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 0),
    FTCOMPUTER_EVENT_END()
};

// 0x8018813C
u8 dFTComputerPlayerInputScript2[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 1),
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOFULL, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188144
u8 dFTComputerPlayerInputScript3[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 0),
    FTCOMPUTER_EVENT_END()
};

// 0x8018814C
u8 dFTComputerPlayerInputScript4[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZRelease, 0),
    FTCOMPUTER_EVENT_STICK_Y(I_CONTROLLER_RANGE_MAX, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188158
u8 dFTComputerPlayerInputScript5[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188160
u8 dFTComputerPlayerInputScript6[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 5),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x8018816C
u8 dFTComputerPlayerInputScript7[/* */] =
{
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188170
u8 dFTComputerPlayerInputScript8[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOFULL, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 0),
    FTCOMPUTER_EVENT_STICK_X(0, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188178
u8 dFTComputerPlayerInputScript9[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOFULL, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBPress, 1),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188184
u8 dFTComputerPlayerInputScript10[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 5),
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOFULL, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBPress, 1),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188190
u8 dFTComputerPlayerInputScript11[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBPress, 1),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x8018819C
u8 dFTComputerPlayerInputScript12[/* */] =
{
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZRelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801881A0
u8 dFTComputerPlayerInputScript13[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(I_CONTROLLER_RANGE_MAX, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBPress, 1),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801881AC
u8 dFTComputerPlayerInputScript14[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 5),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(I_CONTROLLER_RANGE_MAX, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBPress, 1),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801881BC
u8 dFTComputerPlayerInputScript15[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonLPress, 5),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonLRelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801881C8
u8 dFTComputerPlayerInputScript16[/* */] =
{
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZRelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801881CC
u8 dFTComputerPlayerInputScript17[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 5),
    FTCOMPUTER_EVENT_END()
};

// 0x801881D4
u8 dFTComputerPlayerInputScript18[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801881DC
u8 dFTComputerPlayerInputScript19[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801881E4
u8 dFTComputerPlayerInputScript20[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 5),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801881F0
u8 dFTComputerPlayerInputScript21[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOFULL, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801881FC
u8 dFTComputerPlayerInputScript22[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 1),
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOFULL, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188208
u8 dFTComputerPlayerInputScript23[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(I_CONTROLLER_RANGE_MAX / 2, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188214
u8 dFTComputerPlayerInputScript24[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(I_CONTROLLER_RANGE_MAX, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188220
u8 dFTComputerPlayerInputScript25[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 5),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188230
u8 dFTComputerPlayerInputScript26[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 5),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(FTCOMPUTER_STICK_AUTOFULL, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188240
u8 dFTComputerPlayerInputScript27[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(-I_CONTROLLER_RANGE_MAX, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBPress, 1),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x8018824C
u8 dFTComputerPlayerInputScript28[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZRelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188258
u8 dFTComputerPlayerInputScript29[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 5),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZRelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188264
u8 dFTComputerPlayerInputScript30[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 1),
    FTCOMPUTER_EVENT_STICK_X(-I_CONTROLLER_RANGE_MAX, 2),
    FTCOMPUTER_EVENT_STICK_X(0, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x8018826C
u8 dFTComputerPlayerInputScript31[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 1),
    FTCOMPUTER_EVENT_STICK_X(I_CONTROLLER_RANGE_MAX, 2),
    FTCOMPUTER_EVENT_STICK_X(0, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188274
u8 dFTComputerPlayerInputScript32[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(-I_CONTROLLER_RANGE_MAX / 2, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188280
u8 dFTComputerPlayerInputScript33[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 5),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(-I_CONTROLLER_RANGE_MAX / 2, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188290
u8 dFTComputerPlayerInputScript34[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 1),
    FTCOMPUTER_EVENT_STICK_Y(-I_CONTROLLER_RANGE_MAX, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x8018829C
u8 dFTComputerPlayerInputScript35[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZPress, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801882A4
u8 dFTComputerPlayerInputScript36[/* */] =
{
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZRelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801882A8
u8 dFTComputerPlayerInputScript37[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 1),
    FTCOMPUTER_EVENT_STICK_Y(-I_CONTROLLER_RANGE_MAX, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBPress, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801882B0
u8 dFTComputerPlayerInputScript38[/* */] =
{
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801882B4
u8 dFTComputerPlayerInputScript39[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 0),
    FTCOMPUTER_EVENT_STICK_Y(-I_CONTROLLER_RANGE_MAX, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801882BC
u8 dFTComputerPlayerInputScript40[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZRelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801882C8
u8 dFTComputerPlayerInputScript41[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(FTCOMPUTER_STICK_AUTOHALF, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZRelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x801882D4
u8 dFTComputerPlayerInputScript42[/* */] =
{
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZRelease, 5),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZPress, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZRelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 10),
    FTCOMPUTER_EVENT_END()
};

// 0x801882E0
u8 dFTComputerPlayerInputScript43[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZRelease, 0),
    FTCOMPUTER_EVENT_STICK_Y(I_CONTROLLER_RANGE_MAX, 15),
    FTCOMPUTER_EVENT_STICK_Y(0, 15),
    FTCOMPUTER_EVENT_STICK_Y(-I_CONTROLLER_RANGE_MAX / 2, 3),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZPress, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZRelease, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 10),
    FTCOMPUTER_EVENT_END()
};

// 0x801882F4
u8 dFTComputerPlayerInputScript44[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(I_CONTROLLER_RANGE_MAX, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 3),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_X(-I_CONTROLLER_RANGE_MAX, 3),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_STICK_X(I_CONTROLLER_RANGE_MAX, 3),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_X(-I_CONTROLLER_RANGE_MAX, 3),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 1),
    FTCOMPUTER_EVENT_STICK_X(I_CONTROLLER_RANGE_MAX, 3),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonAPress, 1),
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonARelease, 0),
    FTCOMPUTER_EVENT_END()
};

// 0x8018830C
u8 dFTComputerPlayerInputScript45[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZPress, 1),
    FTCOMPUTER_EVENT_STICK_X(0, 1),
    FTCOMPUTER_EVENT_STICK_X(-I_CONTROLLER_RANGE_MAX, 1),
    FTCOMPUTER_EVENT_STICK_X(0, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188318
u8 dFTComputerPlayerInputScript46[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(0, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonZPress, 1),
    FTCOMPUTER_EVENT_STICK_X(0, 1),
    FTCOMPUTER_EVENT_STICK_X(I_CONTROLLER_RANGE_MAX, 1),
    FTCOMPUTER_EVENT_STICK_X(0, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x80188324
u8 dFTComputerPlayerInputScript47[/* */] =
{
    FTCOMPUTER_EVENT_STICK_X(0, 0),
    FTCOMPUTER_EVENT_STICK_Y(I_CONTROLLER_RANGE_MAX, 0),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBPress, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandMoveAuto, 1),
    FTCOMPUTER_EVENT_INSTRUCTION(nFTComputerCommandButtonBRelease, 1),
    FTCOMPUTER_EVENT_END()
};

// 0x8018833C
u8 dFTComputerPlayerInputScript48[/* */] =
{
    FTCOMPUTER_EVENT_PKTHUNDER(),
    FTCOMPUTER_EVENT_END()
};

// 0x80188340 - CPU player input events
u8 *dFTComputerPlayerInputScripts[/* */] =
{
    dFTComputerPlayerInputScript0,
    dFTComputerPlayerInputScript1,
    dFTComputerPlayerInputScript2,
    dFTComputerPlayerInputScript3,
    dFTComputerPlayerInputScript4,
    dFTComputerPlayerInputScript5,
    dFTComputerPlayerInputScript6,
    dFTComputerPlayerInputScript7,
    dFTComputerPlayerInputScript8,
    dFTComputerPlayerInputScript9,
    dFTComputerPlayerInputScript10,
    dFTComputerPlayerInputScript11,
    dFTComputerPlayerInputScript12,
    dFTComputerPlayerInputScript13,
    dFTComputerPlayerInputScript14,
    dFTComputerPlayerInputScript15,
    dFTComputerPlayerInputScript16,
    dFTComputerPlayerInputScript17,
    dFTComputerPlayerInputScript18,
    dFTComputerPlayerInputScript19,
    dFTComputerPlayerInputScript20,
    dFTComputerPlayerInputScript21,
    dFTComputerPlayerInputScript22,
    dFTComputerPlayerInputScript23,
    dFTComputerPlayerInputScript25,     // WARNING: 25 and 24 are swapped!
    dFTComputerPlayerInputScript24,     // WARNING: 25 and 24 are swapped!
    dFTComputerPlayerInputScript26,
    dFTComputerPlayerInputScript27,
    dFTComputerPlayerInputScript28,
    dFTComputerPlayerInputScript29,
    dFTComputerPlayerInputScript30,
    dFTComputerPlayerInputScript31,
    dFTComputerPlayerInputScript32,
    dFTComputerPlayerInputScript33,
    dFTComputerPlayerInputScript34,
    dFTComputerPlayerInputScript35,
    dFTComputerPlayerInputScript36,
    dFTComputerPlayerInputScript37,
    dFTComputerPlayerInputScript38,
    dFTComputerPlayerInputScript39,
    dFTComputerPlayerInputScript40,
    dFTComputerPlayerInputScript41,
    dFTComputerPlayerInputScript42,
    dFTComputerPlayerInputScript43,
    dFTComputerPlayerInputScript44,
    dFTComputerPlayerInputScript45,
    dFTComputerPlayerInputScript46,
    dFTComputerPlayerInputScript47,
    dFTComputerPlayerInputScript48
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x80131B00
WPStruct* ftComputerGetOwnWeapon(FTStruct *fp)
{
    GObj *weapon_gobj = gGCCommonLinks[nGCCommonLinkIDWeapon];

    while (weapon_gobj != NULL)
    {
        WPStruct *wp = wpGetStruct(weapon_gobj);

        if (wp->owner_gobj == fp->fighter_gobj)
        {
            return wp;
        }
        weapon_gobj = weapon_gobj->link_next;
    }
    return NULL;
}

// 0x80131B44
Vec3f* ftComputerGetOwnWeaponPositionKind(FTStruct *fp, s32 kind)
{
    GObj *weapon_gobj = gGCCommonLinks[nGCCommonLinkIDWeapon];

    while (weapon_gobj != NULL)
    {
        WPStruct *wp = wpGetStruct(weapon_gobj);

        if ((wp->owner_gobj == fp->fighter_gobj) && (wp->kind == kind))
        {
            return &DObjGetStruct(wp->weapon_gobj)->translate.vec.f;
        }
        weapon_gobj = weapon_gobj->link_next;
    }
    return NULL;
}

// 0x80131BA0
void ftComputerSetControlPKThunder(FTStruct *fp)
{
    Vec3f *pos = ftComputerGetOwnWeaponPositionKind(fp, nWPKindPKThunderTrail);
    FTComputer *com = &fp->computer;

    if (pos != NULL)
    {
        f32 dist_x = com->target_pos.x - pos->x;
        f32 dist_y = com->target_pos.y - pos->y;

        f32 scale = 1.0F / sqrtf(SQUARE(dist_x) + SQUARE(dist_y));

        fp->input.cp.stick_range.x = (F_CONTROLLER_RANGE_MAX * dist_x * scale);
        fp->input.cp.stick_range.y = (F_CONTROLLER_RANGE_MAX * dist_y * scale);
    }
    else
    {
        fp->input.cp.stick_range.x = 0;
        fp->input.cp.stick_range.y = 0;
    }
}

// 0x80131C68
void ftComputerUpdateInputs(FTStruct *this_fp)
{
    FTComputer *com = &this_fp->computer;
    u8 *p_command;
    u8 command;
    s8 var_t1;
    s16 stick_range_y;
    s16 stick_range_x;
    f32 dist_x;
    f32 dist_y;

    if (com->input_wait != 0)
    {
        com->input_wait--;

        if (com->input_wait == 0)
        {
            p_command = com->p_command;

            while (com->input_wait == 0)
            {
                command = *p_command++;

                if (command < FTCOMPUTER_COMMAND_DEFAULT_MAX)
                {
                    com->input_wait = command & FTCOMPUTER_COMMAND_TIMER_MASK;

                    switch (command & FTCOMPUTER_COMMAND_OPCODE_MASK)
                    {
                    case FTCOMPUTER_COMMAND_BUTTON_A_PRESS:
                        this_fp->input.cp.button_inputs |= A_BUTTON;
                        break;

                    case FTCOMPUTER_COMMAND_BUTTON_A_RELEASE:
                        this_fp->input.cp.button_inputs &= ~A_BUTTON;
                        break;

                    case FTCOMPUTER_COMMAND_BUTTON_B_PRESS:
                        this_fp->input.cp.button_inputs |= B_BUTTON;
                        break;

                    case FTCOMPUTER_COMMAND_BUTTON_B_RELEASE:
                        this_fp->input.cp.button_inputs &= ~B_BUTTON;
                        break;

                    case FTCOMPUTER_COMMAND_BUTTON_Z_PRESS:
                        this_fp->input.cp.button_inputs |= Z_TRIG;
                        break;

                    case FTCOMPUTER_COMMAND_BUTTON_Z_RELEASE:
                        this_fp->input.cp.button_inputs &= ~Z_TRIG;
                        break;

                    case FTCOMPUTER_COMMAND_BUTTON_L_PRESS:
                        this_fp->input.cp.button_inputs |= L_TRIG;
                        break;

                    case FTCOMPUTER_COMMAND_BUTTON_L_RELEASE:
                        this_fp->input.cp.button_inputs &= ~L_TRIG;
                        break;

                    case FTCOMPUTER_COMMAND_BUTTON_START_PRESS:
                        this_fp->input.cp.button_inputs |= START_BUTTON;
                        break;

                    case FTCOMPUTER_COMMAND_BUTTON_START_RELEASE:
                        this_fp->input.cp.button_inputs &= ~START_BUTTON;
                        break;

                    case FTCOMPUTER_COMMAND_STICK_X_TILT:
                        switch (*p_command)
                        {
                        default:
                            this_fp->input.cp.stick_range.x = *p_command++;
                            break;

                        case FTCOMPUTER_STICK_AUTOFULL:
                            this_fp->input.cp.stick_range.x = (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x < com->target_pos.x) ? (I_CONTROLLER_RANGE_MAX) : -(I_CONTROLLER_RANGE_MAX);
                            p_command++;
                            break;

                        case FTCOMPUTER_STICK_AUTOHALF:
                            this_fp->input.cp.stick_range.x = (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x < com->target_pos.x) ? (I_CONTROLLER_RANGE_MAX / 2) : -(I_CONTROLLER_RANGE_MAX / 2);
                            p_command++;
                            break;
                        }
                        break;

                    case FTCOMPUTER_COMMAND_STICK_Y_TILT:
                        switch (*p_command)
                        {
                        default:
                            this_fp->input.cp.stick_range.y = *p_command++;
                            break;

                        case FTCOMPUTER_STICK_AUTOFULL:
                            this_fp->input.cp.stick_range.y = (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y < com->target_pos.y) ? (I_CONTROLLER_RANGE_MAX) : -(I_CONTROLLER_RANGE_MAX);
                            p_command++;
                            break;

                        case FTCOMPUTER_STICK_AUTOHALF:
                            this_fp->input.cp.stick_range.y = (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y < com->target_pos.y) ? (I_CONTROLLER_RANGE_MAX / 2) : -(I_CONTROLLER_RANGE_MAX / 2);
                            p_command++;
                            break;
                        }
                        break;

                    case FTCOMPUTER_COMMAND_MOVEAUTO:
                        dist_x = com->target_pos.x - this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
                        dist_y = com->target_pos.y - this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

                        if ((this_fp->ga == nMPKineticsGround) && (this_fp->level < 5))
                        {
                            stick_range_x = (ABSF(dist_x) > 100.0F) ? (I_CONTROLLER_RANGE_MAX / 2) : 0;
                        }
                        else if (this_fp->ga == nMPKineticsGround)
                        {
                            if ((com->dash_predict * 1.5F) < ABSF(dist_x))
                            {
                                stick_range_x = (I_CONTROLLER_RANGE_MAX);
                            }
                            else
                            {
                                if (com->dash_predict < ABSF(dist_x))
                                {
                                    stick_range_x = ((2.0F * ((ABSF(dist_x) - com->dash_predict) / com->dash_predict) * (F_CONTROLLER_RANGE_MAX / 2)) + (F_CONTROLLER_RANGE_MAX / 2));
                                }
                                else
                                {
                                    stick_range_x = (ABSF(dist_x) > 100.0F) ? (I_CONTROLLER_RANGE_MAX / 2) : 0;
                                }
                            }
                        }
                        else
                        {
                            stick_range_x = ((ABSF(dist_x) > 100.0F) || ((this_fp->lr * dist_x) < 0.0F)) ? (I_CONTROLLER_RANGE_MAX) : (I_CONTROLLER_RANGE_MAX / 4);
                        }
                        stick_range_y = I_CONTROLLER_RANGE_MAX;

                        if (this_fp->ga == nMPKineticsGround)
                        {
                            if (this_fp->status_id != nFTCommonStatusKneeBend)
                            {
                                if (com->target_line_id == this_fp->coll_data.floor_line_id)
                                {
                                    stick_range_y = dist_y = 0.0F;
                                }
                                if 
                                (
                                    (com->ftcom_flags_0x4A_b1) && 
                                    (
                                        (ftGetComTargetFighter(com)->status_id == nFTCommonStatusCliffCatch) || 
                                        (ftGetComTargetFighter(com)->status_id == nFTCommonStatusCliffWait)
                                    )
                                )
                                {
                                    stick_range_y = dist_y = 0.0F;
                                }
                            }
                        }
                        else
                        {
                            if
                            (
                                (
                                    (this_fp->fkind != nFTKindFox) ||
                                    (
                                        (this_fp->status_id != nFTFoxStatusSpecialHiStart) &&
                                        (this_fp->status_id != nFTFoxStatusSpecialAirHiStart) &&
                                        (this_fp->status_id != nFTFoxStatusSpecialHiHold) &&
                                        (this_fp->status_id != nFTFoxStatusSpecialAirHiHold)
                                    )
                                ) 
                                &&
                                (dist_y < 0)
                            )
                            {
                                stick_range_y = dist_y = 0.0F;
                            }
                            switch (com->behavior)
                            {
                            case nFTComputerBehaviorYoshiTeam:
                                if (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y < 0)
                                {
                                    stick_range_y = dist_y = 0.0F;
                                }
                                break;

                            case nFTComputerBehaviorKirbyTeam:
                            case nFTComputerBehaviorPolyTeam:
                                if (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y < -300.0F)
                                {
                                    stick_range_y = dist_y = 0.0F;
                                }
                                break;
                            }
                        }
                        if ((dist_x != 0.0F) && (dist_y != 0.0F))
                        {
                            if (ABSF(dist_y) < ABSF(dist_x))
                            {
                                this_fp->input.cp.stick_range.x = (dist_x > 0.0F) ? stick_range_x : -stick_range_x;

                                this_fp->input.cp.stick_range.y = (ABSF((dist_y / dist_x)) * ((dist_y > 0.0F) ? stick_range_y : -stick_range_y));
                            }
                            else
                            {
                                this_fp->input.cp.stick_range.x = (ABSF((dist_x / dist_y)) * ((dist_x > 0.0F) ? stick_range_x : -stick_range_x));

                                this_fp->input.cp.stick_range.y = (dist_y > 0.0F) ? stick_range_y : -stick_range_y;
                            }
                        }
                        else if (dist_x != 0.0F)
                        {
                            this_fp->input.cp.stick_range.x = (dist_x > 0.0F) ? stick_range_x : -stick_range_x;

                            this_fp->input.cp.stick_range.y = (ABSF((dist_y / dist_x)) * ((dist_y > 0.0F) ? stick_range_y : -stick_range_y));
                        }
                        else if (dist_y != 0.0F)
                        {
                            this_fp->input.cp.stick_range.x = (ABSF((dist_x / dist_y)) * ((dist_x > 0.0F) ? stick_range_x : -stick_range_x));

                            this_fp->input.cp.stick_range.y = (dist_y > 0.0F) ? stick_range_y : -stick_range_y;
                        }
                        else
                        {
                            this_fp->input.cp.stick_range.x = this_fp->input.cp.stick_range.y = 0;
                        }
                        break;

                    case FTCOMPUTER_COMMAND_STICK_X_VAR:
                        this_fp->input.cp.stick_range.x = var_t1;
                        break;

                    case FTCOMPUTER_COMMAND_STICK_Y_VAR:
                        this_fp->input.cp.stick_range.y = var_t1;
                        break;
                    }
                }
                else switch (command)
                {
                case FTCOMPUTER_COMMAND_DEFAULT_MAX + 0:
                    com->input_wait = *p_command++;
                    break;

                case FTCOMPUTER_COMMAND_DEFAULT_MAX + 1:
                    var_t1 = 1;
                    break;

                case FTCOMPUTER_COMMAND_DEFAULT_MAX + 2:
                    com->input_wait = var_t1;
                    break;

                case FTCOMPUTER_COMMAND_DEFAULT_MAX + 3:
                    ftComputerSetControlPKThunder(this_fp);
                    break;

                case FTCOMPUTER_COMMAND_END:
                    com->input_wait = 0;
                    com->p_command = NULL;
                    return;
                }
            }
            com->p_command = p_command;
        }
    }
}

// 0x80132564
void ftComputerSetCommandWaitShort(FTStruct *fp, s32 index)
{
    FTComputer *com = &fp->computer;

    if (fp->ga == nMPKineticsGround)
    {
        com->input_wait = ((2.0F * (syUtilsRandFloat() * (FTCOMPUTER_LEVEL_MAX - fp->level))) + ((FTCOMPUTER_LEVEL_MAX - fp->level) * 2) + 1.0F);
    }
    else com->input_wait = ((syUtilsRandFloat() * (FTCOMPUTER_LEVEL_MAX - fp->level)) + ((FTCOMPUTER_LEVEL_MAX - fp->level) / 2) + 1.0F);
    
    com->p_command = dFTComputerPlayerInputScripts[index];
}

// 0x80132758
void ftComputerSetCommandImmediate(FTStruct *fp, s32 index)
{
    FTComputer *com = &fp->computer;

    com->input_wait = 1;
    com->p_command = dFTComputerPlayerInputScripts[index];
}

// 0x80132778
void ftComputerSetCommandWaitLong(FTStruct *fp, s32 index)
{
    FTComputer *com = &fp->computer;

    if (fp->ga == nMPKineticsGround)
    {
        com->input_wait = ((4 * (syUtilsRandFloat() * (FTCOMPUTER_LEVEL_MAX - fp->level))) + ((FTCOMPUTER_LEVEL_MAX - fp->level) * 4) + 1.0F);
    }
    else com->input_wait = ((syUtilsRandFloat() * (FTCOMPUTER_LEVEL_MAX - fp->level)) + (FTCOMPUTER_LEVEL_MAX - fp->level) + 1.0F);
    
    com->p_command = dFTComputerPlayerInputScripts[index];
}

// 0x8013295C
sb32 ftComputerCheckFindTarget(FTStruct *this_fp)
{
    FTComputer *com = &this_fp->computer;
    FTStruct *other_fp;
    f32 this_pos_x = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
    f32 this_pos_y = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;
    GObj *other_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];
    f32 distance = F32_MAX;
    f32 square_xy;
    f32 other_pos_x;
    f32 other_pos_y;

    while (other_gobj != NULL)
    {
        if (other_gobj != this_fp->fighter_gobj)
        {
            other_fp = ftGetStruct(other_gobj);

            if (this_fp->team != other_fp->team)
            {
                other_pos_x = other_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
                other_pos_y = other_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

                if 
                (
                    (other_fp->status_id >= nFTCommonStatusWait) &&
                    (
                        (
                            (func_ovl2_800F8FFC(&other_fp->joints[nFTPartsJointTopN]->translate.vec.f) != FALSE) &&
                            (other_pos_x <= gMPCollisionBounds.current.right) &&
                            (other_pos_x >= gMPCollisionBounds.current.left) &&
                            (other_pos_y >= gMPCollisionBounds.current.bottom) &&
                            (other_pos_y < gMPCollisionGroundData->camera_bound_top)
                        ) 
                        ||
                        (
                            (this_fp->ga == nMPKineticsGround) &&
                            (
                                (other_fp->status_id == nFTCommonStatusCliffCatch) ||
                                (other_fp->status_id == nFTCommonStatusCliffWait)
                            )
                        )
                    ) 
                    &&
                    (
                        (this_fp->fkind != nFTKindMMario) ||
                        (other_fp->ga == nMPKineticsGround)
                    )
                )
                {
                    square_xy = SQUARE(this_pos_x - other_pos_x) + SQUARE(this_pos_y - other_pos_y);

                    if (square_xy < distance)
                    {
                        com->target_pos.x = other_pos_x;
                        com->target_pos.y = other_pos_y;
                        com->target_user = other_fp;

                        distance = square_xy;
                    }
                }
            }
        }
        other_gobj = other_gobj->link_next;
    }

    if (distance == F32_MAX)
    {
        com->target_line_id = -1;
        com->target_dist = F32_MAX;
        com->ftcom_flags_0x4A_b1 = FALSE;

        return FALSE;
    }
    com->ftcom_flags_0x4A_b1 = TRUE;
    com->target_dist = sqrtf(distance);

    if (ftGetComTargetFighter(com)->ga == nMPKineticsGround)
    {
        com->target_line_id = ftGetComTargetFighter(com)->coll_data.floor_line_id;
    }
    else com->target_line_id = -1;

    return TRUE;
}

// 0x80132BC8
sb32 ftComputerCheckEvadeDistance(FTStruct *this_fp)
{
    GObj *other_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (other_gobj != NULL)
    {
        if (other_gobj != this_fp->fighter_gobj)
        {
            FTStruct *other_fp = ftGetStruct(other_gobj);

            if (this_fp->team != other_fp->team)
            {
                DObj *other_joint = other_fp->joints[nFTPartsJointTopN];
                DObj *this_joint = this_fp->joints[nFTPartsJointTopN];
                f32 other_x = (other_joint->translate.vec.f.x + other_fp->physics.vel_air.x * 3.0F);
                f32 other_y = (other_joint->translate.vec.f.y + other_fp->physics.vel_air.x * 3.0F);
                f32 sqrt_xy = sqrtf(SQUARE(this_joint->translate.vec.f.y - other_y) + SQUARE(this_joint->translate.vec.f.x - other_x));

                if ((other_fp->star_hitstatus == nGMHitStatusInvincible) && (sqrt_xy < 1500.0F))
                {
                    return TRUE;
                }
                else if ((other_fp->item_gobj != NULL) && (itGetStruct(other_fp->item_gobj)->kind == nITKindHammer) && (sqrt_xy < 2500.0F))
                {
                    return TRUE;
                }
            }
        }
        other_gobj = other_gobj->link_next;
    }
    return FALSE;
}

// 0x80132D18
FTStruct* ftComputerWaitGetTarget(FTStruct *this_fp)
{
    FTComputer *com = &this_fp->computer;
    GObj *other_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];
    s32 target_damage = 9999;
    FTStruct *target_fp = NULL;

    while (other_gobj != NULL)
    {
        if (other_gobj != this_fp->fighter_gobj)
        {
            FTStruct *other_fp = ftGetStruct(other_gobj);

            if (this_fp->team != other_fp->team)
            {
                if (other_fp->status_id >= nFTCommonStatusControlStart)
                {
                    if ((com->target_gobj != NULL) && (other_fp->fighter_gobj == com->target_gobj))
                    {
                        if (other_fp->percent_damage == com->target_damage_percent)
                        {
                            if (com->wiggle_wait)
                            {
                                com->wiggle_wait--;
                            }
                            if (!(com->wiggle_wait))
                            {
                                return other_fp;
                            }
                            else return NULL;
                        }
                        com->target_gobj = NULL;
                    }
                    if (other_fp->percent_damage < target_damage)
                    {
                        target_damage = other_fp->percent_damage;
                        target_fp = other_fp;
                    }
                }
            }
        }
        other_gobj = other_gobj->link_next;
    }
    if (target_fp != NULL)
    {
        com->target_damage_percent = target_damage;
        com->target_gobj = target_fp->fighter_gobj;
        com->wiggle_wait = (syUtilsRandFloat() * 300.0F) + 600.0F;
    }
    return NULL;
}

// 0x80132EC0
void func_ovl3_80132EC0(void) // Unused
{
    return;
}

// 0x80132EC8
sb32 ftComputerCheckDetectTarget(FTStruct *this_fp, f32 detect_range_base)
{
    // This was a wildddddddddd match...
    FTComputer *com = &this_fp->computer;
    FTStruct *target_fp = ftGetComTargetFighter(com);
    f32 this_pos_x;
    f32 this_pos_y;
    f32 this_vel_x;
    f32 this_vel_y;
    f32 this_tvel_base;
    f32 this_gravity;
    f32 target_pos_x;
    f32 target_pos_y;
    f32 target_vel_x;
    f32 target_vel_y;
    f32 target_tvel_base;
    f32 target_gravity;
    void *user_data; // Originally "reflect_fp" but need to rename it appropriately for the silly hack in the item kind check
    f32 hurtbox_detect_width;
    f32 hurtbox_detect_height;
    f32 predict_pos_x;
    f32 predict_pos_y;
    f32 damage_coll_size_mul;
    f32 detect_far_x;
    f32 detect_near_x;
    f32 detect_near_y;
    s32 input_kinds[20];
    f32 detect_ranges_x[20];
    s32 hit_frame;
    s32 i;
    sb32 is_attempt_cliffcatch;     // Might be very much incorrectly nanmed
    s32 target_predict_frame;
    s32 this_predict_frame;
    s32 attack_count;
    f32 detect_far_y;
    FTComputerAttack *comattack;
    Vec3f this_detect_pos;
    f32 predict_adjust_y;
    s32 fkind;
    s32 unused[2];

    if (gSCManagerBattleState->gkind == nGRKindInishie)
    {
        if ((this_fp->coll_data.floor_line_id >= 0) && (mpCollisionCheckExistPlatformLineID(this_fp->coll_data.floor_line_id) != FALSE))
        {
            return FALSE;
        }
    }
    if ((gSCManagerBattleState->gkind == nGRKindYamabuki) && (this_fp->ga != nMPKineticsGround))
    {
        if (this_fp->physics.vel_air.x > 0.0F)
        {
            detect_near_x = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
            detect_far_x = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x + (this_fp->physics.vel_air.x * 40.0F);
        }
        else
        {
            detect_far_x = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
            detect_near_x = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x + (this_fp->physics.vel_air.x * 40.0F);
        }
        this_detect_pos.y = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;
        this_detect_pos.x = (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x + detect_near_x) - 100.0F;

        while (this_detect_pos.x < ((this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x + detect_far_x) + 100.0F))
        {
            if (func_ovl2_800F8FFC(&this_detect_pos) == FALSE)
            {
                return FALSE;
            }
            this_detect_pos.x += 100.0F;
        }
    }
    this_pos_x = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
    this_pos_y = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

    damage_coll_size_mul = (((syUtilsRandFloat() - 0.5F) * (FTCOMPUTER_LEVEL_MAX - this_fp->level)) * 0.1F) + 1.0F;

    hurtbox_detect_width = target_fp->damage_coll_size.x * damage_coll_size_mul;
    hurtbox_detect_height = target_fp->damage_coll_size.y * damage_coll_size_mul;

    target_pos_x = target_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
    target_pos_y = target_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

    target_vel_x = target_fp->physics.vel_air.x;
    target_vel_y = target_fp->physics.vel_air.y;

    target_tvel_base = -target_fp->attr->tvel_base;
    target_gravity = target_fp->attr->gravity;

    this_vel_x = this_fp->physics.vel_air.x;
    this_vel_y = this_fp->physics.vel_air.y;

    this_tvel_base = -this_fp->attr->tvel_base;
    this_gravity = this_fp->attr->gravity;

    comattack = dFTComputerAttackList[this_fp->fkind];

    if (this_fp->ga != nMPKineticsGround)
    {
        while (comattack->input_kind != -1)
        {
            comattack++;
        }
        comattack++;
    }
    for (attack_count = 0; TRUE; comattack++) // Does not match as while (TRUE) loop
    {
        if (comattack->input_kind == -1)
        {
            break;
        }
        else if (comattack->hit_start_frame == 0)
        {
            continue;
        }
        else
        {
            predict_pos_x = ((target_pos_x + (target_vel_x * comattack->hit_start_frame)) - (this_pos_x + (this_vel_x * comattack->hit_start_frame))) * this_fp->lr;
            hit_frame = comattack->hit_start_frame;
            is_attempt_cliffcatch = FALSE;
            this_predict_frame = -(this_tvel_base - this_pos_y) / this_gravity;

            if ((this_fp->status_id == nFTCommonStatusPass) || (this_predict_frame <= 0))
            {
                predict_adjust_y = (this_vel_y * hit_frame) + this_pos_y;
            }
            else if (hit_frame < this_predict_frame)
            {
                predict_adjust_y = ((this_vel_y * hit_frame) - ((this_gravity * SQUARE(hit_frame)) * 0.5F)) + this_pos_y;
            }
            else predict_adjust_y = (((hit_frame * this_vel_y) - ((this_gravity * SQUARE(this_predict_frame)) * 0.5F)) + (this_tvel_base * (hit_frame - this_predict_frame))) + this_pos_y;

            if ((target_fp->status_id != nFTCommonStatusPass) && (target_fp->ga != nMPKineticsGround))
            {
                target_predict_frame = -(target_tvel_base - target_vel_y) / target_gravity;

                if (target_predict_frame <= 0)
                {
                    predict_pos_y = ((comattack->hit_start_frame * target_vel_y) + target_pos_y) - predict_adjust_y;
                }
                else if (comattack->hit_start_frame < target_predict_frame)
                {
                    predict_pos_y = (((comattack->hit_start_frame * target_vel_y) + target_pos_y) - ((target_gravity * SQUARE(comattack->hit_start_frame)) * 0.5F)) - predict_adjust_y;
                }
                else predict_pos_y = ((((comattack->hit_start_frame * target_vel_y) + target_pos_y) - ((target_gravity * SQUARE(target_predict_frame)) * 0.5F)) + (target_tvel_base * (comattack->hit_start_frame - target_predict_frame))) - predict_adjust_y;
            }
            else predict_pos_y = ((comattack->hit_start_frame * target_vel_y) + target_pos_y) - predict_adjust_y;

            if (this_fp->lr > 0)
            {
                detect_near_x = comattack->detect_near_x;
                detect_far_x = comattack->detect_far_x;
            }
            else
            {
                detect_near_x = -comattack->detect_far_x;
                detect_far_x = -comattack->detect_near_x;
            }
            detect_near_y = comattack->detect_near_y;
            detect_far_y = comattack->detect_far_y;

            if (this_fp->fkind == nFTKindGDonkey)
            {
                detect_near_x *= 1.4F;
                detect_far_x *= 1.4F;
                detect_near_y *= 1.4F;
                detect_far_y *= 1.4F;
            }
            if (this_fp->ga == nMPKineticsGround)
            {
                switch (comattack->input_kind)
                {
                case nFTComputerInputStickNButtonA:
                case nFTComputerInputStickTiltAutoXButtonA:
                case nFTComputerInputStickSmashAutoXNYButtonA:
                    if (this_fp->item_gobj != NULL)
                    {
                        user_data = itGetStruct(this_fp->item_gobj);

                        if (user_data != NULL)
                        {
                            // B R U H
                            if
                            (
                                (((ITStruct*)user_data)->kind == nITKindSword)  ||
                                (((ITStruct*)user_data)->kind == nITKindBat)    ||
                                (((ITStruct*)user_data)->kind == nITKindStarRod)
                            )
                            {
                                detect_near_x *= 1.3F;
                                detect_far_x *= 1.3F;
                            }
                        }
                    }
                    break;

                default:
                    break;
                }
            }
            switch (this_fp->fkind)
            {
            case nFTKindMario:
            case nFTKindLuigi:
            case nFTKindMMario:
            case nFTKindNMario:
            case nFTKindNLuigi:
                if (comattack->input_kind == nFTComputerInputStickSmashHiButtonB)
                {
                    is_attempt_cliffcatch = TRUE;
                }
                break;

            case nFTKindKirby:
            case nFTKindNKirby:
                if (comattack->input_kind == nFTComputerInputStickSmashHiButtonB)
                {
                    is_attempt_cliffcatch = TRUE;
                }
                /* fallthrough */
            case nFTKindYoshi:
            case nFTKindCaptain:
            case nFTKindNYoshi:
            case nFTKindNCaptain:
                if (comattack->input_kind == nFTComputerInputStickSmashLwButtonB)
                {
                    is_attempt_cliffcatch = TRUE;
                }
                break;

            case nFTKindBoss:
            case nFTKindNFox:
            case nFTKindNDonkey:
            case nFTKindNSamus:
            case nFTKindNLink:
            case nFTKindNPikachu:
            case nFTKindNNess:
            case nFTKindGDonkey:
                break;
            }
            if (is_attempt_cliffcatch != FALSE)
            {
                this_detect_pos.y = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;
                this_detect_pos.x = (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x + detect_near_x) - 100.0F;

                while (this_detect_pos.x < ((this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x + detect_far_x) + 100.0F))
                {
                    if (func_ovl2_800F8FFC(&this_detect_pos) == FALSE)
                    {
                        goto l_continue;
                    }
                    this_detect_pos.x += 100.0F;
                }
                if (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x < com->target_pos.x)
                {
                    if (com->cliff_right_pos.x < (com->target_pos.x + 1200.0F))
                    {
                        goto l_continue;
                    }
                }
                else if (com->cliff_left_pos.x > (com->target_pos.x - 1200.0F))
                {
                    goto l_continue;
                }
            }
            if (this_fp->fkind == nFTKindGDonkey)
            {
                if
                (
                    (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x < (gMPCollisionBounds.current.left + 500.0F)) || 
                    (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x > (gMPCollisionBounds.current.right - 500.0F))
                )
                {
                    switch (comattack->input_kind)
                    {
                    case nFTComputerInputStickSmashHiButtonB:
                        goto l_continue;

                    case nFTComputerInputStickSmashAutoXButtonB:
                        if ((this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x * this_fp->lr) > 0.0F)
                        {
                            goto l_continue;
                        }
                        break;

                    default:
                        if (this_fp->ga != nMPKineticsGround)
                        {
                            goto l_continue;
                        }
                        break;
                    }
                }
            }
            if ((this_fp->fkind != nFTKindGDonkey) || (this_fp->ga == nMPKineticsGround) || (comattack->input_kind != nFTComputerInputStickSmashHiButtonB))
            {
                if (com->ftcom_flags_0x4A_b1)
                {
                    if 
                    (
                        (ftGetComTargetFighter(com)->status_id == nFTCommonStatusCliffCatch) ||
                        (ftGetComTargetFighter(com)->status_id == nFTCommonStatusCliffWait)
                    )
                    {
                        if (detect_near_y < 0.0F)
                        {
                            detect_near_y -= 500.0F;
                        }
                    }
                }
                if 
                (
                    (predict_pos_y < detect_far_y) && 
                    ((detect_near_y - hurtbox_detect_height) < predict_pos_y) && 
                    ((detect_near_x - hurtbox_detect_width) < predict_pos_x) && 
                    (predict_pos_x < (detect_far_x + hurtbox_detect_width))
                )
                {
                    input_kinds[attack_count] = comattack->input_kind;

                    switch (comattack->input_kind)
                    {
                    case nFTComputerInputStickSmashAutoXNYButtonA:
                        if (this_fp->item_gobj != NULL)
                        {
                            if (itGetStruct(this_fp->item_gobj) != NULL)
                            {
                                if (itGetStruct(this_fp->item_gobj)->type == nITTypeSwing)
                                {
                                    detect_range_base = -0.8F;
                                }
                            }
                            if (itGetStruct(this_fp->item_gobj) != NULL)
                            {
                                if (itGetStruct(this_fp->item_gobj)->kind == nITKindBat)
                                {
                                    if (this_fp->ga == nMPKineticsGround)
                                    {
                                        detect_ranges_x[attack_count++] = 4.0F;
                                        break;
                                    }
                                }
                            }
                        }
                        detect_ranges_x[attack_count++] = 1.0F;
                        break;

                    case nFTComputerInputStickSmashAutoXButtonB:
                        user_data = com->target_user;

                        if (this_fp->level >= 5)
                        {
                            if (user_data != NULL)
                            {
                                if ((((FTStruct*)user_data)->fkind == nFTKindNess) || (((FTStruct*)user_data)->fkind == nFTKindFox))
                                {
                                    fkind = (this_fp->fkind == nFTKindKirby) ? this_fp->passive_vars.kirby.copy_id : this_fp->fkind;

                                    switch (fkind)
                                    {
                                    case nFTKindMario:
                                    case nFTKindFox:
                                    case nFTKindSamus:
                                    case nFTKindLuigi:
                                    case nFTKindLink:
                                    case nFTKindPikachu:
                                    case nFTKindMMario:
                                        goto l_continue;
                                    }
                                }
                            }
                        }
                        switch (this_fp->fkind)
                        {
                        case nFTKindDonkey:
                        case nFTKindGDonkey:
                            if (this_fp->passive_vars.donkey.charge_level == FTDONKEY_GIANTPUNCH_CHARGE_MAX)
                            {
                                detect_ranges_x[attack_count++] = 4.0F;
                            }
                            else detect_ranges_x[attack_count++] = 1.0F + detect_range_base;
                            break;

                        case nFTKindSamus:
                            if (this_fp->passive_vars.samus.charge_level == FTSAMUS_CHARGE_MAX)
                            {
                                detect_ranges_x[attack_count++] = 4.0F;
                            }
                            else detect_ranges_x[attack_count++] = 1.0F + detect_range_base;
                            break;

                        case nFTKindKirby:
                            switch (this_fp->passive_vars.kirby.copy_id)
                            {
                            case nFTKindDonkey:
                                if (this_fp->passive_vars.kirby.copydonkey_charge_level == FTKIRBY_COPYDONKEY_GIANTPUNCH_CHARGE_MAX)
                                {
                                    detect_ranges_x[attack_count++] = 4.0F;
                                }
                                else detect_ranges_x[attack_count++] = 1.0F + detect_range_base;
                                break;

                            case nFTKindSamus:
                                if (this_fp->passive_vars.kirby.copysamus_charge_level == FTKIRBY_COPYSAMUS_CHARGE_MAX)
                                {
                                    detect_ranges_x[attack_count++] = 4.0F;
                                }
                                else detect_ranges_x[attack_count++] = 1.0F + detect_range_base;
                                break;

                            default:
                                detect_ranges_x[attack_count++] = 4.0F;
                                break;
                            }
                            break;

                        default:
                            detect_ranges_x[attack_count++] = 1.0F + detect_range_base;
                            break;
                        }
                        break;

                    case nFTComputerInputStickSmashHiButtonB:
                    case nFTComputerInputStickSmashLwButtonB:
                        detect_ranges_x[attack_count++] = (detect_range_base * 0.5F) + 1.0F;
                        break;

                    case nFTComputerInputStickNButtonZButtonA:
                        if ((this_fp->fkind != nFTKindLink) && (this_fp->fkind != nFTKindSamus))
                        {
                            detect_ranges_x[attack_count++] = 4.0F;
                            break;
                        }
                        /* fallthrough */
                    default:
                        detect_ranges_x[attack_count++] = 1.0F;
                        break;
                    }
                }
            }
        }
    l_continue:
        continue;
    }
    if (attack_count != 0)
    {
        if (com->trait == nFTComputerTraitLink)
        {
            if (this_fp->percent_damage < ((syUtilsRandFloat() * 100.0F) + 1.0F))
            {
                com->fighter_follow_since = 0;
                return FALSE;
            }
        }
        if (this_fp->level < 3)
        {
            if ((this_fp->percent_damage + 5.0F) < (syUtilsRandFloat() * (200.0F - (this_fp->level * 50.0F))))
            {
                com->fighter_follow_since = 0;
                return FALSE;
            }
        }
        detect_far_x = 0.0F;

        for (i = 0; i < attack_count; i++)
        {
            if (com->input_kind == input_kinds[i])
            {
                detect_ranges_x[i] *= 0.25F;
            }
            switch (input_kinds[i])
            {
            case nFTComputerInputStickNButtonA:
                detect_ranges_x[i] += com->stickn_button_a_count * 0.2F;
                com->stickn_button_a_count++;
                break;

            case nFTComputerInputStickTiltAutoXButtonA:
                detect_ranges_x[i] += com->sticktilts_button_a_count * 0.2F;
                com->sticktilts_button_a_count++;
                break;

            case nFTComputerInputStickSmashAutoXNYButtonA:
                detect_ranges_x[i] += com->sticksmashs_button_a_count * 0.2F;
                com->sticksmashs_button_a_count++;
                break;

            case nFTComputerInputStickTiltHiButtonA:
                detect_ranges_x[i] += com->sticktilthi_button_a_count * 0.2F;
                com->sticktilthi_button_a_count++;
                break;

            case nFTComputerInputStickSmashHiButtonA:
                detect_ranges_x[i] += com->sticksmashhi_button_a_count * 0.2F;
                com->sticksmashhi_button_a_count++;
                break;

            case nFTComputerInputStickTiltLwButtonA:
                detect_ranges_x[i] += com->sticktiltlw_button_a_count * 0.2F;
                com->sticktiltlw_button_a_count++;
                break;

            case nFTComputerInputStickSmashLwButtonA:
                detect_ranges_x[i] += com->sticksmashlw_button_a_count * 0.2F;
                com->sticksmashlw_button_a_count++;
                break;

            case nFTComputerInputStickSmashAutoXButtonB:
                detect_ranges_x[i] += com->sticksmashs_button_b_count * 0.2F;
                com->sticksmashs_button_b_count++;
                break;

            case nFTComputerInputStickSmashHiButtonB:
                detect_ranges_x[i] += com->sticksmashhi_button_b_count * 0.2F;
                com->sticksmashhi_button_b_count++;
                break;

            case nFTComputerInputStickSmashLwButtonB:
                detect_ranges_x[i] += com->sticksmashlw_button_b_count * 0.2F;
                com->sticksmashlw_button_b_count++;
                break;

            case nFTComputerInputStickNButtonZButtonA:
                detect_ranges_x[i] += com->stickn_button_z_button_a_count * 0.2F;
                com->stickn_button_z_button_a_count++;
                break;

            default:
                break;
            }
            detect_ranges_x[i] += detect_far_x;
            detect_far_x = detect_ranges_x[i];
        }
        detect_far_x = syUtilsRandFloat() * detect_far_x;

        for (i = 0; i < attack_count; i++)
        {
            if (detect_far_x < detect_ranges_x[i])
            {
                if (com->input_kind == input_kinds[i])
                {
                    com->input_repeat_count++;

                    if (com->input_repeat_count >= 4)
                    {
                        ftComputerSetCommandImmediate(this_fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);

                        return TRUE;
                    }
                }
                else com->input_repeat_count = 0;

                ftComputerSetCommandWaitShort(this_fp, input_kinds[i]);

                com->input_kind = input_kinds[i];

                switch (input_kinds[i])
                {
                case nFTComputerInputStickNButtonA:
                    com->stickn_button_a_count = 0;
                    break;

                case nFTComputerInputStickTiltAutoXButtonA:
                    com->sticktilts_button_a_count = 0;
                    break;

                case nFTComputerInputStickSmashAutoXNYButtonA:
                    com->sticksmashs_button_a_count = 0;
                    break;

                case nFTComputerInputStickTiltHiButtonA:
                    com->sticktilthi_button_a_count = 0;
                    break;

                case nFTComputerInputStickSmashHiButtonA:
                    com->sticksmashhi_button_a_count = 0;
                    break;

                case nFTComputerInputStickTiltLwButtonA:
                    com->sticktiltlw_button_a_count = 0;
                    break;

                case nFTComputerInputStickSmashLwButtonA:
                    com->sticksmashlw_button_a_count = 0;
                    break;

                case nFTComputerInputStickSmashAutoXButtonB:
                    com->sticksmashs_button_b_count = 0;
                    break;

                case nFTComputerInputStickSmashHiButtonB:
                    com->sticksmashhi_button_b_count = 0;
                    break;

                case nFTComputerInputStickSmashLwButtonB:
                    com->sticksmashlw_button_b_count = 0;
                    break;

                case nFTComputerInputStickNButtonZButtonA:
                    com->stickn_button_z_button_a_count = 0;
                    break;

                default:
                    break;
                }
                if ((this_fp->fkind == nFTKindPurin) && (com->input_kind == nFTComputerInputStickSmashHiButtonB) && (syUtilsRandFloat() < 0.9F))
                {
                    return FALSE;
                }
                return TRUE;
            }
        }
    }
    else return FALSE;
}

// 0x80134000
sb32 ftComputerCheckSetTargetEdgeRight(FTStruct *fp, sb32 is_find_edge_target)
{
    FTComputer *com = &fp->computer;
    f32 edge_dist_x;
    f32 edge_predict_y;
    f32 edge_offset;
    s32 fall_predict;
    Vec3f edge_pos;
    f32 acid_level_curr;
    f32 acid_level_step;
    s32 edge_predict_x;
    u16 *line_ids;
    s32 i;

    if (fp->physics.vel_air.y >= 0.0F)
    {
        com->target_pos.x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x - 500.0F;
        com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

        return TRUE;
    }
    edge_offset = com->jump_predict * 0.75F;

    if (fp->fkind == nFTKindMMario)
    {
        edge_offset = 0;
    }
    if (gSCManagerBattleState->gkind == nGRKindYamabuki)
    {
        edge_offset = 0;
    }
    line_ids = &gMPCollisionLineGroups[nMPLineKindFloor].line_id[0];

    for (i = 0; i < gMPCollisionLineGroups[nMPLineKindFloor].line_count; i++)
    {
        if (mpCollisionCheckExistLineID(line_ids[i]) != FALSE)
        {
            mpCollisionGetFloorEdgeR(line_ids[i], &edge_pos);

            if (gSCManagerBattleState->gkind == nGRKindZebes)
            {
                grZebesAcidGetLevelInfo(&acid_level_curr, &acid_level_step);

                if ((edge_pos.y < (acid_level_curr + 500.0F)) || (edge_pos.y < ((5.0F * acid_level_step) + acid_level_curr + 500.0F))) 
                {
                    continue;
                }
            }
            if ((gSCManagerBattleState->gkind == nGRKindInishie) && (mpCollisionCheckExistPlatformLineID(line_ids[i]) != FALSE)) 
            {
                continue;
            }
            edge_dist_x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x - edge_pos.x;

            if (edge_dist_x > 0.0F)
            {
                edge_predict_x = (edge_dist_x / fp->attr->air_speed_max_x);
                fall_predict = -(-fp->attr->tvel_base - fp->physics.vel_air.y) / fp->attr->gravity;

                if (fall_predict <= 0)
                {
                    edge_predict_y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y + (fp->physics.vel_air.y * edge_predict_x);
                }
                else if (edge_predict_x < fall_predict)
                {
                    edge_predict_y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y +
                    (
                        (fp->physics.vel_air.y * edge_predict_x) - (fp->attr->gravity * SQUARE(edge_predict_x) * 0.5F)
                    );
                }
                else edge_predict_y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y +
                (
                    ((fp->physics.vel_air.y * edge_predict_x) - (fp->attr->gravity * SQUARE(fall_predict) * 0.5F)) -
                    (fp->attr->tvel_base * (edge_predict_x - fall_predict))
                );
                if ((is_find_edge_target == FALSE) && (edge_predict_y < (edge_pos.y - com->jump_predict)))
                {
                    continue;
                }
                if ((edge_predict_y < edge_pos.y) && ((edge_pos.y - edge_offset) < edge_predict_y)) 
                {
                    continue;
                }
                com->target_pos.x = edge_pos.x - 500.0F;
                com->target_pos.y = edge_pos.y;

                return TRUE;
            }
        }
    }
    return FALSE;
}

// 0x80134368
sb32 ftComputerCheckSetTargetEdgeLeft(FTStruct *fp, sb32 is_find_edge_target)
{
    FTComputer *com = &fp->computer;
    f32 edge_dist_x;
    f32 edge_predict_y;
    f32 edge_offset;
    s32 fall_predict;
    Vec3f edge_pos;
    f32 acid_level_curr;
    f32 acid_level_step;
    s32 edge_predict_x;
    u16 *line_ids;
    s32 i;

    if (fp->physics.vel_air.y >= 0.0F)
    {
        com->target_pos.x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x + 500.0F;
        com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

        return TRUE;
    }
    edge_offset = com->jump_predict * 0.75F;

    if (fp->fkind == nFTKindMMario)
    {
        edge_offset = 0;
    }
    if (gSCManagerBattleState->gkind == nGRKindYamabuki)
    {
        edge_offset = 0;
    }
    line_ids = &gMPCollisionLineGroups[nMPLineKindFloor].line_id[0];

    for (i = 0; i < gMPCollisionLineGroups[nMPLineKindFloor].line_count; i++)
    {
        if (mpCollisionCheckExistLineID(line_ids[i]) != FALSE)
        {
            mpCollisionGetFloorEdgeL(line_ids[i], &edge_pos);

            if (gSCManagerBattleState->gkind == nGRKindZebes)
            {
                grZebesAcidGetLevelInfo(&acid_level_curr, &acid_level_step);

                if ((edge_pos.y < (acid_level_curr + 500.0F)) || (edge_pos.y < ((5.0F * acid_level_step) + acid_level_curr + 500.0F))) 
                {
                    continue;
                }
            }
            if ((gSCManagerBattleState->gkind == nGRKindInishie) && (mpCollisionCheckExistPlatformLineID(line_ids[i]) != FALSE))
            {
                continue;
            }
            edge_dist_x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x - edge_pos.x;

            if (edge_dist_x < 0.0F)
            {
                edge_predict_x = (edge_dist_x / -fp->attr->air_speed_max_x);
                fall_predict = -(-fp->attr->tvel_base - fp->physics.vel_air.y) / fp->attr->gravity;

                if (fall_predict <= 0)
                {
                    edge_predict_y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y + (fp->physics.vel_air.y * edge_predict_x);
                }
                else if (edge_predict_x < fall_predict)
                {
                    edge_predict_y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y +
                    (
                        (fp->physics.vel_air.y * edge_predict_x) - (fp->attr->gravity * SQUARE(edge_predict_x) * 0.5F)
                    );
                }
                else edge_predict_y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y +
                (
                    ((fp->physics.vel_air.y * edge_predict_x) - (fp->attr->gravity * SQUARE(fall_predict) * 0.5F)) -
                    (fp->attr->tvel_base * (edge_predict_x - fall_predict))
                );
                if ((is_find_edge_target == FALSE) && (edge_predict_y < (edge_pos.y - com->jump_predict)))
                {
                    continue;
                }
                if ((edge_predict_y < edge_pos.y) && ((edge_pos.y - edge_offset) < edge_predict_y))
                {
                    continue;
                }
                com->target_pos.x = edge_pos.x + 500.0F;
                com->target_pos.y = edge_pos.y;

                return TRUE;
            }
        }
    }
    return FALSE;
}

// 0x801346D4
void func_ovl3_801346D4(FTStruct *fp)
{
    FTComputer *com = &fp->computer;
    Vec3f pos = fp->joints[nFTPartsJointTopN]->translate.vec.f;
    f32 range = fp->computer.jump_predict;

    if (fp->jumps_used == fp->attr->jumps_max)
    {
        switch (fp->fkind)
        {
        case nFTKindFox:
        case nFTKindDonkey:
        case nFTKindGDonkey:
            range *= 0.5F;
            break;

        case nFTKindNess:
            range = -com->jump_predict;
            break;
        }
    }
    if (fp->fkind == nFTKindMMario)
    {
        range = 0;
    }
    if (fp->fkind == nFTKindGDonkey)
    {
        range = 0;
    }
    if (pos.x > gMPCollisionBounds.current.right)
    {
        if (fp->lr > 0)
        {
            range *= 0.75F;
        }
        com->target_pos.x = pos.x - 1100.0F;

        if (pos.x < (com->cliff_right_pos.x + 300.0F))
        {
            range = 0.0F;
        }
        if (((fp->jumps_used < fp->attr->jumps_max) && ((pos.x - gMPCollisionBounds.current.right) > 1500.0F)) || ((fp->joints[nFTPartsJointTopN]->translate.vec.f.y + fp->physics.vel_air.y) < (com->cliff_right_pos.y - range)))
        {
            com->target_pos.y = pos.y + 1100.0F;
        }
        else com->target_pos.y = pos.y;
    }
    else
    {
        if (fp->lr < 0)
        {
            range *= 0.75F;
        }
        com->target_pos.x = pos.x + 1100.0F;

        if ((com->cliff_left_pos.x - 300.0F) < pos.x)
        {
            range = 0.0F;
        }
        if
        (
            (
                (fp->jumps_used < fp->attr->jumps_max) && 
                (
                    (gMPCollisionBounds.current.left - pos.x) > 1500.0F
                )
            ) 
            || 
            (
                (fp->joints[nFTPartsJointTopN]->translate.vec.f.y + fp->physics.vel_air.y) < (com->cliff_left_pos.y - range)
            )
        )
        {
            com->target_pos.y = pos.y + 1100.0F;
        }
        else com->target_pos.y = pos.y;
    }
#if defined(REGION_JP)
    if (fp->fkind == nFTKindPikachu)
    {
        switch (fp->status_id)
        {
            case nFTPikachuStatusSpecialAirHiStart:
                com->target_pos.x = pos.x;
                com->target_pos.y = pos.y + 1100.0F;
                break;
            case nFTPikachuStatusSpecialAirHi:
                com->target_pos.x = 0.0F;
                com->target_pos.y = pos.y;
                break;
        }
    }
#endif
}

// 0x80134964
void func_ovl3_80134964(FTStruct *fp)
{
    FTComputer *com = &fp->computer;
    Vec3f pos = fp->joints[nFTPartsJointTopN]->translate.vec.f;

    com->target_line_id = -1;
    com->ftcom_flags_0x4A_b1 = FALSE;

    if (!(com->is_within_vertical_bounds) && (pos.x <= gMPCollisionBounds.current.right) && (pos.x >= gMPCollisionBounds.current.left))
    {
        if (fp->physics.vel_air.x < 0.0F)
        {
            if
            (
                (ftComputerCheckSetTargetEdgeRight(fp, FALSE) != FALSE) ||
                (ftComputerCheckSetTargetEdgeLeft(fp, FALSE) != FALSE)  ||
                (ftComputerCheckSetTargetEdgeRight(fp, TRUE) != FALSE)  ||
                (ftComputerCheckSetTargetEdgeLeft(fp, TRUE) != FALSE)
            )
            {
                return;
            }
        }
        else if
        (
            (ftComputerCheckSetTargetEdgeLeft(fp, FALSE) != FALSE)  ||
            (ftComputerCheckSetTargetEdgeRight(fp, FALSE) != FALSE) ||
            (ftComputerCheckSetTargetEdgeLeft(fp, TRUE) != FALSE)   ||
            (ftComputerCheckSetTargetEdgeRight(fp, TRUE) != FALSE)
        )
        {
            return;
        }
        if ((pos.x + (pos.x < 0.0F)) != FALSE)
        {
            com->target_pos.x = 500.0F;
        }
        else com->target_pos.x = -500.0F;

        if ((pos.y + 1100.0F) > gMPCollisionGroundData->camera_bound_top)
        {
            com->target_pos.y = pos.y;
        }
        else com->target_pos.y = (pos.y + 1100.0F);
    }
    else func_ovl3_801346D4(fp);
}

// 0x80134B4C
sb32 ftComputerCheckTargetItemOrTwister(FTStruct *fp)
{
    FTComputer *com = &fp->computer;
    f32 predict_x;
    f32 predict_y;
    f32 dist_x;
    f32 dist_y;
    s32 unused;
    GObj *item_gobj;
    Vec3f twister_pos;

    item_gobj = gGCCommonLinks[nGCCommonLinkIDItem];

    predict_x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x + (fp->physics.vel_air.x * 5.0F);
    predict_y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y + (fp->physics.vel_air.y * 5.0F);

    while (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        if
        (
            (ip->owner_gobj != fp->fighter_gobj) &&
            (
                (gSCManagerBattleState->is_team_battle != TRUE) || (gSCManagerBattleState->is_team_attack != FALSE) || (ip->team != fp->team)
            )
            &&
            (
                (
                    (ip->kind == nITKindMSBomb) && (ip->is_damage_all == TRUE)
                )
                ||
                (
                    (ip->attack_coll.attack_state != nGMAttackStateOff) && (ip->attack_coll.attack_state != nGMAttackStateNew)
                )
                &&
                (ip->attack_coll.interact_mask & GMHITCOLLISION_FLAG_FIGHTER)
            )
        )
        {
            ITAttackColl *it_attack_coll = &ip->attack_coll;
            s32 attack_id = 0;

            if (it_attack_coll->attack_count > 0)
            {
                dist_x = (predict_x < it_attack_coll->attack_pos[attack_id].pos_curr.x) ? 
                        -(predict_x - it_attack_coll->attack_pos[attack_id].pos_curr.x) :    
                         (predict_x - it_attack_coll->attack_pos[attack_id].pos_curr.x) ;

                if (dist_x < it_attack_coll->size)
                {
                    dist_y = (predict_y < it_attack_coll->attack_pos[attack_id].pos_curr.y) ? 
                            -(predict_y - it_attack_coll->attack_pos[attack_id].pos_curr.y) : 
                             (predict_y - it_attack_coll->attack_pos[attack_id].pos_curr.y) ;

                    if (dist_y < it_attack_coll->size)
                    {
                        com->ftcom_flags_0x4A_b1 = FALSE;

                        if (fp->ga != nMPKineticsGround)
                        {
                            com->target_pos.x = (com->target_pos.x < it_attack_coll->attack_pos[attack_id].pos_curr.x) ?
                                                                (it_attack_coll->attack_pos[attack_id].pos_curr.x + 1500.0F) :
                                                                (it_attack_coll->attack_pos[attack_id].pos_curr.x - 1500.0F) ;

                            com->target_pos.y = it_attack_coll->attack_pos[attack_id].pos_curr.y;
                        }
                        else
                        {
                            com->target_pos.x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
                            com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y + 1100.0F;
                        }
                    }
                }
                return TRUE;
            }
        }
        item_gobj = item_gobj->link_next;
    }
    if (gSCManagerBattleState->gkind == nGRKindHyrule)
    {
        if (grHyruleTwisterCheckGetPosition(&twister_pos) != FALSE)
        {
            dist_x = (predict_x < twister_pos.x) ? 
                    -(predict_x - twister_pos.x) :
                     (predict_x - twister_pos.x) ;

            if (dist_x < 600)
            {
                dist_y = predict_y - twister_pos.y;

                if ((dist_y < 600.0F) && (dist_y > -300.0F))
                {
                    com->ftcom_flags_0x4A_b1 = FALSE;

                    if (fp->joints[nFTPartsJointTopN]->translate.vec.f.x < (twister_pos.x - 300.0F))
                    {
                        com->target_pos.x = (twister_pos.x - 1200.0F);
                        com->target_pos.y = (twister_pos.y + 1100.0F);
                    }
                    else
                    {
                        com->target_pos.x = (twister_pos.x + 1200.0F);
                        com->target_pos.y = (twister_pos.y + 1100.0F);
                    }
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

// 0x80134E98
void ftComputerFollowObjectiveWalk(FTStruct *fp)
{
    s32 unused;
    Vec3f target_pos;
    Vec3f ga_last;
    s32 stand_line_id;
    Vec3f edge_left;
    Vec3f edge_right;
    Vec3f sp9C;
    Vec3f sp90;
    FTStruct *target_fp;
    Vec3f sp80;
    Vec3f sp74;
    Vec3f sp68;
    f32 edge_dist_x;
    Vec3f sp58;
    FTComputer *com;

    com = &fp->computer;

    target_pos = fp->joints[nFTPartsJointTopN]->translate.vec.f;

    target_pos.x = fp->computer.target_pos.x;
    target_pos.y = fp->computer.target_pos.y;

    if ((fp->ga == nMPKineticsGround) && (fp->joints[nFTPartsJointTopN]->translate.vec.f.y < com->target_pos.y))
    {
        if (mpCollisionCheckCeilLineCollisionSame(&fp->joints[nFTPartsJointTopN]->translate.vec.f, &target_pos, &ga_last, &stand_line_id, NULL, NULL) != FALSE)
        {
            mpCollisionGetCeilEdgeL(stand_line_id, &edge_left);
            mpCollisionGetCeilEdgeR(stand_line_id, &edge_right);

            if (DISTANCE(edge_left.x, fp->joints[nFTPartsJointTopN]->translate.vec.f.x) < DISTANCE(edge_right.x, fp->joints[nFTPartsJointTopN]->translate.vec.f.x))
            {
                com->target_pos.x = edge_left.x - 600.0F;
                com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y;
            }
            else
            {
                com->target_pos.x = edge_right.x + 600.0F;
                com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y;
            }
        }
    }
    else if
    (
        !(fp->coll_data.floor_flags & MAP_VERTEX_COLL_PASS) &&
        (fp->coll_data.floor_line_id >= 0) &&
        (com->target_pos.y < ((fp->joints[nFTPartsJointTopN]->translate.vec.f.y + fp->coll_data.floor_dist) - 500.0F)) &&
        (DISTANCE(com->target_pos.x, fp->joints[nFTPartsJointTopN]->translate.vec.f.x) < (DISTANCE(com->target_pos.y, fp->joints[nFTPartsJointTopN]->translate.vec.f.y) * 0.2F))
    )
    {
        mpCollisionGetFloorEdgeL(fp->coll_data.floor_line_id, &sp9C);
        mpCollisionGetFloorEdgeR(fp->coll_data.floor_line_id, &sp90);

        if (DISTANCE(sp9C.x, fp->joints[nFTPartsJointTopN]->translate.vec.f.x) < DISTANCE(sp90.x, fp->joints[nFTPartsJointTopN]->translate.vec.f.x))
        {
            com->target_pos.x = sp9C.x - 600.0F;
            com->target_pos.y = sp9C.y;
        }
        else
        {
            com->target_pos.x = sp90.x + 600.0F;
            com->target_pos.y = sp90.y;
        }
    }
    else if ((fp->ga == nMPKineticsGround) || (fp->physics.vel_air.y < 0.0F) || (fp->fkind == nFTKindKirby) || (fp->fkind == nFTKindPurin))
    {
        if (fp->joints[nFTPartsJointTopN]->translate.vec.f.x < com->target_pos.x)
        {
            if (mpCollisionCheckLWallLineCollisionSame(&fp->joints[nFTPartsJointTopN]->translate.vec.f, &target_pos, &ga_last, &stand_line_id, NULL, NULL) != FALSE)
            {
                mpCollisionGetLWallEdgeU(stand_line_id, &ga_last);

                com->target_pos.x = ga_last.x + 100.0;
                com->target_pos.y = ga_last.y + 100.0;

                if ((fp->ga == nMPKineticsGround) && (fp->lr < 0.0F))
                {
                    com->target_pos.x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x + 100.0F;
                    com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

                    ftComputerSetCommandImmediate(fp, nFTComputerInputStickTiltAutoXD5);
                    return;
                }
            }
        }
        else if (mpCollisionCheckRWallLineCollisionSame(&fp->joints[nFTPartsJointTopN]->translate.vec.f, &target_pos, &ga_last, &stand_line_id, NULL, NULL) != FALSE)
        {
            mpCollisionGetRWallEdgeU(stand_line_id, &ga_last);

            com->target_pos.x = ga_last.x - 100.0;
            com->target_pos.y = ga_last.y + 100.0;

            if ((fp->ga == nMPKineticsGround) && (fp->lr > 0.0F))
            {
                com->target_pos.x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x - 100.0F;
                com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

                ftComputerSetCommandImmediate(fp, nFTComputerInputStickTiltAutoXD5);
                return;
            }
        }
    }
    ftComputerCheckTargetItemOrTwister(fp);
    ftComputerSetCommandImmediate(fp, nFTComputerInputMoveAuto);

    if (fp->ga != nMPKineticsGround)
    {
        switch (com->behavior)
        {
        case nFTComputerBehaviorYoshiTeam:
            if (fp->joints[nFTPartsJointTopN]->translate.vec.f.y < 0.0F)
            {
                break;
            }
            else goto block_52;

        case nFTComputerBehaviorKirbyTeam:
        case nFTComputerBehaviorPolyTeam:
            if (fp->joints[nFTPartsJointTopN]->translate.vec.f.y < -300.0F)
            {
                break;
            }
            /* fallthrough */
        default:
        block_52:
            if (fp->joints[nFTPartsJointTopN]->translate.vec.f.y < com->target_pos.y)
            {
                if ((com->objective != nFTComputerObjectiveRecover) && ((com->target_pos.y - 200.0F) < fp->joints[nFTPartsJointTopN]->translate.vec.f.y))
                {
                    com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y;
                    return;
                }
                com->ftcom_flags_0x4A_b0 = FALSE;

                if ((fp->joints[nFTPartsJointTopN]->translate.vec.f.y < (gMPCollisionGroundData->map_bound_top - 4000.0F)) && (fp->physics.vel_air.y < 0.0F))
                {
                    if (fp->jumps_used < fp->attr->jumps_max)
                    {
                        if
                        (
                            (fp->status_id == nFTCommonStatusDamageFlyHi)  ||
                            (fp->status_id == nFTCommonStatusDamageFlyN)   ||
                            (fp->status_id == nFTCommonStatusDamageFlyLw)  ||
                            (fp->status_id == nFTCommonStatusDamageFlyTop) ||
                            (fp->status_id == nFTCommonStatusDamageFlyRoll)
                        )
                        {
                            if (fp->status_vars.common.damage.hitstun_tics == 0)
                            {
                                ftComputerSetCommandImmediate(fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);
                                return;
                            }
                        }
                    }
                    if (fp->jumps_used < fp->attr->jumps_max)
                    {
                        if
                        (
                            (fp->status_id == nFTCommonStatusJumpF)        ||
                            (fp->status_id == nFTCommonStatusJumpB)        ||
                            (fp->status_id == nFTCommonStatusFall)         ||
                            (fp->status_id == nFTCommonStatusDamageFall)   ||
                            (fp->status_id == nFTCommonStatusFallSpecial)
                        )
                        {
                            ftComputerSetCommandImmediate(fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);
                            return;
                        }
                    }
                    if ((com->objective == nFTComputerObjectiveRecover) && !(com->is_attempt_specialhi_recovery))
                    {
                        if ((fp->fkind != nFTKindYoshi) && (fp->fkind != nFTKindPurin))
                        {
                            if
                            (
                                (fp->status_id == nFTCommonStatusJumpAerialF) ||
                                (fp->status_id == nFTCommonStatusJumpAerialB) ||
                                (fp->status_id == nFTCommonStatusFallAerial) ||
                                (fp->status_id == nFTCommonStatusDamageFall)
                            )
                            {
                                com->is_attempt_specialhi_recovery = TRUE;

                                if ((fp->fkind == nFTKindGDonkey) || (syUtilsRandFloat() < (fp->level + 2) / 9.0F))
                                {
                                    ftComputerSetCommandImmediate(fp, nFTComputerInputStickSmashHiButtonB);
                                    return;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (fp->physics.vel_air.y < 0.0F)
                {
                    if (com->objective != nFTComputerObjectiveRecover)
                    {
                        if (!(fp->is_fastfall) && !(com->ftcom_flags_0x4A_b0))
                        {
                            com->ftcom_flags_0x4A_b0 = TRUE;

                            if 
                            (
                                (fp->level > 5) && 
                                (syUtilsRandFloat() * (550 - (fp->level * 50)) < 70.0F) || 
                                (com->behavior == nFTComputerBehaviorYoshiTeam)             ||
                                (com->behavior == nFTComputerBehaviorKirbyTeam)             ||
                                (com->behavior == nFTComputerBehaviorPolyTeam)
                            )
                            {
                                // Fast fall?
                                ftComputerSetCommandWaitShort(fp, nFTComputerInputStickND1MoveAutoSmashLw);
                                return;
                            }
                        }
                    }

                }
            }
            switch (com->behavior)
            {
            case nFTComputerBehaviorYoshiTeam:
            case nFTComputerBehaviorKirbyTeam:
            case nFTComputerBehaviorPolyTeam:
                if (fp->status_id == nFTCommonStatusFall)
                {
                    if (func_ovl2_800F8FFC(&fp->joints[nFTPartsJointTopN]->translate.vec.f) != FALSE)
                    {
                        com->target_pos.x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
                        com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y - 500.0F;
                    }
                }
                break;
            }
            if
            (
                (fp->level >= 5)                                                                     && 
                (com->ftcom_flags_0x4A_b1)                                                           && 
                ((com->target_pos.y + 1100.0F) < fp->joints[nFTPartsJointTopN]->translate.vec.f.y)   && 
                (fp->physics.vel_air.y < 0.0F)
            )
            {
                target_fp = com->target_user;

                sp80.x = target_fp->joints[nFTPartsJointTopN]->translate.vec.f.x - (target_fp->lr * 1500.0F);
                sp80.y = target_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;
                sp80.z = 0.0F;

                if (func_ovl2_800F8FFC(&sp80) != FALSE)
                {
                    com->target_pos.x = sp80.x;
                    com->target_pos.y = sp80.y;
                }
                else
                {
                    sp80.x = target_fp->joints[nFTPartsJointTopN]->translate.vec.f.x + (target_fp->lr * 1500.0F);

                    if (func_ovl2_800F8FFC(&sp80) != FALSE)
                    {
                        com->target_pos.x = sp80.x;
                        com->target_pos.y = sp80.y;
                    }
                }
            }
            if (com->ftcom_flags_0x4A_b1)
            {
                if (com->target_line_id >= 0)
                {
                    mpCollisionGetFloorEdgeL(com->target_line_id, &sp74);
                    mpCollisionGetFloorEdgeR(com->target_line_id, &sp68);

                    if ((sp74.x <= com->target_pos.x) && (sp68.x >= com->target_pos.x))
                    {
                        if (com->target_pos.x < (sp74.x + 200.0F))
                        {
                            com->target_pos.x = (sp74.x + 200.0F);
                        }
                        if (com->target_pos.x > (sp68.x - 200.0F))
                        {
                            com->target_pos.x = (sp68.x - 200.0F);
                        }
                    }
                }
            }
            ftComputerSetCommandImmediate(fp, nFTComputerInputMoveAuto);
            return;
        }
    }
    else
    {
        if (com->dash_predict <= DISTANCE(com->target_pos.x, fp->joints[nFTPartsJointTopN]->translate.vec.f.x))
        {
            if (fp->joints[nFTPartsJointTopN]->translate.vec.f.y < com->target_pos.y)
            {
                if (fp->coll_data.floor_line_id >= 0)
                {
                    if (fp->physics.vel_air.x < 0.0F)
                    {
                        mpCollisionGetFloorEdgeL(fp->coll_data.floor_line_id, &sp58);
                    }
                    else mpCollisionGetFloorEdgeR(fp->coll_data.floor_line_id, &sp58);

                    if (sp58.x < fp->joints[nFTPartsJointTopN]->translate.vec.f.x)
                    {
                        edge_dist_x = -(sp58.x - fp->joints[nFTPartsJointTopN]->translate.vec.f.x);
                    }
                    else edge_dist_x = (sp58.x - fp->joints[nFTPartsJointTopN]->translate.vec.f.x);
                }
                else edge_dist_x = 0.0F;

                if ((edge_dist_x - ABSF(fp->physics.vel_air.x)) < 200.0F)
                {
                    if (fp->status_id != nFTCommonStatusKneeBend)
                    {
                        ftComputerSetCommandWaitShort(fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);
                    }
                }
                else ftComputerSetCommandImmediate(fp, nFTComputerInputMoveAuto);
            }
            else ftComputerSetCommandImmediate(fp, nFTComputerInputMoveAuto);
        }
        else if (fp->status_id != nFTCommonStatusDash)
        {
            if (com->target_line_id != fp->coll_data.floor_line_id)
            {
                if (fp->joints[nFTPartsJointTopN]->translate.vec.f.y < com->target_pos.y)
                {
                    if (fp->status_id != nFTCommonStatusKneeBend)
                    {
                        ftComputerSetCommandWaitShort(fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);
                    }
                }
                else if (fp->coll_data.floor_flags & MAP_VERTEX_COLL_PASS)
                {
                    ftComputerSetCommandWaitShort(fp, nFTComputerInputStickND1MoveAutoSmashLw);
                }
            }
            else ftComputerSetCommandImmediate(fp, nFTComputerInputMoveAuto);
        }
        else ftComputerSetCommandImmediate(fp, nFTComputerInputMoveAuto);
    }
}

// 0x80135B78
sb32 func_ovl3_80135B78(FTStruct *this_fp)
{
    GObj *fighter_gobj;
    GObj *weapon_gobj;
    GObj *item_gobj;
    f32 attack_size;
    f32 predict_vel_x;
    f32 predict_div_x;
    f32 this_pos_x;
    f32 this_pos_y;
    f32 predict_pos_x;
    f32 predict_pos_y;
    WPStruct *wp;
    ITStruct *ip;
    FTStruct *other_fp;
    FTComputer *com;
    WPAttackColl *wp_attack_coll;
    ITAttackColl *it_attack_coll;
    ITAttackPos *it_attack_collpos;
    WPAttackPos *wp_attack_collpos;
    s32 i;

    com = &this_fp->computer;

    this_pos_x = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
    this_pos_y = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

    fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (fighter_gobj != NULL)
    {
        if (this_fp->fighter_gobj == fighter_gobj) goto next_fighter;

        other_fp = ftGetStruct(fighter_gobj);

        if (this_fp->team == ftGetStruct(fighter_gobj)->team) goto next_fighter;

        if (other_fp->fkind == nFTKindDonkey)
        {
            if (other_fp->passive_vars.donkey.charge_level == FTDONKEY_GIANTPUNCH_CHARGE_MAX)
            {
                if (this_fp->level && this_fp->level); // WTF???
            }
        }
    next_fighter:
        fighter_gobj = fighter_gobj->link_next;
    }
    weapon_gobj = gGCCommonLinks[nGCCommonLinkIDWeapon];

    while (weapon_gobj != NULL)
    {
        wp = wpGetStruct(weapon_gobj);

        if (wp->owner_gobj != this_fp->fighter_gobj)
        {
            if ((gSCManagerBattleState->is_team_battle != TRUE) || (gSCManagerBattleState->is_team_attack != FALSE) || (wp->team != this_fp->team))
            {
                if ((wp->attack_coll.attack_state != nGMAttackStateOff) && (wp->attack_coll.attack_state != nGMAttackStateNew) && (wp->attack_coll.interact_mask & GMHITCOLLISION_FLAG_FIGHTER))
                {
                    wp_attack_coll = &wp->attack_coll;

                    for (i = 0; i < wp_attack_coll->attack_count; i++)
                    {
                        predict_vel_x = (wp->physics.vel_air.x - this_fp->physics.vel_air.x) * wp->lr;

                        if (predict_vel_x > 0.0F)
                        {
                            wp_attack_collpos = &wp_attack_coll->attack_pos[i];

                            predict_pos_x = (this_pos_x - wp_attack_coll->attack_pos[i].pos_curr.x) * wp->lr;

                            attack_size = wp_attack_coll->size * 0.5F;

                            predict_pos_x -= (this_fp->damage_coll_size.x + attack_size);

                            if (predict_pos_x > 0.0F)
                            {
                                predict_div_x = predict_pos_x / predict_vel_x;

                                if (predict_div_x < 15.0F)
                                {
                                    predict_pos_y = (this_fp->ga != nMPKineticsGround) ? (this_fp->physics.vel_air.y * predict_div_x) + this_pos_y : this_pos_y;

                                    if ((((wp_attack_coll->attack_pos[i].pos_curr.y - attack_size) - this_fp->damage_coll_size.y) < predict_pos_y) && (predict_pos_y < (wp_attack_coll->attack_pos[i].pos_curr.y + attack_size)))
                                    {
                                        com->target_pos.y = predict_pos_y;
                                        com->unk_ftcom_0x38 = predict_div_x;

                                        if (syUtilsRandFloat() < ((this_fp->level + 2) / 9.0F))
                                        {
                                            if (!(com->ftcom_flags_0x49_b2))
                                            {
                                                if
                                                (
                                                    (this_fp->fkind == (0,nFTKindFox)) || // WTF????????????????????????
                                                    (this_fp->fkind == nFTKindNess)    ||
                                                    (this_fp->fkind == nFTKindNFox)    ||
                                                    (this_fp->fkind == nFTKindNNess)
                                                )
                                                {
                                                    com->is_opponent_ra = TRUE;
                                                }
                                            }
                                            return TRUE;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    next_weapon:
        weapon_gobj = weapon_gobj->link_next;
    }
    item_gobj = gGCCommonLinks[nGCCommonLinkIDItem];

    while (item_gobj != NULL)
    {
        ip = itGetStruct(item_gobj);

        if (ip->owner_gobj != this_fp->fighter_gobj)
        {
            if ((gSCManagerBattleState->is_team_battle != TRUE) || (gSCManagerBattleState->is_team_attack != FALSE) || (ip->team != this_fp->team))
            {
                if ((ip->attack_coll.attack_state != nGMAttackStateOff) && (ip->attack_coll.attack_state != nGMAttackStateNew) && (ip->attack_coll.interact_mask & GMHITCOLLISION_FLAG_FIGHTER))
                {
                    it_attack_coll = &ip->attack_coll;

                    for (i = 0; i < it_attack_coll->attack_count; i++)
                    {
                        predict_vel_x = (ip->physics.vel_air.x - this_fp->physics.vel_air.x) * ip->lr;

                        if (predict_vel_x > 0.0F)
                        {
                            predict_pos_x = (this_pos_x - it_attack_coll->attack_pos[i].pos_curr.x) * ip->lr;

                            attack_size = it_attack_coll->size * 0.5F;

                            predict_pos_x -= (this_fp->damage_coll_size.x + attack_size);

                            if (predict_pos_x > 0.0F)
                            {
                                predict_div_x = predict_pos_x / predict_vel_x;

                                if (predict_div_x < 15.0F)
                                {
                                    predict_pos_y = (this_fp->ga != nMPKineticsGround) ? (this_fp->physics.vel_air.y * predict_div_x) + this_pos_y : this_pos_y;

                                    if ((((it_attack_coll->attack_pos[i].pos_curr.y - attack_size) - this_fp->damage_coll_size.y) < predict_pos_y) && (predict_pos_y < (it_attack_coll->attack_pos[i].pos_curr.y + attack_size)))
                                    {
                                        com->target_pos.y = predict_pos_y;
                                        com->unk_ftcom_0x38 = predict_div_x;

                                        if (syUtilsRandFloat() < ((this_fp->level + 2) / 9.0F))
                                        {
                                            if (!(com->ftcom_flags_0x49_b2))
                                            {
                                                if
                                                (
                                                    (this_fp->fkind == (0, nFTKindFox))  || // I AM LOSING MY MIND
                                                    (this_fp->fkind == nFTKindNess)      ||
                                                    (this_fp->fkind == nFTKindNFox)   ||
                                                    (this_fp->fkind == nFTKindNNess)
                                                )
                                                {
                                                    com->is_opponent_ra = TRUE;
                                                }
                                            }
                                            return TRUE;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    next_item:
        item_gobj = item_gobj->link_next;
    }
    return FALSE;
}

// 0x801361BC
void func_ovl3_801361BC(FTStruct *fp)
{
    FTComputer *com = &fp->computer;
    s32 var_v1;

    com->target_line_id = -1;
    com->ftcom_flags_0x4A_b1 = FALSE;
    com = &fp->computer;

    if (!(com->is_counterattack) && !(com->is_opponent_ra))
    {
        if (fp->ga != nMPKineticsGround)
        {
            com->target_pos.x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x;

            if (((fp->joints[nFTPartsJointTopN]->translate.vec.f.y + fp->damage_coll_size.y) < com->target_pos.y) && (fp->physics.vel_air.y <= 0.0F))
            {
                com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y - 1100.0F;
            }
            else com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y + 1100.0F;
        }
        else
        {
            switch (fp->fkind)
            {
            default:
                var_v1 = 7;
                break;

            case nFTKindDonkey:
            case nFTKindNDonkey:
            case nFTKindGDonkey:
                var_v1 = 11;
                break;

            case nFTKindMMario:
                var_v1 = 20;
                break;
            }
            if ((com->unk_ftcom_0x38 < var_v1) || (fp->status_id == nFTCommonStatusRun) || (fp->status_id == nFTCommonStatusDash))
            {
                com->is_shield_item_weapon = TRUE;
            }
            else
            {
                com->target_pos.x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
                com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y + 1100.0F;
            }
        }
    }
}

// 0x80136310
sb32 ftComputerCheckFindItem(FTStruct *fp)
{
    FTComputer *com = &fp->computer;
    GObj *item_gobj;
    f32 nearest_dist;
    f32 ft_pos_x;
    f32 ft_pos_y;

    if (fp->item_gobj != NULL)
    {
        fp->computer.target_line_id = -1;

        return FALSE;
    }
    nearest_dist = F32_MAX;

    ft_pos_x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
    ft_pos_y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

    com->ftcom_flags_0x4A_b1 = FALSE;

    item_gobj = gGCCommonLinks[nGCCommonLinkIDItem];

    while (item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(item_gobj);

        if (ip->owner_gobj == fp->fighter_gobj) goto next_item;

        if ((gSCManagerBattleState->is_team_battle == TRUE) && (gSCManagerBattleState->is_team_attack == FALSE) && (ip->team == fp->team)) goto next_item;

        if ((ip->is_allow_pickup) && (func_ovl2_800F8FFC(&DObjGetStruct(item_gobj)->translate.vec.f) != FALSE))
        {
            f32 it_pos_x = DObjGetStruct(item_gobj)->translate.vec.f.x;
            f32 it_pos_y = DObjGetStruct(item_gobj)->translate.vec.f.y;

            if ((it_pos_x <= gMPCollisionBounds.current.right) && (it_pos_x >= gMPCollisionBounds.current.left) && (it_pos_y >= gMPCollisionBounds.current.bottom) && (it_pos_y < gMPCollisionGroundData->camera_bound_top))
            {
                f32 current_dist = SQUARE(ft_pos_x - it_pos_x) + SQUARE(ft_pos_y - it_pos_y);

                if (nearest_dist > current_dist)
                {
                    com->target_pos.x = it_pos_x;
                    com->target_pos.y = it_pos_y;

                    com->target_user = ip;

                    nearest_dist = current_dist;
                }
            }
        }
    next_item:
        item_gobj = item_gobj->link_next;
    }
    if (nearest_dist == F32_MAX)
    {
        com->target_line_id = -1;

        return FALSE;
    }
    com->target_dist = sqrtf(nearest_dist);

    com->target_line_id = (ftGetComTargetItem(com)->ga == nMPKineticsGround) ? ftGetComTargetItem(com)->coll_data.floor_line_id : -1;

    return TRUE;
}

// 0x80136550
sb32 ftComputerCheckTargetItemInRange(FTStruct *fp)
{
    FTItemPickup *item_pickup = &fp->attr->item_pickup;
    ITStruct *ip = ftGetComTargetItem(&fp->computer);
    Vec3f *ft_pos = &DObjGetStruct(fp->fighter_gobj)->translate.vec.f;
    Vec3f *it_pos = &DObjGetStruct(ip->item_gobj)->translate.vec.f;
    MPObjectColl *map_coll = &ip->coll_data.map_coll;

    if (ip->weight == nITWeightLight)
    {
        Vec2f range;

        range.x = ft_pos->x + (fp->lr * item_pickup->pickup_offset_light.x);
        range.y = ft_pos->y + item_pickup->pickup_offset_light.y;

        if ((((range.x - item_pickup->pickup_range_light.x) - map_coll->width) < it_pos->x) && (((item_pickup->pickup_range_light.x + range.x) + map_coll->width) > it_pos->x))
        {
            if ((((range.y - item_pickup->pickup_range_light.y) - map_coll->top) < it_pos->y) && (((item_pickup->pickup_range_light.y + range.y) - map_coll->bottom) > it_pos->y))
            {
                return TRUE;
            }
        }
    }
    else
    {
        Vec2f range;

        range.x = ft_pos->x + (fp->lr * item_pickup->pickup_offset_heavy.x);
        range.y = ft_pos->y + item_pickup->pickup_offset_heavy.y;

        if ((((range.x - item_pickup->pickup_range_heavy.x) - map_coll->width) < it_pos->x) && (((item_pickup->pickup_range_heavy.x + range.x) + map_coll->width) > it_pos->x))
        {
            if ((((range.y - item_pickup->pickup_range_heavy.y) - map_coll->top) < it_pos->y) && (((item_pickup->pickup_range_heavy.y + range.y) - map_coll->bottom) > it_pos->y))
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

// 0x801366F0
sb32 ftComputerCheckSetEvadeTarget(FTStruct *this_fp)
{
    FTComputer *com;
    GObj *fighter_gobj;
    f32 current_dist;
    f32 nearest_dist;
    f32 this_pos_x;
    f32 predict_x;
    f32 predict_y;
    f32 this_pos_y;
    FTStruct *target_fp;
    Vec3f edge_pos;
    s32 unused;
    s32 line_id;

    com = &this_fp->computer;

    nearest_dist = 6250000.0F;

    this_pos_x = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
    this_pos_y = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

    com->ftcom_flags_0x4A_b1 = FALSE;

    fighter_gobj = gGCCommonLinks[nGCCommonLinkIDFighter];

    while (fighter_gobj != NULL)
    {
        if (fighter_gobj == this_fp->fighter_gobj) goto next_fighter;

        target_fp = ftGetStruct(fighter_gobj);

        if (this_fp->team == target_fp->team) goto next_fighter;

        predict_x = target_fp->joints[nFTPartsJointTopN]->translate.vec.f.x + (target_fp->physics.vel_air.x * 3.0F);
        predict_y = target_fp->joints[nFTPartsJointTopN]->translate.vec.f.y + (target_fp->physics.vel_air.x * 3.0F);

        if ((target_fp->star_hitstatus == nGMHitStatusInvincible) || ((target_fp->item_gobj != NULL) && (itGetStruct(target_fp->item_gobj)->kind == nITKindHammer)))
        {
            com->target_user = target_fp;

            nearest_dist = SQUARE(this_pos_x - predict_x) + SQUARE(this_pos_y - predict_y);

            break;
        }
        else if ((target_fp->status_id >= nFTCommonStatusWait) && (predict_x <= gMPCollisionBounds.current.right) && (predict_x >= gMPCollisionBounds.current.left) && (predict_y >= gMPCollisionBounds.current.bottom))
        {
            current_dist = SQUARE(this_pos_x - predict_x) + SQUARE(this_pos_y - predict_y);

            if (nearest_dist > current_dist)
            {
                com->target_user = target_fp;

                nearest_dist = current_dist;
            }
        }
    next_fighter:
        fighter_gobj = fighter_gobj->link_next;
    }
    if (nearest_dist == 6250000.0F)
    {
        com->target_pos.x = this_pos_x;
        com->target_pos.y = this_pos_y;
        com->target_line_id = -1;

        return TRUE;
    }
    else
    {
        target_fp = com->target_user;

        predict_x = target_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
        predict_y = target_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

        if (this_fp->coll_data.floor_line_id >= 0)
        {
            line_id = this_fp->coll_data.floor_line_id;

            if (this_pos_x < predict_x)
            {
                mpCollisionGetFloorEdgeL(line_id, &edge_pos);
            }
            else mpCollisionGetFloorEdgeR(line_id, &edge_pos);
        }
        else edge_pos = this_fp->joints[nFTPartsJointTopN]->translate.vec.f;

        if (target_fp->ga == nMPKineticsGround)
        {
            com->target_pos.y = predict_y;
        }
        else com->target_pos.y = this_pos_y;

        if (this_pos_x < predict_x)
        {
            com->target_pos.x = predict_x - 2500.0F;

            if (com->target_pos.x < edge_pos.x)
            {
                com->target_pos.x = predict_x + 2500.0F;

                ftComputerSetCommandWaitShort(this_fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);

                return FALSE;
            }
        }
        else
        {
            com->target_pos.x = predict_x + 2500.0F;

            if (com->target_pos.x > edge_pos.x)
            {
                com->target_pos.x = predict_x - 2500.0F;

                ftComputerSetCommandWaitShort(this_fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);

                return FALSE;
            }
        }
        com->target_dist = sqrtf(nearest_dist);
        com->target_line_id = -1;
        com->objective = nFTComputerObjectiveEvade;
    }
    return TRUE;
}

// 0x80136A20
sb32 ftComputerCheckTryChargeSpecialN(FTStruct *fp)
{
    switch (fp->fkind)
    {
    case nFTKindDonkey:
        if
        (
            (fp->status_id != nFTDonkeyStatusSpecialNStart)    &&
            (fp->status_id != nFTDonkeyStatusSpecialAirNStart) &&
            (fp->status_id != nFTDonkeyStatusSpecialNLoop)     &&
            (fp->status_id != nFTDonkeyStatusSpecialAirNLoop)  &&
            (fp->status_id != nFTDonkeyStatusSpecialLwStart)   &&
            (fp->status_id != nFTDonkeyStatusSpecialLwLoop)    &&
            (fp->status_id != nFTDonkeyStatusSpecialLwEnd)     &&
            (fp->passive_vars.donkey.charge_level < FTDONKEY_GIANTPUNCH_CHARGE_MAX)
        )
        {
            ftComputerSetCommandWaitShort(fp, 0xB);
            return TRUE;
        }
        break;

    case nFTKindGDonkey:
        switch (fp->status_id)
        {
        case nFTCommonStatusWait:
        case nFTCommonStatusWalkSlow:
        case nFTCommonStatusWalkMiddle:
        case nFTCommonStatusWalkFast:
        case nFTCommonStatusDash:
        case nFTCommonStatusRun:
        case nFTCommonStatusRunBrake:
        case nFTCommonStatusSquat:
        case nFTCommonStatusSquatWait:
        case nFTCommonStatusSquatRv:
        case nFTCommonStatusLandingLight:
        case nFTCommonStatusOttottoWait:
        case nFTCommonStatusOttotto:
            if (fp->passive_vars.donkey.charge_level < FTDONKEY_GIANTPUNCH_CHARGE_MAX)
            {
                ftComputerSetCommandWaitShort(fp, 0xB);
                return TRUE;
            }
            break;
        }
        break;

    case nFTKindSamus:
        if
        (
            (fp->status_id != nFTSamusStatusSpecialNStart)     &&
            (fp->status_id != nFTSamusStatusSpecialAirNStart)  &&
            (fp->status_id != nFTSamusStatusSpecialNLoop)      &&
            (fp->status_id != nFTSamusStatusSpecialAirNEnd)    &&
            (fp->passive_vars.samus.charge_level < FTSAMUS_CHARGE_MAX)
        )
        {
            ftComputerSetCommandWaitShort(fp, 0xB);
            return TRUE;
        }
        break;

    case nFTKindKirby:
        switch (fp->passive_vars.kirby.copy_id)
        {
        case nFTKindDonkey:
            if
            (
                (fp->status_id != nFTKirbyStatusCopyDonkeySpecialNStart)      &&
                (fp->status_id != nFTKirbyStatusCopyDonkeySpecialAirNStart)   &&
                (fp->status_id != nFTKirbyStatusCopyDonkeySpecialNLoop)       &&
                (fp->status_id != nFTKirbyStatusCopyDonkeySpecialAirNLoop)    &&
                (fp->passive_vars.kirby.copydonkey_charge_level < FTKIRBY_COPYDONKEY_GIANTPUNCH_CHARGE_MAX)
            )
            {
                ftComputerSetCommandWaitShort(fp, 0xB);
                return TRUE;
            }
            break;

        case nFTKindSamus:
            if
            (
                (fp->status_id != nFTKirbyStatusCopySamusSpecialNStart)   &&
                (fp->status_id != nFTKirbyStatusCopySamusSpecialAirNStart)&&
                (fp->status_id != nFTKirbyStatusCopySamusSpecialNLoop)    &&
                (fp->status_id != nFTKirbyStatusCopySamusSpecialAirNEnd)  &&
                (fp->passive_vars.kirby.copysamus_charge_level < FTKIRBY_COPYSAMUS_CHARGE_MAX)
            )
            {
                ftComputerSetCommandWaitShort(fp, 0xB);
                return TRUE;
            }
            break;
        }
        break;
    }
    return FALSE;
}

// 0x80136C0C
sb32 ftComputerCheckTryCancelSpecialN(FTStruct *fp)
{
    switch ((fp->fkind == nFTKindKirby) ? fp->passive_vars.kirby.copy_id : fp->fkind)
    {
    case nFTKindDonkey:
    case nFTKindGDonkey:
        if
        (
            (fp->status_id == nFTDonkeyStatusSpecialNStart)                ||
            (fp->status_id == nFTDonkeyStatusSpecialAirNStart)             ||
            (fp->status_id == nFTDonkeyStatusSpecialNLoop)                 ||
            (fp->status_id == nFTDonkeyStatusSpecialAirNLoop)              ||
            (fp->status_id == nFTKirbyStatusCopyDonkeySpecialNStart)      ||
            (fp->status_id == nFTKirbyStatusCopyDonkeySpecialAirNStart)   ||
            (fp->status_id == nFTKirbyStatusCopyDonkeySpecialNLoop)       ||
            (fp->status_id == nFTKirbyStatusCopyDonkeySpecialAirNLoop)
        )
        {
            ftComputerSetCommandWaitShort(fp, 0xC);
            return TRUE;
        }
        break;

    case nFTKindSamus:
        if
        (
            (fp->status_id == nFTSamusStatusSpecialNStart)             ||
            (fp->status_id == nFTSamusStatusSpecialAirNStart)          ||
            (fp->status_id == nFTSamusStatusSpecialNLoop)              ||
            (fp->status_id == nFTSamusStatusSpecialAirNEnd)            ||
            (fp->status_id == nFTKirbyStatusCopySamusSpecialNStart)   ||
            (fp->status_id == nFTKirbyStatusCopySamusSpecialAirNStart)||
            (fp->status_id == nFTKirbyStatusCopySamusSpecialNLoop)    ||
            (fp->status_id == nFTKirbyStatusCopySamusSpecialAirNEnd)
        )
        {
            ftComputerSetCommandWaitShort(fp, 0xC);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// 0x80136D0C - 0 = Do not process current objective, 1 = new objective has been given out, -1 = process current objective
s32 ftComputerGetObjectiveStatus(GObj *this_gobj)
{
    FTStruct *this_fp = ftGetStruct(this_gobj);
    FTComputer *com = &this_fp->computer;
    s32 action_wait;
    Vec3f tarucann_pos;
    f32 acid_level_curr;
    f32 acid_level_step;
    f32 fvar;

    if (this_fp->status_id == nFTCommonStatusCliffWait)
    {
        action_wait = (this_fp->status_vars.common.cliffwait.fall_wait > 480) ?
                      (1080- this_fp->status_vars.common.cliffwait.fall_wait) :
                      (480 - this_fp->status_vars.common.cliffwait.fall_wait) ;

        if (syUtilsRandFloat() < 0.01F)
        {
            action_wait *= 2;
        }
        if (((FTCOMPUTER_LEVEL_MAX - this_fp->level) * 15) < action_wait)
        {
            if (syUtilsRandFloat() < 0.4F)
            {
                ftComputerSetCommandWaitShort(this_fp, 0x28);

                return 0;
            }
            else if (syUtilsRandFloat() < 0.5F)
            {
                ftComputerSetCommandWaitShort(this_fp, nFTComputerInputButtonZ1);

                return 0;
            }
            else ftComputerSetCommandWaitShort(this_fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);

            return 0;
        }
    }
    if ((this_fp->status_id == nFTCommonStatusDownWaitD) || (this_fp->status_id == nFTCommonStatusDownWaitU))
    {
        action_wait = 180 - this_fp->status_vars.common.downwait.stand_wait;

        if (((FTCOMPUTER_LEVEL_MAX - this_fp->level) * 25) < action_wait)
        {
            ftComputerCheckFindTarget(this_fp);

            if ((this_fp->fkind != nFTKindGDonkey) && (com->target_dist < 800.0F) && (this_fp->level >= 4))
            {
                if ((syUtilsRandFloat() * (11 - this_fp->level)) < 1.0F)
                {
                    if (com->target_pos.x < this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x)
                    {
                        ftComputerSetCommandWaitShort(this_fp, nFTComputerInputEscapeL);

                        return 0;
                    }
                    ftComputerSetCommandWaitShort(this_fp, nFTComputerInputEscapeR);

                    return 0;
                }
            }
            ftComputerSetCommandWaitShort(this_fp, nFTComputerInputStickNButtonBZReleaseAPress);

            return 0;
        }
    }
    if 
    (
        (this_fp->status_id == nFTCommonStatusOttottoWait) ||
        (this_fp->status_id == nFTCommonStatusOttotto)     ||
        (this_fp->status_id == nFTCommonStatusSquatWait)
    )
    {
        ftComputerSetCommandWaitShort(this_fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);

        return 0;
    }
    if (this_fp->status_id == nFTCommonStatusTaruCann)
    {
        grJungleTaruCannGetPosition(&tarucann_pos);

        tarucann_pos.y = 2000.0F;

        if (func_ovl2_800F8FFC(&tarucann_pos) != FALSE)
        {
            fvar = ((grJungleTaruCannGetRotate() < 0.0F) ? -grJungleTaruCannGetRotate() : grJungleTaruCannGetRotate());

            if (fvar < 0.05F)
            {
                ftComputerSetCommandImmediate(this_fp, nFTComputerInputStickNButtonBZReleaseAPress);

                return 0;
            }
        }
    }
    if (this_fp->status_id == nFTCommonStatusCatchWait)
    {
        if (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x < 0.0F)
        {
            ftComputerSetCommandImmediate(this_fp, nFTComputerInputStickSmashL);
            return 0;
        }
        else ftComputerSetCommandImmediate(this_fp, nFTComputerInputStickSmashR);

        return 0;
    }

    if
    (
        (this_fp->status_id == nFTCommonStatusCaptureWaitKirby) ||
        (this_fp->status_id == nFTCommonStatusYoshiEgg)         ||
        (this_fp->status_id == nFTCommonStatusFuraFura)         ||
        (this_fp->status_id == nFTCommonStatusFuraSleep)        ||
        (this_fp->status_id == nFTCommonStatusShouldered)
    )
    {
        com->target_find_wait++;

        if (((FTCOMPUTER_LEVEL_MAX - this_fp->level) * 15) < com->target_find_wait)
        {
            ftComputerSetCommandWaitShort(this_fp, nFTComputerInputWiggle);

            return 0;
        }
    }
    else com->target_find_wait = 0;

    if (this_fp->fkind == nFTKindKirby)
    {
        if
        (
            (this_fp->status_id == nFTKirbyStatusSpecialNCatch)    ||
            (this_fp->status_id == nFTKirbyStatusSpecialAirNCatch) ||
            (this_fp->status_id == nFTKirbyStatusSpecialNEat)      ||
            (this_fp->status_id == nFTKirbyStatusSpecialAirNEat)
        )
        {
            ftComputerSetCommandImmediate(this_fp, nFTComputerInputStickN);
            return 0;
        }
        else if ((this_fp->status_id == nFTKirbyStatusSpecialNWait) || (this_fp->status_id == nFTKirbyStatusSpecialAirNWait))
        {
            ftComputerSetCommandWaitShort(this_fp, nFTComputerInputStickSmashLwButtonB);

            return 0;
        }
    }
    if (this_fp->fkind == nFTKindNess)
    {
        if ((this_fp->status_id == nFTNessStatusSpecialHiHold) || (this_fp->status_id == nFTNessStatusSpecialAirHiHold))
        {
            if (func_ovl2_800F8FFC(&this_fp->joints[nFTPartsJointTopN]->translate.vec.f) == FALSE)
            {
                com->target_pos.x = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;

                if (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x > 0.0F)
                {
                    com->target_pos.x += 200.0F;
                }
                else com->target_pos.x -= 200.0F;

                com->target_pos.y = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y - 100.0F;
            }
            else ftComputerCheckFindTarget(this_fp);

            ftComputerSetCommandImmediate(this_fp, nFTComputerInputNessSpecialHiAim);

            return 0;
        }
    }
    if (this_fp->ga == nMPKineticsGround)
    {
        if
        (
            (
                SQUARE(this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x - com->stand_pos.x) + 
                SQUARE(this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y - com->stand_pos.y) > SQUARE(100.0F)
            ) 
            ||
            (com->behavior == nFTComputerBehaviorStand)
        )
        {
            com->stand_pos.x = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
            com->stand_pos.y = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;
            com->stand_stop_wait = 0;
            com->is_stop_stand = FALSE;
        }
        else
        {
            com->stand_stop_wait++;

            if (com->stand_stop_wait > 300)
            {
                com->is_stop_stand = TRUE;
            }
        }
    }
    else
    {
        com->stand_stop_wait = 0;
        com->is_stop_stand = FALSE;
    }
    if (com->is_stop_stand)
    {
        ftComputerSetCommandImmediate(this_fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);

        return 0;
    }
    if (this_fp->percent_damage != 0)
    {
        switch (com->behavior)
        {
        case nFTComputerBehaviorYoshiTeam:
            if (((this_fp->level - 4) * 0.01F) <= syUtilsRandFloat())
            {
                return -1;
            }
            /* fallthrough */
        case nFTComputerBehaviorKirbyTeam:
            if (((this_fp->level - 3) * 0.03F) <= syUtilsRandFloat())
            {
                return -1;
            }
            break;

        case nFTComputerBehaviorPolyTeam:
            if ((this_fp->level * 0.04F) <= syUtilsRandFloat())
            {
                return -1;
            }
            break;
        }
    }
    if
    (
        (func_ovl2_800F8FFC(&this_fp->joints[nFTPartsJointTopN]->translate.vec.f) == FALSE) ||
        (gSCManagerBattleState->gkind == nGRKindInishie) &&
        (this_fp->coll_data.floor_line_id >= 0) &&
        (mpCollisionCheckExistPlatformLineID(this_fp->coll_data.floor_line_id) != FALSE) &&
        (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y < -100.0F)
    )
    {
        if (com->objective != nFTComputerObjectiveRecover)
        {
            com->is_attempt_specialhi_recovery = FALSE;
        }
        com->objective = nFTComputerObjectiveRecover;

        return 1;
    }
    if (gSCManagerBattleState->gkind == nGRKindZebes)
    {
        grZebesAcidGetLevelInfo(&acid_level_curr, &acid_level_step);

        if
        (
            (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y < (acid_level_curr + 500.0F)) ||
            (this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y < (acid_level_curr + (5.0F * acid_level_step) + 500.0F))
        )
        {
            com->objective = nFTComputerObjectiveRecover;

            return 1;
        }
    }
    com->is_within_vertical_bounds = FALSE;

    if (com->objective == nFTComputerObjectiveRecover)
    {
        com->objective = nFTComputerObjectiveWalk;
    }
    if ((this_fp->status_id == nFTCommonStatusDamageFall) && (((this_fp->physics.vel_air.y * 5.0F) + -this_fp->coll_data.floor_dist) <= 0.0F))
    {
        if (!(com->ftcom_flags_0x49_b3))
        {
            com->ftcom_flags_0x49_b3 = TRUE;

            if (this_fp->level >= 3)
            {
                if ((syUtilsRandFloat() * (325 - (this_fp->level * 25))) < 70.0F)
                {
                    com->objective = nFTComputerObjectiveCounterAttack;
                    com->is_counterattack = TRUE;

                    return 1;
                }
            }
        }
    }
    else com->ftcom_flags_0x49_b3 = FALSE;

    if (this_fp->computer.attack_count != this_fp->attack_count)
    {
        this_fp->computer.attack_count = this_fp->attack_count;

        if (this_fp->attack_knockback > 160.0F)
        {
            com->appeal_attempt_frames = 60;
        }
    }
    if (com->appeal_attempt_frames != 0)
    {
        com->appeal_attempt_frames--;

        if ((gSCManagerBattleState->gkind == nGRKindInishie) || (gSCManagerBattleState->gkind == nGRKindYoster))
        {
            if (this_fp->coll_data.floor_line_id >= 0)
            {
                if (mpCollisionCheckExistPlatformLineID(this_fp->coll_data.floor_line_id) != FALSE)
                {
                    return -1;
                }
            }
        }
        if (com->target_dist > 2500.0F)
        {
            switch (this_fp->status_id)
            {
            case nFTCommonStatusWait:
            case nFTCommonStatusWalkSlow:
            case nFTCommonStatusWalkMiddle:
            case nFTCommonStatusWalkFast:
            case nFTCommonStatusDash:
            case nFTCommonStatusRun:
            case nFTCommonStatusRunBrake:
            case nFTCommonStatusSquat:
            case nFTCommonStatusSquatWait:
            case nFTCommonStatusSquatRv:
            case nFTCommonStatusLandingLight:
            case nFTCommonStatusOttottoWait:
            case nFTCommonStatusOttotto:
                com->appeal_attempt_frames = 0;

                ftComputerSetCommandWaitShort(this_fp, nFTComputerInputStickNButtonL);

                return 0;
            }
        }
    }
    return -1;
}

// 0x80137778
s32 ftComputerProcDefault(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTComputer *com = &fp->computer;
    s32 process_result = ftComputerGetObjectiveStatus(fighter_gobj);

    if ((process_result == 0) || (process_result == 1))
    {
        return process_result;
    }
    if (ftComputerCheckEvadeDistance(fp) != FALSE)
    {
        com->objective = nFTComputerObjectiveEvade;
        return TRUE;
    }
    if ((fp->fkind != nFTKindMMario) && (fp->fkind != nFTKindGDonkey))
    {
        if (func_ovl3_80135B78(fp) != FALSE)
        {
            com->objective = nFTComputerObjectiveCounterAttack;
            return TRUE;
        }
    }
    switch (fp->fkind)
    {
    case nFTKindFox:
    case nFTKindNFox:
        if ((fp->status_id >= nFTFoxStatusSpecialLwScopeStart) && (fp->status_id <= nFTFoxStatusSpecialLwScopeEnd))
        {
            ftComputerSetCommandWaitShort(fp, nFTComputerInputStickNButtonBRelease);
            return FALSE;
        }
        break;

    case nFTKindNess:
    case nFTKindNNess:
        if ((fp->status_id >= nFTNessStatusSpecialLwScopeStart) && (fp->status_id <= nFTNessStatusSpecialLwScopeEnd))
        {
            ftComputerSetCommandWaitShort(fp, nFTComputerInputStickNButtonBRelease);
            return FALSE;
        }
        break;
    }
    if ((fp->status_id >= nFTCommonStatusGuardStart) && (fp->status_id <= nFTCommonStatusGuardEnd))
    {
        ftComputerSetCommandWaitShort(fp, 0x24);
        return FALSE;
    }
    else ftComputerCheckFindTarget(fp);

    if (com->target_dist < 350.0F)
    {
        com->objective = nFTComputerObjectiveAttack;
        return TRUE;
    }
    else if (ftComputerCheckFindItem(fp) != FALSE)
    {
        if (com->target_dist < (400.0F * (fp->level + 3)))
        {
            s32 track_wait = (gSCManagerBattleState->game_type == nSCBattleGameType1PGame) ? (-fp->level * 35) + 315 : (-fp->level * 25) + 225;

            com->item_track_wait++;

            if (track_wait < com->item_track_wait)
            {
                com->objective = nFTComputerObjectiveTrackItem;
                return TRUE;
            }
        }
    }
    else com->item_track_wait = 0;

    if (fp->item_gobj != NULL)
    {
        ITStruct *ip = itGetStruct(fp->item_gobj);

        switch (ip->type)
        {
        case nITTypeDamage:
        case nITTypeShoot:
        case nITTypeThrow:
            com->objective = nFTComputerObjectiveUseItem;
            return TRUE;

        default:
            com->objective = nFTComputerObjectiveAttack;
            return TRUE;
        }
    }
    else com->objective = com->objective_base;
    return TRUE;
}

// 0x80137A18
s32 ftComputerProcStand(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTComputer *com = &fp->computer;
    s32 objective_status = ftComputerGetObjectiveStatus(fighter_gobj);

    com->ftcom_flags_0x4A_b1 = FALSE;

    if ((objective_status == 0) || (objective_status == 1))
    {
        return objective_status;
    }
    if (fp->status_id == nFTCommonStatusRebirthWait)
    {
        com->target_pos.x = com->origin_pos.x;
        com->target_pos.y = com->origin_pos.y;
        com->target_line_id = com->floor_line_id;

        com->objective = nFTComputerObjectiveWalk;
    }
    else com->objective = nFTComputerObjectiveStand;

    return 1;
}

// 0x80137AA4
s32 ftComputerProcWalk(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTComputer *com = &fp->computer;
    s32 objective_status = ftComputerGetObjectiveStatus(fighter_gobj);
    Vec3f edge_left_pos;
    Vec3f edge_right_pos;
    f32 dist;

    com->ftcom_flags_0x4A_b1 = FALSE;

    if ((objective_status == 0) || (objective_status == 1))
    {
        return objective_status;
    }
    com->target_pos.y = com->origin_pos.y;
    com->target_line_id = com->floor_line_id;

    if (DISTANCE(com->target_pos.x, fp->joints[nFTPartsJointTopN]->translate.vec.f.x) < 100.0F)
    {
        com->target_pos.x = (((2.0 * syUtilsRandFloat()) - 1.0) * 2500.0) + com->origin_pos.x;

        if ((com->floor_line_id < 0) || (mpCollisionCheckExistLineID(com->floor_line_id) == FALSE))
        {
            com->target_pos.x = com->origin_pos.x;
        }
        else
        {
            mpCollisionGetFloorEdgeL(com->floor_line_id, &edge_left_pos);
            mpCollisionGetFloorEdgeR(com->floor_line_id, &edge_right_pos);

            if (com->target_pos.x < edge_left_pos.x)
            {
                com->target_pos.x = edge_left_pos.x;
                com->target_pos.y = edge_left_pos.y;
            }
            if (com->target_pos.x > edge_right_pos.x)
            {
                com->target_pos.x = edge_right_pos.x;
                com->target_pos.y = edge_right_pos.y;
            }
        }
    }
    com->objective = nFTComputerObjectiveWalk;

    dist = SQUARE(com->target_pos.x - fp->joints[nFTPartsJointTopN]->translate.vec.f.x) + SQUARE(com->target_pos.y - fp->joints[nFTPartsJointTopN]->translate.vec.f.y);

    if (dist < SQUARE(100.0F))
    {
        com->objective = nFTComputerObjectiveStand;
    }
    return 1;
}

// 0x80137C7C
s32 ftComputerProcEvade(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTComputer *com = &fp->computer;
    s32 objective_status = ftComputerGetObjectiveStatus(fighter_gobj);

    com->ftcom_flags_0x4A_b1 = FALSE;

    if ((objective_status == 0) || (objective_status == 1))
    {
        return objective_status;
    }
    com->objective = nFTComputerObjectiveEvade;

    return 1;
}

// 0x80137CD8
s32 ftComputerProcJump(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTComputer *com = &fp->computer;
    s32 objective_status = ftComputerGetObjectiveStatus(fighter_gobj);
    f32 dist;

    com->ftcom_flags_0x4A_b1 = FALSE;

    if ((objective_status == 0) || (objective_status == 1))
    {
        return objective_status;
    }
    com->target_pos.x = com->origin_pos.x;

    if (com->jump_wait == 0)
    {
        com->jump_wait = (syUtilsRandFloat() * 30.0F) + 30.0F;
        com->target_pos.y = com->origin_pos.y + 1100.0F;
        com->target_line_id = -1;
    }
    else
    {
        com->target_pos.y = com->origin_pos.y;
        com->target_line_id = com->floor_line_id;
        com->jump_wait--;
    }
    com->objective = nFTComputerObjectiveWalk;

    dist = SQUARE(com->target_pos.x - fp->joints[nFTPartsJointTopN]->translate.vec.f.x) + SQUARE(com->target_pos.y - fp->joints[nFTPartsJointTopN]->translate.vec.f.y);

    if (dist < SQUARE(100.0F))
    {
        com->objective = nFTComputerObjectiveStand;
    }
    return 1;
}

// 0x80137E70
s32 func_ovl3_80137E70(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTComputer *com = &fp->computer;
    s32 objective_status = ftComputerGetObjectiveStatus(fighter_gobj);
    f32 dist;

    com->ftcom_flags_0x4A_b1 = FALSE;

    if ((objective_status == 0) || (objective_status == 1))
    {
        return objective_status;
    }
    com->target_pos.x = com->origin_pos.x;
    com->target_pos.y = com->origin_pos.y;
    com->target_line_id = com->floor_line_id;

    com->objective = nFTComputerObjectiveWalk;

    dist = SQUARE(com->target_pos.x - fp->joints[nFTPartsJointTopN]->translate.vec.f.x) + SQUARE(com->target_pos.y - fp->joints[nFTPartsJointTopN]->translate.vec.f.y);

    if (dist < SQUARE(100.0F))
    {
        com->objective = nFTComputerObjectiveStand;
    }
    return 1;
}

// 0x80137F24
void ftComputerFollowObjectiveRecover(FTStruct *fp)
{
    FTComputer *com = &fp->computer;

    if (ftComputerCheckTryCancelSpecialN(fp) == FALSE)
    {
        func_ovl3_80134964(fp);

#if defined(REGION_US)
        if (fp->fkind == nFTKindPikachu)
        {
            switch (fp->status_id)
            {
            case nFTPikachuStatusSpecialAirHiStart:
                com->target_pos.x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
                com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y + 1100.0F;
                break;
            case nFTPikachuStatusSpecialAirHi:
                com->target_pos.x = 0.0F;
                com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y;
                break;
            }
        }
#endif

        ftComputerFollowObjectiveWalk(fp);
    }
}

// 0x80137FD4 - Not entirely sure about this one, seems to run when a CPU is about to land from DamageFall
void ftComputerFollowObjectiveCounterAttack(FTStruct *fp)
{
    FTComputer *com = &fp->computer;

    func_ovl3_801361BC(fp);

    if (com->is_counterattack)
    {
        ftComputerSetCommandImmediate(fp, nFTComputerInputButtonZ2);

        com->is_counterattack = FALSE;
    }
    else if (com->is_opponent_ra)
    {
        com->is_opponent_ra = FALSE;

        switch (fp->fkind)
        {
        case nFTKindFox:
        case nFTKindNFox:
            if ((fp->status_id < nFTFoxStatusSpecialLwScopeStart) || (fp->status_id > nFTFoxStatusSpecialLwScopeEnd))
            {
                ftComputerSetCommandImmediate(fp, nFTComputerInputStickNXSmashLwButtonBReleaseBHold);
            }
            break;

        case nFTKindNess:
        case nFTKindNNess:
            if ((fp->status_id < nFTNessStatusSpecialLwScopeStart) || (fp->status_id > nFTNessStatusSpecialLwScopeEnd))
            {
                ftComputerSetCommandImmediate(fp, nFTComputerInputStickNXSmashLwButtonBReleaseBHold);
            }
            break;
        }
    }
    else if (com->is_shield_item_weapon)
    {
        com->is_shield_item_weapon = FALSE;

        if ((fp->status_id < nFTCommonStatusGuardStart) || (fp->status_id > nFTCommonStatusGuardEnd))
        {
            ftComputerSetCommandImmediate(fp, nFTComputerInputStickNButtonZHold);
        }
    }
    else ftComputerFollowObjectiveWalk(fp);
}

// 0x80138104 - Try to navigate to target item
void ftComputerFollowObjectiveTrackItem(FTStruct *fp)
{
    if ((ftComputerCheckTargetItemInRange(fp) != FALSE))
    {
        if ((fp->status_id == nFTCommonStatusWait) || (fp->status_id == nFTCommonStatusSquat))
        {
            ftComputerSetCommandWaitShort(fp, nFTComputerInputStickNButtonBZReleaseAPress);
            return;
        }
    }
    ftComputerFollowObjectiveWalk(fp);
}

// 0x8013815C - Try to use shootable, throwable and ground (?) items
void ftComputerFollowObjectiveUseItem(FTStruct *fp)
{
    FTComputer *com = &fp->computer;
    ITStruct *ip = itGetStruct(fp->item_gobj);
    FTStruct *target_fp;

    switch (ip->type)
    {
    case nITTypeShoot:
        if (ip->multi != 0)
        {
            ftComputerCheckFindTarget(fp);
            target_fp = ftGetComTargetFighter(com);

            if ((fp->level >= 5) && (target_fp != NULL))
            {
                if ((target_fp->fkind == nFTKindNess) || (target_fp->fkind == nFTKindFox))
                {
                    ftComputerSetCommandWaitShort(fp, nFTComputerInputThrowItemImmediate);
                    return;
                }
            }
            if (DISTANCE(fp->joints[nFTPartsJointTopN]->translate.vec.f.y, com->target_pos.y) < 400.0F)
            {
                if
                (
                    (ip->kind == nITKindFFlower) &&
                    (
                        ((com->target_pos.x - fp->joints[nFTPartsJointTopN]->translate.vec.f.x) * fp->lr < 0.0F)     ||
                        ((com->target_pos.x - fp->joints[nFTPartsJointTopN]->translate.vec.f.x) * fp->lr > 1500.0F)
                    )
                )
                {
                    ftComputerFollowObjectiveWalk(fp);
                }
                else ftComputerSetCommandWaitShort(fp, nFTComputerInputStickTiltAutoXButtonBZReleaseAPress);
            }
            else ftComputerFollowObjectiveWalk(fp);

            com->item_throw_wait = 0;
            return;
        }
        com->item_throw_wait++;
        /* fallthrough */
    case nITTypeDamage:
    case nITTypeThrow:
        ftComputerCheckFindTarget(fp);

        if ((com->item_throw_wait >= 3) || (ip->kind == nITKindMBall))
        {
            ftComputerSetCommandWaitShort(fp, nFTComputerInputThrowItemWait);

            com->item_throw_wait = 0;
        }
        else ftComputerSetCommandWaitShort(fp, nFTComputerInputThrowItemImmediate);
        break;
    }
}

// 0x8013834C
void ftComputerFollowObjectiveEvade(FTStruct *fp)
{
    if (ftComputerCheckSetEvadeTarget(fp) != FALSE)
    {
        ftComputerFollowObjectiveWalk(fp);
    }
}

// 0x8013837C
s32 func_ovl3_8013837C(FTStruct *this_fp)
{
    FTComputer *com = &this_fp->computer;

    if ((this_fp->computer.behavior != nFTComputerBehaviorYoshiTeam) && (this_fp->fkind != nFTKindGDonkey))
    {
        FTStruct *target_fp = ftComputerWaitGetTarget(this_fp);

        if (target_fp != NULL)
        {
            com->target_user = target_fp;
            com->target_pos.x = target_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
            com->target_pos.y = target_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

            com->target_dist = sqrtf
            (
                SQUARE(this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x - com->target_pos.x) +
                SQUARE(this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y - com->target_pos.y)
            );
            com->ftcom_flags_0x4A_b1 = TRUE;

            if (ftGetComTargetFighter(com)->ga == nMPKineticsGround)
            {
                com->target_line_id = ftGetComTargetFighter(com)->coll_data.floor_line_id;
            }
            else com->target_line_id = -1;

            return 1;
        }
    }
    if ((ftComputerCheckFindTarget(this_fp) == FALSE) || (com->fighter_follow_since == 0))
    {
        com->fighter_follow_since = 1;

        // NOTE: This only trims the (syUtilsRandFloat() * (FTCOMPUTER_LEVEL_MAX - this_fp->level) * 4.0F) part to 8-bits.
        com->fighter_follow_wait = (u8)(syUtilsRandFloat() * (FTCOMPUTER_LEVEL_MAX - this_fp->level) * 4.0F) + ((FTCOMPUTER_LEVEL_MAX - this_fp->level) * 16);

        if (com->behavior == nFTComputerBehaviorYoshiTeam)
        {
            com->fighter_follow_wait += (300.0F + (syUtilsRandFloat() * 300.0F));
        }
        com->fighter_follow_end = ((syUtilsRandFloat() * 120.0F) + (com->fighter_follow_wait + 120));

        return 0;
    }
    if (com->fighter_follow_wait < ++com->fighter_follow_since)
    {
        if (com->fighter_follow_end < com->fighter_follow_since)
        {
            com->fighter_follow_since = 0;
        }
        if (com->target_dist < 1200.0F)
        {
            if (ftComputerCheckTryCancelSpecialN(this_fp) != FALSE)
            {
                return -1;
            }
        }
        else if ((this_fp->ga == nMPKineticsGround) && (ftComputerCheckTryChargeSpecialN(this_fp) != FALSE))
        {
            return -1;
        }
        return 1;
    }
    return 0;
}

// 0x8013877C
void func_ovl3_8013877C(FTStruct *this_fp)
{
    FTComputer *com = &this_fp->computer;
    Vec3f edge_left_pos;
    Vec3f edge_right_pos;
    s32 unused[2];

    com->walk_stop_wait++;

    if (com->walk_stop_wait >= 30)
    {
        if (com->walk_stop_wait == 30)
        {
            if (this_fp->coll_data.floor_line_id >= 0)
            {
                com->floor_line_id = this_fp->coll_data.floor_line_id;

                com->edge_pos.x = com->origin_pos.x = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x;

                if (this_fp->ga == nMPKineticsGround)
                {
                    com->edge_pos.y = com->origin_pos.y = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y;
                }
                else com->edge_pos.y = com->origin_pos.y = this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y + this_fp->coll_data.floor_dist;
            }
            else
            {
                com->edge_pos.x = com->origin_pos.x;
                com->edge_pos.y = com->origin_pos.y;
            }
        }
        com->target_line_id = com->floor_line_id;

        if ((com->target_line_id < 0) || (mpCollisionCheckExistLineID(com->target_line_id) == FALSE))
        {
            if (ftComputerCheckFindTarget(this_fp) != FALSE)
            {
                ftComputerFollowObjectiveWalk(this_fp);
            }
        }
        else
        {
            if (DISTANCE(com->edge_pos.x, this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x) < 100.0F)
            {
                com->edge_pos.x = (((2.0 * syUtilsRandFloat()) - 1.0) * 2500.0) + com->origin_pos.x;

                mpCollisionGetFloorEdgeL(com->floor_line_id, &edge_left_pos);
                mpCollisionGetFloorEdgeR(com->floor_line_id, &edge_right_pos);

                if (com->edge_pos.x < edge_left_pos.x)
                {
                    com->edge_pos.x = edge_left_pos.x;
                    com->edge_pos.y = edge_left_pos.y;
                }
                if (com->edge_pos.x > edge_right_pos.x)
                {
                    com->edge_pos.x = edge_right_pos.x;
                    com->edge_pos.y = edge_right_pos.y;
                }
            }
            com->target_pos.x = com->edge_pos.x;
            com->target_pos.y = com->edge_pos.y;

            com->ftcom_flags_0x4A_b1 = FALSE;

            ftComputerFollowObjectiveWalk(this_fp);

            if (com->walk_stop_wait > 150)
            {
                com->walk_stop_wait = 0;
            }
        }
    }
    else
    {
        com->input_wait = 1;
        com->p_command = dFTComputerPlayerInputScripts[nFTComputerInputStickN];

        if ((com->walk_stop_wait == ((-this_fp->level * 2) + 18)) && (this_fp->ga == nMPKineticsGround))
        {
            ftComputerCheckTryChargeSpecialN(this_fp);

            switch (this_fp->fkind)
            {
            case nFTKindLink:
                if (ftComputerCheckFindTarget(this_fp) != FALSE)
                {
                    if (com->target_dist < 1500.0F)
                    {
                        ftComputerSetCommandWaitShort(this_fp, nFTComputerInputStickSmashLwButtonB);
                    }
                }
                break;

            case nFTKindNess:
                if (syUtilsRandFloat() < 0.25F)
                {
                    ftComputerSetCommandWaitShort(this_fp, nFTComputerInputStickSmashHiButtonB);
                }
                break;
            }
        }
    }
}

// 0x80138AA8
sb32 func_ovl3_80138AA8(FTStruct *this_fp, sb32 is_delay)
{
    FTComputer *com = &this_fp->computer;
    Vec3f pos;
    Vec3f ga_last;
    s32 stand_line_id;
    s32 fkind;
    FTStruct *target_fp;

    target_fp = com->target_user;

    if (DISTANCE(this_fp->joints[nFTPartsJointTopN]->translate.vec.f.y, com->target_pos.y) < 400.0F)
    {
        if (com->unk_ftcom_0x35 == 0)
        {
            com->unk_ftcom_0x35 = 2.0F * (syUtilsRandFloat() * (FTCOMPUTER_LEVEL_MAX - this_fp->level));
        }
        if ((syUtilsRandFloat() < ((this_fp->level - 1) / 9.0F)) && (target_fp != NULL))
        {
            if ((target_fp->fkind == nFTKindNess) || (target_fp->fkind == nFTKindFox))
            {
                return FALSE;
            }
        }
        fkind = (this_fp->fkind == nFTKindKirby) ? this_fp->passive_vars.kirby.copy_id : this_fp->fkind;

        if (fkind == nFTKindSamus)
        {
            if
            (
                (this_fp->status_id == nFTSamusStatusSpecialNStart)                ||
                (this_fp->status_id == nFTSamusStatusSpecialAirNStart)             ||
                (this_fp->status_id == nFTSamusStatusSpecialNLoop)                 ||
                (this_fp->status_id == nFTSamusStatusSpecialAirNEnd)               ||
                (this_fp->status_id == nFTKirbyStatusCopySamusSpecialNStart)      ||
                (this_fp->status_id == nFTKirbyStatusCopySamusSpecialAirNStart)   ||
                (this_fp->status_id == nFTKirbyStatusCopySamusSpecialNLoop)       ||
                (this_fp->status_id == nFTKirbyStatusCopySamusSpecialAirNEnd)
            )
            {
                return FALSE;
            }
        }
        switch (fkind)
        {
        case nFTKindLink:
            if (com->target_dist < 1500.0F)
            {
                if (syUtilsRandFloat() < 0.3F)
                {
                    ftComputerSetCommandWaitShort(this_fp, nFTComputerInputStickSmashLwButtonB);
                }
            }
            /* fallthrough */
        case nFTKindMario:
        case nFTKindFox:
        case nFTKindSamus:
        case nFTKindLuigi:
        case nFTKindPikachu:
        case nFTKindMMario:
            com->unk_ftcom_0x35++;

            if (com->unk_ftcom_0x35 >= 5)
            {
                ftComputerFollowObjectiveWalk(this_fp);
                return TRUE;
            }
            pos = this_fp->joints[nFTPartsJointTopN]->translate.vec.f;
            pos.x = com->target_pos.x;
            pos.y = com->target_pos.y;

            if (com->target_pos.x < this_fp->joints[nFTPartsJointTopN]->translate.vec.f.x)
            {
                if (mpCollisionCheckRWallLineCollisionSame(&this_fp->joints[nFTPartsJointTopN]->translate.vec.f, &pos, &ga_last, &stand_line_id, NULL, NULL) != FALSE)
                {
                    return FALSE;
                }
                if (mpCollisionCheckFloorLineCollisionSame(&this_fp->joints[nFTPartsJointTopN]->translate.vec.f, &pos, &ga_last, &stand_line_id, NULL, NULL) != FALSE)
                {
                    return FALSE;
                }
                if (mpCollisionCheckCeilLineCollisionSame(&this_fp->joints[nFTPartsJointTopN]->translate.vec.f, &pos, &ga_last, &stand_line_id, NULL, NULL) != FALSE)
                {
                    return FALSE;
                }
            }
            else if (mpCollisionCheckLWallLineCollisionSame(&this_fp->joints[nFTPartsJointTopN]->translate.vec.f, &pos, &ga_last, &stand_line_id, NULL, NULL) != FALSE)
            {
                return FALSE;
            }
            else if (mpCollisionCheckFloorLineCollisionSame(&this_fp->joints[nFTPartsJointTopN]->translate.vec.f, &pos, &ga_last, &stand_line_id, NULL, NULL) != FALSE)
            {
                return FALSE;
            }
            else if (mpCollisionCheckCeilLineCollisionSame(&this_fp->joints[nFTPartsJointTopN]->translate.vec.f, &pos, &ga_last, &stand_line_id, NULL, NULL) != FALSE)
            {
                return FALSE;
            }
            if (is_delay == FALSE)
            {
                ftComputerSetCommandWaitLong(this_fp, nFTComputerInputStickSmashAutoXButtonB);
            }
            else ftComputerSetCommandWaitLong(this_fp, nFTComputerInputStickTiltAutoXNYD5SmashAutoXButtonB);

            return TRUE;
        }
    }
    return FALSE;
}

// 0x80138EE4
sb32 func_ovl3_80138EE4(FTStruct *fp)
{
    FTComputer *com = &fp->computer;
    s32 fkind;
    f32 random;
    Vec3f edge_left_pos;
    Vec3f edge_right_pos;

    if (fp->fkind == nFTKindGDonkey)
    {
        return FALSE;
    }
    fkind = (fp->fkind == nFTKindKirby) ? fp->passive_vars.kirby.copy_id : fp->fkind;

    if (fkind == nFTKindSamus)
    {
        if
        (
           (fp->status_id == nFTSamusStatusSpecialNStart)             ||
           (fp->status_id == nFTSamusStatusSpecialAirNStart)          ||
           (fp->status_id == nFTSamusStatusSpecialNLoop)              ||
           (fp->status_id == nFTSamusStatusSpecialAirNEnd)            ||
           (fp->status_id == nFTKirbyStatusCopySamusSpecialNStart)   ||
           (fp->status_id == nFTKirbyStatusCopySamusSpecialAirNStart)||
           (fp->status_id == nFTKirbyStatusCopySamusSpecialNLoop)    ||
           (fp->status_id == nFTKirbyStatusCopySamusSpecialAirNEnd)
        )
        {
            return FALSE;
        }
    }
#if defined(REGION_US)
    if ((fkind == nFTKindDonkey) || (fkind == nFTKindNDonkey) || (fkind == nFTKindGDonkey))
#else
    if ((fkind == nFTKindDonkey))
#endif
    {
        if
        (
            (fp->status_id == nFTDonkeyStatusSpecialNStart)                ||
            (fp->status_id == nFTDonkeyStatusSpecialAirNStart)             ||
            (fp->status_id == nFTDonkeyStatusSpecialNLoop)                 ||
            (fp->status_id == nFTDonkeyStatusSpecialAirNLoop)              ||
            (fp->status_id == nFTKirbyStatusCopyDonkeySpecialNStart)       ||
            (fp->status_id == nFTKirbyStatusCopyDonkeySpecialAirNStart)    ||
            (fp->status_id == nFTKirbyStatusCopyDonkeySpecialNLoop)        ||
#if defined(REGION_US)
            (fp->status_id == nFTKirbyStatusCopyDonkeySpecialAirNLoop)     ||
            (fp->status_id == nFTDonkeyStatusThrowFWait)                   ||
            (fp->status_id == nFTDonkeyStatusThrowFWalkSlow)               ||
            (fp->status_id == nFTDonkeyStatusThrowFWalkMiddle)             ||
            (fp->status_id == nFTDonkeyStatusThrowFWalkFast)               ||
            (fp->status_id == nFTDonkeyStatusThrowFTurn)
#else
            (fp->status_id == nFTKirbyStatusCopyDonkeySpecialAirNLoop)     
#endif
        )
        {
            return FALSE;
        }
    }
    if (com->ftcom_flags_0x4A_b1)
    {
        if ((ftGetComTargetFighter(com)->status_id == nFTCommonStatusCliffCatch) || (ftGetComTargetFighter(com)->status_id == nFTCommonStatusCliffWait))
        {
            return FALSE;
        }
    }
    random = syUtilsRandFloat();
    if
    (
        (fp->ga == nMPKineticsGround) &&
        (
            (
                (com->target_dist < ((random * 450.0F) + 350.0F) &&
                (
                    (
                        (fp->level >= 4) &&
                        (syUtilsRandFloat() * (11 - fp->level)) < 1.0F) ||
                        (
                            (fp->level >= 3) &&
                            (((com->target_pos.x - fp->joints[nFTPartsJointTopN]->translate.vec.f.x) * fp->lr) < 0.0F)
                        )
                    )
                )
                ||
                (
                    (com->target_dist < 800.0F) &&
                    (fp->level >= 7) &&
                    ((syUtilsRandFloat() * (10 - fp->level)) < 1.0F) &&
                    (((com->target_pos.x - fp->joints[nFTPartsJointTopN]->translate.vec.f.x) * ftGetComTargetFighter(com)->lr) < 0.0F)
                )
            )
        )
    )
    {
        if (fp->coll_data.floor_line_id >= 0)
        {
            mpCollisionGetFloorEdgeL(fp->coll_data.floor_line_id, &edge_left_pos);
            mpCollisionGetFloorEdgeR(fp->coll_data.floor_line_id, &edge_right_pos);

            if (com->target_pos.x < fp->joints[nFTPartsJointTopN]->translate.vec.f.x)
            {
                if (fp->joints[nFTPartsJointTopN]->translate.vec.f.x < (edge_left_pos.x + 500.0F))
                {
                    return FALSE;
                }
            }
            else if (fp->joints[nFTPartsJointTopN]->translate.vec.f.x > (edge_right_pos.x - 500.0F))
            {
                return FALSE;
            }
        }
        if (com->target_pos.x < fp->joints[nFTPartsJointTopN]->translate.vec.f.x)
        {
            ftComputerSetCommandWaitShort(fp, nFTComputerInputEscapeL);
        }
        else ftComputerSetCommandWaitShort(fp, nFTComputerInputEscapeR);

        return TRUE;
    }
    return FALSE;
}

// 0x801392C8
void ftComputerFollowObjectiveAttack(FTStruct *fp)
{
    FTComputer *com = &fp->computer;
    s32 temp_v0 = func_ovl3_8013837C(fp);
    f32 random;
    sb32 var_a1;
    s32 unused[2];

    if (temp_v0 != 0)
    {
        if (temp_v0 != -1)
        {
            syUtilsRandFloat();
            syUtilsRandFloat();
            random = syUtilsRandFloat();

            if (((fp->joints[nFTPartsJointTopN]->translate.vec.f.x < com->target_pos.x) && (fp->lr == +1)) || ((fp->joints[nFTPartsJointTopN]->translate.vec.f.x > com->target_pos.x) && (fp->lr == -1)))
            {
                var_a1 = FALSE;
            }
            else var_a1 = TRUE;

            if (com->target_dist < ((random * 300.0F) + 1200.0F))
            {
                if (func_ovl3_80138EE4(fp) != FALSE)
                {
                    com->unk_ftcom_0x35 = 0;
                    return;
                }
                if (ftComputerCheckDetectTarget(fp, 0.0F) != FALSE)
                {
                    com->unk_ftcom_0x20 = 0;
                    com->unk_ftcom_0x35 = 0;
                    return;
                }
                if (ftGetComTargetFighter(com)->physics.vel_air.y > 30.0F)
                {
                    if (syUtilsRandFloat() < ((fp->level + 2) / 9.0F))
                    {
                        if
                        (
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyHi)   ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyN)    ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyLw)   ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyTop)  ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFall)    ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyRoll)
                        )
                        {
                            ftComputerSetCommandImmediate(fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);
                            com->walk_stop_wait = 0;
                            return;
                        }
                    }
                }
                if ((DISTANCE(com->target_pos.y, fp->joints[nFTPartsJointTopN]->translate.vec.f.y) > 1500.0F) || (com->unk_ftcom_0x20 != 0))
                {
                    ftComputerFollowObjectiveWalk(fp);

                    com->walk_stop_wait = 0;

                    if (com->unk_ftcom_0x20 != 0)
                    {
                        com->unk_ftcom_0x20--;
                    }
                }
                else
                {
                    if (syUtilsRandFloat() < 0.05F)
                    {
                        com->unk_ftcom_0x20 = (syUtilsRandFloat() * 60.0F) + 30.0F;

                        ftComputerFollowObjectiveWalk(fp);

                        com->walk_stop_wait = 0;

                        return;
                    }
                    func_ovl3_8013877C(fp);
                }
            }
            else
            {
                com->item_throw_wait = 0;

                if (func_ovl3_80138AA8(fp, var_a1) != FALSE)
                {
                    com->walk_stop_wait = 0;
                    return;
                }
                if ((fp->star_hitstatus == nGMHitStatusInvincible) || ((fp->item_gobj != NULL) && (itGetStruct(fp->item_gobj)->kind == nITKindHammer)))
                {
                    ftComputerFollowObjectiveWalk(fp);

                    com->walk_stop_wait = 0;

                    return;
                }
                if ((DISTANCE(com->target_pos.y, fp->joints[nFTPartsJointTopN]->translate.vec.f.y) > 1500.0F) || (com->unk_ftcom_0x20 != 0))
                {
                    ftComputerFollowObjectiveWalk(fp);

                    com->walk_stop_wait = 0;

                    if (com->unk_ftcom_0x20 != 0)
                    {
                        com->unk_ftcom_0x20--;
                    }
                }
                else
                {
                    if (syUtilsRandFloat() < 0.05F)
                    {
                        com->unk_ftcom_0x20 = (syUtilsRandFloat() * 60.0F) + 30.0F;

                        ftComputerFollowObjectiveWalk(fp);

                        com->walk_stop_wait = 0;

                        return;
                    }
                    com->unk_ftcom_0x35 = 0;

                    func_ovl3_8013877C(fp);
                }
            }
        }
    }
    else func_ovl3_8013877C(fp);
}

// 0x801397F4
void func_ovl3_801397F4(FTStruct *fp)
{
    FTComputer *com = &fp->computer;
    s32 temp_v0 = func_ovl3_8013837C(fp);
    f32 random;
    sb32 var_a1;
    s32 unused[2];

    if (temp_v0 != 0)
    {
        if (temp_v0 != -1)
        {
            syUtilsRandFloat();
            syUtilsRandFloat();
            random = syUtilsRandFloat();

            if (((fp->joints[nFTPartsJointTopN]->translate.vec.f.x < com->target_pos.x) && (fp->lr == +1)) || ((fp->joints[nFTPartsJointTopN]->translate.vec.f.x > com->target_pos.x) && (fp->lr == -1)))
            {
                var_a1 = FALSE;
            }
            else var_a1 = TRUE;

            if (com->target_dist < ((random * 300.0F) + 1200.0F))
            {
                if (func_ovl3_80138EE4(fp) != FALSE)
                {
                    com->unk_ftcom_0x35 = 0;
                    return;
                }
                if (ftComputerCheckDetectTarget(fp, 0.0F) != FALSE)
                {
                    com->unk_ftcom_0x35 = 0;
                    return;
                }
                if (ftGetComTargetFighter(com)->physics.vel_air.y > 30.0F)
                {
                    if (syUtilsRandFloat() < ((fp->level + 5) / 9.0F))
                    {
                        if
                        (
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyHi)   ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyN)    ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyLw)   ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyTop)  ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFall)    ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyRoll)
                        )
                        {
                            ftComputerSetCommandImmediate(fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);
                            com->walk_stop_wait = 0;
                            return;
                        }
                    }
                }
                ftComputerFollowObjectiveWalk(fp);
                com->walk_stop_wait = 0;
            }
            else
            {
                com->item_throw_wait = 0;

                if (func_ovl3_80138AA8(fp, var_a1) != FALSE)
                {
                    com->walk_stop_wait = 0;
                }
                else if ((fp->star_hitstatus == nGMHitStatusInvincible) || ((fp->item_gobj != NULL) && (itGetStruct(fp->item_gobj)->kind == nITKindHammer)))
                {
                    ftComputerFollowObjectiveWalk(fp);

                    com->walk_stop_wait = 0;
                }
                else
                {
                    com->unk_ftcom_0x35 = 0;

                    ftComputerFollowObjectiveWalk(fp);

                    com->walk_stop_wait = 0;
                }
            }
        }
    }
    else func_ovl3_8013877C(fp);
}

// 0x80139A60
void ftComputerFollowObjectiveAlly(FTStruct *fp)
{
    FTComputer *com = &fp->computer;
    s32 temp_v0 = func_ovl3_8013837C(fp);
    f32 random;
    sb32 var_a1;
    s32 unused[2];

    if (temp_v0 != 0)
    {
        if (temp_v0 != -1)
        {
            syUtilsRandFloat();
            syUtilsRandFloat();
            random = syUtilsRandFloat();

            if (((fp->joints[nFTPartsJointTopN]->translate.vec.f.x < com->target_pos.x) && (fp->lr == +1)) || ((fp->joints[nFTPartsJointTopN]->translate.vec.f.x > com->target_pos.x) && (fp->lr == -1)))
            {
                var_a1 = FALSE;
            }
            else var_a1 = TRUE;

            if (com->target_dist < ((random * 300.0F) + 1200.0F))
            {
                if (func_ovl3_80138EE4(fp) != FALSE)
                {
                    com->unk_ftcom_0x35 = 0;
                    return;
                }
                if (ftComputerCheckDetectTarget(fp, -0.5F) != FALSE)
                {
                    com->unk_ftcom_0x35 = 0;
                    return;
                }
                func_ovl3_8013877C(fp);
                return;
            }
            com->item_throw_wait = 0;

            if (func_ovl3_80138AA8(fp, var_a1) != FALSE)
            {
                com->walk_stop_wait = 0;
                return;
            }
            if ((fp->star_hitstatus == nGMHitStatusInvincible) || ((fp->item_gobj != NULL) && (itGetStruct(fp->item_gobj)->kind == nITKindHammer)))
            {
                ftComputerFollowObjectiveWalk(fp);

                com->walk_stop_wait = 0;
                return;
            }
            if ((DISTANCE(com->target_pos.y, fp->joints[nFTPartsJointTopN]->translate.vec.f.y) > 1500.0F) || (com->unk_ftcom_0x20 != 0))
            {
                ftComputerFollowObjectiveWalk(fp);

                com->walk_stop_wait = 0;

                if (com->unk_ftcom_0x20 != 0)
                {
                    com->unk_ftcom_0x20--;
                }
            }
            else
            {
                if (syUtilsRandFloat() < 0.01F)
                {
                    com->unk_ftcom_0x20 = (syUtilsRandFloat() * 60.0F) + 30.0F;

                    ftComputerFollowObjectiveWalk(fp);

                    com->walk_stop_wait = 0;
                    return;
                }
                com->unk_ftcom_0x35 = 0;
                func_ovl3_8013877C(fp);
            }
        }
    }
    else func_ovl3_8013877C(fp);
}

// 0x80139D6C
void ftComputerFollowObjectivePatrol(FTStruct *fp)
{
    FTComputer *com = &fp->computer;
    s32 temp_v0 = func_ovl3_8013837C(fp);
    f32 random;
    sb32 var_a1;
    s32 unused[2];

    if (temp_v0 != 0)
    {
        if (temp_v0 != -1)
        {
            syUtilsRandFloat();
            syUtilsRandFloat();
            random = syUtilsRandFloat();

            if (((fp->joints[nFTPartsJointTopN]->translate.vec.f.x < com->target_pos.x) && (fp->lr == +1)) || ((fp->joints[nFTPartsJointTopN]->translate.vec.f.x > com->target_pos.x) && (fp->lr == -1)))
            {
                var_a1 = FALSE;
            }
            else var_a1 = TRUE;

            if (com->target_dist < ((random * 300.0F) + 1200.0F))
            {
                if (func_ovl3_80138EE4(fp) != FALSE)
                {
                    com->unk_ftcom_0x35 = 0;
                    return;
                }
                if (ftComputerCheckDetectTarget(fp, 2.0F) != FALSE)
                {
                    com->unk_ftcom_0x20 = 0;
                    com->unk_ftcom_0x35 = 0;
                    return;
                }
                if (ftGetComTargetFighter(com)->physics.vel_air.y > 30.0F)
                {
                    if (syUtilsRandFloat() < ((fp->level + 2) / 9.0F))
                    {
                        if
                        (
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyHi)   ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyN)    ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyLw)   ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyTop)  ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFall)    ||
                            (ftGetComTargetFighter(com)->status_id == nFTCommonStatusDamageFlyRoll)
                        )
                        {
                            ftComputerSetCommandImmediate(fp, nFTComputerInputMoveAutoStickTiltHiReleaseZ);
                            com->walk_stop_wait = 0;
                            return;
                        }
                    }
                }
                if ((DISTANCE(com->target_pos.y, fp->joints[nFTPartsJointTopN]->translate.vec.f.y) > 1500.0F) || (com->unk_ftcom_0x20 != 0))
                {
                    ftComputerFollowObjectiveWalk(fp);

                    com->walk_stop_wait = 0;

                    if (com->unk_ftcom_0x20 != 0)
                    {
                        com->unk_ftcom_0x20--;
                    }
                }
                else
                {
                    if (syUtilsRandFloat() < 0.05F)
                    {
                        com->unk_ftcom_0x20 = (syUtilsRandFloat() * 60.0F) + 15.0F;

                        ftComputerFollowObjectiveWalk(fp);

                        com->walk_stop_wait = 0;
                        return;
                    }
                    else func_ovl3_8013877C(fp);
                }
            }
            else
            {
                com->item_throw_wait = 0;

                if (func_ovl3_80138AA8(fp, var_a1) != FALSE)
                {
                    com->walk_stop_wait = 0;
                    return;
                }
                if ((fp->star_hitstatus == nGMHitStatusInvincible) || ((fp->item_gobj != NULL) && (itGetStruct(fp->item_gobj)->kind == nITKindHammer)))
                {
                    ftComputerFollowObjectiveWalk(fp);

                    com->walk_stop_wait = 0;
                    return;
                }
                if ((DISTANCE(com->target_pos.y, fp->joints[nFTPartsJointTopN]->translate.vec.f.y) > 1500.0F) || (com->unk_ftcom_0x20 != 0))
                {
                    ftComputerFollowObjectiveWalk(fp);

                    com->walk_stop_wait = 0;

                    if (com->unk_ftcom_0x20 != 0)
                    {
                        com->unk_ftcom_0x20--;
                    }
                }
                else
                {
                    if (syUtilsRandFloat() < 0.05F)
                    {
                        com->unk_ftcom_0x20 = (syUtilsRandFloat() * 60.0F) + 30.0F;

                        ftComputerFollowObjectiveWalk(fp);

                        com->walk_stop_wait = 0;
                        return;
                    }
                    com->unk_ftcom_0x35 = 0;
                    func_ovl3_8013877C(fp);
                }
            }
        }
    }
    else func_ovl3_8013877C(fp);
}

// 0x8013A298
void ftComputerFollowObjectiveRush(FTStruct *fp)
{
    FTComputer *com = &fp->computer;
    f32 random;
    sb32 var_a1;
    s32 unused;

    if (ftComputerCheckFindTarget(fp) != FALSE)
    {
        syUtilsRandFloat();
        random = syUtilsRandFloat();

        if (((fp->joints[nFTPartsJointTopN]->translate.vec.f.x < com->target_pos.x) && (fp->lr == +1)) || ((fp->joints[nFTPartsJointTopN]->translate.vec.f.x > com->target_pos.x) && (fp->lr == -1)))
        {
            var_a1 = FALSE;
        }
        else var_a1 = TRUE;

        if (com->target_dist < ((random * 300.0F) + 1200.0F))
        {
            if (ftComputerCheckDetectTarget(fp, 0.0F) != FALSE)
            {
                com->walk_stop_wait = 0;
            }
            else
            {
                ftComputerFollowObjectiveWalk(fp);
                com->walk_stop_wait = 0;
            }
        }
        else if (TRUE)
        {
            ftComputerFollowObjectiveWalk(fp);
            com->walk_stop_wait = 0;
        }
    }
    else func_ovl3_8013877C(fp);
}

// 0x8013A38C
void ftComputerProcessObjective(FTStruct *fp)
{
    FTComputer *com = &fp->computer;

    if (com->proc_com(fp->fighter_gobj) != FALSE)
    {
        switch (com->objective)
        {
        case nFTComputerObjectiveStand:
            com->input_wait = 1;
            com->p_command = dFTComputerPlayerInputScripts[nFTComputerInputStickN];
            break;

        case nFTComputerObjectiveWalk:
            ftComputerFollowObjectiveWalk(fp);
            break;

        case nFTComputerObjectiveAttack:
            ftComputerFollowObjectiveAttack(fp);
            break;

        case nFTComputerObjectiveEvade:
            ftComputerFollowObjectiveEvade(fp);
            break;

        case nFTComputerObjectiveRecover:
            ftComputerFollowObjectiveRecover(fp);
            break;

        case nFTComputerObjectiveTrackItem:
            ftComputerFollowObjectiveTrackItem(fp);
            break;

        case nFTComputerObjectiveUseItem:
            ftComputerFollowObjectiveUseItem(fp);
            break;

        case nFTComputerObjectiveCounterAttack:
            ftComputerFollowObjectiveCounterAttack(fp);
            break;

        case nFTComputerObjectiveUnknown1:
            func_ovl3_801397F4(fp);
            break;

        case nFTComputerObjectiveAlly:
            ftComputerFollowObjectiveAlly(fp);
            break;

        case nFTComputerObjectivePatrol:
            ftComputerFollowObjectivePatrol(fp);
            break;

        case nFTComputerObjectiveRush:
            ftComputerFollowObjectiveRush(fp);
            break;
        }
    }
}

// 0x8013A4AC
void ftComputerProcessBehavior(FTStruct *fp)
{
    FTComputer *com = &fp->computer;

    switch (com->behavior)
    {
    case nFTComputerBehaviorDefault:
        com->proc_com = ftComputerProcDefault;
        com->objective_base = nFTComputerObjectiveAttack;
        break;

    case nFTComputerBehaviorUnk1:
        com->proc_com = ftComputerProcDefault;
        com->objective_base = nFTComputerObjectiveEvade;
        break;

    case nFTComputerBehaviorUnk2:
        com->proc_com = ftComputerProcDefault;
        com->objective_base = nFTComputerObjectiveUnknown1;
        break;

    case nFTComputerBehaviorAlly:
        com->proc_com = ftComputerProcDefault;
        com->objective_base = nFTComputerObjectiveAlly;
        break;

    case nFTComputerBehaviorCaptain:
        com->proc_com = ftComputerProcDefault;
        com->objective_base = nFTComputerObjectivePatrol;
        break;

    case nFTComputerBehaviorUnk3:
        com->proc_com = ftComputerProcDefault;
        com->objective_base = nFTComputerObjectiveAttack;
        break;

    case nFTComputerBehaviorYoshiTeam:
        com->proc_com = ftComputerProcDefault;
        com->objective_base = nFTComputerObjectiveAlly;
        break;

    case nFTComputerBehaviorKirbyTeam:
        com->proc_com = ftComputerProcDefault;
        com->objective_base = nFTComputerObjectivePatrol;
        break;

    case nFTComputerBehaviorPolyTeam:
        com->proc_com = ftComputerProcDefault;
        com->objective_base = nFTComputerObjectiveAttack;
        break;

    case nFTComputerBehaviorBonus3:
        com->proc_com = ftComputerProcDefault;
        com->objective_base = nFTComputerObjectiveRush;
        break;

    case nFTComputerBehaviorStand:
        com->proc_com = ftComputerProcStand;
        break;

    case nFTComputerBehaviorWalk:
        com->proc_com = ftComputerProcWalk;
        break;

    case nFTComputerBehaviorEvade:
        com->proc_com = ftComputerProcEvade;
        break;

    case nFTComputerBehaviorJump:
        com->proc_com = ftComputerProcJump;
        break;

    case nFTComputerBehaviorUnk5:
        com->proc_com = func_ovl3_80137E70;
        break;
    }
}

// 0x8013A63C
void ftComputerProcessTrait(FTStruct *fp)
{
    FTComputer *com = &fp->computer;

    switch (com->trait)
    {
    case nFTComputerTraitDefault:
        if (com->behavior_change_wait == 0)
        {
            com->behavior_change_wait = (syUtilsRandFloat() * 900.0F) + 900.0F;

            switch (syUtilsRandUShort() & 3)
            {
            case 0:
                com->behavior = nFTComputerBehaviorDefault;
                break;

            case 1:
                com->behavior = nFTComputerBehaviorUnk2;
                break;

            case 2:
                com->behavior = nFTComputerBehaviorAlly;
                com->behavior_change_wait >>= 2;
                break;

            case 3:
                com->behavior = nFTComputerBehaviorCaptain;
                break;
            }
        }
        break;

    case nFTComputerTraitLink:
#if defined(REGION_US)
        if ((fp->percent_damage >= 14) || (com->behavior_change_wait == 0))
#else
        if ((fp->percent_damage >= 20) || (com->behavior_change_wait == 0))
#endif
        {
            com->behavior = nFTComputerBehaviorDefault;
        }
        else com->behavior = nFTComputerBehaviorStand;
        break;

    case nFTComputerTraitYoshiTeam:
        com->behavior = nFTComputerBehaviorYoshiTeam;
        break;

    case nFTComputerTraitKirbyTeam:
        com->behavior = nFTComputerBehaviorKirbyTeam;
        break;

    case nFTComputerTraitPolyTeam:
        com->behavior = nFTComputerBehaviorPolyTeam;
        break;

    case nFTComputerTraitMarioBros:
        if (fp->fkind == nFTKindMario)
        {
            com->behavior = nFTComputerBehaviorDefault;
        }
        else com->behavior = nFTComputerBehaviorAlly;
        break;

    case nFTComputerTraitGDonkey:
        com->behavior = nFTComputerBehaviorDefault;
        break;

    case nFTComputerTraitUnk1:
        com->behavior = nFTComputerBehaviorUnk2;
        break;

    case nFTComputerTraitBonus3:
        com->behavior = nFTComputerBehaviorBonus3;
        break;

    case nFTComputerTraitAlly:
        com->behavior = nFTComputerBehaviorAlly;
        break;

    case nFTComputerTraitNone:
        break;
    }
}

// 0x8013A834
void ftComputerProcessAll(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTComputer *com = &fp->computer;

    if (fp->fkind != nFTKindBoss)
    {
        if (com->behavior_change_wait != 0)
        {
            com->behavior_change_wait--;
        }
        if (com->input_wait == 0)
        {
            ftComputerProcessTrait(fp);
            ftComputerProcessBehavior(fp);
            ftComputerProcessObjective(fp);
        }
        ftComputerUpdateInputs(fp);
    }
}

// 0x8013A8A8
void ftComputerSetupAll(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTComputer *com = &fp->computer;
    f32 dash_speed;
    f32 jump;
    f32 edge_left_nearest;
    f32 edge_right_nearest;
    u16 *line_ids;
    Vec3f edge_pos;
    s32 i;

    bzero(com, sizeof(*com));

    if (fp->fkind != nFTKindBoss)
    {
        com->origin_pos.x = com->target_pos.x = fp->joints[nFTPartsJointTopN]->translate.vec.f.x;

        if (fp->ga == nMPKineticsGround)
        {
            com->origin_pos.y = com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y;
        }
        else com->origin_pos.y = com->target_pos.y = fp->joints[nFTPartsJointTopN]->translate.vec.f.y + fp->coll_data.floor_dist;

        com->floor_line_id = fp->coll_data.floor_line_id;

        com->proc_com = NULL;

        com->trait = 0;
        com->behavior = 0;

#if defined(REGION_US)
        i = 1440 - (fp->level * 240);
        if (i < 0)
        {
            i = 0;
        }
        com->behavior_change_wait = i;
#else
        com->behavior_change_wait = 2160 - (fp->level * 360);

        if (com->behavior_change_wait < 0)
        {
            com->behavior_change_wait = 0;
        }
#endif

        com->dash_predict = 0.0F;

        dash_speed = fp->attr->dash_speed;

        for (i = 0; i < fp->attr->dash_to_run; i++)
        {
            if (i >= 7)
            {
                dash_speed -= fp->attr->dash_decel;
            }
            com->dash_predict += dash_speed;
        }
        com->jump_predict = -200.0F;

        jump = (fp->attr->jump_height_mul * F_CONTROLLER_RANGE_MAX) + fp->attr->jump_height_base;

        while (jump > 0.0F)
        {
            jump -= fp->attr->gravity;

            com->jump_predict += jump;
        }
        edge_left_nearest = 2000.0F;
        edge_right_nearest = -2000.0F;

        com->cliff_left_pos.y = com->cliff_right_pos.y = 9999.9F;

        line_ids = gMPCollisionLineGroups[nMPLineKindFloor].line_id;

        for (i = 0; i < gMPCollisionLineGroups[nMPLineKindFloor].line_count; i++)
        {
            if ((mpCollisionCheckExistLineID(line_ids[i]) != FALSE) && (mpCollisionCheckExistPlatformLineID(line_ids[i]) == FALSE))
            {
                mpCollisionGetFloorEdgeL(line_ids[i], &edge_pos);

                if (edge_left_nearest > edge_pos.x)
                {
                    edge_left_nearest = edge_pos.x;

                    if (com->cliff_left_pos.y > edge_pos.y)
                    {
                        com->cliff_left_pos.x = edge_pos.x;
                        com->cliff_left_pos.y = edge_pos.y;
                    }
                }
                else if (((edge_pos.x - gMPCollisionBounds.current.left) < 500.0F) && (com->cliff_left_pos.y > edge_pos.y))
                {
                    com->cliff_left_pos.x = edge_pos.x;
                    com->cliff_left_pos.y = edge_pos.y;
                }
                mpCollisionGetFloorEdgeR(line_ids[i], &edge_pos);

                if (edge_right_nearest < edge_pos.x)
                {
                    edge_right_nearest = edge_pos.x;

                    if (com->cliff_right_pos.y > edge_pos.y)
                    {
                        com->cliff_right_pos.x = edge_pos.x;
                        com->cliff_right_pos.y = edge_pos.y;
                    }
                }
                else if (((gMPCollisionBounds.current.right - edge_pos.x) < 500.0F) && (com->cliff_right_pos.y > edge_pos.y))
                {
                    com->cliff_right_pos.x = edge_pos.x;
                    com->cliff_right_pos.y = edge_pos.y;
                }
            }
        }
    }
    else return;
}

// 0x8013AC00
void ftComputerSetFighterDamageDetectSize(GObj *fighter_gobj)
{
    FTStruct *fp = ftGetStruct(fighter_gobj);
    FTDamageColl *damage_coll;
    FTParts *parts;
    Vec3f joint_world_pos;
    f32 nearest_left;
    f32 nearest_bottom;
    f32 nearest_right;
    f32 nearest_top;
    s32 i, j;
    DObj *joint;

    nearest_right = fp->joints[nFTPartsJointTopN]->translate.vec.f.x;
    nearest_top = fp->joints[nFTPartsJointTopN]->translate.vec.f.y;

    nearest_left = nearest_right;
    nearest_bottom = nearest_top;

    for (i = 0; i < ARRAY_COUNT(fp->damage_colls); i++)
    {
        damage_coll = &fp->damage_colls[i];

        if (damage_coll->hitstatus == nGMHitStatusNormal)
        {
            joint = damage_coll->joint;
            parts = ftGetParts(joint);

            if (parts->unk_dobjtrans_0x5 == 0)
            {
                func_ovl2_800EDBA4(joint);
            }
            for (j = 0; j < 8; j++)
            {
                joint_world_pos.x = (j & 1) ? damage_coll->offset.x + damage_coll->size.x : damage_coll->offset.x - damage_coll->size.x;
                joint_world_pos.y = (j & 2) ? damage_coll->offset.y + damage_coll->size.y : damage_coll->offset.y - damage_coll->size.y;
                joint_world_pos.z = (j & 4) ? damage_coll->offset.z + damage_coll->size.z : damage_coll->offset.z - damage_coll->size.z;

                gmCollisionGetWorldPosition(parts->mtx_translate, &joint_world_pos);

                if (nearest_left > joint_world_pos.x)
                {
                    nearest_left = joint_world_pos.x;
                }
                if (nearest_right < joint_world_pos.x)
                {
                    nearest_right = joint_world_pos.x;
                }
                if (nearest_bottom > joint_world_pos.y)
                {
                    nearest_bottom = joint_world_pos.y;
                }
                if (nearest_top < joint_world_pos.y)
                {
                    nearest_top = joint_world_pos.y;
                }
            }
        }
    }
    fp->damage_coll_size.x = (nearest_right - nearest_left) * 0.55F;
    fp->damage_coll_size.y = (nearest_top - nearest_bottom) * 0.55F;
}
