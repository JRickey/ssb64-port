#include <sys/netreplay.h>

#include <ft/fighter.h>
#include <if/ifcommon.h>
#include <sc/scmanager.h>
#include <sys/netinput.h>
#include <sys/utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef PORT
extern void port_log(const char *fmt, ...);
extern char *getenv(const char *name);
extern int atoi(const char *s);
#endif

#define SYNETREPLAY_DEFAULT_RECORD_FRAMES 1800

typedef struct SYNetReplayFileHeader
{
	u32 magic;
	u32 version;
	u32 metadata_size;
	u32 frame_size;
	u32 frame_count;
	u32 player_count;
	u32 input_checksum;

} SYNetReplayFileHeader;

const char *sSYNetReplayRecordPath;
const char *sSYNetReplayPlayPath;
u32 sSYNetReplayRecordFrameLimit = SYNETREPLAY_DEFAULT_RECORD_FRAMES;
u32 sSYNetReplayLoadedFrameCount;
u32 sSYNetReplayLoadedInputChecksum;
sb32 sSYNetReplayIsRecording;
sb32 sSYNetReplayIsRecordWritten;
sb32 sSYNetReplayIsPlaybackLoaded;
sb32 sSYNetReplayIsPlaybackActive;
sb32 sSYNetReplayIsPlaybackVerified;
SYNetInputReplayMetadata sSYNetReplayLoadedMetadata;
SYNetInputFrame sSYNetReplayLoadedFrames[MAXCONTROLLERS][SYNETINPUT_REPLAY_MAX_FRAMES];

void syNetReplayClearLoadedFrames(void)
{
	s32 player;
	s32 tick;

	sSYNetReplayLoadedFrameCount = 0;
	sSYNetReplayLoadedInputChecksum = 0;

	for (player = 0; player < MAXCONTROLLERS; player++)
	{
		for (tick = 0; tick < SYNETINPUT_REPLAY_MAX_FRAMES; tick++)
		{
			memset(&sSYNetReplayLoadedFrames[player][tick], 0, sizeof(SYNetInputFrame));
		}
	}
}

void syNetReplayCaptureBattleMetadata(SCBattleState *battle_state, SYNetInputReplayMetadata *metadata)
{
	s32 player;

	memset(metadata, 0, sizeof(SYNetInputReplayMetadata));

	metadata->magic = SYNETINPUT_REPLAY_MAGIC;
	metadata->version = SYNETINPUT_REPLAY_VERSION;
	metadata->scene_kind = nSCKindVSBattle;
	metadata->rng_seed = syUtilsRandSeed();

	if (battle_state == NULL)
	{
		return;
	}
	metadata->player_count = battle_state->pl_count + battle_state->cp_count;
	metadata->stage_kind = battle_state->gkind;
	metadata->stocks = battle_state->stocks;
	metadata->time_limit = battle_state->time_limit;
	metadata->item_switch = battle_state->item_appearance_rate;
	metadata->item_toggles = battle_state->item_toggles;
	metadata->game_type = battle_state->game_type;
	metadata->game_rules = battle_state->game_rules;
	metadata->is_team_battle = battle_state->is_team_battle;
	metadata->handicap = battle_state->handicap;
	metadata->is_team_attack = battle_state->is_team_attack;
	metadata->is_stage_select = battle_state->is_stage_select;
	metadata->damage_ratio = battle_state->damage_ratio;
	metadata->item_appearance_rate = battle_state->item_appearance_rate;
	metadata->is_not_teamshadows = battle_state->is_not_teamshadows;

	for (player = 0; player < MAXCONTROLLERS; player++)
	{
		metadata->player_kinds[player] = battle_state->players[player].pkind;
		metadata->fighter_kinds[player] = battle_state->players[player].fkind;
		metadata->costumes[player] = battle_state->players[player].costume;
		metadata->teams[player] = battle_state->players[player].team;
		metadata->handicaps[player] = battle_state->players[player].handicap;
		metadata->levels[player] = battle_state->players[player].level;
		metadata->shades[player] = battle_state->players[player].shade;
	}
}

