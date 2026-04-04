#include <ft/fighter.h>

// 0x8012C830
f32 dFTCommonDataHandicapTable[/* */][2] = 
{
    { 0.55F,               1.8181818F },        
    {  0.6F,             15.0F / 9.0F },
    {  0.7F,             10.0F / 7.0F },
    { 0.75F,             12.0F / 9.0F },
    {  0.8F,            11.25F / 9.0F },
    {  0.9F,             10.0F / 9.0F },
    { 0.95F,             10.0F / 9.5F },
    {  1.0F,              9.0F / 9.0F },
#if defined(REGION_US)
    { 1.09F,               0.9174312F },
    {  0.5F,             45.0F / 9.0F },
    { 0.55F,             36.0F / 9.0F },
    {  0.6F,             30.0F / 9.0F },
    { 0.65F,             20.0F / 7.0F },
    {  0.7F,             22.5F / 9.0F },
    { 0.75F,               1.8181818F },
    {  0.8F,               1.6949153F },
    { 0.85F,               1.5873016F },
    {  0.9F,               1.4925373F },
    { 0.95F,               1.4084507F },
    { 0.65F,             30.0F / 9.0F },
    {  0.7F,             20.0F / 7.0F },
    { 0.74F,             22.5F / 9.0F },
    { 0.77F,             20.0F / 9.0F },
    {  0.8F,             18.0F / 9.0F },
    {  1.0F,              4.0F / 7.0F },
    { 1.05F,              0.53763443F },
    {  1.1F,               0.5128205F },        // 10.0 / 19.5
    { 1.15F,               0.4761905F },
    { 1.23F,              0.45045045F },
    { 1.05F,              0.43478262F },
    {  1.1F,              3.0F / 7.5F },
    { 1.15F,    (10.0F / 3.0F) / 9.0F },
    {  1.2F,              0.35714287F },
    { 1.25F,              3.0F / 9.0F },
#else
    {  1.1F,               0.9090909F }, // 10.0 / 11.0F
    {  0.6F,             45.0F / 9.0F },
    {  0.6F,             36.0F / 9.0F },
    {  0.6F,             30.0F / 9.0F },
    {  0.6F,             20.0F / 7.0F },
    {  0.6F,             22.5F / 9.0F },
    {  0.9F,               1.8181818F },
    { 0.95F,               1.6949153F },
    {  1.0F,               1.5873016F },
    {  1.0F,               1.4925373F },
    {  1.0F,               1.4084507F },
    {  0.7F,             30.0F / 9.0F },
    { 0.75F,             25.0F / 8.0F },
    {  0.8F,             20.0F / 7.0F },
    {  1.0F,             25.0F / 9.5F },
    {  0.9F,             22.5F / 9.0F },
    { 1.05F,              4.0F / 7.0F },
    { 1.09F,              0.53763443F },
    { 1.13F,               0.5128205F },        // 10.0 / 19.5
    { 1.17F,               0.4761905F },
    { 1.23F,              0.45045045F },
    { 0.95F,              0.43478262F },
    {  1.0F,              3.0F / 7.5F },
    { 1.05F,    (10.0F / 3.0F) / 9.0F },
    {  1.1F,              0.35714287F },
    {  1.1F,              3.0F / 9.0F },
#endif
    {  0.9F,              9.0F / 9.0F },
    {  1.0F,              9.0F / 9.0F },
    {  1.1F,              9.0F / 9.0F },
    { 1.22F,              9.0F / 9.0F },
    {  1.5F,              9.0F / 9.0F },
#if defined(REGION_US)
    { 1.08F,  (200.0F / 24.0F) / 9.0F }
#else
    {  1.0F,              9.0F / 9.0F }
#endif
};

