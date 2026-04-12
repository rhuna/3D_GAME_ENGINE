#pragma once

#include <string>

namespace fw {

struct EquipmentComponent {
    std::string weaponSlot = "starter_staff";
    std::string armorSlot = "traveler_clothes";
    std::string accessorySlot;
};

} // namespace fw
