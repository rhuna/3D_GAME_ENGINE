#pragma once

#include <string>

namespace fw {

struct ArenaGameState;

struct CampaignProfile {
    int runsStarted = 0;
    int runsWon = 0;
    int totalKills = 0;
    int totalPickups = 0;
    int bestScore = 0;
    int highestWave = 0;
    int arenasCleared = 0;
    std::string lastArena = "Arena Alpha";

    void RecordRunStart();
    void RecordRunEnd(const ArenaGameState& state);
};

} // namespace fw
