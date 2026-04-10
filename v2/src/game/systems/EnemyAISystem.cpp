#include "game/systems/EnemyAISystem.h"
#include "raylib.h"
#include "raymath.h"

#include "core/Application.h"
#include "ecs/World.h"
#include "ecs/components/RigidbodyComponent.h"
#include "ecs/components/TransformComponent.h"
#include "game/components/EnemyComponent.h"
#include "game/components/HealthComponent.h"

namespace fw {

void EnemyAISystem::Update(Application& app, World& world, float deltaTime) {
    (void)app;
    const Entity player = world.FindByTag("player");
    const TransformComponent* playerTransform = world.GetComponent<TransformComponent>(player);
    const HealthComponent* playerHealth = world.GetComponent<HealthComponent>(player);
    if (!playerTransform || !playerHealth || playerHealth->current <= 0) {
        return;
    }

    for (const Entity entity : world.Entities()) {
        auto* enemy = world.GetComponent<EnemyComponent>(entity);
        auto* transform = world.GetComponent<TransformComponent>(entity);
        auto* body = world.GetComponent<RigidbodyComponent>(entity);
        auto* health = world.GetComponent<HealthComponent>(entity);
        if (!enemy || !transform || !body || !health || health->current <= 0) {
            continue;
        }

        Vector3 toPlayer = Vector3Subtract(playerTransform->position, transform->position);
        toPlayer.y = 0.0f;
        const float distance = Vector3Length(toPlayer);
        if (distance > 0.001f) {
            const Vector3 dir = Vector3Scale(Vector3Normalize(toPlayer), enemy->moveSpeed);
            body->velocity.x = dir.x;
            body->velocity.z = dir.z;
            transform->rotationEuler.y = atan2f(dir.x, dir.z) * RAD2DEG;
        }

        if (enemy->contactDamageCooldownRemaining > 0.0f) {
            enemy->contactDamageCooldownRemaining -= deltaTime;
        }
    }
}

} // namespace fw
