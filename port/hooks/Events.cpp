/*
 * Events.cpp — Allocates the storage for BattleShip's event IDs and
 * registers each event with libultraship's EventSystem at engine init.
 *
 * Allocation: this is the ONE translation unit that defines
 * INIT_EVENT_IDS before #include'ing Events.h. Every other file that
 * fires or listens for an event sees an extern declaration only.
 *
 * Registration: PortRegisterEvents() must be called ONCE after
 * sContext->InitEventSystem() succeeds. It allocates a runtime EventID
 * for each declared event so REGISTER_LISTENER / CALL_EVENT can dispatch.
 */
#define INIT_EVENT_IDS
#include "hooks/Events.h"

#include "libultraship/bridge/eventsbridge.h"

extern "C" void PortRegisterEvents(void) {
    REGISTER_EVENT(GamePreUpdateEvent);
    REGISTER_EVENT(GamePostUpdateEvent);
    REGISTER_EVENT(DisplayPreUpdateEvent);
    REGISTER_EVENT(DisplayPostUpdateEvent);
    REGISTER_EVENT(EngineRenderMenubarEvent);
    REGISTER_EVENT(EngineRenderModsEvent);
}
