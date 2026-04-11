#include "game/systems/EquipmentSystem.h"

#include "core/Application.h"
#include "ecs/World.h"
#include "game/components/EquipmentComponent.h"
#include "game/inventory/InventoryComponent.h"

namespace fw {

void EquipmentSystem::Update(Application&, World& world, float) {
    if (!m_state) return;
    const Entity player = world.FindByTag("player");
    if (player == 0) return;
    auto* inventory = world.GetComponent<InventoryComponent>(player);
    auto* equipment = world.GetComponent<EquipmentComponent>(player);
    if (!inventory || !equipment) return;
    if (equipment->weaponSlot.empty()) equipment->weaponSlot = inventory->equippedWeaponId;
    if (equipment->armorSlot.empty()) equipment->armorSlot = inventory->equippedArmorId;
}

} // namespace fw
