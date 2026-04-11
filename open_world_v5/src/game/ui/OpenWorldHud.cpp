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
                          const std::vector<ActiveNpcRoutineInfo>& activeRoutines,
                          const char* interactionPrompt,
                          const char* statusText)
    {
        DrawText(TextFormat("Time: %s", formattedTime), 20, 250, 20, SKYBLUE);
        DrawText(TextFormat("Region: %s", profile.currentRegion.c_str()), 20, 275, 20, WHITE);
        DrawText(TextFormat("Health: %i / %i", profile.playerHealth, profile.playerMaxHealth), 20, 300, 20, WHITE);
        DrawText(TextFormat("Gold: %i", profile.inventory.gold), 20, 325, 20, GOLD);
        DrawText(TextFormat("Weapon: %s", profile.inventory.equippedWeaponId.empty() ? "(none)" : profile.inventory.equippedWeaponId.c_str()), 20, 350, 20, RAYWHITE);
        DrawText(TextFormat("Tracked Quests: %i", (int)profile.quests.size()), 20, 375, 20, LIME);
        DrawText(TextFormat("Safe Zone: %s", simState.safeZone ? "Yes" : "No"), 20, 400, 20, simState.safeZone ? GREEN : ORANGE);

        if (interactionPrompt && interactionPrompt[0] != '\0')
            DrawText(interactionPrompt, 20, 430, 22, YELLOW);
        if (statusText && statusText[0] != '\0')
            DrawText(statusText, 20, 455, 20, ORANGE);

        int y = 490;
        DrawText("Inventory:", 20, y, 20, SKYBLUE);
        y += 24;
        for (const auto& item : profile.inventory.items)
        {
            DrawText(item.c_str(), 30, y, 18, LIGHTGRAY);
            y += 18;
            if (y > 760) break;
        }

        y += 12;
        DrawText("NPC Routines:", 20, y, 20, LIME);
        y += 24;
        for (const auto& info : activeRoutines)
        {
            DrawText(TextFormat("%s -> %s @ %s", info.npcName.c_str(), info.activity.c_str(), info.locationTag.c_str()),
                     30, y, 18, LIGHTGRAY);
            y += 20;
            if (y > 860) break;
        }
    }
}