// 0x8012C970
u16 dFTCommonDataDownBounceSFX[/* */] =
{
    nSYAudioFGMMarioDownBounce,        // Mario
    nSYAudioFGMFoxDownBounce,          // Fox
    nSYAudioFGMDonkeyDownBounce,       // Donkey Kong
    nSYAudioFGMSamusDownBounce,        // Samus
    nSYAudioFGMMarioDownBounce,        // Luigi
    nSYAudioFGMLinkDownBounce,         // Link
    nSYAudioFGMYoshiDownBounce,        // Yoshi
    nSYAudioFGMCaptainDownBounce,      // Captain Falcon
    nSYAudioFGMKirbyDownBounce,        // Kirby
    nSYAudioFGMPikachuDownBounce,      // Pikachu
    nSYAudioFGMPurinDownBounce,        // Jigglypuff
    nSYAudioFGMNessDownBounce,         // Ness
    nSYAudioFGMMarioDownBounce,        // Master Hand
    nSYAudioFGMMarioDownBounce,        // Metal Mario
    nSYAudioFGMMarioDownBounce,        // Poly Mario
    nSYAudioFGMFoxDownBounce,          // Poly Fox
    nSYAudioFGMDonkeyDownBounce,       // Poly Donkey Kong
    nSYAudioFGMSamusDownBounce,        // Poly Samus
    nSYAudioFGMMarioDownBounce,        // Poly Luigi
    nSYAudioFGMLinkDownBounce,         // Poly Link
    nSYAudioFGMYoshiDownBounce,        // Poly Yoshi
    nSYAudioFGMCaptainDownBounce,      // Poly Captain Falcon
    nSYAudioFGMKirbyDownBounce,        // Poly Kirby
    nSYAudioFGMPikachuDownBounce,      // Poly Pikachu
    nSYAudioFGMPurinDownBounce,        // Poly Jigglypuff
    nSYAudioFGMNessDownBounce,         // Poly Ness
    nSYAudioFGMDonkeyDownBounce        // Giant Donkey Kong
};

// 0x8012C9A8
u16 dFTCommonDataPublicFighterCallFGMs[/* */] =
{
    nSYAudioVoicePublicMario,
    nSYAudioVoicePublicFox,
    nSYAudioVoicePublicDonkey,
    nSYAudioVoicePublicSamus,
    nSYAudioVoicePublicLuigi,
    nSYAudioVoicePublicLink,
    nSYAudioVoicePublicYoshi,
    nSYAudioVoicePublicCaptain,
    nSYAudioVoicePublicKirby,
    nSYAudioVoicePublicPikachu,
    nSYAudioVoicePublicPurin,
    nSYAudioVoicePublicNess,
    nSYAudioFGMVoiceEnd,
    nSYAudioVoicePublicMario,
    nSYAudioFGMVoiceEnd,
    nSYAudioFGMVoiceEnd,
    nSYAudioFGMVoiceEnd,
    nSYAudioFGMVoiceEnd,
    nSYAudioFGMVoiceEnd,
    nSYAudioFGMVoiceEnd,
    nSYAudioFGMVoiceEnd,
    nSYAudioFGMVoiceEnd,
    nSYAudioFGMVoiceEnd,
    nSYAudioFGMVoiceEnd,
    nSYAudioFGMVoiceEnd,
    nSYAudioFGMVoiceEnd,
    nSYAudioFGMVoiceEnd
};

