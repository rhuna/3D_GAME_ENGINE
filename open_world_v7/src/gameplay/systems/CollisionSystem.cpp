#include "gameplay/systems/CollisionSystem.h"

#include <algorithm>

namespace fw {

namespace {
BoundingBox MakeBounds(const EntityRecord& entity) {
    const Vector3 position = entity.transform ? entity.transform->position : Vector3{0.0f, 0.0f, 0.0f};
    const Vector3 half = entity.collider ? entity.collider->halfExtents : Vector3{0.5f, 0.5f, 0.5f};
    return BoundingBox{
        Vector3{position.x - half.x, position.y - half.y, position.z - half.z},
        Vector3{position.x + half.x, position.y + half.y, position.z + half.z}
    };
}
}

void CollisionSystem::FixedUpdate(World& world) const {
    world.ClearTransientState();
    auto& entities = world.Entities();

    for (std::size_t i = 0; i < entities.size(); ++i) {
        auto& a = entities[i];
        if (!a.active || !a.transform || !a.collider) {
            continue;
        }

        for (std::size_t j = i + 1; j < entities.size(); ++j) {
            auto& b = entities[j];
            if (!b.active || !b.transform || !b.collider) {
                continue;
            }

            if (!CheckCollisionBoxes(MakeBounds(a), MakeBounds(b))) {
                continue;
            }

            world.AddCollision(CollisionInfo{a.id, b.id});

            if (a.collider->isTrigger || b.collider->isTrigger) {
                continue;
            }

            EntityRecord* dynamicEntity = nullptr;
            const EntityRecord* staticEntity = nullptr;

            if (a.rigidbody && !a.collider->isStatic && b.collider->isStatic) {
                dynamicEntity = &a;
                staticEntity = &b;
            } else if (b.rigidbody && !b.collider->isStatic && a.collider->isStatic) {
                dynamicEntity = &b;
                staticEntity = &a;
            }

            if (dynamicEntity && staticEntity && dynamicEntity->transform && dynamicEntity->collider && dynamicEntity->rigidbody && staticEntity->transform && staticEntity->collider) {
                const float staticTop = staticEntity->transform->position.y + staticEntity->collider->halfExtents.y;
                dynamicEntity->transform->position.y = staticTop + dynamicEntity->collider->halfExtents.y;
                dynamicEntity->rigidbody->velocity.y = 0.0f;
            }
        }
    }

    entities.erase(std::remove_if(entities.begin(), entities.end(), [](const EntityRecord& e) {
        return !e.active;
    }), entities.end());
}

} // namespace fw
