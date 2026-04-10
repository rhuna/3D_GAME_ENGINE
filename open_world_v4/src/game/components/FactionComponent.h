#pragma once

#include <string>

namespace fw {

struct FactionComponent {
    std::string factionId = "neutral";
    int disposition = 0;
};

} // namespace fw
