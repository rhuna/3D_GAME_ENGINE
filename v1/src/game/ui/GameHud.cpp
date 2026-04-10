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

    DrawRectangle(18, 18, 280, 118, Fade(BLACK, 0.55f));
    DrawRectangleLines(18, 18, 280, 118, SKYBLUE);
    DrawText("Wizard Arena Prototype", 30, 28, 24, RAYWHITE);
    DrawText(TextFormat("Wave: %d", state.wave), 30, 58, 20, GOLD);
    DrawText(TextFormat("Enemies: %d", state.enemiesRemaining), 30, 82, 20, RAYWHITE);
    DrawText(TextFormat("Kills: %d", state.kills), 160, 82, 20, RAYWHITE);

    const int barX = 30;
    const int barY = 108;
    const int barW = 240;
    const int barH = 18;
    DrawRectangle(barX, barY, barW, barH, Fade(MAROON, 0.65f));
    const float hpRatio = playerMaxHp > 0 ? static_cast<float>(playerHp) / static_cast<float>(playerMaxHp) : 0.0f;
    DrawRectangle(barX, barY, static_cast<int>(barW * hpRatio), barH, RED);
    DrawRectangleLines(barX, barY, barW, barH, RAYWHITE);
    DrawText(TextFormat("HP %d / %d", playerHp, playerMaxHp), barX + 70, barY - 2, 18, WHITE);

    if (state.phase == ArenaPhase::Victory) {
        DrawText("VICTORY - Press F5 to play again", GetScreenWidth() / 2 - 220, 24, 28, LIME);
    } else if (state.phase == ArenaPhase::Defeat) {
        DrawText("DEFEAT - Press F5 to retry", GetScreenWidth() / 2 - 180, 24, 28, RED);
    } else {
        DrawText("WASD move  |  LMB cast  |  RMB look  |  F5 restart", GetScreenWidth() / 2 - 250, 24, 22, RAYWHITE);
    }
}

} // namespace fw
