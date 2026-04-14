#include "editor/runtime/EditorSelection.h"

#include <algorithm>

#include "ecs/World.h"
#include "ecs/components/TransformComponent.h"

namespace fw {

namespace {
Rectangle NormalizeRect(Vector2 a, Vector2 b) {
    Rectangle rect {};
    rect.x = (a.x < b.x) ? a.x : b.x;
    rect.y = (a.y < b.y) ? a.y : b.y;
    rect.width = (a.x < b.x) ? (b.x - a.x) : (a.x - b.x);
    rect.height = (a.y < b.y) ? (b.y - a.y) : (a.y - b.y);
    return rect;
}
}

void EditorSelection::Clear() {
    m_selected = 0;
    m_hovered = 0;
    m_selectedEntities.clear();
    m_boxSelecting = false;
}

void EditorSelection::Select(Entity entity) {
    m_selected = entity;
    m_selectedEntities.clear();
    if (entity != 0) {
        m_selectedEntities.push_back(entity);
    }
}

void EditorSelection::ToggleSelection(Entity entity) {
    if (entity == 0) return;

    auto it = std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity);
    if (it != m_selectedEntities.end()) {
        m_selectedEntities.erase(it);
        if (m_selected == entity) {
            m_selected = m_selectedEntities.empty() ? 0 : m_selectedEntities.front();
        }
        return;
    }

    m_selectedEntities.push_back(entity);
    m_selected = entity;
}

void EditorSelection::SetPrimary(Entity entity) {
    if (entity == 0 || !IsSelected(entity)) return;
    m_selected = entity;
}

bool EditorSelection::IsSelected(Entity entity) const {
    return std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity) != m_selectedEntities.end();
}

bool EditorSelection::HasSelection(const World& world) const {
    return m_selected != 0 && world.IsAlive(m_selected);
}

bool EditorSelection::HasHovered(const World& world) const {
    return m_hovered != 0 && world.IsAlive(m_hovered);
}

void EditorSelection::PruneDead(const World& world) {
    m_selectedEntities.erase(
        std::remove_if(m_selectedEntities.begin(), m_selectedEntities.end(), [&](Entity entity) {
            return !world.IsAlive(entity);
        }),
        m_selectedEntities.end());

    if (!world.IsAlive(m_selected) || !IsSelected(m_selected)) {
        m_selected = m_selectedEntities.empty() ? 0 : m_selectedEntities.front();
    }
    if (!world.IsAlive(m_hovered)) {
        m_hovered = 0;
    }
}

void EditorSelection::SelectNext(const World& world) {
    const auto& entities = world.Entities();
    if (entities.empty()) {
        Clear();
        return;
    }
    if (m_selected == 0) {
        Select(entities.front());
        return;
    }
    for (std::size_t i = 0; i < entities.size(); ++i) {
        if (entities[i] == m_selected) {
            Select(entities[(i + 1) % entities.size()]);
            return;
        }
    }
    Select(entities.front());
}

void EditorSelection::BeginBoxSelect(Vector2 start) {
    m_boxSelecting = true;
    m_boxStart = start;
    m_boxEnd = start;
}

void EditorSelection::UpdateBoxSelect(Vector2 current) {
    if (!m_boxSelecting) return;
    m_boxEnd = current;
}

void EditorSelection::EndBoxSelect(const World& world, const Camera3D& camera, bool additive) {
    if (!m_boxSelecting) return;

    const Rectangle rect = NormalizeRect(m_boxStart, m_boxEnd);
    m_boxSelecting = false;

    if (!additive) {
        m_selectedEntities.clear();
        m_selected = 0;
    }

    if (rect.width < 4.0f || rect.height < 4.0f) {
        return;
    }

    for (Entity entity : world.Entities()) {
        const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        if (!transform) continue;

        const Vector2 screen = GetWorldToScreen(transform->position, camera);
        if (CheckCollisionPointRec(screen, rect) && !IsSelected(entity)) {
            m_selectedEntities.push_back(entity);
            if (m_selected == 0) m_selected = entity;
        }
    }
}

Rectangle EditorSelection::BoxSelectRect() const {
    return NormalizeRect(m_boxStart, m_boxEnd);
}

} // namespace fw
