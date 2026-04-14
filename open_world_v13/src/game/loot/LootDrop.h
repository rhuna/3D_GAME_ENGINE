#pragma once

#include <string>

namespace fw {

struct LootDrop {
    std::string itemId;
    int minQuantity = 1;
    int maxQuantity = 1;
    float weight = 1.0f;
};

} // namespace fw
