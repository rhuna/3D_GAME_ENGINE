#pragma once

#include "ecs/Entity.h"

namespace fw {
class Application;
class World;
class EditorSelection;

class InspectorPanel {
public:
    void Update(Application& app, World& world, EditorSelection& selection, float deltaTime);
    void Draw(const World& world, const EditorSelection& selection) const;
};

} // namespace fw
