#pragma once

#include <string>
#include <vector>

namespace fw {

struct PartyRuntimeState {
    std::vector<std::string> activePartyIds;
    std::string selectedCompanionId;
    int maxPartySize = 2;
};

} // namespace fw
