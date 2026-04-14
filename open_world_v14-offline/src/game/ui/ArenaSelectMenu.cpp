#include "game/ui/ArenaSelectMenu.h"

#include "raylib.h"

namespace fw {

void ArenaSelectMenu::Draw(const ArenaGameState& state) const {
    const int sw = GetScreenWidth();
    const int x = sw / 2 - 300;
    const int y = 210;
    DrawRectangle(x, y, 600, 124, Fade(BLACK, 0.55f));
    DrawRectangleLines(x, y, 600, 124, ORANGE);
    DrawText("Arena / difficulty", x + 18, y + 12, 24, ORANGE);
    DrawText(TextFormat("Arena: %s   ([ / ] to change)", state.levelName.c_str()), x + 18, y + 46, 22, RAYWHITE);
    DrawText(TextFormat("Difficulty: %d   (- / = to change)", state.difficultyTier), x + 18, y + 76, 22, GOLD);
}

} // namespace fw
