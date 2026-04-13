#pragma once

namespace fw {

struct BoilerplateReadinessChecklist {
    bool unifiedSpawnPath = false;
    bool unifiedQuestRuntime = false;
    bool unifiedInventoryRuntime = false;
    bool persistentWorldDeltaPath = false;
    bool cleanedBuildGraph = false;
    bool starterVerticalSlice = false;
};

} // namespace fw
