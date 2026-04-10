#include "game/systems/PlayerControllerSystem.h"

#include "raylib.h"
#include "raymath.h"

#include "core/Application.h"
#include "core/Input.h"
#include "ecs/World.h"
#include "ecs/components/RigidbodyComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "game/components/HealthComponent.h"
#include "game/components/PlayerComponent.h"
#include "game/components/ProjectileComponent.h"
#include "game/components/TeamComponent.h"
#include "gameplay/prefabs/PrefabDefinition.h"
#include "gameplay/prefabs/SpawnFactory.h"

namespace fw {

namespace {
constexpr int kPlayerTeam = 1;
}

void PlayerControllerSystem::Update(Application& app, World& world, float deltaTime) {
    const Entity player = world.FindByTag("player");
    if (player == 0) {
        return;
    }

    auto* transform = world.GetComponent<TransformComponent>(player);
    auto* body = world.GetComponent<RigidbodyComponent>(player);
    auto* playerComponent = world.GetComponent<PlayerComponent>(player);
    auto* health = world.GetComponent<HealthComponent>(player);
    if (!transform || !body || !playerComponent || !health || health->current <= 0) {
        return;
    }

    Input& input = app.GetInput();

    Vector3 move {0.0f, 0.0f, 0.0f};
    const Vector3 cameraForwardRaw = Vector3Subtract(app.GetCamera().target, app.GetCamera().position);
    Vector3 cameraForward {cameraForwardRaw.x, 0.0f, cameraForwardRaw.z};
    if (Vector3Length(cameraForward) <= 0.0001f) {
        cameraForward = Vector3{0.0f, 0.0f, 1.0f};
    } else {
        cameraForward = Vector3Normalize(cameraForward);
    }
    const Vector3 cameraRight = Vector3Normalize(Vector3CrossProduct(cameraForward, Vector3{0.0f, 1.0f, 0.0f}));

    if (input.IsKeyDown(KEY_W)) move = Vector3Add(move, cameraForward);
    if (input.IsKeyDown(KEY_S)) move = Vector3Subtract(move, cameraForward);
    if (input.IsKeyDown(KEY_D)) move = Vector3Add(move, cameraRight);
    if (input.IsKeyDown(KEY_A)) move = Vector3Subtract(move, cameraRight);

    if (Vector3Length(move) > 0.0001f) {
        move = Vector3Normalize(move);
        body->velocity.x = move.x * playerComponent->moveSpeed;
        body->velocity.z = move.z * playerComponent->moveSpeed;
        transform->rotationEuler.y = atan2f(move.x, move.z) * RAD2DEG;
    } else {
        body->velocity.x = 0.0f;
        body->velocity.z = 0.0f;
    }

    if (playerComponent->fireCooldownRemaining > 0.0f) {
        playerComponent->fireCooldownRemaining -= deltaTime;
    }

    if (input.IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && playerComponent->fireCooldownRemaining <= 0.0f) {
        const Vector3 projectileForward = Vector3Normalize(cameraForwardRaw);
        const Vector3 spawnPos = Vector3Add(transform->position, Vector3{0.0f, 0.75f, 0.0f});

        Entity projectile = 0;
        if (const PrefabDefinition* prefab = app.GetPrefabLibrary().Find("projectile")) {
            PrefabDefinition instance = *prefab;
            instance.transform.position = Vector3Add(spawnPos, Vector3Scale(projectileForward, 1.2f));
            if (instance.hasRigidbody) {
                instance.rigidbody.velocity = Vector3Scale(projectileForward, playerComponent->projectileSpeed);
            }
            projectile = spawn::SpawnFromPrefab(world, instance);
        } else {
            projectile = spawn::SpawnProjectile(world,
                                               Vector3Add(spawnPos, Vector3Scale(projectileForward, 1.2f)),
                                               Vector3Scale(projectileForward, playerComponent->projectileSpeed));
        }

        if (projectile != 0) {
            world.AddComponent<ProjectileComponent>(projectile, ProjectileComponent{25, kPlayerTeam});
            world.AddComponent<TeamComponent>(projectile, TeamComponent{kPlayerTeam});
        }

        playerComponent->fireCooldownRemaining = playerComponent->fireCooldown;
    }
}

} // namespace fw
