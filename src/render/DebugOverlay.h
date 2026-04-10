#pragma once

#include <string>

#include "core/Time.h"
#include "ecs/World.h"
#include "raylib.h"

namespace fw {

class DebugOverlay {
public:
    void Draw(const Time& time, const World& world, const Camera3D& camera, const std::string& sceneName) const;
};

} // namespace fw
