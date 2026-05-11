#pragma once

#ifdef __cplusplus
extern "C" {
    #endif

    // Volume getters exposed to the C codebase
    float port_get_music_volume();
    float port_get_sfx_volume();
    float port_get_voice_volume();

    #ifdef __cplusplus
}
#endif
