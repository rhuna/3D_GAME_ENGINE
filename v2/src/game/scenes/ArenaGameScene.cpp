#include "game/scenes/ArenaGameScene.h"

#include <memory>

#include "raymath.h"

#include "core/Application.h"
#include "core/Input.h"
#include "ecs/World.h"
#include "ecs/components/RigidbodyComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "ecs/systems/CollisionSystem.h"
#include "ecs/systems/MovementSystem.h"
#include "game/components/HealthComponent.h"
#include "game/components/PickupComponent.h"
#include "game/components/PlayerComponent.h"
#include "game/components/SpawnerComponent.h"
#include "game/components/TeamComponent.h"
#include "game/rules/WaveDirector.h"
#include "game/state/ArenaGameState.h"
#include "game/systems/DamageSystem.h"
#include "game/systems/EnemyAISystem.h"
#include "game/systems/PickupSystem.h"
#include "game/systems/PlayerControllerSystem.h"
#include "gameplay/prefabs/PrefabDefinition.h"
#include "gameplay/prefabs/SpawnFactory.h"
#include "gameplay/registry/SystemRegistry.h"
#include "gameplay/systems/ProjectileCleanupSystem.h"
#include "scene/data/SceneDefinition.h"
#include "scene/spawning/SceneSpawner.h"

namespace fw {

namespace {
constexpr int kPlayerTeam = 1;

void AddArenaGameplayComponents(World& world) {
    const Entity player = world.FindByTag("player");
    if (player != 0) {
        world.AddComponent<PlayerComponent>(player, PlayerComponent{});
        world.AddComponent<HealthComponent>(player, HealthComponent{100, 100});
        world.AddComponent<TeamComponent>(player, TeamComponent{kPlayerTeam});
        if (RigidbodyComponent* body = world.GetComponent<RigidbodyComponent>(player)) {
            body->useGravity = true;
            body->drag = 10.0f;
        }
    }

    for (const Entity entity : world.Entities()) {
        const TagComponent* tag = world.GetComponent<TagComponent>(entity);
        const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        if (!tag || !transform) {
            continue;
        }

        if (tag->value == "enemy_spawner_marker") {
            world.AddComponent<SpawnerComponent>(entity, SpawnerComponent{transform->position, true});
            TagComponent newTag = *tag;
            newTag.value = "enemy_spawner";
            world.AddComponent<TagComponent>(entity, newTag);
        } else if (tag->value == "health_pickup") {
            world.AddComponent<PickupComponent>(entity, PickupComponent{});
        }
    }
}
}

void ArenaGameScene::OnEnter(Application& app) {
    auto& world = app.GetWorld();
    auto& prefabs = app.GetPrefabLibrary();
    auto& scenes = app.GetSceneLibrary();
    auto& systems = app.GetSystemRegistry();

    world.Clear();
    systems.Clear();

    systems.RegisterUpdate(std::make_unique<PlayerControllerSystem>());
    systems.RegisterUpdate(std::make_unique<EnemyAISystem>());
    systems.RegisterUpdate(std::make_unique<PickupSystem>(&m_waveDirector.GetState()));
    systems.RegisterUpdate(std::make_unique<DamageSystem>(&m_waveDirector.GetState()));
    systems.RegisterFixed(std::make_unique<MovementSystem>());
    systems.RegisterFixed(std::make_unique<CollisionSystem>());
    systems.RegisterFixed(std::make_unique<ProjectileCleanupSystem>());

    const SceneDefinition* scene = scenes.Find("arena_game");
    if (!scene || !SceneSpawner::Spawn(world, prefabs, *scene)) {
        spawn::SpawnStaticBox(world, "ground", Vector3{0.0f, -0.5f, 0.0f}, Vector3{24.0f, 1.0f, 24.0f}, Color{70, 95, 70, 255});
        spawn::SpawnPlayer(world, Vector3{0.0f, 1.5f, 0.0f});
    }

    AddArenaGameplayComponents(world);
    m_waveDirector.PrepareRun(app, world, prefabs);
}

void ArenaGameScene::OnExit(Application& app) {
    app.GetSystemRegistry().Clear();
    app.GetWorld().Clear();
}

void ArenaGameScene::Update(Application& app, float deltaTime) {
    auto& world = app.GetWorld();
    auto& state = m_waveDirector.GetState();
    Input& input = app.GetInput();

    if (input.IsKeyPressed(KEY_ENTER)) {
        if (state.phase == ArenaPhase::WaitingToStart || state.phase == ArenaPhase::Victory || state.phase == ArenaPhase::Defeat) {
            OnEnter(app);
            m_waveDirector.StartGame(app, world, app.GetPrefabLibrary());
            return;
        }
    }

    if (input.IsKeyPressed(KEY_P)) {
        m_waveDirector.TogglePause();
    }

    const bool gameplayActive = state.phase == ArenaPhase::Playing;
    if (gameplayActive) {
        app.GetSystemRegistry().UpdateAll(app, world, deltaTime);
    }
    m_waveDirector.Update(app, world, app.GetPrefabLibrary(), deltaTime);

    const Entity player = world.FindByTag("player");
    const TransformComponent* playerTransform = world.GetComponent<TransformComponent>(player);
    if (playerTransform) {
        Camera3D& camera = app.GetCamera();
        const Vector3 desired = Vector3Add(playerTransform->position, Vector3{0.0f, 7.5f, 10.5f});
        camera.position = Vector3Lerp(camera.position, desired, gameplayActive ? 0.08f : 0.04f);
        camera.target = Vector3Lerp(camera.target, Vector3Add(playerTransform->position, Vector3{0.0f, 1.4f, 0.0f}), gameplayActive ? 0.12f : 0.06f);
    }
}

void ArenaGameScene::FixedUpdate(Application& app, float fixedDeltaTime) {
    if (m_waveDirector.GetState().phase != ArenaPhase::Playing) {
        return;
    }
    app.GetSystemRegistry().FixedUpdateAll(app, app.GetWorld(), fixedDeltaTime);
}

void ArenaGameScene::Render(Application& app) {
    app.GetRenderer().DrawGrid(40, 1.0f);
    app.GetRenderer().DrawWorld(app.GetWorld(), app.GetAssets());
    app.GetSystemRegistry().RenderAll(app, app.GetWorld());
}

void ArenaGameScene::DrawUi(Application& app) {
    m_hud.Draw(app.GetWorld(), m_waveDirector.GetState());
}

} // namespace fw
