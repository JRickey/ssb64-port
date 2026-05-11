#include "volume.h"
#include <libultraship/bridge/consolevariablebridge.h>

// Volume level getters pulled from the UI cvars
float port_get_music_volume() {
    return CVarGetFloat("gSettings.Audio.MusicVolume", 1.0f);
}

float port_get_sfx_volume() {
    return CVarGetFloat("gSettings.Audio.SFXVolume", 1.0f);
}

float port_get_voice_volume() {
    return CVarGetFloat("gSettings.Audio.VoiceVolume", 1.0f);
}
