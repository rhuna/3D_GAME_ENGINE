#include "render/Renderer.h"

#include "assets/AssetManager.h"
#include "raymath.h"

namespace fw {
namespace {

std::string SelectMeshId(const RenderComponent& render, const Vector3& worldPosition, const Camera3D& camera) {
    const float distance = Vector3Distance(worldPosition, camera.position);

    if (!render.lod2MeshId.empty() && distance >= render.lod2Distance) {
        return render.lod2MeshId;
    }
    if (!render.lod1MeshId.empty() && distance >= render.lod1Distance) {
        return render.lod1MeshId;
    }
    if (!render.lod0MeshId.empty()) {
        return render.lod0MeshId;
    }
    if (!render.meshId.empty()) {
        return render.meshId;
    }
    return render.modelPath;
}

} // namespace

void Renderer::BeginFrame() {
    BeginDrawing();
    ClearBackground(Color{20, 24, 32, 255});
}

void Renderer::Begin3D(const Camera3D& camera) {
    BeginMode3D(camera);
}

void Renderer::DrawWorld(const World& world, AssetManager& assets, const Camera3D& camera) const {
    for (const Entity entity : world.Entities()) {
        const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        const RenderComponent* render = world.GetComponent<RenderComponent>(entity);
        if (!transform || !render || !render->visible) {
            continue;
        }

        if (render->useModel) {
            const std::string meshId = SelectMeshId(*render, transform->position, camera);
            if (!meshId.empty()) {
                if (Model* model = assets.GetModel(meshId)) {
                    DrawModelEx(*model,
                                transform->position,
                                Vector3{0.0f, 1.0f, 0.0f},
                                transform->rotationEuler.y,
                                transform->scale,
                                render->tint);
                    continue;
                }
            }
        }

        if (render->drawCube) {
            const Vector3 size {
                transform->scale.x * render->cubeSize,
                transform->scale.y * render->cubeSize,
                transform->scale.z * render->cubeSize
            };
            DrawCubeV(transform->position, size, render->tint);
            DrawCubeWiresV(transform->position, size, BLACK);
        }

        if (render->drawSphere) {
            DrawSphere(transform->position, render->sphereRadius * transform->scale.x, render->tint);
            DrawSphereWires(transform->position, render->sphereRadius * transform->scale.x, 8, 8, BLACK);
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
