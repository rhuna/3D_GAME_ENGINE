#pragma once

#include <string>

namespace fw {

struct PartyMemberComponent {
    std::string memberId;
    int partySlot = -1;
    bool active = false;
};

} // namespace fw
