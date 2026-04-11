#include "game/ui/OpenWorldHud.h"
#include "game/save/SaveGameProfile.h"
#include "game/world/RegionSimulationState.h"
#include "game/simulation/NpcRoutineSystem.h"
#include "raylib.h"
namespace fw { void DrawOpenWorldHud(const SaveGameProfile& profile, const char* formattedTime, const RegionSimulationState& simState, const std::vector<ActiveNpcRoutineInfo>&, const char* interactionPrompt, const char* statusText, int aliveEnemies, int attack, int defense){ DrawText(TextFormat("Time: %s", formattedTime),20,250,20,SKYBLUE); DrawText(TextFormat("Region: %s", profile.currentRegion.c_str()),20,275,20,WHITE); DrawText(TextFormat("Health: %i / %i", profile.playerHealth, profile.playerMaxHealth),20,300,20,WHITE); DrawText(TextFormat("Gold: %i", profile.inventory.gold),20,325,20,GOLD); DrawText(TextFormat("Atk: %i  Def: %i", attack, defense),20,350,20,ORANGE); DrawText(TextFormat("Tracked Quests: %i", (int)profile.quests.size()),20,375,20,LIME); DrawText(TextFormat("Enemies Alive: %i", aliveEnemies),20,400,20,RED); DrawText(TextFormat("Safe Zone: %s", simState.safeZone?"Yes":"No"),20,425,20,simState.safeZone?GREEN:ORANGE); if(interactionPrompt&&interactionPrompt[0]) DrawText(interactionPrompt,20,455,22,YELLOW); if(statusText&&statusText[0]) DrawText(statusText,20,480,20,ORANGE); }
}
