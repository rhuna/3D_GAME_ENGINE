#pragma once

namespace fw {

struct StarterBaseContract {
    bool singleRuntimePath = true;
    bool contentPackAuthoringEnabled = true;
    bool multiRegionSliceSupported = true;
    bool persistentWorldDeltasSupported = true;
    bool extensionDocsAvailable = true;
};

} // namespace fw
