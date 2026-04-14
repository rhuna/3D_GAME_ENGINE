#pragma once

namespace fw {

struct StarterBaseCompletionChecklist {
    bool hasSingleRuntimePath = true;
    bool hasSingleSpawnPath = true;
    bool hasDocumentedExtensionFlow = true;
    bool hasStarterMultiRegionSlice = true;
    bool hasHardeningPass = true;
};

} // namespace fw
