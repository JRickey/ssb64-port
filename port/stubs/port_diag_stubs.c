/**
 * port_diag_stubs.c — tiny accessors that let port/ layer code read game
 * state (currently the active scene) without pulling the entire sc/ header
 * chain into port/*.cpp files. Compiled as part of the decomp target so it
 * has full visibility of SCCommonData and the nSCKind* enum.
 */

#include <ssb_types.h>
#include <sc/scene.h>
#include <sc/scmanager.h>

u8 port_diag_get_scene_curr(void)
{
	return gSCManagerSceneData.scene_curr;
}

u8 port_diag_get_scene_prev(void)
{
	return gSCManagerSceneData.scene_prev;
}

const char *port_diag_get_scene_name(u8 id)
{
	switch (id) {
	case nSCKindNoController:     return "NoController";
	case nSCKindTitle:            return "Title";
	case nSCKindDebugMaps:        return "DebugMaps";
	case nSCKindDebugCube:        return "DebugCube";
	case nSCKindDebugBattle:      return "DebugBattle";
	case nSCKindDebugFalls:       return "DebugFalls";
	case nSCKindDebugUnknown:     return "DebugUnknown";
	case nSCKindModeSelect:       return "ModeSelect";
	case nSCKind1PMode:           return "1PMode";
	case nSCKindVSMode:           return "VSMode";
	case nSCKindVSOptions:        return "VSOptions";
	case nSCKindVSItemSwitch:     return "VSItemSwitch";
	case nSCKindMessage:          return "Message";
	case nSCKind1PChallenger:     return "1PChallenger";
	case nSCKind1PIntro:          return "1PIntro";
	case nSCKindScreenAdjust:     return "ScreenAdjust";
	case nSCKindPlayersVS:        return "PlayersVS";
	case nSCKind1PGamePlayers:    return "1PGamePlayers";
	case nSCKindPlayers1PTraining:return "Players1PTraining";
	case nSCKind1PBonus1Players:  return "1PBonus1Players";
	case nSCKind1PBonus2Players:  return "1PBonus2Players";
	case nSCKindMaps:             return "Maps";
	case nSCKindVSBattle:         return "VSBattle";
	case nSCKindUnknownMario:     return "UnknownMario";
	case nSCKindVSResults:        return "VSResults";
	case nSCKindVSRecord:         return "VSRecord";
	case nSCKindCharacters:       return "Characters";
#if defined(REGION_US)
	case nSCKindStartup:          return "Startup";
#endif
	case nSCKindOpeningRoom:      return "OpeningRoom";
	case nSCKindOpeningPortraits: return "OpeningPortraits";
	case nSCKindOpeningMario:     return "OpeningMario";
	case nSCKindOpeningDonkey:    return "OpeningDonkey";
	case nSCKindOpeningSamus:     return "OpeningSamus";
	case nSCKindOpeningFox:       return "OpeningFox";
	case nSCKindOpeningLink:      return "OpeningLink";
	case nSCKindOpeningYoshi:     return "OpeningYoshi";
	case nSCKindOpeningPikachu:   return "OpeningPikachu";
	case nSCKindOpeningKirby:     return "OpeningKirby";
	case nSCKindOpeningRun:       return "OpeningRun";
	case nSCKindOpeningYoster:    return "OpeningYoster";
	case nSCKindOpeningCliff:     return "OpeningCliff";
	case nSCKindOpeningStandoff:  return "OpeningStandoff";
	case nSCKindOpeningYamabuki:  return "OpeningYamabuki";
	case nSCKindOpeningClash:     return "OpeningClash";
	case nSCKindOpeningSector:    return "OpeningSector";
	case nSCKindOpeningJungle:    return "OpeningJungle";
	case nSCKindOpeningNewcomers: return "OpeningNewcomers";
	case nSCKindBackupClear:      return "BackupClear";
	case nSCKindEnding:           return "Ending";
	case nSCKind1PContinue:       return "1PContinue";
	case nSCKind1PScoreUnk:       return "1PScoreUnk";
	case nSCKind1PStageClear:     return "1PStageClear";
	case nSCKind1PGame:           return "1PGame";
	case nSCKind1PBonusStage:     return "1PBonusStage";
	case nSCKind1PTrainingMode:   return "1PTrainingMode";
#if defined(REGION_US)
	case nSCKindCongra:           return "Congra";
#endif
	case nSCKindStaffroll:        return "Staffroll";
	case nSCKindOption:           return "Option";
	case nSCKindData:             return "Data";
	case nSCKindSoundTest:        return "SoundTest";
	case nSCKindExplain:          return "Explain";
	case nSCKindAutoDemo:         return "AutoDemo";
	default:                      return "?";
	}
}

/* Whether the current scene has authored climax-freeze frames whose timing
 * is preserved by the RCP cost-model in port/gameloop.cpp.
 *
 * The cost model treats DLs that exceed budget+rect_gate as "authored freezes"
 * and stretches them across multiple VIs to mimic real-N64 RDP overrun. That
 * fidelity is only needed in scenes that play the pre-rendered character
 * intro sequences whose climaxes (Yoshi tongue, Samus grapple, DK smash, etc.)
 * the game authors timed around the natural overrun:
 *   - AutoDemo (attract loop cycles through them)
 *   - the various Opening scenes (first-boot intros, mvOpening*)
 *   - Ending cutscene
 *
 * Everywhere else (CSS, menus, normal battle, 1P game, bonus stages, training,
 * stage clear, etc.) the cost+rect heuristic just mis-fires on UI-fillrate-
 * heavy DLs — the 4-fighter VS CSS panels cluster at cost ≈420k, rect_px ≈236k,
 * which trips both gates every frame and halves the game-tick rate. Issue #78.
 */
sb32 port_scene_wants_freeze_simulation(u8 scene_id)
{
	switch (scene_id) {
	case nSCKindOpeningRoom:
	case nSCKindOpeningPortraits:
	case nSCKindOpeningMario:
	case nSCKindOpeningDonkey:
	case nSCKindOpeningSamus:
	case nSCKindOpeningFox:
	case nSCKindOpeningLink:
	case nSCKindOpeningYoshi:
	case nSCKindOpeningPikachu:
	case nSCKindOpeningKirby:
	case nSCKindOpeningRun:
	case nSCKindOpeningYoster:
	case nSCKindOpeningCliff:
	case nSCKindOpeningStandoff:
	case nSCKindOpeningYamabuki:
	case nSCKindOpeningClash:
	case nSCKindOpeningSector:
	case nSCKindOpeningJungle:
	case nSCKindOpeningNewcomers:
	case nSCKindEnding:
	case nSCKindAutoDemo:
		return TRUE;
	default:
		return FALSE;
	}
}
