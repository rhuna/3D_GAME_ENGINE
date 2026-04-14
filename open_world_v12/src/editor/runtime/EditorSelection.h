#pragma once

#include <vector>

#include "ecs/Entity.h"
#include "raylib.h"

namespace fw {
class World;

class EditorSelection {
public:
    void Clear();
    void Select(Entity entity);
    void ToggleSelection(Entity entity);
    void SetPrimary(Entity entity);
    void SelectNext(const World& world);
    void SetHovered(Entity entity) { m_hovered = entity; }

    Entity Selected() const { return m_selected; }
    Entity Hovered() const { return m_hovered; }
    const std::vector<Entity>& SelectedEntities() const { return m_selectedEntities; }
    bool IsSelected(Entity entity) const;

    bool HasSelection(const World& world) const;
    bool HasHovered(const World& world) const;
    void PruneDead(const World& world);

    void BeginBoxSelect(Vector2 start);
    void UpdateBoxSelect(Vector2 current);
    void EndBoxSelect(const World& world, const Camera3D& camera, bool additive);
    bool IsBoxSelecting() const { return m_boxSelecting; }
    Rectangle BoxSelectRect() const;

private:
    Entity m_selected = 0;
    Entity m_hovered = 0;
    std::vector<Entity> m_selectedEntities;

    bool m_boxSelecting = false;
    Vector2 m_boxStart {0.0f, 0.0f};
    Vector2 m_boxEnd {0.0f, 0.0f};
};

} // namespace fw
