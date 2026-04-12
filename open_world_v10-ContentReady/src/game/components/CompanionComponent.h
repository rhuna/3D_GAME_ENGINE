#pragma once

#include <string>

namespace fw {

struct CompanionComponent {
    std::string companionId;
    std::string displayName;
    bool recruited = false;
    bool followingPlayer = true;
    bool canFight = true;
};

} // namespace fw
