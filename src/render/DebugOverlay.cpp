#include "render/DebugOverlay.h"

#include <cstdio>

namespace fw {

void DebugOverlay::Draw(const Time& time, const World& world, const Camera3D& camera, const std::string& sceneName) const {
    DrawRectangle(10, 10, 360, 160, Fade(BLACK, 0.65f));
    DrawRectangleLines(10, 10, 360, 160, RAYWHITE);

    char buffer[256];
    std::snprintf(buffer, sizeof(buffer), "Scene: %s", sceneName.c_str());
    DrawText(buffer, 20, 20, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "FPS: %d", GetFPS());
    DrawText(buffer, 20, 45, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "Frame: %llu", time.FrameCount());
    DrawText(buffer, 20, 70, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "Entities: %d", static_cast<int>(world.Entities().size()));
    DrawText(buffer, 20, 95, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "Camera: (%.2f, %.2f, %.2f)",
                  camera.position.x, camera.position.y, camera.position.z);
    DrawText(buffer, 20, 120, 20, RAYWHITE);

    DrawText("F1 Toggle Debug | F5 Reload Scene | RMB Look", 20, 145, 18, LIGHTGRAY);
}

} // namespace fw
