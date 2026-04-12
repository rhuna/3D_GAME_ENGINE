#pragma once

#include <string>

namespace fw {

struct EncounterSpawnerComponent {
    std::string encounterId;
    std::string enemyPrefab = "enemy_walker";
    int maxAlive = 3;
    float respawnDelay = 20.0f;
    float cooldown = 0.0f;
};

} // namespace fw
