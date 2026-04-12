#include "game/ui/QuestLogWindow.h"

#include "raylib.h"
#include "game/quests/QuestState.h"

namespace fw {

static const char* StatusLabel(QuestProgressState status) {
    switch (status) {
        case QuestProgressState::NotStarted: return "Not Started";
        case QuestProgressState::Active: return "Active";
        case QuestProgressState::Completed: return "Completed";
    }
    return "Unknown";
}

void QuestLogWindow::Draw(const OpenWorldGameState& state) const {
    if (!state.questLogOpen) return;
    DrawRectangle(GetScreenWidth() - 360, 90, 320, 280, Fade(BLACK, 0.65f));
    DrawRectangleLines(GetScreenWidth() - 360, 90, 320, 280, ORANGE);
    DrawText("Quest Log", GetScreenWidth() - 344, 104, 22, ORANGE);
    int y = 136;
    for (const auto& quest : state.saveProfile.quests) {
        DrawText(quest.title.c_str(), GetScreenWidth() - 344, y, 18, RAYWHITE);
        y += 20;
        DrawText(StatusLabel(quest.state), GetScreenWidth() - 344, y, 16, LIGHTGRAY);
        y += 28;
        if (y > 320) break;
    }
}

} // namespace fw
