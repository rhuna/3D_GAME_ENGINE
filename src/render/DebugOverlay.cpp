#include "render/DebugOverlay.h"

#include <cstdio>

namespace fw {

void DebugOverlay::Draw(const Time& time, const World& world, const Camera3D& camera, const std::string& sceneName) const {
    DrawRectangle(10, 10, 460, 205, Fade(BLACK, 0.65f));
    DrawRectangleLines(10, 10, 460, 205, RAYWHITE);

    char buffer[256];
    std::snprintf(buffer, sizeof(buffer), "Scene: %s", sceneName.c_str());
    DrawText(buffer, 20, 20, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "FPS: %d", GetFPS());
    DrawText(buffer, 20, 45, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "Frame: %llu", time.FrameCount());
    DrawText(buffer, 20, 70, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "Entities: %d", static_cast<int>(world.Entities().size()));
    DrawText(buffer, 20, 95, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "Collisions this frame: %d", static_cast<int>(world.Collisions().size()));
    DrawText(buffer, 20, 120, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "Camera: (%.2f, %.2f, %.2f)",
                  camera.position.x, camera.position.y, camera.position.z);
    DrawText(buffer, 20, 145, 20, RAYWHITE);

    DrawText("F1 Debug | F5 Reload | F6 Save | F7 Load | LMB Spawn Orb | RMB Look", 20, 175, 18, LIGHTGRAY);
}

} // namespace fw
