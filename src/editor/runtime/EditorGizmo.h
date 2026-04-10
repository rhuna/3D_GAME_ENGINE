#pragma once

#include "ecs/Entity.h"

namespace fw {
class Application;
class World;
class EditorSelection;

enum class GizmoMode {
    Translate = 0,
    Rotate = 1,
    Scale = 2
};

class EditorGizmo {
public:
    void SetMode(GizmoMode mode) { m_mode = mode; }
    [[nodiscard]] GizmoMode Mode() const { return m_mode; }

    void Update(Application& app, World& world, EditorSelection& selection, float deltaTime);
    void Draw(const World& world, const EditorSelection& selection) const;

private:
    GizmoMode m_mode = GizmoMode::Translate;
};

} // namespace fw
