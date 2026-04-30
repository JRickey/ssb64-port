// Audio playback bridge — Phase 1: silence pipeline
//
// Generates zeroed PCM and feeds it to libultraship's AudioPlayerPlayFrame()
// to prove the audio output path works end-to-end. The audio coroutine calls
// portAudioPushSilence() once per tick.

#include "audio_playback.h"
#include "port_log.h"

#include <libultraship/bridge/audiobridge.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

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

/* ---------------------------------------------------------------------- */
/*  WAV dump for offline inspection                                       */
/*                                                                        */
/*  Writes the first ~16 seconds of submitted PCM to /tmp/ssb64_dump.wav  */
/*  (interleaved stereo s16 @ 32 kHz).  Open in Audacity to look at the   */
/*  waveform / spectrogram and inspect the intro's 10-15s high-note       */
/*  section where small aliasing/tinniness artifacts are easiest to hear. */
/* ---------------------------------------------------------------------- */

static FILE *sWavFile = nullptr;
static uint32_t sWavBytesWritten = 0;
static const uint32_t kWavMaxBytes = 32000u * 2u * 2u * 16u; /* 16 s stereo s16 @ 32 kHz */

static void wavWriteHeader(FILE *fp, uint32_t dataBytes)
{
    /* Standard 44-byte RIFF/WAVE/fmt/data header for s16 stereo 32 kHz. */
    const uint16_t channels   = 2;
    const uint32_t sampleRate = 32000;
    const uint16_t bitsPer    = 16;
    const uint16_t blockAlign = channels * (bitsPer / 8);
    const uint32_t byteRate   = sampleRate * blockAlign;
    const uint32_t riffSize   = 36 + dataBytes;

    auto w16 = [&](uint16_t v) { std::fwrite(&v, 1, 2, fp); };
    auto w32 = [&](uint32_t v) { std::fwrite(&v, 1, 4, fp); };

    std::fwrite("RIFF", 1, 4, fp);  w32(riffSize);
    std::fwrite("WAVE", 1, 4, fp);
    std::fwrite("fmt ", 1, 4, fp);  w32(16); /* PCM fmt chunk size */
    w16(1); /* PCM format */
    w16(channels);
    w32(sampleRate);
    w32(byteRate);
    w16(blockAlign);
    w16(bitsPer);
    std::fwrite("data", 1, 4, fp);  w32(dataBytes);
}

static void wavMaybeOpen(void)
{
    if (sWavFile != nullptr || sWavBytesWritten >= kWavMaxBytes) return;
    sWavFile = std::fopen("/tmp/ssb64_dump.wav", "wb");
    if (sWavFile == nullptr) {
        port_log("SSB64 Audio: WAV dump open failed (errno preserved by fopen)\n");
        return;
    }
    /* Placeholder header — finalized when we close. */
    wavWriteHeader(sWavFile, 0);
    port_log("SSB64 Audio: WAV dump opened /tmp/ssb64_dump.wav (will capture %u bytes ~ 16 s)\n",
             (unsigned)kWavMaxBytes);
}

static void wavAppend(const void *buf, size_t bytes)
{
    if (sWavFile == nullptr) return;
    if (sWavBytesWritten >= kWavMaxBytes) return;
    if (bytes > kWavMaxBytes - sWavBytesWritten) {
        bytes = kWavMaxBytes - sWavBytesWritten;
    }
    std::fwrite(buf, 1, bytes, sWavFile);
    sWavBytesWritten += (uint32_t)bytes;
    if (sWavBytesWritten >= kWavMaxBytes) {
        /* Finalize: rewrite header with actual data size, then close. */
        std::fseek(sWavFile, 0, SEEK_SET);
        wavWriteHeader(sWavFile, sWavBytesWritten);
        std::fclose(sWavFile);
        sWavFile = nullptr;
        port_log("SSB64 Audio: WAV dump finalized (%u bytes captured)\n",
                 (unsigned)sWavBytesWritten);
    }
}

static int16_t clampS16FromDouble(double v)
{
    long out = std::lrint(v);
    if (out < -32768) return -32768;
    if (out > 32767) return 32767;
    return static_cast<int16_t>(out);
}

