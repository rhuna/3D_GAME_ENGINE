#pragma once

#include <string>
#include <unordered_map>

#include "game/crafting/CraftingRecipe.h"

namespace fw {

class CraftingDatabase {
public:
    void AddRecipe(const CraftingRecipe& recipe) {
        m_recipes[recipe.id] = recipe;
    }

    const CraftingRecipe* FindRecipe(const std::string& id) const {
        auto it = m_recipes.find(id);
        return it == m_recipes.end() ? nullptr : &it->second;
    }

private:
    std::unordered_map<std::string, CraftingRecipe> m_recipes;
};

} // namespace fw
