#include "render/Renderer.h"

#include "assets/AssetManager.h"

namespace fw {

void Renderer::BeginFrame() {
    BeginDrawing();
    ClearBackground(Color{20, 24, 32, 255});
}

void Renderer::Begin3D(const Camera3D& camera) {
    BeginMode3D(camera);
}

void Renderer::DrawWorld(const World& world, AssetManager& assets) const {
    for (const auto& entity : world.Entities()) {
        if (!entity.active || !entity.transform || !entity.render || !entity.render->visible) {
            continue;
        }

        const auto& transform = *entity.transform;
        const auto& render = *entity.render;

        if (render.useModel && !render.modelPath.empty()) {
            if (Model* model = assets.LoadModelCached(render.modelPath)) {
                DrawModelEx(*model,
                            transform.position,
                            Vector3{0.0f, 1.0f, 0.0f},
                            transform.rotationEuler.y,
                            transform.scale,
                            render.tint);
                continue;
            }
        }

        if (render.drawCube) {
            const Vector3 size {
                transform.scale.x * render.cubeSize,
                transform.scale.y * render.cubeSize,
                transform.scale.z * render.cubeSize
            };
            DrawCubeV(transform.position, size, render.tint);
            DrawCubeWiresV(transform.position, size, BLACK);
        }

        if (render.drawSphere) {
            DrawSphere(transform.position, render.sphereRadius * transform.scale.x, render.tint);
            DrawSphereWires(transform.position, render.sphereRadius * transform.scale.x, 8, 8, BLACK);
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
