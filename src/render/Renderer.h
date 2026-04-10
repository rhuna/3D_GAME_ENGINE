#pragma once

#include "ecs/World.h"
#include "raylib.h"

namespace fw {

class Renderer {
public:
    void BeginFrame();
    void Begin3D(const Camera3D& camera);
    void DrawWorld(const World& world) const;
    void DrawGrid(int slices, float spacing) const;
    void End3D();
    void EndFrame();
};

} // namespace fw