void syNetReplayApplyBattleMetadata(const SYNetInputReplayMetadata *metadata)
{
	SCBattleState *battle_state = &gSCManagerTransferBattleState;
	s32 player;

	*battle_state = dSCManagerDefaultBattleState;
	battle_state->game_type = metadata->game_type;
	battle_state->gkind = metadata->stage_kind;
	battle_state->is_team_battle = metadata->is_team_battle;
	battle_state->game_rules = metadata->game_rules;
	battle_state->time_limit = metadata->time_limit;
	battle_state->stocks = metadata->stocks;
	battle_state->handicap = metadata->handicap;
	battle_state->is_team_attack = metadata->is_team_attack;
	battle_state->is_stage_select = FALSE;
	battle_state->damage_ratio = metadata->damage_ratio;
	battle_state->item_toggles = metadata->item_toggles;
	battle_state->item_appearance_rate = metadata->item_appearance_rate;
	battle_state->is_not_teamshadows = metadata->is_not_teamshadows;
	battle_state->pl_count = 0;
	battle_state->cp_count = 0;

	for (player = 0; player < MAXCONTROLLERS; player++)
	{
		battle_state->players[player].player = (metadata->is_team_battle != FALSE) ? metadata->teams[player] : player;
		battle_state->players[player].team = metadata->teams[player];
		battle_state->players[player].pkind = metadata->player_kinds[player];
		battle_state->players[player].fkind = metadata->fighter_kinds[player];
		battle_state->players[player].costume = metadata->costumes[player];
		battle_state->players[player].shade = metadata->shades[player];
		battle_state->players[player].handicap = metadata->handicaps[player];
		battle_state->players[player].level = metadata->levels[player];
		battle_state->players[player].tag = (metadata->player_kinds[player] == nFTPlayerKindMan) ? player : GMCOMMON_PLAYERS_MAX;
		battle_state->players[player].is_single_stockicon = (metadata->game_rules & SCBATTLE_GAMERULE_TIME) ? TRUE : FALSE;

		if (metadata->player_kinds[player] == nFTPlayerKindMan)
		{
			battle_state->players[player].color =
			(metadata->is_team_battle == FALSE) ? player : dIFCommonPlayerTeamColorIDs[metadata->teams[player]];
			battle_state->pl_count++;
		}
		else if (metadata->player_kinds[player] == nFTPlayerKindCom)
		{
			battle_state->players[player].color =
			(metadata->is_team_battle == FALSE) ? GMCOMMON_PLAYERS_MAX : dIFCommonPlayerTeamColorIDs[metadata->teams[player]];
			battle_state->cp_count++;
		}
	}
	gSCManagerSceneData.gkind = metadata->stage_kind;
}

void syNetReplayInitDebugEnv(void)
{
#ifdef PORT
	const char *frame_limit_env;

	sSYNetReplayRecordPath = getenv("SSB64_REPLAY_RECORD");
	sSYNetReplayPlayPath = getenv("SSB64_REPLAY_PLAY");
	frame_limit_env = getenv("SSB64_REPLAY_RECORD_FRAMES");

	if (frame_limit_env != NULL)
	{
		s32 frame_limit = atoi(frame_limit_env);

		if ((frame_limit > 0) && (frame_limit < SYNETINPUT_REPLAY_MAX_FRAMES))
		{
			sSYNetReplayRecordFrameLimit = frame_limit;
		}
	}
	if (sSYNetReplayPlayPath != NULL)
	{
		if (syNetReplayLoadDebugFile(sSYNetReplayPlayPath) != FALSE)
		{
			syNetReplayApplyBattleMetadata(&sSYNetReplayLoadedMetadata);
			syUtilsSetRandomSeed(sSYNetReplayLoadedMetadata.rng_seed);
			gSCManagerSceneData.scene_prev = nSCKindVSMode;
			gSCManagerSceneData.scene_curr = nSCKindVSBattle;
		}
	}
#endif
}