static bool outputFilterEnabled()
{
    static int sEnabled = -1;
    if (sEnabled < 0) {
        const char* env = std::getenv("SSB64_AUDIO_OUTPUT_FILTER");
        sEnabled = !(env && (env[0] == '0' || env[0] == 'n' || env[0] == 'N'));
    }
    return sEnabled != 0;
}

static const int16_t* applyOutputFilter(const int16_t* input, int sampleCount)
{
    /* 32 kHz N64 output lands close to Nyquist on the intro's highest notes.
     * This light 14.5 kHz Butterworth stage mimics the final output chain's
     * anti-imaging rolloff without touching the actual mixer/resampler math. */
    static int16_t sFilteredBuf[MAX_SAMPLES_STEREO];
    static double sZ1[2] = { 0.0, 0.0 };
    static double sZ2[2] = { 0.0, 0.0 };
    static bool sLogged = false;

    /* fc = 14.5 kHz at fs = 32 kHz lands at 0.91·Nyquist, where the bilinear
     * transform warps the analog Butterworth response: the discrete biquad
     * step response peaks at ~1.15 instead of the 1.043 the analog design
     * implies.  For input near int16 max (loud SFX like DK growl), that 15%
     * overshoot drives the output past int16 → clampS16FromDouble flat-tops
     * → square-wave harmonics → audible "garbled noise."
     *
     * Mitigation: pre-attenuate the numerator so the filter's actual peak
     * step response stays within int16 even on int16-max input.  Costs ~1.4
     * dB of overall loudness on the filtered path, which is fine for an
     * anti-imaging filter. */
    constexpr double kHeadroom = 0.85;
    constexpr double b0 = 0.8118317459078658 * kHeadroom;
    constexpr double b1 = 1.6236634918157316 * kHeadroom;
    constexpr double b2 = 0.8118317459078658 * kHeadroom;
    constexpr double a1 = 1.5879371063123660;
    constexpr double a2 = 0.6593898773190974;

    if (!outputFilterEnabled() || sampleCount <= 0 ||
        sampleCount * 2 > MAX_SAMPLES_STEREO) {
        return input;
    }

    if (!sLogged) {
        sLogged = true;
        port_log("SSB64 Audio: output low-pass enabled (set SSB64_AUDIO_OUTPUT_FILTER=0 to disable)\n");
    }

    for (int i = 0; i < sampleCount; i++) {
        for (int ch = 0; ch < 2; ch++) {
            double x = static_cast<double>(input[i * 2 + ch]);
            double y = (b0 * x) + sZ1[ch];
            sZ1[ch] = (b1 * x) - (a1 * y) + sZ2[ch];
            sZ2[ch] = (b2 * x) - (a2 * y);
            sFilteredBuf[i * 2 + ch] = clampS16FromDouble(y);
        }
    }

    return sFilteredBuf;
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

    // One-time log: confirm the synthesis pipeline is flowing.
    static bool sFirstSubmit = true;
    static bool sLoggedNonzero = false;
    if (sFirstSubmit) {
        sFirstSubmit = false;
        port_log("SSB64 Audio: first synth frame submitted (sampleCount=%d bytes=%zu)\n",
                 sampleCount, byteLen);
    }
    if (!sLoggedNonzero) {
        const int16_t *s = reinterpret_cast<const int16_t *>(buf);
        size_t count = byteLen / sizeof(int16_t);
        for (size_t i = 0; i < count; i++) {
            if (s[i] != 0) {
                sLoggedNonzero = true;
                port_log("SSB64 Audio: first non-zero sample detected (idx=%zu v=%d)\n",
                         i, (int)s[i]);
                wavMaybeOpen();
                break;
            }
        }
    }
    const int16_t* pcm = reinterpret_cast<const int16_t*>(buf);
    pcm = applyOutputFilter(pcm, sampleCount);

    wavAppend(pcm, byteLen);

    AudioPlayerPlayFrame(reinterpret_cast<const uint8_t*>(pcm), byteLen);
}
