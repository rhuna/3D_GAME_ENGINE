#include "game/systems/PatrolSystem.h"

#include "raylib.h"
#include "raymath.h"

#include "ecs/World.h"
#include "ecs/components/TransformComponent.h"
#include "game/components/PatrolComponent.h"

namespace fw {

void PatrolSystem::Update(Application&, World& world, float deltaTime) {
    for (Entity entity : world.Entities()) {
        auto* patrol = world.GetComponent<PatrolComponent>(entity);
        auto* transform = world.GetComponent<TransformComponent>(entity);
        if (!patrol || !transform || patrol->points.empty()) continue;
        const Vector3 target = patrol->points[patrol->currentIndex];
        const Vector3 delta = Vector3Subtract(target, transform->position);
        const float distance = Vector3Length(delta);
        if (distance < 0.5f) {
            patrol->currentIndex = (patrol->currentIndex + 1) % static_cast<int>(patrol->points.size());
            continue;
        }
        transform->position = Vector3Add(transform->position, Vector3Scale(Vector3Normalize(delta), patrol->moveSpeed * deltaTime));
    }
}

} // namespace fw
