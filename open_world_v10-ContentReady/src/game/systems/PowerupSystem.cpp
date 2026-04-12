#include "game/systems/PowerupSystem.h"

#include "core/Application.h"
#include "ecs/World.h"
#include "game/components/PlayerComponent.h"

namespace fw {

void PowerupSystem::Update(Application& app, World& world, float deltaTime) {
    (void)app;
    const Entity player = world.FindByTag("player");
    auto* playerComp = world.GetComponent<PlayerComponent>(player);
    if (!playerComp) {
        return;
    }

    if (playerComp->damageBoostRemaining > 0.0f) {
        playerComp->damageBoostRemaining -= deltaTime;
        if (playerComp->damageBoostRemaining <= 0.0f) {
            playerComp->damageBoostRemaining = 0.0f;
            playerComp->bonusProjectileDamage = 0;
            if (m_state) {
                m_state->playerDamageBoostRemaining = 0.0f;
                m_state->statusText = "Damage boost expired";
                m_state->statusTextTimer = 1.2f;
            }
        } else if (m_state) {
            m_state->playerDamageBoostRemaining = playerComp->damageBoostRemaining;
        }
    } else if (m_state) {
        m_state->playerDamageBoostRemaining = 0.0f;
    }
}

} // namespace fw
