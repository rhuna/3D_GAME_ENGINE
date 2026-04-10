#pragma once

#include <string>

namespace fw {

enum class EnemyArchetype {
    Walker,
    Runner
};

struct EnemyComponent {
    EnemyArchetype archetype = EnemyArchetype::Walker;
    float moveSpeed = 2.5f;
    int contactDamage = 10;
    float contactDamageCooldown = 0.8f;
    float contactDamageCooldownRemaining = 0.0f;
    const char* displayName = "Walker";
};

} // namespace fw
