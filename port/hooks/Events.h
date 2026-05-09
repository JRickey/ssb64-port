/*
 * Events.h — Convenience aggregator for BattleShip-specific engine events.
 *
 * Engine-side: include this then CALL_EVENT(GamePostUpdateEvent) at the
 * appropriate firing point. Mod-side: include this then REGISTER_LISTENER
 * (GamePostUpdateEvent, EVENT_PRIORITY_NORMAL, OnFrameUpdate).
 *
 * Per-category event headers live under list/. Add new categories there;
 * keep this file thin.
 */
#pragma once

#include "list/EngineEvent.h"
