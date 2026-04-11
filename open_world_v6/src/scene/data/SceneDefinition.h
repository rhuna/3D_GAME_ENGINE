#pragma once

#include <string>
#include <vector>

#include "ecs/components/RenderComponent.h"
#include "ecs/components/TransformComponent.h"

namespace fw {

struct SceneSpawnEntry {
    std::string prefabName;
    std::string variantName;
    std::string tagOverride;

    bool hasPosition = false;
    bool hasRotation = false;
    bool hasScale = false;
    TransformComponent transform {};

    bool hasTint = false;
    Color tint = WHITE;
};

struct SceneDefinition {
    std::string name;
    std::vector<SceneSpawnEntry> entries;
};

} // namespace fw
