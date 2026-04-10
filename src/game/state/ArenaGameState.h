#pragma once

namespace fw {

enum class ArenaPhase {
    WaitingToStart,
    Playing,
    Victory,
    Defeat
};

struct ArenaGameState {
    ArenaPhase phase = ArenaPhase::WaitingToStart;
    int wave = 0;
    int enemiesRemaining = 0;
    int kills = 0;
    float timeSeconds = 0.0f;
    float restartDelay = 0.0f;
};

} // namespace fw
