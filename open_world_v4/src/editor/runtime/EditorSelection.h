#pragma once

#include <vector>

#include "ecs/Entity.h"

namespace fw {
class World;

class EditorSelection {
public:
    void Clear();
    void Select(Entity entity);
    void SelectNext(const World& world);
    void SetHovered(Entity entity) { m_hovered = entity; }
    Entity Selected() const { return m_selected; }
    Entity Hovered() const { return m_hovered; }
    bool HasSelection(const World& world) const;
    bool HasHovered(const World& world) const;

private:
    Entity m_selected = 0;
    Entity m_hovered = 0;
};

} // namespace fw
