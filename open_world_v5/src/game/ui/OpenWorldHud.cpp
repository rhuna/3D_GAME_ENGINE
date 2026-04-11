#include "game/ui/OpenWorldHud.h"
#include "game/save/SaveGameProfile.h"
#include "game/world/RegionSimulationState.h"
#include "game/simulation/NpcRoutineSystem.h"
#include "raylib.h"

namespace fw
{
    void DrawOpenWorldHud(const SaveGameProfile& profile,
                          const char* formattedTime,
                          const RegionSimulationState& simState,
                          const std::vector<ActiveNpcRoutineInfo>& activeRoutines)
    {
        DrawText(TextFormat("Time: %s", formattedTime), 20, 250, 20, SKYBLUE);
        DrawText(TextFormat("Region: %s", profile.currentRegion.c_str()), 20, 275, 20, WHITE);
        DrawText(TextFormat("Health: %i / %i", profile.playerHealth, profile.playerMaxHealth), 20, 300, 20, WHITE);
        DrawText(TextFormat("Gold: %i", profile.inventory.gold), 20, 325, 20, GOLD);
        DrawText(TextFormat("Weapon: %s", profile.inventory.equippedWeaponId.empty() ? "(none)" : profile.inventory.equippedWeaponId.c_str()), 20, 350, 20, RAYWHITE);

        DrawText(TextFormat("Safe Zone: %s", simState.safeZone ? "Yes" : "No"), 20, 385, 20, simState.safeZone ? GREEN : ORANGE);
        DrawText(TextFormat("Ambient Population: %i", simState.ambientPopulation), 20, 410, 20, LIGHTGRAY);
        DrawText(TextFormat("Active Encounters: %i", simState.activeEncounters), 20, 435, 20, LIGHTGRAY);

        int y = 470;
        DrawText("Active NPC Routines:", 20, y, 20, LIME);
        y += 24;
        for (const auto& info : activeRoutines)
        {
            DrawText(TextFormat("%s -> %s @ %s", info.npcName.c_str(), info.activity.c_str(), info.locationTag.c_str()),
                     30, y, 18, LIGHTGRAY);
            y += 20;
            if (y > 820) break;
        }
    }
}
