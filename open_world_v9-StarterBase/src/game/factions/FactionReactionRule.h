#pragma once

#include <string>

namespace fw {

struct FactionReactionRule {
    std::string factionId;
    int hostileAtOrBelow = -50;
    int unfriendlyAtOrBelow = -10;
    int friendlyAtOrAbove = 25;
    int alliedAtOrAbove = 75;
};

} // namespace fw
