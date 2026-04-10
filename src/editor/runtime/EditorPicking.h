#pragma once

#include "raylib.h"
#include "ecs/Entity.h"

namespace fw {
class World;

class EditorPicking {
public:
    static Entity PickEntity(const World& world, const ::Camera3D& camera);
};

} // namespace fw
