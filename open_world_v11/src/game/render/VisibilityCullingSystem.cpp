#include "game/render/VisibilityCullingSystem.h"
#include "ecs/World.h"
#include "ecs/components/TransformComponent.h"
#include "game/components/VisibilityComponent.h"

namespace fw {

void VisibilityCullingSystem::Update(World& world) {
    const auto entities = world.QueryEntities<TransformComponent, VisibilityComponent>();
    for (const auto entity : entities) {
        auto* visibility = world.GetComponent<VisibilityComponent>(entity);
        if (!visibility) {
            continue;
        }

        if (visibility->forceVisible) {
            visibility->visible = true;
            continue;
        }

        // Starter hook:
        // A real implementation would evaluate camera frustum, occlusion state,
        // distance culling, and streaming visibility before toggling this field.
        visibility->visible = true;
    }
}

} // namespace fw
