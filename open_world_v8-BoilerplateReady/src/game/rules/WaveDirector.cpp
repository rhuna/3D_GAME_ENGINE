#include "game/rules/WaveDirector.h"

#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "core/Application.h"
#include "ecs/World.h"
#include "ecs/components/BoxColliderComponent.h"
#include "ecs/components/RenderComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "game/components/EnemyComponent.h"
#include "game/components/HealthComponent.h"
#include "game/components/PickupComponent.h"
#include "game/components/SpawnerComponent.h"
#include "game/components/TeamComponent.h"
#include "gameplay/prefabs/PrefabDefinition.h"
#include "gameplay/prefabs/SpawnFactory.h"

namespace fw {

namespace {
constexpr int kEnemyTeam = 2;
}

void WaveDirector::Reset() {
    m_state = ArenaGameState{};
    m_state.levelName = "Arena Alpha";
    m_waveCooldown = 0.0f;
}

void WaveDirector::PrepareRun(Application& app, World& world, const PrefabLibrary& prefabs) {
    (void)app;
    (void)world;
    (void)prefabs;
    Reset();
    m_state.phase = ArenaPhase::WaitingToStart;
    m_state.statusText = "Press Enter to start the arena run";
    m_state.statusTextTimer = 9999.0f;
}

void WaveDirector::StartGame(Application& app, World& world, const PrefabLibrary& prefabs) {
    (void)app;
    Reset();
    m_state.phase = ArenaPhase::Playing;
    m_state.statusText = "Wave 1 incoming";
    m_state.statusTextTimer = 2.0f;
    SpawnWave(world, prefabs, 1);
}

void WaveDirector::TogglePause() {
    if (m_state.phase == ArenaPhase::Playing) {
        m_state.previousPhase = m_state.phase;
        m_state.phase = ArenaPhase::Paused;
        m_state.statusText = "Paused";
        m_state.statusTextTimer = 9999.0f;
    } else if (m_state.phase == ArenaPhase::Paused) {
        m_state.phase = m_state.previousPhase == ArenaPhase::Paused ? ArenaPhase::Playing : m_state.previousPhase;
        if (m_state.phase == ArenaPhase::WaitingToStart) {
            m_state.phase = ArenaPhase::Playing;
        }
        m_state.statusText = "Back in the fight";
        m_state.statusTextTimer = 1.2f;
    }
}

void WaveDirector::Update(Application& app, World& world, const PrefabLibrary& prefabs, float deltaTime) {
    (void)app;

    if (m_state.statusTextTimer > 0.0f && m_state.statusTextTimer < 9000.0f) {
        m_state.statusTextTimer -= deltaTime;
        if (m_state.statusTextTimer <= 0.0f) {
            m_state.statusText.clear();
        }
    }

    if (m_state.phase == ArenaPhase::WaitingToStart || m_state.phase == ArenaPhase::Paused) {
        return;
    }

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
        m_state.statusText = "You fell. Press Enter or F5 to retry";
        m_state.statusTextTimer = 9999.0f;
        return;
    }

    if (enemyCount == 0) {
        if (m_state.wave >= 4) {
            m_state.phase = ArenaPhase::Victory;
            m_state.victoryBannerTimer = 3.0f;
            m_state.score += 500;
            m_state.statusText = "Arena cleared. Press Enter or F5 to run again";
            m_state.statusTextTimer = 9999.0f;
            return;
        }

        m_waveCooldown -= deltaTime;
        if (m_waveCooldown <= 0.0f) {
            SpawnPickup(world, Vector3{0.0f, 1.0f, 0.0f}, PickupType::Health);
            if (m_state.wave >= 2) {
                SpawnPickup(world, Vector3{5.0f, 1.0f, 0.0f}, PickupType::DamageBoost);
            }
            m_state.statusText = TextFormat("Wave %d incoming", m_state.wave + 1);
            m_state.statusTextTimer = 1.8f;
            SpawnWave(world, prefabs, m_state.wave + 1);
        }
    }
}

void WaveDirector::SpawnWave(World& world, const PrefabLibrary& prefabs, int wave) {
    m_state.wave = wave;
    m_waveCooldown = 3.0f;
    m_state.score += 25 * wave;

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

    const int walkerCount = 2 + wave * 2;
    const int runnerCount = wave >= 2 ? wave - 1 : 0;
    const int rangedCount = wave >= 3 ? wave - 2 : 0;
    int index = 0;

    for (int i = 0; i < walkerCount; ++i) {
        const Vector3& spawnPoint = spawnPoints[static_cast<std::size_t>(index++) % spawnPoints.size()];
        SpawnEnemy(world, prefabs, spawnPoint, EnemyArchetype::Walker);
    }
    for (int i = 0; i < runnerCount; ++i) {
        const Vector3& spawnPoint = spawnPoints[static_cast<std::size_t>(index++) % spawnPoints.size()];
        SpawnEnemy(world, prefabs, Vector3Add(spawnPoint, Vector3{0.5f * static_cast<float>(i), 0.0f, 0.5f}), EnemyArchetype::Runner);
    }
    for (int i = 0; i < rangedCount; ++i) {
        const Vector3& spawnPoint = spawnPoints[static_cast<std::size_t>(index++) % spawnPoints.size()];
        SpawnEnemy(world, prefabs, Vector3Add(spawnPoint, Vector3{-0.75f * static_cast<float>(i), 0.0f, 0.75f}), EnemyArchetype::Ranged);
    }

    m_state.enemiesRemaining = walkerCount + runnerCount + rangedCount;
}

