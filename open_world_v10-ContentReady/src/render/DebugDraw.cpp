#include "DebugDraw.h"

#include "core/Time.h"
#include "ecs/World.h"
#include "scene/Scene.h"

#include <raylib.h>

namespace fw {
void DebugDraw::DrawWorldAxes() {
    DrawLine3D({0, 0, 0}, {3, 0, 0}, RED);
    DrawLine3D({0, 0, 0}, {0, 3, 0}, GREEN);
    DrawLine3D({0, 0, 0}, {0, 0, 3}, BLUE);
}

void DebugDraw::DrawOverlay(const Scene& scene, const World& world) {
    const Camera3D& cam = scene.MainCamera();

    DrawRectangle(10, 10, 340, 135, Fade(BLACK, 0.6f));
    DrawText("FireWizard3DEngine Debug", 20, 18, 20, RAYWHITE);
    DrawFPS(20, 45);
    DrawText(TextFormat("Time: %.2f", Time::ElapsedSeconds()), 20, 72, 18, RAYWHITE);
    DrawText(TextFormat("Entities: %i", static_cast<int>(world.Entities().size())), 20, 95, 18, RAYWHITE);
    DrawText(TextFormat("Cam: %.2f %.2f %.2f", cam.position.x, cam.position.y, cam.position.z), 20, 118, 18,
             RAYWHITE);
}
}  // namespace fw
