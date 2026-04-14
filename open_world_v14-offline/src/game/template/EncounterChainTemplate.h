#pragma once

#include <string>
#include <vector>

namespace fw {

struct EncounterStageTemplate {
    std::string stageId;
    std::string encounterId;
    std::string onSuccessRewardId;
    std::string onFailureFallbackId;
};

struct EncounterChainTemplate {
    std::string chainId;
    std::string displayName;
    std::vector<EncounterStageTemplate> stages;
};

} // namespace fw
