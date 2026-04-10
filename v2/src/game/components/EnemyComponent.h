#pragma once

namespace fw {

struct EnemyComponent {
    float moveSpeed = 2.5f;
    float contactDamageCooldown = 0.8f;
    float contactDamageCooldownRemaining = 0.0f;
};

} // namespace fw
