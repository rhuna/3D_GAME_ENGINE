#include "ecs/systems/CollisionSystem.h"

#include "core/Application.h"
#include "ecs/World.h"

namespace fw {

namespace {
BoundingBox MakeBounds(const TransformComponent& transform, const BoxColliderComponent& collider) {
    const Vector3& position = transform.position;
    const Vector3& half = collider.halfExtents;
    return BoundingBox{
        Vector3{position.x - half.x, position.y - half.y, position.z - half.z},
        Vector3{position.x + half.x, position.y + half.y, position.z + half.z}
    };
}
}

void CollisionSystem::FixedUpdate(Application& app, World& world, float fixedDeltaTime) {
    (void)app;
    (void)fixedDeltaTime;

    world.ClearTransientState();
    const auto& entities = world.Entities();

    for (std::size_t i = 0; i < entities.size(); ++i) {
        const Entity a = entities[i];
        auto* aTransform = world.GetComponent<TransformComponent>(a);
        auto* aCollider = world.GetComponent<BoxColliderComponent>(a);
        if (!aTransform || !aCollider) {
            continue;
        }

        for (std::size_t j = i + 1; j < entities.size(); ++j) {
            const Entity b = entities[j];
            auto* bTransform = world.GetComponent<TransformComponent>(b);
            auto* bCollider = world.GetComponent<BoxColliderComponent>(b);
            if (!bTransform || !bCollider) {
                continue;
            }

            if (!CheckCollisionBoxes(MakeBounds(*aTransform, *aCollider), MakeBounds(*bTransform, *bCollider))) {
                continue;
            }

            world.AddCollision(CollisionInfo{a, b});

            if (aCollider->isTrigger || bCollider->isTrigger) {
                continue;
            }

            RigidbodyComponent* aBody = world.GetComponent<RigidbodyComponent>(a);
            RigidbodyComponent* bBody = world.GetComponent<RigidbodyComponent>(b);

            if (aBody && !aCollider->isStatic && bCollider->isStatic) {
                const float staticTop = bTransform->position.y + bCollider->halfExtents.y;
                aTransform->position.y = staticTop + aCollider->halfExtents.y;
                aBody->velocity.y = 0.0f;
            } else if (bBody && !bCollider->isStatic && aCollider->isStatic) {
                const float staticTop = aTransform->position.y + aCollider->halfExtents.y;
                bTransform->position.y = staticTop + bCollider->halfExtents.y;
                bBody->velocity.y = 0.0f;
            }
        }
    }
}

} // namespace fw
