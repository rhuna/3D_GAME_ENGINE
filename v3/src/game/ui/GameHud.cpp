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

    if (state.phase == ArenaPhase::WaitingToStart) {
        m_mainMenu.Draw();
    }

    DrawRectangle(18, 18, 380, 198, Fade(BLACK, 0.58f));
    DrawRectangleLines(18, 18, 380, 198, SKYBLUE);
    DrawText("Wizard Arena v12", 30, 28, 26, RAYWHITE);
    DrawText(state.levelName.c_str(), 30, 56, 18, SKYBLUE);
    DrawText(TextFormat("Wave: %d", state.wave), 30, 82, 20, GOLD);
    DrawText(TextFormat("Enemies: %d", state.enemiesRemaining), 30, 106, 20, RAYWHITE);
    DrawText(TextFormat("Kills: %d", state.kills), 205, 106, 20, RAYWHITE);
    DrawText(TextFormat("Pickups: %d", state.pickupsCollected), 30, 130, 20, GREEN);
    DrawText(TextFormat("Score: %d", state.score), 205, 130, 20, GOLD);
    DrawText(TextFormat("Time: %.1fs", state.timeSeconds), 30, 154, 20, RAYWHITE);

    const int barX = 30;
    const int barY = 184;
    const int barW = 336;
    const int barH = 16;
    DrawRectangle(barX, barY, barW, barH, Fade(MAROON, 0.65f));
    const float hpRatio = playerMaxHp > 0 ? static_cast<float>(playerHp) / static_cast<float>(playerMaxHp) : 0.0f;
    DrawRectangle(barX, barY, static_cast<int>(barW * hpRatio), barH, RED);
    DrawRectangleLines(barX, barY, barW, barH, RAYWHITE);
    DrawText(TextFormat("HP %d / %d", playerHp, playerMaxHp), barX + 110, barY - 1, 16, WHITE);

    if (state.playerDamageBoostRemaining > 0.0f) {
        const int boostX = 30;
        const int boostY = 210;
        const int boostW = 336;
        const int boostH = 12;
        DrawRectangle(boostX, boostY, boostW, boostH, Fade(DARKPURPLE, 0.55f));
        const float r = state.playerDamageBoostRemaining / 8.0f;
        DrawRectangle(boostX, boostY, static_cast<int>(boostW * (r > 1.0f ? 1.0f : r)), boostH, VIOLET);
        DrawRectangleLines(boostX, boostY, boostW, boostH, PURPLE);
        DrawText(TextFormat("Damage boost %.1fs", state.playerDamageBoostRemaining), boostX, boostY + 16, 18, VIOLET);
    }

    const int centerX = GetScreenWidth() / 2;
    if (state.phase == ArenaPhase::Paused) {
        DrawText("PAUSED - Press P to resume", centerX - 180, 24, 28, YELLOW);
    } else if (state.phase == ArenaPhase::Playing) {
        DrawText("WASD move  |  LMB cast  |  P pause  |  F5 restart", centerX - 250, 24, 22, RAYWHITE);
    }

    if (!state.statusText.empty()) {
        const int boxW = 520;
        const int boxH = 36;
        const int boxX = centerX - boxW / 2;
        const int boxY = GetScreenHeight() - 60;
        Color border = SKYBLUE;
        if (state.damageFlashTimer > 0.0f) border = RED;
        if (state.healFlashTimer > 0.0f) border = GREEN;
        DrawRectangle(boxX, boxY, boxW, boxH, Fade(BLACK, 0.58f));
        DrawRectangleLines(boxX, boxY, boxW, boxH, border);
        DrawText(state.statusText.c_str(), boxX + 12, boxY + 8, 20, WHITE);
    }

    if (state.phase == ArenaPhase::Victory || state.phase == ArenaPhase::Defeat) {
        m_endScreen.Draw(state);
    }

    if (state.damageFlashTimer > 0.0f) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RED, 0.10f));
    } else if (state.healFlashTimer > 0.0f) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GREEN, 0.08f));
    }
}

} // namespace fw
