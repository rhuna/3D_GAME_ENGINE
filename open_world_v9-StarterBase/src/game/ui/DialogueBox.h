#pragma once
#include <string>
namespace fw { struct DialogueBoxState { bool active = false; std::string speaker; std::string text; }; void DrawDialogueBox(const DialogueBoxState& state); }
