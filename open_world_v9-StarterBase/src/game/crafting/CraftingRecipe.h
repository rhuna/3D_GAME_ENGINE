#pragma once

#include <string>
#include <vector>

namespace fw {

struct CraftingIngredient {
    std::string itemId;
    int quantity = 1;
};

struct CraftingRecipe {
    std::string id;
    std::string displayName;
    std::vector<CraftingIngredient> inputs;
    std::string outputItemId;
    int outputQuantity = 1;
};

} // namespace fw
