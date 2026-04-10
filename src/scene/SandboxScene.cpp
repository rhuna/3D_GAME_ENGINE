#include <cmath>

#include "scene/SandboxScene.h"

#include "core/Application.h"
#include "ecs/World.h"

namespace fw {

void SandboxScene::OnEnter(Application& app) {
    auto& world = app.GetWorld();
    world.Clear();

    for (int z = -2; z <= 2; ++z) {
        for (int x = -2; x <= 2; ++x) {
            const Entity entity = world.CreateEntity();
            EntityRecord* record = world.FindEntity(entity);
            if (!record) {
                continue;
            }

            record->transform.position = Vector3{static_cast<float>(x) * 2.5f, 0.5f, static_cast<float>(z) * 2.5f};
            record->transform.scale = Vector3{1.0f, 1.0f + ((x + z + 4) % 3), 1.0f};
            record->render.cubeSize = 1.0f;
            record->render.tint = Color{
                static_cast<unsigned char>(80 + (x + 2) * 30),
                static_cast<unsigned char>(120 + (z + 2) * 20),
                static_cast<unsigned char>(180),
                255
            };
        }
    }
}

void SandboxScene::OnExit(Application& app) {
    app.GetWorld().Clear();
}

void SandboxScene::Update(Application& app, float deltaTime) {
    (void)app;
    (void)deltaTime;
}

void SandboxScene::FixedUpdate(Application& app, float fixedDeltaTime) {
    (void)fixedDeltaTime;

    auto& entities = app.GetWorld().Entities();
    for (std::size_t i = 0; i < entities.size(); ++i) {
        const float phase = static_cast<float>(i) * 0.15f;
        entities[i].transform.position.y = 0.75f + std::sin(static_cast<float>(app.GetTime().TotalTime()) + phase) * 0.15f;
    }
}

void SandboxScene::Render(Application& app) {
    app.GetRenderer().DrawGrid(40, 1.0f);
    app.GetRenderer().DrawWorld(app.GetWorld());
}

} // namespace fw
