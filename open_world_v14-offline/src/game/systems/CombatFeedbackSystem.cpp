#include "game/systems/CombatFeedbackSystem.h"

#include "core/Application.h"
#include "ecs/World.h"

namespace fw {

void CombatFeedbackSystem::Update(Application& app, World& world, float deltaTime) {
    (void)app;
    (void)world;
    if (!m_state) {
        return;
    }

    if (m_state->damageFlashTimer > 0.0f) {
        m_state->damageFlashTimer -= deltaTime;
        if (m_state->damageFlashTimer < 0.0f) m_state->damageFlashTimer = 0.0f;
    }
    if (m_state->healFlashTimer > 0.0f) {
        m_state->healFlashTimer -= deltaTime;
        if (m_state->healFlashTimer < 0.0f) m_state->healFlashTimer = 0.0f;
    }
    if (m_state->victoryBannerTimer > 0.0f) {
        m_state->victoryBannerTimer -= deltaTime;
        if (m_state->victoryBannerTimer < 0.0f) m_state->victoryBannerTimer = 0.0f;
    }
}

} // namespace fw
