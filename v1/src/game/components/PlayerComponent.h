#pragma once

namespace fw {

struct PlayerComponent {
    float moveSpeed = 8.0f;
    float projectileSpeed = 18.0f;
    float fireCooldown = 0.18f;
    float fireCooldownRemaining = 0.0f;
};

} // namespace fw
