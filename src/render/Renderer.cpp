#include "render/Renderer.h"

namespace fw {

void Renderer::BeginFrame() {
    BeginDrawing();
    ClearBackground(Color{20, 24, 32, 255});
}

void Renderer::Begin3D(const Camera3D& camera) {
    BeginMode3D(camera);
}

void Renderer::DrawWorld(const World& world) const {
    for (const auto& entity : world.Entities()) {
        if (!entity.active) {
            continue;
        }

        if (entity.render.drawCube) {
            DrawCubeV(entity.transform.position,
                      Vector3{entity.transform.scale.x * entity.render.cubeSize,
                              entity.transform.scale.y * entity.render.cubeSize,
                              entity.transform.scale.z * entity.render.cubeSize},
                      entity.render.tint);

            DrawCubeWiresV(entity.transform.position,
                           Vector3{entity.transform.scale.x * entity.render.cubeSize,
                                   entity.transform.scale.y * entity.render.cubeSize,
                                   entity.transform.scale.z * entity.render.cubeSize},
                           BLACK);
        }
    }
}

void Renderer::DrawGrid(int slices, float spacing) const {
    ::DrawGrid(slices, spacing);
}

void Renderer::End3D() {
    EndMode3D();
}

void Renderer::EndFrame() {
    EndDrawing();
}

} // namespace fw
