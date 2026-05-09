#include "enhancements.h"

#include <libultraship/bridge/consolevariablebridge.h>

#include <cstddef>
#include <cstdint>

constexpr const char* kStageHazardsDisabledCVar = "gEnhancements.StageHazardsDisabled";

constexpr unsigned char kSceneVSBattle = 22;
constexpr unsigned char kScene1PGame = 52;
constexpr unsigned char kScene1PTrainingMode = 54;

constexpr unsigned char kBattleStageEnd = 8;
constexpr unsigned char kStageSector = 1;

constexpr int kLinkGround = 1;
constexpr int kLinkItem = 4;
constexpr int kLinkWeapon = 5;

constexpr int kItemPowerBlock = 22;
constexpr int kItemGBumper = 23;
constexpr int kItemPakkun = 24;
constexpr int kItemGroundMonsterStart = 27;
constexpr int kItemGroundMonsterEnd = 31;

constexpr int kWeaponArwingLaser2D = 18;
constexpr int kWeaponArwingLaser3D = 19;

struct GObj;

union GCUserData {
    int32_t s;
    uint32_t u;
    void* p;
};

struct GObjProcess {
    GObjProcess* link_next;
    GObjProcess* link_prev;
    GObjProcess* priority_next;
    GObjProcess* priority_prev;
    int32_t priority;
    uint8_t kind;
    uint8_t is_paused;
    GObj* parent_gobj;
    union {
        void* gobjthread;
        void (*func)(GObj*);
    } exec;
    void (*func_id)(GObj*);
};

struct GObjScript {
    GObj* next_gobj;
    int32_t id;
};

struct GObj {
    uint32_t id;
    GObj* link_next;
    GObj* link_prev;
    uint8_t link_id;
    uint8_t dl_link_id;
    uint8_t frame_draw_last;
    uint8_t obj_kind;
    uint32_t link_priority;
    void (*func_run)(GObj*);
    GObjProcess* gobjproc_head;
    union {
        int32_t unk_0x1C;
        GObj* unk_gobj_0x1C;
        GObjProcess* gobjproc_tail;
    };
    GObj* dl_link_next;
    GObj* dl_link_prev;
    uint32_t dl_link_priority;
    void (*proc_display)(GObj*);
    uint64_t camera_mask;
    uint32_t camera_tag;
    uint64_t buffer_mask;
    GObjScript gobjscripts[5];
    int32_t gobjscripts_num;
    void* obj;
    float anim_frame;
    uint32_t flags;
    void (*func_anim)(void*, int32_t, float);
    GCUserData user_data;
};

struct ITStructPrefix {
    ITStructPrefix* next;
    GObj* item_gobj;
    GObj* owner_gobj;
    int32_t kind;
};

struct WPStructPrefix {
    WPStructPrefix* next;
    GObj* weapon_gobj;
    GObj* owner_gobj;
    int32_t kind;
};

struct SCBattleStatePrefix {
    uint8_t game_type;
    uint8_t gkind;
    uint8_t is_team_battle;
    uint8_t game_rules;
    uint8_t pl_count;
    uint8_t cp_count;
    uint8_t time_limit;
    uint8_t stocks;
    uint8_t handicap;
    uint8_t is_team_attack;
    uint8_t is_stage_select;
    uint8_t damage_ratio;
    uint32_t item_toggles;
    uint8_t is_reset_players;
    uint8_t game_status;
    uint8_t pad_12[2];
    uint32_t time_remain;
    uint32_t time_passed;
};

struct SCCommonDataPrefix {
    uint8_t scene_curr;
    uint8_t scene_prev;
};

static_assert(offsetof(GObj, link_next) == 0x08);
static_assert(offsetof(GObj, gobjproc_head) == 0x28);
static_assert(offsetof(GObj, user_data) == 0xE0);
static_assert(offsetof(GObjProcess, priority) == 0x20);
static_assert(offsetof(GObjProcess, is_paused) == 0x25);
static_assert(offsetof(ITStructPrefix, kind) == 0x18);
static_assert(offsetof(WPStructPrefix, kind) == 0x18);
static_assert(offsetof(SCBattleStatePrefix, gkind) == 0x01);
static_assert(offsetof(SCBattleStatePrefix, time_passed) == 0x18);

extern "C" {
    extern SCCommonDataPrefix gSCManagerSceneData;
    extern SCBattleStatePrefix* gSCManagerBattleState;
    extern GObj* gGCCommonLinks[];
    void gcEjectGObj(GObj* gobj);
}

