/*
 * EngineEvent.h — Engine-loop and UI events for BattleShip mods.
 *
 * These are the events fired from BattleShip's main loop and from the
 * port's ImGui render. Mods subscribe via REGISTER_LISTENER from
 * libultraship's bridge/eventsbridge.
 *
 * Event firing sites (engine side):
 *   - GamePreUpdateEvent  — fired in port.cpp before the per-frame game
 *                           tick (gmMain dispatch).
 *   - GamePostUpdateEvent — fired in port.cpp after the per-frame game
 *                           tick. Most common hook for mods that want
 *                           to read/write game state per frame.
 *   - DisplayPreUpdateEvent / DisplayPostUpdateEvent — wraps the GFX
 *                           dispatch (RCP frame submit).
 *   - EngineRenderMenubarEvent — fired during ImGui menubar render so
 *                           mods can add their own menu items.
 *   - EngineRenderModsEvent — fired during ImGui main render so mods
 *                           can draw their own ImGui windows.
 *
 * Convention: REGISTER_EVENT(eventType) is called once at engine init
 * (port.cpp or PortMenu.cpp) so the EventID is allocated. CALL_EVENT
 * fires it.
 */
#pragma once

/* C-mode mods compile against EventTypes.h directly (DEFINE_EVENT macro,
 * IEvent/EventID/ListenerID typedefs) without dragging in the C++
 * EventSystem class. The C-callable bridge symbols REGISTER_LISTENER /
 * UNREGISTER_LISTENER / CALL_EVENT use are extern "C" in eventsbridge.h. */
#include "ship/events/EventTypes.h"
#include "libultraship/bridge/eventsbridge.h"

DEFINE_EVENT(GamePreUpdateEvent);
DEFINE_EVENT(GamePostUpdateEvent);

DEFINE_EVENT(DisplayPreUpdateEvent);
DEFINE_EVENT(DisplayPostUpdateEvent);

DEFINE_EVENT(EngineRenderMenubarEvent);
DEFINE_EVENT(EngineRenderModsEvent);
