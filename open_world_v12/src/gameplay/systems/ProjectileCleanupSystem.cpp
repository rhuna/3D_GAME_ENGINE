#include "gameplay/systems/ProjectileCleanupSystem.h"

#include <vector>

#include "ecs/World.h"
#include "ecs/components/TagComponent.h"
#include "gameplay/GameplayTags.h"

namespace fw {

void ProjectileCleanupSystem::FixedUpdate(Application& app, World& world, float fixedDeltaTime) {
    (void)app;
    (void)fixedDeltaTime;

    std::vector<Entity> toDestroy;

    for (const auto& collision : world.Collisions()) {
        const auto* aTag = world.GetComponent<TagComponent>(collision.a);
        const auto* bTag = world.GetComponent<TagComponent>(collision.b);

        const bool aProjectile = aTag && aTag->value == tags::kProjectile;
        const bool bProjectile = bTag && bTag->value == tags::kProjectile;
        const bool aGround = aTag && aTag->value == tags::kGround;
        const bool bGround = bTag && bTag->value == tags::kGround;

        if (aProjectile && !bProjectile) {
            toDestroy.push_back(collision.a);
        }
        if (bProjectile && !aProjectile) {
            toDestroy.push_back(collision.b);
        }
        if (aProjectile && bGround) {
            toDestroy.push_back(collision.a);
        }
        if (bProjectile && aGround) {
            toDestroy.push_back(collision.b);
        }
    }

    for (const Entity entity : toDestroy) {
        world.DestroyEntity(entity);
    }
}

} // namespace fw
