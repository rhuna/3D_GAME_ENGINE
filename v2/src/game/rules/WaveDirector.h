#pragma once

#include "game/state/ArenaGameState.h"
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

    [[nodiscard]] const ArenaGameState& GetState() const { return m_state; }
    [[nodiscard]] ArenaGameState& GetState() { return m_state; }

private:
    void SpawnWave(World& world, const PrefabLibrary& prefabs, int wave);
    void SpawnEnemy(World& world, const PrefabLibrary& prefabs, const Vector3& position, bool runner) const;
    void SpawnPickup(World& world, const Vector3& position) const;

    ArenaGameState m_state {};
    float m_waveCooldown = 0.0f;
};

} // namespace fw
