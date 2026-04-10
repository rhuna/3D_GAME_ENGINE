#include "render/DebugOverlay.h"

#include <cstdio>

namespace fw {

void DebugOverlay::Draw(const Time& time, const World& world, const Camera3D& camera, const std::string& sceneName, Entity selectedEntity, const std::vector<ValidationMessage>& validationMessages, const std::string& lastExportPath, bool inspectorVisible) const {
    DrawRectangle(10, 10, 980, 235, Color{0, 0, 0, 150});

    char buffer[512];
    std::snprintf(buffer, sizeof(buffer), "Scene: %s", sceneName.c_str());
    DrawText(buffer, 20, 20, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "FPS: %d", GetFPS());
    DrawText(buffer, 20, 45, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "Delta: %.4f | Fixed: %.4f", time.DeltaTime(), 1.0f / 60.0f);
    DrawText(buffer, 20, 70, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "Entities: %d | Collisions: %d | Selected: %u", static_cast<int>(world.Entities().size()), static_cast<int>(world.Collisions().size()), selectedEntity);
    DrawText(buffer, 20, 95, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "Camera: (%.2f, %.2f, %.2f)", camera.position.x, camera.position.y, camera.position.z);
    DrawText(buffer, 20, 120, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "Validation: %zu message(s) | Inspector: %s", validationMessages.size(), inspectorVisible ? "visible" : "hidden");
    DrawText(buffer, 20, 145, 20, RAYWHITE);

    std::snprintf(buffer, sizeof(buffer), "Last export: %s", lastExportPath.empty() ? "<none>" : lastExportPath.c_str());
    DrawText(buffer, 20, 170, 20, RAYWHITE);

    DrawText("F1 Debug | F2 Cycle | F3 Export | F4 Validate | F5 Reload | F6 Snapshot Save | F7 Snapshot Load | F8 Save Scene", 20, 195, 18, LIGHTGRAY);
    DrawText("LMB Pick | Shift+LMB Projectile | 1/2/3 Gizmo | J/L/I/K/U/O Edit | [/] Variant | N Spawn | Del Remove", 20, 217, 18, LIGHTGRAY);
}

} // namespace fw
