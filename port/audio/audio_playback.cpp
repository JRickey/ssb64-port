// Audio playback bridge — Phase 1: silence pipeline
//
// Generates zeroed PCM and feeds it to libultraship's AudioPlayerPlayFrame()
// to prove the audio output path works end-to-end. The audio coroutine calls
// portAudioPushSilence() once per tick.

#include "audio_playback.h"
#include "port_log.h"

#include <libultraship/bridge/audiobridge.h>

#include <cstring>

// SSB64 original output rate is 32 kHz.
// At 60 fps that's ~533 samples/frame. We use the same high/low watermarks
// as Starship to keep the audio player's ring buffer comfortably filled.
static constexpr int32_t SAMPLES_PER_FRAME_HIGH = 560;
static constexpr int32_t SAMPLES_PER_FRAME_LOW  = 528;

// Maximum buffer size in s16 samples (stereo: 2 channels * SAMPLES_HIGH).
static constexpr int32_t MAX_SAMPLES_STEREO = SAMPLES_PER_FRAME_HIGH * 2;

// Persistent silence buffer — zeroed once, reused every frame.
static int16_t sSilenceBuf[MAX_SAMPLES_STEREO];
static bool    sInitialized = false;

extern "C" void portAudioPushSilence(void)
{
    if (!sInitialized) {
        std::memset(sSilenceBuf, 0, sizeof(sSilenceBuf));
        sInitialized = true;
        port_log("SSB64 Audio: silence pipeline active (phase 1)\n");
    }

    int32_t numChannels = GetNumAudioChannels();
    if (numChannels < 1) {
        numChannels = 2; // fallback to stereo
    }

    // Determine how many samples to submit this frame.
    // Mirror the Starship approach: if the player is under-buffered, send the
    // high watermark; otherwise send the low watermark.
    int32_t buffered = AudioPlayerBuffered();
    int32_t desired  = AudioPlayerGetDesiredBuffered();

    int32_t samplesPerFrame;
    if (buffered < desired) {
        samplesPerFrame = SAMPLES_PER_FRAME_HIGH;
    } else {
        samplesPerFrame = SAMPLES_PER_FRAME_LOW;
    }

    // Total s16 values = samples * channels.  Byte length = that * 2.
    size_t totalSamples = (size_t)samplesPerFrame * (size_t)numChannels;
    size_t byteLen      = totalSamples * sizeof(int16_t);

    // Safety: clamp to buffer size.
    if (byteLen > sizeof(sSilenceBuf)) {
        byteLen = sizeof(sSilenceBuf);
    }

    AudioPlayerPlayFrame(reinterpret_cast<const uint8_t*>(sSilenceBuf), byteLen);
}

extern "C" void portAudioSubmitFrame(const void *buf, int sampleCount)
{
    if (buf == nullptr || sampleCount <= 0) {
        portAudioPushSilence();
        return;
    }

    // n_alAudioFrame produces interleaved stereo s16 PCM.
    // Total bytes = sampleCount * 2 channels * 2 bytes per sample.
    size_t byteLen = (size_t)sampleCount * 4;

    AudioPlayerPlayFrame(reinterpret_cast<const uint8_t*>(buf), byteLen);
}
