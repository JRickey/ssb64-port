#include "enhancements.h"
#include <stdint.h>
#include <time.h>
#include <string.h>

// Include your port's logging system
#include "port_log.h"

// LibUltraShip includes for CVar access
#include <libultraship/libultraship.h>
#include <libultraship/bridge.h>

// Discord SDK header
#include <discord_rpc.h>

#define DISCORD_CLIENT_ID "1503877021615390741"

extern "C" {
    // Fetches the current scene ID from the game engine
    unsigned char port_diag_get_scene_curr(void);
}

static bool g_drpInitialized = false;
static int64_t g_startTime = 0;

// ---------------------------------------------------------
// DISCORD SDK CALLBACKS
// ---------------------------------------------------------

static void handleDiscordReady(const DiscordUser* connectedUser) {
    port_log("SSB64 DRP: Successfully connected to Discord as %s\n", connectedUser->username);
}

static void handleDiscordDisconnected(int errcode, const char* message) {
    port_log("SSB64 DRP: Disconnected from Discord (Code %d: %s)\n", errcode, message);
}

static void handleDiscordError(int errcode, const char* message) {
    port_log("SSB64 DRP: Discord SDK Error (Code %d: %s)\n", errcode, message);
}

// ---------------------------------------------------------
// INTERNAL LIFECYCLE
// ---------------------------------------------------------

static void DRP_Init() {
    if (g_drpInitialized) return;

    port_log("SSB64 DRP: Initializing Discord SDK...\n");

    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));

    // Attach our logging callbacks
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;
    handlers.errored = handleDiscordError;

    Discord_Initialize(DISCORD_CLIENT_ID, &handlers, 1, NULL);

    g_startTime = time(NULL);
    g_drpInitialized = true;
}

static void DRP_Shutdown() {
    if (!g_drpInitialized) return;

    port_log("SSB64 DRP: Shutting down Discord SDK...\n");

    Discord_ClearPresence();
    Discord_Shutdown();

    g_drpInitialized = false;
}

// ---------------------------------------------------------
// PUBLIC FUNCTIONS
// ---------------------------------------------------------

namespace ssb64 {
    namespace enhancements {

        void UpdateDiscordPresence(const char* gameState, const char* matchDetails) {
            bool isDRPEnabled = CVarGetInteger("gSettings.Menu.EnableDRP", 0) != 0;

            if (!isDRPEnabled) {
                if (g_drpInitialized) {
                    port_log("SSB64 DRP: CVar turned off, triggering shutdown.\n");
                    DRP_Shutdown();
                }
                return;
            }

            if (!g_drpInitialized) {
                DRP_Init();
            }

            port_log("SSB64 DRP: Updating presence -> State: '%s' | Details: '%s'\n", gameState, matchDetails);

            DiscordRichPresence discordPresence;
            memset(&discordPresence, 0, sizeof(discordPresence));

            discordPresence.state = gameState;
            discordPresence.details = matchDetails;
            discordPresence.startTimestamp = g_startTime;
            discordPresence.largeImageKey = "battleship_logo";
            discordPresence.largeImageText = "Unofficial Smash 64 PC Port";

            Discord_UpdatePresence(&discordPresence);
        }

        static void UpdatePresenceForScene(unsigned char sceneId) {
            switch (sceneId) {
                // --- MENUS ---
                case 1:  // Title
                    UpdateDiscordPresence("Title Screen", "Press Start");
                    break;
                case 7:  // Main Menu
                case 8:  // 1P Mode Menu
                case 9:  // VS Mode Menu
                case 10: // VS Options
                case 11: // VS Item Switch
                case 15: // Screen Adjust
                    UpdateDiscordPresence("In Menus", "Choosing a Game Mode");
                    break;
                case 25: // Records Screen
                case 26: // Character Data Screen
                    UpdateDiscordPresence("In Menus", "Looking at Stats");
                    break;

                // --- CHARACTER SELECT (CSS) ---
                case 16: // VS Character Select
                    // --- 1P / TRAINING CHARACTER SELECT ---
                case 17: // 1P Game Players
                    UpdateDiscordPresence("CSS", "Choosing a Fighter");
                    break;
                case 18: // Training Mode Players
                    UpdateDiscordPresence("Training Mode", "Choosing a Fighter");
                    break;
                case 19: // Bonus 1 Players
                    UpdateDiscordPresence("Break the Targets", "Choosing a Fighter");
                    break;
                case 20: // Bonus 2 Players
                    UpdateDiscordPresence("Board the Platforms", "Choosing a Fighter");
                    break;

                // --- STAGE SELECT ---
                case 21: // Stage Select
                    UpdateDiscordPresence("Stage Select", "Picking a Stage");
                    break;

                // --- IN-GAME & RESULTS ---
                case 14: // 1P VS Screen (The "VS" splash before a fight)

                // --- GAMEPLAY ---
                case 22: // VS Battle In-Game
                    UpdateDiscordPresence("Versus Mode", "Brawling");
                    break;
                case 23: // 1P Gameplay (Classic, Training, Bonus)
                    UpdateDiscordPresence("Single Player", "In a Match");
                    break;
                case 24: // VS Results Screen
                    UpdateDiscordPresence("Post-Match", "Viewing Results");
                    break;
                case 13: // Challenger Approaching Screen
                    UpdateDiscordPresence("Challenger Approaching!", "A New Foe Has Appeared");
                    break;

                // --- INTRO & CUTSCENES ---
                case 27: // N64 Logo / Startup
                case 28: // Opening Room (Master Hand desk)
                case 29: case 30: case 31: case 32: case 33: case 34:
                case 35: case 36: case 37: case 38: case 39: case 40:
                case 41: case 42: case 43: case 44: case 45: case 46:
                    UpdateDiscordPresence("Watching the Intro", "Nostalgia Trip");
                    break;

                // --- FALLBACK (Including Debug Menus & Messages) ---
                case 0:  // No Controller Screen
                case 12: // Unlock Message
                default:
                    UpdateDiscordPresence("in the game", "Somewhere");
                    break;
            }
        }

        void TickDiscordPresence() {
            // 1. Read the menu toggle
            bool isDRPEnabled = CVarGetInteger("gSettings.Menu.EnableDRP", 0) != 0;

            // Remember the last scene so we don't spam Discord's API every frame
            static unsigned char s_lastScene = 255;

            if (isDRPEnabled) {
                // Fetch the current scene from the game engine
                unsigned char currentScene = port_diag_get_scene_curr();

                // If the scene changed (or the game just booted), push an update!
                if (currentScene != s_lastScene) {
                    s_lastScene = currentScene;
                    UpdatePresenceForScene(currentScene);
                }

                // Process SDK callbacks if it's currently running
                if (g_drpInitialized) {
                    Discord_RunCallbacks();
                }
            }
            else if (g_drpInitialized) {
                // If the player unchecks the box mid-game, shut it down immediately
                DRP_Shutdown();

                // Reset our tracker so it updates instantly if they check the box again
                s_lastScene = 255;
            }
        }

    } // namespace enhancements
} // namespace ssb64