void syNetReplayStartVSSession(SCBattleState *battle_state)
{
	SYNetInputReplayMetadata metadata;
	u32 tick;
	s32 player;

	if (sSYNetReplayIsPlaybackLoaded != FALSE)
	{
		syNetInputClearReplayFrames();
		syNetInputSetReplayMetadata(&sSYNetReplayLoadedMetadata);
		syUtilsSetRandomSeed(sSYNetReplayLoadedMetadata.rng_seed);

		for (tick = 0; tick < sSYNetReplayLoadedFrameCount; tick++)
		{
			for (player = 0; player < MAXCONTROLLERS; player++)
			{
				syNetInputSetReplayFrame(player, tick, &sSYNetReplayLoadedFrames[player][tick]);
			}
		}
		for (player = 0; player < MAXCONTROLLERS; player++)
		{
			syNetInputSetSlotSource(player, nSYNetInputSourceSaved);
		}
		sSYNetReplayIsPlaybackActive = TRUE;
		sSYNetReplayIsPlaybackVerified = FALSE;

#ifdef PORT
		port_log("SSB64 Replay: playback start path=%s frames=%u checksum=0x%08X stage=%u seed=%u\n",
		         sSYNetReplayPlayPath, sSYNetReplayLoadedFrameCount, sSYNetReplayLoadedInputChecksum,
		         sSYNetReplayLoadedMetadata.stage_kind, sSYNetReplayLoadedMetadata.rng_seed);
#endif
		return;
	}
	if (sSYNetReplayRecordPath != NULL)
	{
		syNetReplayCaptureBattleMetadata(battle_state, &metadata);
		syNetInputClearReplayFrames();
		syNetInputSetReplayMetadata(&metadata);
		syNetInputSetRecordingEnabled(TRUE);
		sSYNetReplayIsRecording = TRUE;
		sSYNetReplayIsRecordWritten = FALSE;

#ifdef PORT
		port_log("SSB64 Replay: recording start path=%s limit=%u stage=%u seed=%u players=%u\n",
		         sSYNetReplayRecordPath, sSYNetReplayRecordFrameLimit, metadata.stage_kind,
		         metadata.rng_seed, metadata.player_count);
#endif
	}
}

void syNetReplayUpdate(void)
{
	if ((sSYNetReplayIsRecording != FALSE) && (sSYNetReplayIsRecordWritten == FALSE) &&
		(syNetInputGetRecordedFrameCount() >= sSYNetReplayRecordFrameLimit))
	{
		syNetReplayFinishVSSession();
	}
	if ((sSYNetReplayIsPlaybackActive != FALSE) && (sSYNetReplayIsPlaybackVerified == FALSE) &&
		(syNetInputGetTick() >= sSYNetReplayLoadedFrameCount))
	{
		u32 checksum = syNetInputGetHistoryInputChecksum(sSYNetReplayLoadedFrameCount);

#ifdef PORT
		port_log("SSB64 Replay: playback verify frames=%u expected=0x%08X actual=0x%08X result=%s\n",
		         sSYNetReplayLoadedFrameCount, sSYNetReplayLoadedInputChecksum, checksum,
		         (checksum == sSYNetReplayLoadedInputChecksum) ? "PASS" : "FAIL");
#endif
		sSYNetReplayIsPlaybackVerified = TRUE;
		sSYNetReplayIsPlaybackActive = FALSE;
	}
}

void syNetReplayFinishVSSession(void)
{
	if ((sSYNetReplayIsRecording != FALSE) && (sSYNetReplayIsRecordWritten == FALSE))
	{
		syNetReplayWriteDebugFile(sSYNetReplayRecordPath);
		syNetInputSetRecordingEnabled(FALSE);
		sSYNetReplayIsRecording = FALSE;
		sSYNetReplayIsRecordWritten = TRUE;
	}
}

