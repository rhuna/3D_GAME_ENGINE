#pragma once

namespace fw {
class Scene;
class World;

class DebugDraw {
public:
    static void DrawWorldAxes();
    static void DrawOverlay(const Scene& scene, const World& world);
};
}  // namespace fw
