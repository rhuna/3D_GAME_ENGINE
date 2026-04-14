#pragma once

namespace fw {

struct LargeWorldScalabilityManifest {
    bool enableLOD = true;
    bool enableVisibilityCulling = true;
    bool enableChunkStreaming = true;
    bool enableMaterialBudgetValidation = true;
    bool enableAnimationRuntime = true;
    bool enableStreamingTooling = true;
};

} // namespace fw
