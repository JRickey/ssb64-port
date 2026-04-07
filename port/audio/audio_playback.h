#pragma once

// Audio playback bridge — feeds PCM audio to libultraship's audio player.

#ifdef __cplusplus
extern "C" {
#endif

// Push one frame of silence to the LUS audio player.
// Phase 1 fallback — used only if synthesis is not yet wired.
void portAudioPushSilence(void);

// Submit synthesized audio from n_alAudioFrame to the LUS audio player.
// buf: interleaved stereo s16 PCM, sampleCount: mono sample count per channel.
// Total bytes submitted = sampleCount * 2 (channels) * 2 (bytes per s16).
void portAudioSubmitFrame(const void *buf, int sampleCount);

#ifdef __cplusplus
}
#endif
