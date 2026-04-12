#pragma once
#include <string>
#include <vector>

namespace fw {

struct ObjectiveTemplate {
    std::string id;
    std::string type;
    int requiredCount = 1;
};

struct ObjectiveBundleTemplate {
    std::string id;
    std::vector<ObjectiveTemplate> objectives;
};

} // namespace fw
