#pragma once

#include <string>

namespace fw {

enum class ArenaPhase {
    WaitingToStart,
    Playing,
    Victory,
    Defeat,
    Paused
};

struct ArenaGameState {
    ArenaPhase phase = ArenaPhase::WaitingToStart;
    ArenaPhase previousPhase = ArenaPhase::WaitingToStart;
    int wave = 0;
    int enemiesRemaining = 0;
    int kills = 0;
    int pickupsCollected = 0;
    float timeSeconds = 0.0f;
    float restartDelay = 0.0f;
    float statusTextTimer = 0.0f;
    std::string statusText;
};

} // namespace fw
