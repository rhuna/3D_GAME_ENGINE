#include "ecs/systems/MovementSystem.h"

#include <algorithm>

#include "core/Application.h"
#include "ecs/World.h"

namespace fw {

void MovementSystem::FixedUpdate(Application& app, World& world, float fixedDeltaTime) {
    (void)app;

    for (const Entity entity : world.Entities()) {
        TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        RigidbodyComponent* body = world.GetComponent<RigidbodyComponent>(entity);
        if (!transform || !body) {
            continue;
        }

        if (body->kinematic) {
            continue;
        }

        if (body->useGravity) {
            body->velocity.y -= 9.81f * fixedDeltaTime;
        }

        body->velocity.x += body->acceleration.x * fixedDeltaTime;
        body->velocity.y += body->acceleration.y * fixedDeltaTime;
        body->velocity.z += body->acceleration.z * fixedDeltaTime;

        const float dragFactor = std::max(0.0f, 1.0f - body->drag * fixedDeltaTime);
        body->velocity.x *= dragFactor;
        body->velocity.z *= dragFactor;

        transform->position.x += body->velocity.x * fixedDeltaTime;
        transform->position.y += body->velocity.y * fixedDeltaTime;
        transform->position.z += body->velocity.z * fixedDeltaTime;

        LifetimeComponent* lifetime = world.GetComponent<LifetimeComponent>(entity);
        if (lifetime) {
            lifetime->secondsRemaining -= fixedDeltaTime;
        }
    }

    world.CollectGarbage();
}

} // namespace fw
