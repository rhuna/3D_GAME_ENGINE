#pragma once

#include <string>
#include <vector>

namespace fw {

struct RespawnEntry {
    std::string regionId;
    std::string entityTag;
    bool active = true;
};

struct RespawnProfile {
    std::vector<RespawnEntry> entries;
};

} // namespace fw
