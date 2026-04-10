#include "scene/scenes/SandboxScene.h"

#include <memory>

#include "raymath.h"

#include "core/Application.h"
#include "ecs/systems/CollisionSystem.h"
#include "ecs/systems/MovementSystem.h"
#include "gameplay/GameplayTags.h"
#include "gameplay/prefabs/PrefabDefinition.h"
#include "gameplay/prefabs/SpawnFactory.h"
#include "gameplay/registry/SystemRegistry.h"
#include "gameplay/systems/ProjectileCleanupSystem.h"

namespace fw {

void SandboxScene::OnEnter(Application& app) {
    auto& world = app.GetWorld();
    auto& prefabs = app.GetPrefabLibrary();
    auto& systems = app.GetSystemRegistry();

    world.Clear();
    systems.Clear();

    systems.RegisterFixed(std::make_unique<MovementSystem>());
    systems.RegisterFixed(std::make_unique<CollisionSystem>());
    systems.RegisterFixed(std::make_unique<ProjectileCleanupSystem>());

    if (!spawn::SpawnFromPrefab(world, prefabs, "ground")) {
        spawn::SpawnStaticBox(world, tags::kGround, Vector3{0.0f, -0.5f, 0.0f}, Vector3{20.0f, 1.0f, 20.0f}, Color{90, 110, 90, 255});
    }

    if (const PrefabDefinition* player = prefabs.Find("player")) {
        PrefabDefinition instance = *player;
        instance.transform.position = Vector3{0.0f, 2.0f, 0.0f};
        spawn::SpawnFromPrefab(world, instance);
    } else {
        spawn::SpawnPlayer(world, Vector3{0.0f, 2.0f, 0.0f});
    }

    const PrefabDefinition* cratePrefab = prefabs.Find("dynamic_crate");
    for (int z = -2; z <= 2; ++z) {
        for (int x = -2; x <= 2; ++x) {
            if (x == 0 && z == 0) {
                continue;
            }

            const Vector3 pos {
                static_cast<float>(x) * 2.2f,
                2.5f + static_cast<float>((x + z + 4) % 3),
                static_cast<float>(z) * 2.2f
            };

            if (cratePrefab) {
                PrefabDefinition crate = *cratePrefab;
                crate.transform.position = pos;
                crate.render.tint = Color{
                    static_cast<unsigned char>(100 + (x + 2) * 20),
                    static_cast<unsigned char>(110 + (z + 2) * 22),
                    200,
                    255
                };
                spawn::SpawnFromPrefab(world, crate);
            } else {
                spawn::SpawnDynamicBox(world, tags::kDynamicCrate, pos, Vector3{1.0f, 1.0f, 1.0f}, Color{160, 160, 200, 255});
            }
        }
    }
}

void SandboxScene::OnExit(Application& app) {
    app.GetSystemRegistry().Clear();
    app.GetWorld().Clear();
}

void SandboxScene::Update(Application& app, float deltaTime) {
    auto& input = app.GetInput();
    auto& world = app.GetWorld();
    auto& prefabs = app.GetPrefabLibrary();

    const Entity player = world.FindByTag(tags::kPlayer);
    if (player != 0 && input.IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        const Vector3 origin = app.GetCamera().position;
        const Vector3 forward = Vector3Normalize(Vector3Subtract(app.GetCamera().target, app.GetCamera().position));

        if (const PrefabDefinition* projectile = prefabs.Find("projectile")) {
            PrefabDefinition instance = *projectile;
            instance.transform.position = Vector3Add(origin, Vector3Scale(forward, 1.5f));
            if (instance.hasRigidbody) {
                instance.rigidbody.velocity = Vector3Scale(forward, 14.0f);
            }
            spawn::SpawnFromPrefab(world, instance);
        } else {
            spawn::SpawnProjectile(world,
                                   Vector3Add(origin, Vector3Scale(forward, 1.5f)),
                                   Vector3Scale(forward, 14.0f));
        }
    }

    app.GetSystemRegistry().UpdateAll(app, world, deltaTime);
}

void SandboxScene::FixedUpdate(Application& app, float fixedDeltaTime) {
    app.GetSystemRegistry().FixedUpdateAll(app, app.GetWorld(), fixedDeltaTime);
}

void SandboxScene::Render(Application& app) {
    app.GetRenderer().DrawGrid(40, 1.0f);
    app.GetRenderer().DrawWorld(app.GetWorld(), app.GetAssets());
    app.GetSystemRegistry().RenderAll(app, app.GetWorld());
}

} // namespace fw
