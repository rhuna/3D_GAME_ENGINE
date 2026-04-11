#include "game/systems/PickupSystem.h"

#include "raylib.h"
#include "raymath.h"

#include "core/Application.h"
#include "ecs/World.h"
#include "ecs/components/TransformComponent.h"
#include "game/audio/AudioBus.h"
#include "game/components/HealthComponent.h"
#include "game/components/PickupComponent.h"
#include "game/components/PlayerComponent.h"
#include "game/state/ArenaGameState.h"

namespace fw {

void PickupSystem::Update(Application& app, World& world, float deltaTime) {
    (void)app;

    const Entity player = world.FindByTag("player");
    auto* playerTransform = world.GetComponent<TransformComponent>(player);
    auto* playerHealth = world.GetComponent<HealthComponent>(player);
    auto* playerComponent = world.GetComponent<PlayerComponent>(player);
    if (!playerTransform || !playerHealth || !playerComponent || playerHealth->current <= 0) {
        return;
    }

    Entity consumedPickup = 0;
    for (const Entity entity : world.Entities()) {
        auto* pickup = world.GetComponent<PickupComponent>(entity);
        auto* transform = world.GetComponent<TransformComponent>(entity);
        if (!pickup || !transform) {
            continue;
        }

        pickup->bobTime += deltaTime;
        const float bobOffset = sinf(pickup->bobTime * 3.0f) * 0.005f;
        transform->position.y += bobOffset;
        transform->rotationEuler.y += 55.0f * deltaTime;

        const float distance = Vector3Distance(transform->position, playerTransform->position);
        if (distance > 1.35f) {
            continue;
        }

        if (pickup->type == PickupType::Health) {
            const int before = playerHealth->current;
            playerHealth->current += pickup->value;
            if (playerHealth->current > playerHealth->maximum) {
                playerHealth->current = playerHealth->maximum;
            }
            const int gained = playerHealth->current - before;
            if (gained > 0 && m_state) {
                ++m_state->pickupsCollected;
                m_state->score += gained * 2;
                m_state->healFlashTimer = 0.25f;
                m_state->statusText = TextFormat("Picked up health orb (+%d)", gained);
                m_state->statusTextTimer = 1.8f;
                AudioBus::PlayPickup();
            }
        } else if (pickup->type == PickupType::DamageBoost) {
            playerComponent->bonusProjectileDamage = pickup->value;
            playerComponent->damageBoostRemaining = 8.0f;
            if (m_state) {
                ++m_state->pickupsCollected;
                m_state->score += 50;
                m_state->playerDamageBoostRemaining = playerComponent->damageBoostRemaining;
                m_state->statusText = TextFormat("Arcane surge active (+%d damage)", pickup->value);
                m_state->statusTextTimer = 2.0f;
                AudioBus::PlayPickup();
            }
        }

        if (pickup->consumeOnTouch) {
            consumedPickup = entity;
            break;
        }
    }

    if (consumedPickup != 0) {
        world.DestroyEntity(consumedPickup);
    }
}

} // namespace fw
