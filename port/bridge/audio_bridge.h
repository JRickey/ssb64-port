#ifndef PORT_AUDIO_BRIDGE_H
#define PORT_AUDIO_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Port-side audio asset loader.
 *
 * Replaces syAudioLoadAssets() for the PC port.  Loads audio BLOBs from the
 * .o2r archive, parses the big-endian N64 binary format (32-bit pointer
 * fields), and constructs native C structs with correct 64-bit pointer width.
 *
 * Called from the PORT guard in syAudioThreadMain (src/sys/audio.c).
 * After this returns the game's audio globals (sSYAudioSequenceBank1/2,
 * sSYAudioSeqFile, FGM tables, Acmd buffers) are populated and ready for
 * syAudioMakeBGMPlayers().
 */
void portAudioLoadAssets(void);

/**
 * Release shared_ptr<Ship::IResource> references held by the audio bridge's
 * BLOB table.  Must run before Ship::Context is torn down — otherwise those
 * references survive into __cxa_finalize_ranges and the resulting
 * IResource::~IResource() call lands on a shut-down spdlog logger.
 */
void portAudioShutdownAssets(void);

#ifdef __cplusplus
}
#endif

#endif /* PORT_AUDIO_BRIDGE_H */
