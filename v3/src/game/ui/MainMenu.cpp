#include "game/ui/MainMenu.h"

#include "raylib.h"

namespace fw {

void MainMenu::Draw() const {
    const int sw = GetScreenWidth();
    const int sh = GetScreenHeight();
    const int panelW = 700;
    const int panelH = 220;
    const int x = sw / 2 - panelW / 2;
    const int y = sh / 2 - panelH / 2;

    DrawRectangle(x, y, panelW, panelH, Fade(BLACK, 0.68f));
    DrawRectangleLines(x, y, panelW, panelH, SKYBLUE);
    DrawText("3D_GAME_ENGINE v12", x + 26, y + 22, 36, RAYWHITE);
    DrawText("Wizard Arena Prototype", x + 28, y + 68, 28, GOLD);
    DrawText("Press ENTER to start", x + 28, y + 112, 28, LIME);
    DrawText("WASD move  |  LMB cast  |  P pause  |  F11 fullscreen", x + 28, y + 152, 22, RAYWHITE);
    DrawText("Health orbs heal. Purple orbs boost spell damage.", x + 28, y + 182, 20, VIOLET);
}

} // namespace fw