void WaveDirector::SpawnEnemy(World& world, const PrefabLibrary& prefabs, const Vector3& position, bool runner) const {
    SpawnEnemy(world, prefabs, position, runner ? EnemyArchetype::Runner : EnemyArchetype::Walker);
}

void WaveDirector::SpawnEnemy(World& world, const PrefabLibrary& prefabs, const Vector3& position, EnemyArchetype archetype) const {
    Entity enemy = 0;
    const char* prefabName = archetype == EnemyArchetype::Runner ? "enemy_runner" :
                             archetype == EnemyArchetype::Ranged ? "enemy_ranged" : "enemy_walker";
    if (const PrefabDefinition* prefab = prefabs.Find(prefabName)) {
        PrefabDefinition instance = *prefab;
        instance.transform.position = position;
        enemy = spawn::SpawnFromPrefab(world, instance);
    } else {
        const Color tint = archetype == EnemyArchetype::Runner ? Color{255, 180, 80, 255} :
                           archetype == EnemyArchetype::Ranged ? Color{180, 120, 255, 255} :
                           Color{220, 90, 90, 255};
        enemy = spawn::SpawnDynamicBox(world, "enemy", position, Vector3{1.0f, 1.8f, 1.0f}, tint, true);
    }

    if (enemy == 0) {
        return;
    }

    EnemyComponent enemyComponent;
    HealthComponent health;
    switch (archetype) {
        case EnemyArchetype::Runner:
            enemyComponent.archetype = EnemyArchetype::Runner;
            enemyComponent.moveSpeed = 4.6f;
            enemyComponent.contactDamage = 8;
            enemyComponent.contactDamageCooldown = 0.55f;
            enemyComponent.displayName = "Runner";
            health = HealthComponent{30, 30};
            break;
        case EnemyArchetype::Ranged:
            enemyComponent.archetype = EnemyArchetype::Ranged;
            enemyComponent.moveSpeed = 2.1f;
            enemyComponent.contactDamage = 6;
            enemyComponent.contactDamageCooldown = 1.0f;
            enemyComponent.preferredRange = 8.5f;
            enemyComponent.projectileCooldown = 1.6f;
            enemyComponent.projectileDamage = 10;
            enemyComponent.displayName = "Arcanist";
            health = HealthComponent{40, 40};
            break;
        default:
            enemyComponent.archetype = EnemyArchetype::Walker;
            enemyComponent.moveSpeed = 2.5f;
            enemyComponent.contactDamage = 12;
            enemyComponent.contactDamageCooldown = 0.85f;
            enemyComponent.displayName = "Walker";
            health = HealthComponent{55, 55};
            break;
    }

    world.AddComponent<EnemyComponent>(enemy, enemyComponent);
    world.AddComponent<HealthComponent>(enemy, health);
    world.AddComponent<TeamComponent>(enemy, TeamComponent{kEnemyTeam});
}

void WaveDirector::SpawnPickup(World& world, const Vector3& position) const {
    SpawnPickup(world, position, PickupType::Health);
}

void WaveDirector::SpawnPickup(World& world, const Vector3& position, PickupType type) const {
    const char* tag = type == PickupType::DamageBoost ? "damage_boost_pickup" : "health_pickup";
    const Color tint = type == PickupType::DamageBoost ? Color{210, 120, 255, 255} : Color{80, 255, 140, 255};
    const Entity pickup = spawn::SpawnStaticBox(world, tag, position, Vector3{0.7f, 0.7f, 0.7f}, tint);
    if (pickup == 0) {
        return;
    }

    if (type == PickupType::DamageBoost) {
        world.AddComponent<PickupComponent>(pickup, PickupComponent{PickupType::DamageBoost, 15, 0.0f, true});
    } else {
        world.AddComponent<PickupComponent>(pickup, PickupComponent{PickupType::Health, 25, 0.0f, true});
    }

    if (auto* render = world.GetComponent<RenderComponent>(pickup)) {
        render->drawCube = false;
        render->drawSphere = true;
        render->sphereRadius = 0.35f;
        render->tint = tint;
    }
    if (auto* collider = world.GetComponent<BoxColliderComponent>(pickup)) {
        collider->halfExtents = Vector3{0.35f, 0.35f, 0.35f};
        collider->isTrigger = true;
        collider->isStatic = true;
    }
}

} // namespace fw
