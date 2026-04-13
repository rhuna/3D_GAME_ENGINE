#include "scene/SandboxScene.h"

#include "raylib.h"
#include "raymath.h"

#include "core/Application.h"
#include "gameplay/GameplayTags.h"
#include "gameplay/Spawn.h"
#include "gameplay/systems/CollisionSystem.h"
#include "gameplay/systems/MovementSystem.h"

namespace fw {

namespace {
MovementSystem g_movementSystem;
CollisionSystem g_collisionSystem;
}

void SandboxScene::OnEnter(Application& app) {
    auto& world = app.GetWorld();
    world.Clear();

    spawn::SpawnStaticBox(world, tags::kGround, Vector3{0.0f, -0.5f, 0.0f}, Vector3{20.0f, 1.0f, 20.0f}, Color{90, 110, 90, 255});
    spawn::SpawnPlayer(world, Vector3{0.0f, 2.0f, 0.0f});

    for (int z = -2; z <= 2; ++z) {
        for (int x = -2; x <= 2; ++x) {
            if (x == 0 && z == 0) {
                continue;
            }
            spawn::SpawnDynamicBox(world,
                                   tags::kDynamicCrate,
                                   Vector3{static_cast<float>(x) * 2.2f, 2.5f + static_cast<float>((x + z + 4) % 3), static_cast<float>(z) * 2.2f},
                                   Vector3{1.0f, 1.0f, 1.0f},
                                   Color{static_cast<unsigned char>(100 + (x + 2) * 20),
                                         static_cast<unsigned char>(110 + (z + 2) * 22),
                                         200,
                                         255});
        }
    }
}

void SandboxScene::OnExit(Application& app) {
    app.GetWorld().Clear();
}

void SandboxScene::Update(Application& app, float deltaTime) {
    auto& input = app.GetInput();
    auto& world = app.GetWorld();

    EntityRecord* player = world.FindByTag(tags::kPlayer);
    if (player && player->rigidbody && input.IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        const Vector3 origin = app.GetCamera().position;
        const Vector3 forward = Vector3Normalize(Vector3Subtract(app.GetCamera().target, app.GetCamera().position));
        spawn::SpawnProjectile(world,
                               Vector3Add(origin, Vector3Scale(forward, 1.5f)),
                               Vector3Scale(forward, 14.0f));
    }

    (void)deltaTime;
}

void SandboxScene::FixedUpdate(Application& app, float fixedDeltaTime) {
    auto& world = app.GetWorld();
    g_movementSystem.FixedUpdate(world, fixedDeltaTime);
    g_collisionSystem.FixedUpdate(world);

    for (const auto& collision : world.Collisions()) {
        EntityRecord* a = world.FindEntity(collision.a);
        EntityRecord* b = world.FindEntity(collision.b);
        if (!a || !b) {
            continue;
        }

        const bool aProjectile = a->tag && a->tag->value == tags::kProjectile;
        const bool bProjectile = b->tag && b->tag->value == tags::kProjectile;
        const bool aGround = a->tag && a->tag->value == tags::kGround;
        const bool bGround = b->tag && b->tag->value == tags::kGround;

        if (aProjectile && !bProjectile) {
            a->active = false;
        }
        if (bProjectile && !aProjectile) {
            b->active = false;
        }
        if (aProjectile && bGround) {
            a->active = false;
        }
        if (bProjectile && aGround) {
            b->active = false;
        }
    }
}

void SandboxScene::Render(Application& app) {
    app.GetRenderer().DrawGrid(40, 1.0f);
    app.GetRenderer().DrawWorld(app.GetWorld(), app.GetAssets(), app.GetCamera());
}

} // namespace fw
