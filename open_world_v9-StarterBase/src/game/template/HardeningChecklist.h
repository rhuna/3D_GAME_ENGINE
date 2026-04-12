#pragma once

#include <string>
#include <vector>

namespace fw {

struct HardeningChecklist {
    std::vector<std::string> buildChecks;
    std::vector<std::string> runtimeChecks;
    std::vector<std::string> authoringChecks;
};

} // namespace fw
