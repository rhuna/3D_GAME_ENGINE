#include "editor/runtime/EditorPicking.h"

#include <limits>

#include "ecs/World.h"
#include "ecs/components/BoxColliderComponent.h"
#include "ecs/components/RenderComponent.h"
#include "ecs/components/TransformComponent.h"
#include "raylib.h"

namespace fw {

namespace {

BoundingBox BuildBoundingBox(const TransformComponent& transform, const BoxColliderComponent* collider, const RenderComponent* render) {
    Vector3 halfExtents{0.5f, 0.5f, 0.5f};

    if (collider) {
        halfExtents = collider->halfExtents;
    } else if (render) {
        if (render->drawSphere) {
            const float r = render->sphereRadius * transform.scale.x;
            halfExtents = Vector3{r, r, r};
        } else {
            halfExtents = Vector3{
                transform.scale.x * render->cubeSize * 0.5f,
                transform.scale.y * render->cubeSize * 0.5f,
                transform.scale.z * render->cubeSize * 0.5f
            };
        }
    } else {
        halfExtents = Vector3{
            transform.scale.x * 0.5f,
            transform.scale.y * 0.5f,
            transform.scale.z * 0.5f
        };
    }

    return BoundingBox{
        Vector3{transform.position.x - halfExtents.x, transform.position.y - halfExtents.y, transform.position.z - halfExtents.z},
        Vector3{transform.position.x + halfExtents.x, transform.position.y + halfExtents.y, transform.position.z + halfExtents.z}
    };
}

Entity PickEntityFromRay(const World& world, const Ray& ray) {
    Entity bestEntity = 0;
    float bestDistance = std::numeric_limits<float>::max();

    for (Entity entity : world.Entities()) {
        const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        const RenderComponent* render = world.GetComponent<RenderComponent>(entity);
        const BoxColliderComponent* collider = world.GetComponent<BoxColliderComponent>(entity);
        if (!transform || !render) continue;

        const BoundingBox bounds = BuildBoundingBox(*transform, collider, render);
        const RayCollision hit = GetRayCollisionBox(ray, bounds);
        if (hit.hit && hit.distance < bestDistance) {
            bestDistance = hit.distance;
            bestEntity = entity;
        }
    }

    return bestEntity;
}

} // namespace

Entity EditorPicking::PickEntity(const World& world, const Camera3D& camera) {
    return PickEntityAtScreenPoint(world, camera, GetMousePosition());
}

Entity EditorPicking::PickEntityAtScreenPoint(const World& world, const Camera3D& camera, Vector2 screenPoint) {
    const Ray ray = GetMouseRay(screenPoint, camera);
    return PickEntityFromRay(world, ray);
}

} // namespace fw
