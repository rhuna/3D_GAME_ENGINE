#pragma once

#include <string>

namespace fw {

struct InventoryItem {
    std::string id;
    std::string displayName;
    int quantity = 1;
    std::string iconId;
};

} // namespace fw
