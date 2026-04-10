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
    Entity Selected() const { return m_selected; }
    bool HasSelection(const World& world) const;

private:
    Entity m_selected = 0;
};

} // namespace fw
