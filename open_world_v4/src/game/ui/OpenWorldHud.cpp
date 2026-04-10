#include "game/ui/OpenWorldHud.h"

#include <string>

#include "raylib.h"

#include "game/components/HealthComponent.h"

namespace fw {

void OpenWorldHud::Draw(const World& world, const OpenWorldGameState& state) const {
    DrawRectangle(18, 18, 360, 120, Fade(BLACK, 0.55f));
    DrawRectangleLines(18, 18, 360, 120, SKYBLUE);
    DrawText(state.regionBanner.c_str(), 30, 28, 24, RAYWHITE);

    const Entity player = world.FindByTag("player");
    const HealthComponent* health = world.GetComponent<HealthComponent>(player);
    if (health) {
        DrawText(TextFormat("Health: %d / %d", health->current, health->maximum), 30, 58, 20, GREEN);
    }

    DrawText("E interact | I inventory | J quest log | F5 reload", 30, 84, 18, LIGHTGRAY);
    if (!state.hoveredPrompt.empty()) {
        DrawRectangle(18, GetScreenHeight() - 70, 460, 42, Fade(BLACK, 0.6f));
        DrawText(state.hoveredPrompt.c_str(), 30, GetScreenHeight() - 58, 20, YELLOW);
    }

    if (state.dialogue.active) {
        DrawRectangle(40, GetScreenHeight() - 190, GetScreenWidth() - 80, 120, Fade(BLACK, 0.82f));
        DrawRectangleLines(40, GetScreenHeight() - 190, GetScreenWidth() - 80, 120, ORANGE);
        DrawText(state.dialogue.speakerName.c_str(), 58, GetScreenHeight() - 176, 24, ORANGE);
        DrawText(state.dialogue.CurrentLine().c_str(), 58, GetScreenHeight() - 138, 22, RAYWHITE);
        DrawText("Press E to continue", 58, GetScreenHeight() - 102, 18, LIGHTGRAY);
    }

    if (state.inventoryOpen) {
        DrawRectangle(GetScreenWidth() - 360, 24, 330, 180, Fade(BLACK, 0.75f));
        DrawRectangleLines(GetScreenWidth() - 360, 24, 330, 180, GOLD);
        DrawText("Inventory", GetScreenWidth() - 338, 36, 24, GOLD);
        DrawText(TextFormat("Gold: %d", state.saveProfile.inventory.gold), GetScreenWidth() - 338, 68, 20, YELLOW);
        DrawText(TextFormat("Weapon: %s", state.saveProfile.inventory.equippedWeaponId.c_str()), GetScreenWidth() - 338, 96, 18, RAYWHITE);
        DrawText(TextFormat("Armor: %s", state.saveProfile.inventory.equippedArmorId.c_str()), GetScreenWidth() - 338, 122, 18, RAYWHITE);
        int y = 148;
        for (const auto& item : state.saveProfile.inventory.items) {
            DrawText(TextFormat("- %s x%d", item.displayName.c_str(), item.quantity), GetScreenWidth() - 338, y, 18, LIGHTGRAY);
            y += 20;
            if (y > 188) break;
        }
    }

    if (state.questLogOpen) {
        DrawRectangle(GetScreenWidth() - 360, 220, 330, 190, Fade(BLACK, 0.75f));
        DrawRectangleLines(GetScreenWidth() - 360, 220, 330, 190, SKYBLUE);
        DrawText("Quest Log", GetScreenWidth() - 338, 232, 24, SKYBLUE);
        int y = 266;
        for (const auto& quest : state.saveProfile.quests) {
            DrawText(quest.title.c_str(), GetScreenWidth() - 338, y, 18, RAYWHITE);
            y += 22;
            DrawText(quest.description.c_str(), GetScreenWidth() - 338, y, 16, LIGHTGRAY);
            y += 34;
            if (y > 380) break;
        }
    }
}

} // namespace fw
