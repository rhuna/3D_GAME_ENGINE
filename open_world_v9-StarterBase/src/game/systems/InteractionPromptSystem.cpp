#include "game/systems/InteractionPromptSystem.h"

#include "core/Application.h"

namespace fw {

void InteractionPromptSystem::Update(Application&, World&, float) {
    if (!m_state) return;
    if (m_state->hoveredPrompt.empty()) {
        m_state->lastInteraction.clear();
    }
}

} // namespace fw
