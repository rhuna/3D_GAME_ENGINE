#include "game/dialogue/DialogueState.h"

namespace fw {

void DialogueState::Begin(const std::string& speaker, std::vector<std::string> dialogueLines) {
    active = true;
    speakerName = speaker;
    lines = std::move(dialogueLines);
    currentLine = 0;
}

void DialogueState::Advance() {
    if (!active) {
        return;
    }
    ++currentLine;
    if (currentLine >= static_cast<int>(lines.size())) {
        Close();
    }
}

void DialogueState::Close() {
    active = false;
    speakerName.clear();
    lines.clear();
    currentLine = 0;
}

std::string DialogueState::CurrentLine() const {
    if (!active || currentLine < 0 || currentLine >= static_cast<int>(lines.size())) {
        return {};
    }
    return lines[currentLine];
}

} // namespace fw
