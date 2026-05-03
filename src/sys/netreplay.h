#ifndef _SYNETREPLAY_H_
#define _SYNETREPLAY_H_

#include <PR/ultratypes.h>
#include <sc/scene.h>
#include <sys/netinput.h>

extern void syNetReplayInitDebugEnv(void);
extern void syNetReplayCaptureBattleMetadata(SCBattleState *battle_state, SYNetInputReplayMetadata *metadata);
extern void syNetReplayApplyBattleMetadata(const SYNetInputReplayMetadata *metadata);
extern void syNetReplayStartVSSession(SCBattleState *battle_state);
extern void syNetReplayUpdate(void);
extern void syNetReplayFinishVSSession(void);
extern sb32 syNetReplayWriteDebugFile(const char *path);
extern sb32 syNetReplayLoadDebugFile(const char *path);

#endif /* _SYNETREPLAY_H_ */
