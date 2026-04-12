#include "game/progression/CampaignProfile.h"

#include "game/state/ArenaGameState.h"

namespace fw {

void CampaignProfile::RecordRunStart() {
    ++runsStarted;
}

void CampaignProfile::RecordRunEnd(const ArenaGameState& state) {
    totalKills += state.kills;
    totalPickups += state.pickupsCollected;
    if (state.score > bestScore) bestScore = state.score;
    if (state.wave > highestWave) highestWave = state.wave;
    if (!state.levelName.empty()) lastArena = state.levelName;
    if (state.phase == ArenaPhase::Victory) {
        ++runsWon;
        ++arenasCleared;
    }
}

} // namespace fw
