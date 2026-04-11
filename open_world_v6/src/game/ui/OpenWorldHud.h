#pragma once
#include <vector>
namespace fw
{
    struct SaveGameProfile;
    struct RegionSimulationState;
    struct ActiveNpcRoutineInfo;
    void DrawOpenWorldHud(const SaveGameProfile& profile,
                          const char* formattedTime,
                          const RegionSimulationState& simState,
                          const std::vector<ActiveNpcRoutineInfo>& activeRoutines,
                          const char* interactionPrompt,
                          const char* statusText,
                          int aliveEnemies);
}
