#include "game/rules/WaveDirector.h"

#include <vector>

#include "core/Application.h"
#include "ecs/World.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "game/components/EnemyComponent.h"
#include "game/components/HealthComponent.h"
#include "game/components/SpawnerComponent.h"
#include "gameplay/prefabs/PrefabDefinition.h"
#include "gameplay/prefabs/SpawnFactory.h"

namespace fw {

namespace {
constexpr int kPlayerTeam = 1;
constexpr int kEnemyTeam = 2;
}

void WaveDirector::Reset() {
    m_state = ArenaGameState{};
    m_waveCooldown = 0.0f;
}

void WaveDirector::StartGame(Application& app, World& world, const PrefabLibrary& prefabs) {
    (void)app;
    Reset();
    m_state.phase = ArenaPhase::Playing;
    SpawnWave(world, prefabs, 1);
}

void WaveDirector::Update(Application& app, World& world, const PrefabLibrary& prefabs, float deltaTime) {
    (void)app;
    if (m_state.phase != ArenaPhase::Playing) {
        if (m_state.restartDelay > 0.0f) {
            m_state.restartDelay -= deltaTime;
        }
        return;
    }

    m_state.timeSeconds += deltaTime;

    int enemyCount = 0;
    bool playerAlive = false;
    for (const Entity entity : world.Entities()) {
        if (world.HasComponent<EnemyComponent>(entity) && world.HasComponent<HealthComponent>(entity)) {
            const HealthComponent* health = world.GetComponent<HealthComponent>(entity);
            if (health && health->current > 0) {
                ++enemyCount;
            }
        }
        if (world.HasComponent<HealthComponent>(entity)) {
            const TagComponent* tag = world.GetComponent<TagComponent>(entity);
            const HealthComponent* health = world.GetComponent<HealthComponent>(entity);
            if (tag && health && tag->value == "player" && health->current > 0) {
                playerAlive = true;
            }
        }
    }

    m_state.enemiesRemaining = enemyCount;

    if (!playerAlive) {
        m_state.phase = ArenaPhase::Defeat;
        m_state.restartDelay = 1.25f;
        return;
    }

    if (enemyCount == 0) {
        if (m_state.wave >= 3) {
            m_state.phase = ArenaPhase::Victory;
            return;
        }

        m_waveCooldown -= deltaTime;
        if (m_waveCooldown <= 0.0f) {
            SpawnWave(world, prefabs, m_state.wave + 1);
        }
    }
}

void WaveDirector::SpawnWave(World& world, const PrefabLibrary& prefabs, int wave) {
    m_state.wave = wave;
    m_waveCooldown = 2.0f;

    std::vector<Vector3> spawnPoints;
    for (const Entity entity : world.Entities()) {
        const TagComponent* tag = world.GetComponent<TagComponent>(entity);
        const SpawnerComponent* spawner = world.GetComponent<SpawnerComponent>(entity);
        if (tag && spawner && tag->value == "enemy_spawner" && spawner->active) {
            spawnPoints.push_back(spawner->spawnPoint);
        }
    }

    if (spawnPoints.empty()) {
        spawnPoints = {
            Vector3{-8.0f, 1.0f, -8.0f},
            Vector3{8.0f, 1.0f, -8.0f},
            Vector3{-8.0f, 1.0f, 8.0f},
            Vector3{8.0f, 1.0f, 8.0f}
        };
    }

    const int targetCount = 2 + wave * 2;
    for (int i = 0; i < targetCount; ++i) {
        const Vector3& spawnPoint = spawnPoints[static_cast<std::size_t>(i) % spawnPoints.size()];
        SpawnEnemy(world, prefabs, spawnPoint);
    }

    m_state.enemiesRemaining = targetCount;
}

void WaveDirector::SpawnEnemy(World& world, const PrefabLibrary& prefabs, const Vector3& position) const {
    Entity enemy = 0;
    if (const PrefabDefinition* prefab = prefabs.Find("enemy_walker")) {
        PrefabDefinition instance = *prefab;
        instance.transform.position = position;
        enemy = spawn::SpawnFromPrefab(world, instance);
    } else {
        enemy = spawn::SpawnDynamicBox(world, "enemy", position, Vector3{1.0f, 1.8f, 1.0f}, Color{220, 90, 90, 255}, true);
    }

    if (enemy == 0) {
        return;
    }

    world.AddComponent<EnemyComponent>(enemy, EnemyComponent{});
    world.AddComponent<HealthComponent>(enemy, HealthComponent{50, 50});
    world.AddComponent<TeamComponent>(enemy, TeamComponent{kEnemyTeam});
}

} // namespace fw
