#pragma once

namespace fw {

struct StarterBaseCompletionContract {
    bool runtimePathFrozen = true;
    bool contentWorkflowFrozen = true;
    bool starterSliceReady = true;
    bool hardeningChecklistReviewed = true;
    bool extensionDocsPresent = true;
};

} // namespace fw
