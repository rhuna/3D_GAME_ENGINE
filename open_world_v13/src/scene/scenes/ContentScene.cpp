#include "scene/scenes/ContentScene.h"

#include <memory>

#include "core/Application.h"
#include "core/Logger.h"
#include "ecs/systems/CollisionSystem.h"
#include "ecs/systems/MovementSystem.h"
#include "gameplay/GameplayTags.h"
#include "gameplay/prefabs/PrefabDefinition.h"
#include "gameplay/prefabs/SpawnFactory.h"
#include "gameplay/registry/SystemRegistry.h"
#include "gameplay/systems/ProjectileCleanupSystem.h"
#include "scene/data/SceneDefinition.h"
#include "scene/spawning/SceneSpawner.h"

namespace fw {

void ContentScene::OnEnter(Application& app) {
    auto& world = app.GetWorld();
    auto& prefabs = app.GetPrefabLibrary();
    auto& scenes = app.GetSceneLibrary();
    auto& systems = app.GetSystemRegistry();

    world.Clear();
    systems.Clear();

    systems.RegisterFixed(std::make_unique<MovementSystem>());
    systems.RegisterFixed(std::make_unique<CollisionSystem>());
    systems.RegisterFixed(std::make_unique<ProjectileCleanupSystem>());

    SceneDefinition scene;
    const bool resolved = scenes.BuildResolvedScene(m_sceneName, scene);
    if (resolved) {
        m_displayName = scene.displayName;
        app.GetAssets().PreloadAssets(scene.preloadAssets);
        SceneSpawner::Spawn(world, prefabs, scene);

        if (scene.autoSpawnPlayer && world.FindByTag(tags::kPlayer) == 0) {
            if (const PrefabDefinition* player = prefabs.Find(scene.playerPrefab)) {
                PrefabDefinition instance = *player;
                if (instance.transform.position.y < 1.0f) {
                    instance.transform.position = Vector3{0.0f, 2.0f, 0.0f};
                }
                spawn::SpawnFromPrefab(world, instance);
            } else {
                spawn::SpawnPlayer(world, Vector3{0.0f, 2.0f, 0.0f});
            }
        }
    }

    if (world.Entities().empty()) {
        Logger::Warn("Content scene fallback spawn used for: " + m_sceneName);
        spawn::SpawnStaticBox(world, tags::kGround, Vector3{0.0f, -0.5f, 0.0f}, Vector3{20.0f, 1.0f, 20.0f}, Color{90, 110, 90, 255});
        spawn::SpawnPlayer(world, Vector3{0.0f, 2.0f, 0.0f});
    }
}

void ContentScene::OnExit(Application& app) {
    app.GetSystemRegistry().Clear();
    app.GetWorld().Clear();
}

void ContentScene::Update(Application& app, float deltaTime) {
    app.GetSystemRegistry().UpdateAll(app, app.GetWorld(), deltaTime);
}

void ContentScene::FixedUpdate(Application& app, float fixedDeltaTime) {
    app.GetSystemRegistry().FixedUpdateAll(app, app.GetWorld(), fixedDeltaTime);
}

void ContentScene::Render(Application& app) {
    app.GetRenderer().DrawGrid(40, 1.0f);
    app.GetRenderer().DrawWorld(app.GetWorld(), app.GetAssets(), app.GetCamera());
    app.GetSystemRegistry().RenderAll(app, app.GetWorld());
}

void ContentScene::DrawUi(Application& app) {
    (void)app;
    const std::string label = m_displayName.empty() ? m_sceneName : m_displayName;
    DrawText(label.c_str(), 20, 20, 24, WHITE);
    DrawText("Content scene mode", 20, 48, 18, Fade(WHITE, 0.75f));
}

} // namespace fw
