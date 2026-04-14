#pragma once

#include <string>

#include "ecs/Entity.h"
#include "raylib.h"

namespace fw {
class World;
class EditorSelection;
class PrefabLibrary;
class SceneLibrary;

class KitPlacement {
public:
    static int PlaceKitAt(World& world,
                          EditorSelection& selection,
                          PrefabLibrary& prefabs,
                          const SceneLibrary& scenes,
                          const std::string& kitName,
                          const Vector3& rootPosition,
                          bool replaceSelection = true);
};

} // namespace fw
