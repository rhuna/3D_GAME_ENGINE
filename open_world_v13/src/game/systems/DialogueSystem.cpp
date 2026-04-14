#include "game/systems/DialogueSystem.h"

#include "core/Application.h"
#include "core/Input.h"
#include "core/Hotkeys.h"
#include "raylib.h"

namespace fw {

void DialogueSystem::Update(Application& app, World&, float) {
    if (!m_state) return;
    if (!m_state->dialogue.active) return;
    if (hotkeys::DialogueClosePressed()) {
        m_state->dialogue.active = false;
    }
}

} // namespace fw
