#pragma once

namespace fw {

struct RegionAuthoringChecklist {
    bool hasRegionManifest = false;
    bool hasEncounterCoverage = false;
    bool hasMerchantCoverage = false;
    bool hasGatherCoverage = false;
    bool hasCraftingCoverage = false;
    bool hasFactionHooks = false;
};

} // namespace fw
