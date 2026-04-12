#include "game/ui/InventoryWindow.h"

#include "raylib.h"

namespace fw {

void InventoryWindow::Draw(const OpenWorldGameState& state) const {
    if (!state.inventoryOpen) return;
    DrawRectangle(30, 90, 320, 260, Fade(BLACK, 0.65f));
    DrawRectangleLines(30, 90, 320, 260, RAYWHITE);
    DrawText("Inventory", 44, 104, 22, RAYWHITE);
    int y = 136;
    for (const auto& itemId : state.saveProfile.inventory.items) {
        DrawText(itemId.c_str(), 44, y, 18, LIGHTGRAY);
        y += 24;
        if (y > 320) break;
    }
    DrawText(TextFormat("Gold: %d", state.saveProfile.inventory.gold), 44, 326, 18, GOLD);
}

} // namespace fw
