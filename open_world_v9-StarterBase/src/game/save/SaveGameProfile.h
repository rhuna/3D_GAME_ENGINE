#pragma once
#include <string>
#include <vector>
#include "raylib.h"
#include "game/quests/QuestState.h"
#include "game/inventory/InventoryComponent.h"
namespace fw { struct SaveGameProfile { std::string currentRegion; int playerHealth = 100; int playerMaxHealth = 100; Vector3 playerPosition{}; InventoryComponent inventory; std::vector<QuestEntry> quests; bool SaveToFile(const std::string& path) const; bool LoadFromFile(const std::string& path); void ResetToDefaults(); }; }
