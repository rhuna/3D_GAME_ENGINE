#pragma once

#include "ecs/World.h"
#include "gameplay/prefabs/PrefabLibrary.h"
#include "scene/data/SceneDefinition.h"

namespace fw {

class SceneSpawner {
public:
    static bool Spawn(World& world, const PrefabLibrary& prefabs, const SceneDefinition& scene);
};

} // namespace fw
