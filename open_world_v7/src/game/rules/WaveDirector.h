#pragma once

#include "game/state/ArenaGameState.h"
#include "game/components/PickupComponent.h"
#include "game/components/EnemyComponent.h"
#include "raylib.h"

namespace fw {

class Application;
class World;
class PrefabLibrary;

class WaveDirector {
public:
    void Reset();
    void PrepareRun(Application& app, World& world, const PrefabLibrary& prefabs);
    void StartGame(Application& app, World& world, const PrefabLibrary& prefabs);
    void TogglePause();
    void Update(Application& app, World& world, const PrefabLibrary& prefabs, float deltaTime);

    ArenaGameState& GetState() { return m_state; }
    const ArenaGameState& GetState() const { return m_state; }

private:
    void SpawnWave(World& world, const PrefabLibrary& prefabs, int wave);
    void SpawnEnemy(World& world, const PrefabLibrary& prefabs, const Vector3& position, bool runner) const;
    void SpawnEnemy(World& world, const PrefabLibrary& prefabs, const Vector3& position, EnemyArchetype archetype) const;
    void SpawnPickup(World& world, const Vector3& position) const;
    void SpawnPickup(World& world, const Vector3& position, PickupType type) const;

    ArenaGameState m_state {};
    float m_waveCooldown = 0.0f;
};

} // namespace fw
