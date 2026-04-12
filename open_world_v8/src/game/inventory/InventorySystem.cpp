#include "game/inventory/InventorySystem.h"

#include "core/Application.h"
#include "core/Input.h"
#include "ecs/World.h"
#include "ecs/components/TagComponent.h"
#include "game/inventory/InventoryComponent.h"

namespace fw {

void InventorySystem::Update(Application& app, World& world, float) {
    if (!m_state) return;
    if (app.GetInput().IsKeyPressed(KEY_I)) {
        m_state->inventoryOpen = !m_state->inventoryOpen;
    }
    if (app.GetInput().IsKeyPressed(KEY_J)) {
        m_state->questLogOpen = !m_state->questLogOpen;
    }

    const Entity player = world.FindByTag("player");
    InventoryComponent* inventory = world.GetComponent<InventoryComponent>(player);
    if (!inventory) return;
    m_state->saveProfile.inventory = *inventory;
}

} // namespace fw
