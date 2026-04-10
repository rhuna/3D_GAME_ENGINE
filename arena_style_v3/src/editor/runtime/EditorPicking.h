#pragma once

#include "ecs/Entity.h"
#include "raylib.h"

namespace fw {
class World;
class EditorPicking {
public:
    static Entity PickEntity(const World& world, const Camera3D& camera);
    static Entity PickEntityAtScreenPoint(const World& world, const Camera3D& camera, Vector2 screenPoint);
};

} // namespace fw
