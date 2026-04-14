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
    std::string kitName;

    bool hasPosition = false;
    bool hasRotation = false;
    bool hasScale = false;
    TransformComponent transform {};

    bool hasTint = false;
    Color tint = WHITE;
};

struct SceneDefinition {
    std::string name;
    std::string displayName;
    std::string playerPrefab;
    bool autoSpawnPlayer = false;
    std::vector<std::string> preloadAssets;
    std::vector<std::string> includedScenes;
    std::vector<SceneSpawnEntry> entries;
};

} // namespace fw
