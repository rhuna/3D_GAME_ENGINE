#include "game/ui/GameHud.h"

#include <string>

#include "ecs/World.h"
#include "ecs/components/TagComponent.h"
#include "game/components/HealthComponent.h"
#include "raylib.h"

namespace fw {

void GameHud::Draw(const World& world, const ArenaGameState& state) const {
    int playerHp = 0;
    int playerMaxHp = 100;
    for (const Entity entity : world.Entities()) {
        const TagComponent* tag = world.GetComponent<TagComponent>(entity);
        const HealthComponent* health = world.GetComponent<HealthComponent>(entity);
        if (tag && health && tag->value == "player") {
            playerHp = health->current;
            playerMaxHp = health->maximum;
            break;
        }
    }

    DrawRectangle(18, 18, 320, 150, Fade(BLACK, 0.58f));
    DrawRectangleLines(18, 18, 320, 150, SKYBLUE);
    DrawText("Wizard Arena v11", 30, 28, 26, RAYWHITE);
    DrawText(TextFormat("Wave: %d", state.wave), 30, 62, 20, GOLD);
    DrawText(TextFormat("Enemies: %d", state.enemiesRemaining), 30, 86, 20, RAYWHITE);
    DrawText(TextFormat("Kills: %d", state.kills), 175, 86, 20, RAYWHITE);
    DrawText(TextFormat("Pickups: %d", state.pickupsCollected), 30, 110, 20, GREEN);
    DrawText(TextFormat("Time: %.1fs", state.timeSeconds), 175, 110, 20, RAYWHITE);

    const int barX = 30;
    const int barY = 138;
    const int barW = 280;
    const int barH = 18;
    DrawRectangle(barX, barY, barW, barH, Fade(MAROON, 0.65f));
    const float hpRatio = playerMaxHp > 0 ? static_cast<float>(playerHp) / static_cast<float>(playerMaxHp) : 0.0f;
    DrawRectangle(barX, barY, static_cast<int>(barW * hpRatio), barH, RED);
    DrawRectangleLines(barX, barY, barW, barH, RAYWHITE);
    DrawText(TextFormat("HP %d / %d", playerHp, playerMaxHp), barX + 92, barY - 2, 18, WHITE);

    const int centerX = GetScreenWidth() / 2;
    if (state.phase == ArenaPhase::WaitingToStart) {
        DrawText("Press ENTER to start", centerX - 170, 24, 30, LIME);
        DrawText("WASD move  |  LMB cast  |  P pause  |  F5 restart", centerX - 250, 58, 22, RAYWHITE);
    } else if (state.phase == ArenaPhase::Victory) {
        DrawText("VICTORY - Press ENTER or F5 to play again", centerX - 280, 24, 28, LIME);
    } else if (state.phase == ArenaPhase::Defeat) {
        DrawText("DEFEAT - Press ENTER or F5 to retry", centerX - 230, 24, 28, RED);
    } else if (state.phase == ArenaPhase::Paused) {
        DrawText("PAUSED - Press P to resume", centerX - 180, 24, 28, YELLOW);
    } else {
        DrawText("WASD move  |  LMB cast  |  P pause  |  F5 restart", centerX - 250, 24, 22, RAYWHITE);
    }

    if (!state.statusText.empty()) {
        const int boxW = 420;
        const int boxH = 34;
        const int boxX = centerX - boxW / 2;
        const int boxY = GetScreenHeight() - 56;
        DrawRectangle(boxX, boxY, boxW, boxH, Fade(BLACK, 0.55f));
        DrawRectangleLines(boxX, boxY, boxW, boxH, Fade(SKYBLUE, 0.7f));
        DrawText(state.statusText.c_str(), boxX + 12, boxY + 7, 20, WHITE);
    }
}

} // namespace fw
