#include "editor/runtime/EditorSelection.h"

#include "ecs/World.h"

namespace fw {

void EditorSelection::Clear() { m_selected = 0; }
void EditorSelection::Select(Entity entity) { m_selected = entity; }

bool EditorSelection::HasSelection(const World& world) const {
    return m_selected != 0 && world.IsAlive(m_selected);
}

void EditorSelection::SelectNext(const World& world) {
    const auto& entities = world.Entities();
    if (entities.empty()) { m_selected = 0; return; }
    if (m_selected == 0) { m_selected = entities.front(); return; }
    for (std::size_t i = 0; i < entities.size(); ++i) {
        if (entities[i] == m_selected) {
            m_selected = entities[(i + 1) % entities.size()];
            return;
        }
    }
    m_selected = entities.front();
}

} // namespace fw
