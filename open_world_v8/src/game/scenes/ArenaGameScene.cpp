#include "game/scenes/ArenaGameScene.h"

#include <memory>

#include "raylib.h"
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
#include "game/progression/CampaignProfile.h"
#include "game/rules/WaveDirector.h"
#include "game/state/ArenaGameState.h"
#include "game/systems/CombatFeedbackSystem.h"
#include "game/systems/DamageSystem.h"
#include "game/systems/EnemyAISystem.h"
#include "game/systems/PickupSystem.h"
#include "game/systems/PlayerControllerSystem.h"
#include "game/systems/PowerupSystem.h"
#include "game/systems/RangedEnemyAISystem.h"
#include "gameplay/prefabs/PrefabDefinition.h"
#include "gameplay/prefabs/SpawnFactory.h"
#include "gameplay/registry/SystemRegistry.h"
#include "gameplay/systems/ProjectileCleanupSystem.h"
#include "scene/data/SceneDefinition.h"
#include "scene/spawning/SceneSpawner.h"

namespace fw {

namespace {
constexpr const char* kArenaSceneNames[] = {"arena_game", "arena_game_02", "arena_game_03"};
constexpr const char* kArenaLevelNames[] = {"Arena Alpha", "Arena Beta", "Arena Gamma"};
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
            world.AddComponent<PickupComponent>(entity, PickupComponent{PickupType::Health, 25, 0.0f, true});
        } else if (tag->value == "damage_boost_pickup") {
            world.AddComponent<PickupComponent>(entity, PickupComponent{PickupType::DamageBoost, 15, 0.0f, true});
        }
    }
}
}


const char* ArenaGameScene::ResolveSceneName() const {
    const int idx = m_waveDirector.GetState().selectedArenaIndex;
    const int count = static_cast<int>(sizeof(kArenaSceneNames) / sizeof(kArenaSceneNames[0]));
    return kArenaSceneNames[(idx % count + count) % count];
}

void ArenaGameScene::AdvanceArenaSelection(int delta) {
    auto& state = m_waveDirector.GetState();
    const int count = static_cast<int>(sizeof(kArenaSceneNames) / sizeof(kArenaSceneNames[0]));
    state.selectedArenaIndex = (state.selectedArenaIndex + delta + count) % count;
    state.levelName = kArenaLevelNames[state.selectedArenaIndex];
    state.nextArenaName = state.levelName;
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
    systems.RegisterUpdate(std::make_unique<RangedEnemyAISystem>());
    systems.RegisterUpdate(std::make_unique<PickupSystem>(&m_waveDirector.GetState()));
    systems.RegisterUpdate(std::make_unique<PowerupSystem>(&m_waveDirector.GetState()));
    systems.RegisterUpdate(std::make_unique<DamageSystem>(&m_waveDirector.GetState()));
    systems.RegisterUpdate(std::make_unique<CombatFeedbackSystem>(&m_waveDirector.GetState()));
    systems.RegisterFixed(std::make_unique<MovementSystem>());
    systems.RegisterFixed(std::make_unique<CollisionSystem>());
    systems.RegisterFixed(std::make_unique<ProjectileCleanupSystem>());

    auto& state = m_waveDirector.GetState();
    state.levelName = kArenaLevelNames[state.selectedArenaIndex];
    state.nextArenaName = state.levelName;

    const SceneDefinition* scene = scenes.Find(ResolveSceneName());
    if (!scene) {
        scene = scenes.Find("arena_game_02");
    }
    if (!scene) {
        scene = scenes.Find("arena_game");
    }

    if (!scene || !SceneSpawner::Spawn(world, prefabs, *scene)) {
        spawn::SpawnStaticBox(world, "ground", Vector3{0.0f, -0.5f, 0.0f}, Vector3{24.0f, 1.0f, 24.0f}, Color{70, 95, 70, 255});
        spawn::SpawnPlayer(world, Vector3{0.0f, 1.5f, 0.0f});
    }

    AddArenaGameplayComponents(world);
    m_waveDirector.PrepareRun(app, world, prefabs);
    state.bestScore = m_campaignProfile.bestScore;
    state.arenasCleared = m_campaignProfile.arenasCleared;
}

void ArenaGameScene::OnExit(Application& app) {
    app.GetSystemRegistry().Clear();
    app.GetWorld().Clear();
}

void ArenaGameScene::Update(Application& app, float deltaTime) {
    auto& world = app.GetWorld();
    auto& state = m_waveDirector.GetState();
    Input& input = app.GetInput();


if (state.phase == ArenaPhase::WaitingToStart) {
    if (input.IsKeyPressed(KEY_LEFT_BRACKET)) {
        AdvanceArenaSelection(-1);
    } else if (input.IsKeyPressed(KEY_RIGHT_BRACKET)) {
        AdvanceArenaSelection(1);
    } else if (input.IsKeyPressed(KEY_MINUS)) {
        state.difficultyTier = state.difficultyTier > 1 ? state.difficultyTier - 1 : 1;
    } else if (input.IsKeyPressed(KEY_EQUAL)) {
        state.difficultyTier = state.difficultyTier < 3 ? state.difficultyTier + 1 : 3;
    }
}

if (input.IsKeyPressed(KEY_ENTER)) {
        if (state.phase == ArenaPhase::WaitingToStart || state.phase == ArenaPhase::Victory || state.phase == ArenaPhase::Defeat) {
            OnEnter(app);
            m_campaignProfile.RecordRunStart();
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
    const ArenaPhase beforePhase = state.phase;
    m_waveDirector.Update(app, world, app.GetPrefabLibrary(), deltaTime);
    if ((state.phase == ArenaPhase::Victory || state.phase == ArenaPhase::Defeat) && beforePhase != state.phase) {
        m_campaignProfile.RecordRunEnd(state);
        state.bestScore = m_campaignProfile.bestScore;
        state.arenasCleared = m_campaignProfile.arenasCleared;
    }

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
