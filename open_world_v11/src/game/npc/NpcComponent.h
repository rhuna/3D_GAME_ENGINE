#pragma once

#include <string>
#include <vector>

namespace fw {

struct NpcComponent {
    std::string npcId;
    std::string displayName = "Villager";
    std::vector<std::string> dialogueLines;
    bool questGiver = false;
    bool shopkeeper = false;
};

} // namespace fw
