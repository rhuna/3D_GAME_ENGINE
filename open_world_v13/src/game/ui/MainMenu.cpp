#include "game/ui/MainMenu.h"

#include "raylib.h"

namespace fw {

void MainMenu::Draw(const ArenaGameState& state) const {
    const int sw = GetScreenWidth();
    const int sh = GetScreenHeight();
    const int panelW = 760;
    const int panelH = 280;
    const int x = sw / 2 - panelW / 2;
    const int y = sh / 2 - panelH / 2;

    DrawRectangle(x, y, panelW, panelH, Fade(BLACK, 0.72f));
    DrawRectangleLines(x, y, panelW, panelH, SKYBLUE);
    DrawText("3D_GAME_ENGINE v13", x + 26, y + 22, 36, RAYWHITE);
    DrawText("Wizard Arena Campaign Slice", x + 28, y + 68, 28, GOLD);
    DrawText("Press ENTER to start", x + 28, y + 112, 28, LIME);
    DrawText(TextFormat("Arena: %s  |  Difficulty: %d", state.levelName.c_str(), state.difficultyTier), x + 28, y + 152, 24, ORANGE);
    DrawText(TextFormat("Best score: %d  |  Arenas cleared: %d", state.bestScore, state.arenasCleared), x + 28, y + 184, 22, SKYBLUE);
    DrawText("[ / ] arena   |   - / = difficulty   |   WASD move   |   LMB cast", x + 28, y + 220, 20, RAYWHITE);
    DrawText("Health orbs heal. Purple orbs boost spell damage. v13 adds arena rotation + progression.", x + 28, y + 246, 18, VIOLET);
}

} // namespace fw
