#pragma once

#include <string>
#include <vector>

namespace fw {

struct InventoryEntry {
    std::string itemId;
    std::string displayName;
    int quantity = 1;
};

struct InventoryComponent {
    std::vector<InventoryEntry> items;
    std::string equippedWeaponId = "starter_staff";
    std::string equippedArmorId = "traveler_clothes";
    int gold = 25;
};

} // namespace fw
