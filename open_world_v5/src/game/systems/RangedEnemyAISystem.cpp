#include "game/systems/RangedEnemyAISystem.h"

#include "raylib.h"
#include "raymath.h"

#include "core/Application.h"
#include "ecs/World.h"
#include "ecs/components/RigidbodyComponent.h"
#include "ecs/components/TransformComponent.h"
#include "game/audio/AudioBus.h"
#include "game/components/EnemyComponent.h"
#include "game/components/HealthComponent.h"
#include "game/components/ProjectileComponent.h"
#include "game/components/TeamComponent.h"
#include "gameplay/prefabs/PrefabDefinition.h"
#include "gameplay/prefabs/SpawnFactory.h"

namespace fw {

namespace {
constexpr int kEnemyTeam = 2;
}

void RangedEnemyAISystem::Update(Application& app, World& world, float deltaTime) {
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
        if (!enemy || enemy->archetype != EnemyArchetype::Ranged || !transform || !body || !health || health->current <= 0) {
            continue;
        }

        Vector3 toPlayer = Vector3Subtract(playerTransform->position, transform->position);
        toPlayer.y = 0.0f;
        const float distance = Vector3Length(toPlayer);
        if (distance <= 0.001f) {
            continue;
        }

        const Vector3 dir = Vector3Normalize(toPlayer);
        transform->rotationEuler.y = atan2f(dir.x, dir.z) * RAD2DEG;
        enemy->projectileCooldownRemaining -= deltaTime;

        const float stopRange = enemy->preferredRange;
        if (distance > stopRange + 0.8f) {
            body->velocity.x = dir.x * enemy->moveSpeed;
            body->velocity.z = dir.z * enemy->moveSpeed;
        } else if (distance < stopRange - 1.0f) {
            body->velocity.x = -dir.x * enemy->moveSpeed * 0.75f;
            body->velocity.z = -dir.z * enemy->moveSpeed * 0.75f;
        } else {
            body->velocity.x = 0.0f;
            body->velocity.z = 0.0f;
        }

        if (distance <= enemy->preferredRange + 3.0f && enemy->projectileCooldownRemaining <= 0.0f) {
            const Vector3 spawnPos = Vector3Add(transform->position, Vector3{0.0f, 0.9f, 0.0f});
            Entity projectile = 0;
            const Vector3 velocity = Vector3Scale(dir, 11.0f);

            if (const PrefabDefinition* prefab = app.GetPrefabLibrary().Find("projectile")) {
                PrefabDefinition instance = *prefab;
                instance.transform.position = Vector3Add(spawnPos, Vector3Scale(dir, 1.2f));
                if (instance.hasRigidbody) {
                    instance.rigidbody.velocity = velocity;
                }
                projectile = spawn::SpawnFromPrefab(world, instance);
            } else {
                projectile = spawn::SpawnProjectile(world, Vector3Add(spawnPos, Vector3Scale(dir, 1.2f)), velocity);
            }

            if (projectile != 0) {
                world.AddComponent<ProjectileComponent>(projectile, ProjectileComponent{enemy->projectileDamage, kEnemyTeam});
                world.AddComponent<TeamComponent>(projectile, TeamComponent{kEnemyTeam});
                enemy->projectileCooldownRemaining = enemy->projectileCooldown;
                AudioBus::PlayCast();
            }
        }
    }
}

} // namespace fw