namespace {

struct BattleKey {
    unsigned char scene;
    unsigned char gkind;
    unsigned int time_passed;
    const void* battle_state;
};

bool sWasInBattle = false;
bool sDisableHazardsForBattle = false;
BattleKey sBattleKey = {};

bool IsHazardControlledBattleScene(unsigned char scene) {
    return scene == kSceneVSBattle ||
           scene == kScene1PGame ||
           scene == kScene1PTrainingMode;
}

bool IsBattleStage() {
    return gSCManagerBattleState != nullptr &&
           gSCManagerBattleState->gkind <= kBattleStageEnd;
}

BattleKey CurrentBattleKey() {
    return {
        gSCManagerSceneData.scene_curr,
        gSCManagerBattleState != nullptr ? gSCManagerBattleState->gkind : static_cast<uint8_t>(0),
        gSCManagerBattleState != nullptr ? gSCManagerBattleState->time_passed : 0,
        gSCManagerBattleState,
    };
}

bool IsNewBattle(const BattleKey& key) {
    return !sWasInBattle ||
           key.scene != sBattleKey.scene ||
           key.gkind != sBattleKey.gkind ||
           key.battle_state != sBattleKey.battle_state ||
           key.time_passed < sBattleKey.time_passed;
}

bool IsCurrentStage(unsigned char gkind) {
    return gSCManagerBattleState != nullptr && gSCManagerBattleState->gkind == gkind;
}

void LatchBattleSettingIfNeeded() {
    const bool is_battle =
        IsHazardControlledBattleScene(gSCManagerSceneData.scene_curr) && IsBattleStage();

    if (!is_battle) {
        sWasInBattle = false;
        sDisableHazardsForBattle = false;
        sBattleKey = {};
        return;
    }

    BattleKey key = CurrentBattleKey();
    if (IsNewBattle(key)) {
        sDisableHazardsForBattle = CVarGetInteger(kStageHazardsDisabledCVar, 0) != 0;
    }

    sWasInBattle = true;
    sBattleKey = key;
}

void PauseGroundHazardProcesses() {
    for (GObj* gobj = gGCCommonLinks[kLinkGround]; gobj != nullptr; gobj = gobj->link_next) {
        for (GObjProcess* proc = gobj->gobjproc_head; proc != nullptr; proc = proc->link_next) {
            if (proc->priority >= 4) {
                proc->is_paused = true;
            }
        }
    }
}

bool HasProcessPriority(GObj* gobj, int32_t priority) {
    for (GObjProcess* proc = gobj->gobjproc_head; proc != nullptr; proc = proc->link_next) {
        if (proc->priority == priority) {
            return true;
        }
    }

    return false;
}

void EjectSectorArwingActors() {
    if (!IsCurrentStage(kStageSector)) {
        return;
    }

    GObj* gobj = gGCCommonLinks[kLinkGround];

    while (gobj != nullptr) {
        GObj* next = gobj->link_next;

        // Sector Z has a separate animation-only ground actor for Arwings.
        // The main Sector ground actor has the priority-4 hazard scheduler.
        if (HasProcessPriority(gobj, 5) && !HasProcessPriority(gobj, 4)) {
            gcEjectGObj(gobj);
        }

        gobj = next;
    }
}

bool IsGroundHazardItemKind(int kind) {
    // Only stage-owned hazard items. Do not touch normal item spawns,
    // Poke Ball monsters, fighter items, or player-created items.
    return kind == kItemPowerBlock ||
           kind == kItemGBumper ||
           kind == kItemPakkun ||
           (kind >= kItemGroundMonsterStart && kind <= kItemGroundMonsterEnd);
}

bool IsGroundHazardItemGObj(GObj* item_gobj) {
    if (item_gobj == nullptr) {
        return false;
    }

    ITStructPrefix* ip = static_cast<ITStructPrefix*>(item_gobj->user_data.p);
    return ip != nullptr && IsGroundHazardItemKind(ip->kind);
}

void EjectGroundHazardItems() {
    GObj* item_gobj = gGCCommonLinks[kLinkItem];

    while (item_gobj != nullptr) {
        GObj* next = item_gobj->link_next;

        if (IsGroundHazardItemGObj(item_gobj)) {
            gcEjectGObj(item_gobj);
        }

        item_gobj = next;
    }
}

bool IsStageHazardWeaponKind(int kind) {
    return kind == kWeaponArwingLaser2D || kind == kWeaponArwingLaser3D;
}

void EjectGroundHazardWeapons() {
    GObj* weapon_gobj = gGCCommonLinks[kLinkWeapon];

    while (weapon_gobj != nullptr) {
        GObj* next = weapon_gobj->link_next;
        WPStructPrefix* wp = static_cast<WPStructPrefix*>(weapon_gobj->user_data.p);

        if (wp != nullptr && (IsStageHazardWeaponKind(wp->kind) || IsGroundHazardItemGObj(wp->owner_gobj))) {
            gcEjectGObj(weapon_gobj);
        }

        weapon_gobj = next;
    }
}

void ApplyHazardDisable() {
    EjectSectorArwingActors();
    PauseGroundHazardProcesses();
    EjectGroundHazardWeapons();
    EjectGroundHazardItems();
}

} // namespace

extern "C" void port_enhancement_stage_hazards_tick(void) {
    LatchBattleSettingIfNeeded();

    if (sWasInBattle && sDisableHazardsForBattle) {
        ApplyHazardDisable();
    }
}

namespace ssb64 {
namespace enhancements {

const char* StageHazardsDisabledCVarName() {
    return kStageHazardsDisabledCVar;
}

} // namespace enhancements
} // namespace ssb64