// 0x8012C9E0
FTItemThrow dFTCommonDataItemThrowDescs[/* */] =
{
    // LightThrowDrop
    {
        FALSE,  // Is Smash throw?
        36,     // Velocity
        110,    // Angle
        50      // Damage Multiplier %
    },

    // LightThrowDash
    {
        FALSE,  // Is Smash throw?
        120,    // Velocity
        10,     // Angle
        100     // Damage Multiplier %
    },

    // LightThrowF
    {
        FALSE,  // Is Smash throw?
        60,     // Velocity
        15,     // Angle
        100     // Damage Multiplier %
    },

    // LightThrowB
    {
        FALSE,  // Is Smash throw?
        60,     // Velocity
        15,     // Angle
        100     // Damage Multiplier %
    },

    // LightThrowHi
    {
        FALSE,  // Is Smash throw?
        65,     // Velocity
        90,     // Angle
        100     // Damage Multiplier %
    },

    // LightThrowLw
    {
        FALSE,  // Is Smash throw?
        65,     // Velocity
        -70,    // Angle
        100     // Damage Multiplier %
    },

    // LightThrowF4
    {
        TRUE,   // Is Smash throw?
        110,    // Velocity
        8,      // Angle
        100     // Damage Multiplier %
    },

    // LightThrowB4
    {
        TRUE,   // Is Smash throw?
        110,    // Velocity
        8,      // Angle
        100     // Damage Multiplier %
    },

    // LightThrowHi4
    {
        TRUE,   // Is Smash throw?
        110,    // Velocity
        90,     // Angle
        100     // Damage Multiplier %
    },

    // LightThrowLw4
    {
       TRUE,   // Is Smash throw?
       110,    // Velocity
       -70,    // Angle
       100     // Damage Multiplier %
    },

    // LightThrowAirF
    {
        FALSE,  // Is Smash throw?
        75,     // Velocity
        8,      // Angle
        100     // Damage Multiplier %
    },

    // LightThrowAirB
    {
        FALSE,  // Is Smash throw?
        75,     // Velocity
        8,      // Angle
        100     // Damage Multiplier %
    },

    // LightThrowAirHi
    {
        FALSE,  // Is Smash throw?
        80,     // Velocity
        90,     // Angle
        100     // Damage Multiplier %
    },

    // LightThrowAirLw
    {
        FALSE,  // Is Smash throw?
        75,     // Velocity
        -90,    // Angle
        100     // Damage Multiplier %
    },

    // LightThrowAirF4
    {
        TRUE,   // Is Smash throw?
        120,    // Velocity
        7,      // Angle
        100     // Damage Multiplier %
    },

    // LightThrowAirB4
    {
        TRUE,   // Is Smash throw?
        120,    // Velocity
        7,      // Angle
        100     // Damage Multiplier %
    },

    // LightThrowAirHi4
    {
        TRUE,   // Is Smash throw?
        120,    // Velocity
        90,     // Angle
        100     // Damage Multiplier %
    },

    // LightThrowAirLw4
    {
        TRUE,   // Is Smash throw?
        140,    // Velocity
        -90,    // Angle
        100     // Damage Multiplier %
    },

    // HeavyThrowF
    {
        FALSE,  // Is Smash throw?
        70,     // Velocity
        60,     // Angle
        100     // Damage Multiplier %
    },

    // HeavyThrowB
    {
        FALSE,  // Is Smash throw?
        70,     // Velocity
        60,     // Angle
        100     // Damage Multiplier %
    },

    // HeavyThrowF4
    {
        TRUE,   // Is Smash throw?
        90,     // Velocity
        20,     // Angle
        100     // Damage Multiplier %
    },

    // HeavyThrowB4
    {
        TRUE,   // Is Smash throw?
        90,     // Velocity
        20,     // Angle
        100     // Damage Multiplier %
    }
};

// 0x8012CA38
FTItemSwing dFTCommonDataItemSwingAnimSpeeds[/* */] =
{
    100,        // SwordSwing1
    100,        // SwordSwing3
    100,        // SwordSwing4
    100,        // SwordSwingDash

    100,        // BatSwing1
    100,        // BatSwing3
    75,         // BatSwing4
    100,        // BatSwingDash

    200,        // HarisenSwing1
    200,        // HarisenSwing3
    200,        // HarisenSwing4
    100,        // HarisenSwingDash

    100,        // StarRodSwing1
    100,        // StarRodSwing3
    100,        // StarRodSwing4
    100         // StarRodSwingDash
};

// 0x8012CA78
SYColorRGBA dFTCommonDataShadowColorDefault = { 0x00, 0x00, 0x00, 0xA0 };

// 0x8012CA7C
SYColorRGBA dFTCommonDataShadowColorTeams[/* */] =
{   
    { 0x80, 0x00, 0x00, 0xB4 },     // Red Team
    { 0x00, 0x00, 0x80, 0xB4 },     // Blue Team
    { 0x00, 0x80, 0x00, 0xB4 }      // Green Team
};
