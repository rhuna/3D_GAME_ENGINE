#include "game/systems/DialogueSystem.h"

#include "core/Application.h"
#include "core/Input.h"
#include "raylib.h"

namespace fw {

void DialogueSystem::Update(Application& app, World&, float) {
    if (!m_state) return;
    if (!m_state->dialogue.active) return;
    if (app.GetInput().IsKeyPressed(KEY_Q)) {
        m_state->dialogue.active = false;
    }
}

} // namespace fw
