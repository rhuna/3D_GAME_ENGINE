#pragma once
#include <string>
#include <vector>

namespace fw
{
    struct InventoryComponent
    {
        int gold = 0;
        std::string equippedWeaponId;
        std::string equippedArmorId;
        std::vector<std::string> items;

        void Clear()
        {
            gold = 0;
            equippedWeaponId.clear();
            equippedArmorId.clear();
            items.clear();
        }
    };
}
