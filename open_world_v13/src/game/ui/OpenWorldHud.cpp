#include "game/ui/OpenWorldHud.h"

#include <vector>

#include "game/save/SaveGameProfile.h"
#include "game/world/RegionSimulationState.h"
#include "raylib.h"

namespace fw {
namespace {

void DrawPanel(int x, int y, int w, int h, Color border = SKYBLUE)
{
    DrawRectangle(x, y, w, h, Fade(BLACK, 0.58f));
    DrawRectangleLines(x, y, w, h, border);
}

void DrawCompactKeyBar()
{
    const char* text = "[F1] Controls  [E] Interact  [TAB] Quests  [I] Gear  [F5/F9] Save/Load  [1-3] Regions";
    const int fontSize = 18;
    const int paddingX = 12;
    const int width = MeasureText(text, fontSize) + paddingX * 2;
    const int height = 30;
    const int x = 20;
    const int y = GetScreenHeight() - height - 18;

    DrawPanel(x, y, width, height, GRAY);
    DrawText(text, x + paddingX, y + 6, fontSize, RAYWHITE);
}

void DrawControlsLegend(int x, int y)
{
    const int width = 360;
    const int height = 276;
    const int left = x + 14;
    const int right = x + 188;
    int rowY = y + 40;
    const int row = 24;

    DrawPanel(x, y, width, height, SKYBLUE);
    DrawText("Controls", x + 14, y + 10, 22, RAYWHITE);
    DrawText("Press F1 to hide", x + width - 150, y + 12, 18, LIGHTGRAY);

    DrawText("Move", left, rowY, 20, SKYBLUE);
    DrawText("WASD", right, rowY, 20, RAYWHITE);
    rowY += row;
    DrawText("Sprint", left, rowY, 20, SKYBLUE);
    DrawText("Left Shift", right, rowY, 20, RAYWHITE);
    rowY += row;
    DrawText("Jump", left, rowY, 20, SKYBLUE);
    DrawText("Space", right, rowY, 20, RAYWHITE);
    rowY += row;
    DrawText("Interact / Next", left, rowY, 20, SKYBLUE);
    DrawText("E or Enter", right, rowY, 20, RAYWHITE);
    rowY += row;
    DrawText("Quest Log", left, rowY, 20, SKYBLUE);
    DrawText("Tab", right, rowY, 20, RAYWHITE);
    rowY += row;
    DrawText("Equipment", left, rowY, 20, SKYBLUE);
    DrawText("I", right, rowY, 20, RAYWHITE);
    rowY += row;
    DrawText("Save / Load", left, rowY, 20, SKYBLUE);
    DrawText("F5 / F9", right, rowY, 20, RAYWHITE);
    rowY += row;
    DrawText("World Save / Load", left, rowY, 20, SKYBLUE);
    DrawText("O / P", right, rowY, 20, RAYWHITE);
    rowY += row;
    DrawText("Reset Region", left, rowY, 20, SKYBLUE);
    DrawText("K", right, rowY, 20, RAYWHITE);
    rowY += row;
    DrawText("Restore Starter", left, rowY, 20, SKYBLUE);
    DrawText("F6", right, rowY, 20, RAYWHITE);
    rowY += row;
    DrawText("Debug Regions", left, rowY, 20, SKYBLUE);
    DrawText("1 / 2 / 3", right, rowY, 20, RAYWHITE);

    DrawText("Current build note:", x + 14, y + height - 46, 18, GOLD);
    DrawText("HUD legend matches the active v13 keys.", x + 14, y + height - 24, 18, RAYWHITE);
}

} // namespace

void DrawOpenWorldHud(const SaveGameProfile& profile,
                      const char* formattedTime,
                      const RegionSimulationState& simState,
                      const std::vector<ActiveNpcRoutineInfo>&,
                      const char* interactionPrompt,
                      const char* statusText,
                      int aliveEnemies,
                      int attack,
                      int defense,
                      const char* reputationSummary)
{
    static bool s_showControlsLegend = true;
    if (IsKeyPressed(KEY_F1))
    {
        s_showControlsLegend = !s_showControlsLegend;
    }

    DrawText(TextFormat("Time: %s", formattedTime), 20, 250, 20, SKYBLUE);
    DrawText(TextFormat("Region: %s", profile.currentRegion.c_str()), 20, 275, 20, WHITE);
    DrawText(TextFormat("Health: %i / %i", profile.playerHealth, profile.playerMaxHealth), 20, 300, 20, WHITE);
    DrawText(TextFormat("Gold: %i", profile.inventory.gold), 20, 325, 20, GOLD);
    DrawText(TextFormat("Atk: %i  Def: %i", attack, defense), 20, 350, 20, ORANGE);
    DrawText(TextFormat("Tracked Quests: %i", static_cast<int>(profile.quests.size())), 20, 375, 20, LIME);
    DrawText(TextFormat("Enemies Alive: %i", aliveEnemies), 20, 400, 20, RED);
    DrawText(TextFormat("Safe Zone: %s", simState.safeZone ? "Yes" : "No"), 20, 425, 20, simState.safeZone ? GREEN : ORANGE);
    if (reputationSummary && reputationSummary[0]) DrawText(reputationSummary, 20, 450, 20, SKYBLUE);
    if (interactionPrompt && interactionPrompt[0]) DrawText(interactionPrompt, 20, 475, 22, YELLOW);
    if (statusText && statusText[0]) DrawText(statusText, 20, 500, 20, ORANGE);

    DrawCompactKeyBar();
    if (s_showControlsLegend)
    {
        DrawControlsLegend(GetScreenWidth() - 380, 20);
    }
}

} // namespace fw
