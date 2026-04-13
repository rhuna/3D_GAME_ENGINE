#include "game/animation/AnimationSystem.h"
#include "ecs/World.h"
#include "game/animation/AnimationClipState.h"

namespace fw {

void AnimationSystem::Update(World& world, float dt) {
    const auto entities = world.QueryEntities<AnimationClipState>();
    for (const auto entity : entities) {
        auto* clip = world.GetComponent<AnimationClipState>(entity);
        if (!clip) {
            continue;
        }

        clip->timeSeconds += dt * clip->playbackSpeed;

        if (!clip->looping && clip->timeSeconds < 0.0f) {
            clip->timeSeconds = 0.0f;
        }
    }
}

} // namespace fw
