#pragma once

#include <string>

namespace fw {

enum class EnemyArchetype {
    Walker,
    Runner,
    Ranged
};

struct EnemyComponent {
    EnemyArchetype archetype = EnemyArchetype::Walker;
    float moveSpeed = 2.5f;
    int contactDamage = 10;
    float contactDamageCooldown = 0.8f;
    float contactDamageCooldownRemaining = 0.0f;
    float preferredRange = 6.0f;
    float projectileCooldown = 1.8f;
    float projectileCooldownRemaining = 0.0f;
    int projectileDamage = 8;
    const char* displayName = "Walker";
};

} // namespace fw
