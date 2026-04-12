#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "game/combat/EnemySystem.h"
#include "game/loot/LootSystem.h"
#include "raylib.h"

namespace fw {

struct PersistentRegionState {
    std::vector<EnemyInstance> enemies;
    std::vector<Vector3> gatherPoints;
    std::vector<WorldLootDrop> lootDrops;
    std::unordered_set<std::string> harvestedNodeIds;
    std::unordered_set<std::string> usedSavePointIds;
    bool initialized = false;
    bool encounterCleared = false;
};

class WorldStatePersistence {
public:
    PersistentRegionState& GetOrCreate(const std::string& regionId);
    void ClearRegion(const std::string& regionId);
    void ResetAll();

    bool IsNodeHarvested(const std::string& regionId, const std::string& persistentId) const;
    void MarkNodeHarvested(const std::string& regionId, const std::string& persistentId);

    bool IsSavePointUsed(const std::string& regionId, const std::string& persistentId) const;
    void MarkSavePointUsed(const std::string& regionId, const std::string& persistentId);

    bool SaveToFile(const std::string& path) const;
    bool LoadFromFile(const std::string& path);

private:
    std::unordered_map<std::string, PersistentRegionState> m_regions;
};

} // namespace fw
