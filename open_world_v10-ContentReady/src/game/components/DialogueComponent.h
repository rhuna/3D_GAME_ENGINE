#pragma once

#include <string>
#include <vector>

namespace fw {

struct DialogueChoice {
    std::string text;
    std::string nextNodeId;
};

struct DialogueComponent {
    std::string nodeId = "intro";
    std::vector<std::string> lines;
    std::vector<DialogueChoice> choices;
};

} // namespace fw
