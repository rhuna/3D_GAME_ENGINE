#include "gameplay/systems/MovementSystem.h"

#include <algorithm>

namespace fw {

void MovementSystem::FixedUpdate(World& world, float fixedDeltaTime) const {
    for (auto& entity : world.Entities()) {
        if (!entity.active || !entity.transform || !entity.rigidbody) {
            continue;
        }

        auto& transform = *entity.transform;
        auto& body = *entity.rigidbody;

        if (body.kinematic) {
            continue;
        }

        if (body.useGravity) {
            body.velocity.y -= 9.81f * fixedDeltaTime;
        }

        body.velocity.x += body.acceleration.x * fixedDeltaTime;
        body.velocity.y += body.acceleration.y * fixedDeltaTime;
        body.velocity.z += body.acceleration.z * fixedDeltaTime;

        const float dragFactor = std::max(0.0f, 1.0f - body.drag * fixedDeltaTime);
        body.velocity.x *= dragFactor;
        body.velocity.z *= dragFactor;

        transform.position.x += body.velocity.x * fixedDeltaTime;
        transform.position.y += body.velocity.y * fixedDeltaTime;
        transform.position.z += body.velocity.z * fixedDeltaTime;

        if (entity.lifetime) {
            entity.lifetime->secondsRemaining -= fixedDeltaTime;
            if (entity.lifetime->secondsRemaining <= 0.0f) {
                entity.active = false;
            }
        }
    }
}

} // namespace fw
