#include "game/ui/EndScreen.h"

#include "raylib.h"

namespace fw {

void EndScreen::Draw(const ArenaGameState& state) const {
    const int sw = GetScreenWidth();
    const int sh = GetScreenHeight();
    const int panelW = 720;
    const int panelH = 220;
    const int x = sw / 2 - panelW / 2;
    const int y = sh / 2 - panelH / 2;
    const bool victory = state.phase == ArenaPhase::Victory;
    const Color accent = victory ? LIME : RED;

    DrawRectangle(x, y, panelW, panelH, Fade(BLACK, 0.72f));
    DrawRectangleLines(x, y, panelW, panelH, accent);
    DrawText(victory ? "VICTORY" : "DEFEAT", x + 26, y + 22, 40, accent);
    DrawText(TextFormat("Wave: %d", state.wave), x + 28, y + 82, 24, RAYWHITE);
    DrawText(TextFormat("Kills: %d", state.kills), x + 190, y + 82, 24, RAYWHITE);
    DrawText(TextFormat("Score: %d", state.score), x + 340, y + 82, 24, GOLD);
    DrawText(TextFormat("Time: %.1fs", state.timeSeconds), x + 520, y + 82, 24, RAYWHITE);
    DrawText("Press ENTER to replay", x + 28, y + 130, 28, RAYWHITE);
    DrawText("Press F5 to reload scene instantly", x + 28, y + 170, 22, SKYBLUE);
}

} // namespace fw