sb32 syNetReplayWriteDebugFile(const char *path)
{
	SYNetReplayFileHeader header;
	SYNetInputReplayMetadata metadata;
	SYNetInputFrame frame;
	FILE *fp;
	u32 tick;
	s32 player;

	if ((path == NULL) || (syNetInputGetReplayMetadata(&metadata) == FALSE))
	{
		return FALSE;
	}
	fp = fopen(path, "wb");

	if (fp == NULL)
	{
#ifdef PORT
		port_log("SSB64 Replay: failed to open record path=%s\n", path);
#endif
		return FALSE;
	}
	header.magic = SYNETINPUT_REPLAY_MAGIC;
	header.version = SYNETINPUT_REPLAY_VERSION;
	header.metadata_size = sizeof(SYNetInputReplayMetadata);
	header.frame_size = sizeof(SYNetInputFrame);
	header.frame_count = syNetInputGetRecordedFrameCount();
	header.player_count = MAXCONTROLLERS;
	header.input_checksum = syNetInputGetReplayInputChecksum();

	fwrite(&header, sizeof(header), 1, fp);
	fwrite(&metadata, sizeof(metadata), 1, fp);

	for (tick = 0; tick < header.frame_count; tick++)
	{
		for (player = 0; player < MAXCONTROLLERS; player++)
		{
			if (syNetInputGetReplayFrame(player, tick, &frame) == FALSE)
			{
				memset(&frame, 0, sizeof(frame));
				frame.tick = tick;
			}
			fwrite(&frame, sizeof(frame), 1, fp);
		}
	}
	fclose(fp);

#ifdef PORT
	port_log("SSB64 Replay: wrote path=%s frames=%u checksum=0x%08X\n",
	         path, header.frame_count, header.input_checksum);
#endif
	return TRUE;
}

sb32 syNetReplayLoadDebugFile(const char *path)
{
	SYNetReplayFileHeader header;
	FILE *fp;
	u32 tick;
	s32 player;

	if (path == NULL)
	{
		return FALSE;
	}
	fp = fopen(path, "rb");

	if (fp == NULL)
	{
#ifdef PORT
		port_log("SSB64 Replay: failed to open playback path=%s\n", path);
#endif
		return FALSE;
	}
	if (fread(&header, sizeof(header), 1, fp) != 1)
	{
		fclose(fp);
		return FALSE;
	}
	if ((header.magic != SYNETINPUT_REPLAY_MAGIC) ||
		(header.version != SYNETINPUT_REPLAY_VERSION) ||
		(header.metadata_size != sizeof(SYNetInputReplayMetadata)) ||
		(header.frame_size != sizeof(SYNetInputFrame)) ||
		(header.frame_count > SYNETINPUT_REPLAY_MAX_FRAMES) ||
		(header.player_count != MAXCONTROLLERS))
	{
		fclose(fp);
		return FALSE;
	}
	if (fread(&sSYNetReplayLoadedMetadata, sizeof(sSYNetReplayLoadedMetadata), 1, fp) != 1)
	{
		fclose(fp);
		return FALSE;
	}
	syNetReplayClearLoadedFrames();
	sSYNetReplayLoadedFrameCount = header.frame_count;
	sSYNetReplayLoadedInputChecksum = header.input_checksum;

	for (tick = 0; tick < header.frame_count; tick++)
	{
		for (player = 0; player < MAXCONTROLLERS; player++)
		{
			if (fread(&sSYNetReplayLoadedFrames[player][tick], sizeof(SYNetInputFrame), 1, fp) != 1)
			{
				fclose(fp);
				return FALSE;
			}
		}
	}
	fclose(fp);
	sSYNetReplayIsPlaybackLoaded = TRUE;

#ifdef PORT
	port_log("SSB64 Replay: loaded path=%s frames=%u checksum=0x%08X stage=%u seed=%u\n",
	         path, sSYNetReplayLoadedFrameCount, sSYNetReplayLoadedInputChecksum,
	         sSYNetReplayLoadedMetadata.stage_kind, sSYNetReplayLoadedMetadata.rng_seed);
#endif
	return TRUE;
}
