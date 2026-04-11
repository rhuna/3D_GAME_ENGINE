#pragma once

#include <string>
#include <vector>

namespace fw {

struct DialogueState {
    bool active = false;
    std::string speakerName;
    std::vector<std::string> lines;
    int currentLine = 0;

    void Begin(const std::string& speaker, std::vector<std::string> dialogueLines);
    void Advance();
    void Close();
    std::string CurrentLine() const;
};

} // namespace fw
