#include "game/ui/DialogueWindow.h"

#include "raylib.h"

namespace fw {

void DialogueWindow::Draw(const OpenWorldGameState& state) const {
    if (!state.dialogue.active) return;
    DrawRectangle(40, GetScreenHeight() - 200, GetScreenWidth() - 80, 150, Fade(BLACK, 0.72f));
    DrawRectangleLines(40, GetScreenHeight() - 200, GetScreenWidth() - 80, 150, SKYBLUE);
    DrawText(state.dialogue.speakerName.c_str(), 58, GetScreenHeight() - 184, 22, SKYBLUE);
    DrawText(state.dialogue.CurrentLine().c_str(), 58, GetScreenHeight() - 146, 20, RAYWHITE);
    DrawText("E / LMB advance | Q close", 58, GetScreenHeight() - 92, 18, LIGHTGRAY);
}

} // namespace fw
