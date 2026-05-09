/*
 * demo.c — minimal BattleShip TCC mod sample.
 *
 * Subscribes to GamePostUpdateEvent and prints a heartbeat once per
 * second to the engine's log. This is the simplest possible mod shape:
 * ModInit registers a listener, ModExit unregisters it.
 *
 * To extend:
 *   - Subscribe to additional events from port/hooks/list/EngineEvent.h.
 *   - Call into engine functions that are exported in BattleShip.def
 *     (auto-generated post-build via tcc.exe -impdef).
 *   - Cross-mod calls go through ScriptGetFunction("ModName", "func").
 */
#include "mod.h"

#include "port/hooks/Events.h"

ListenerID gFrameUpdateListenerID;
static unsigned gFrames = 0;

void OnFrameUpdate(IEvent* event) {
    gFrames++;
    if ((gFrames % 60) == 0) {
        mod_log("[DemoMod] frame heartbeat: %u\n", gFrames);
    }
}

MOD_INIT() {
    gFrameUpdateListenerID = REGISTER_LISTENER(GamePostUpdateEvent, EVENT_PRIORITY_NORMAL, OnFrameUpdate);
    mod_log("[DemoMod] init OK\n");
}

MOD_EXIT() {
    UNREGISTER_LISTENER(GamePostUpdateEvent, gFrameUpdateListenerID);
    mod_log("[DemoMod] exit OK (saw %u frames)\n", gFrames);
}
