#pragma once

namespace fw {

struct VerticalSliceCompletionChecklist {
    bool hasLinkedRegions = false;
    bool hasQuestHandoff = false;
    bool hasMerchantCoverage = false;
    bool hasEncounterCoverage = false;
    bool hasCraftingCoverage = false;
    bool hasPersistenceHooks = false;
};

} // namespace fw
