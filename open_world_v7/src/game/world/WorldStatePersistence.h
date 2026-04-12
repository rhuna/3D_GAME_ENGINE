#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "game/combat/EnemySystem.h"
#include "game/loot/LootSystem.h"
#include "raylib.h"
namespace fw { struct PersistentRegionState { std::vector<EnemyInstance> enemies; std::vector<Vector3> gatherPoints; std::vector<WorldLootDrop> lootDrops; bool initialized = false; bool encounterCleared = false; }; class WorldStatePersistence { public: PersistentRegionState& GetOrCreate(const std::string& regionId); void ClearRegion(const std::string& regionId); void ResetAll(); bool SaveToFile(const std::string& path) const; bool LoadFromFile(const std::string& path); private: std::unordered_map<std::string, PersistentRegionState> m_regions; }; }
