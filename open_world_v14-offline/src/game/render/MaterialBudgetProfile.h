#pragma once

#include <string>

namespace fw {

enum class MaterialBudgetTier {
    Low,
    Medium,
    High,
    Hero
};

struct MaterialBudgetProfile {
    std::string materialId;
    MaterialBudgetTier tier = MaterialBudgetTier::Medium;
    int maxTextureResolution = 1024;
    int maxMaterialSlots = 1;
    bool allowTransparency = false;
};

} // namespace fw
