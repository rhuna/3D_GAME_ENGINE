#pragma once

#include <string>
#include <vector>

#include "raylib.h"
#include "game/inventory/InventoryComponent.h"
#include "game/quests/QuestState.h"

namespace fw {

struct SaveGameProfile {
    std::string currentRegion = "village";

    int playerHealth = 100;
    int playerMaxHealth = 100;
    Vector3 playerPosition{0.0f, 0.0f, 0.0f};

    InventoryComponent inventory {};
    std::vector<QuestEntry> quests;

    bool SaveToFile(const std::string& path) const;
    bool LoadFromFile(const std::string& path);
    void ResetToDefaults();
};

} // namespace fw
