#pragma once

#include <cstdint>
#include <vector>

#include "raylib.h"

namespace fw {

using Entity = std::uint32_t;

struct TransformComponent {
    Vector3 position {0.0f, 0.0f, 0.0f};
    Vector3 rotationEuler {0.0f, 0.0f, 0.0f};
    Vector3 scale {1.0f, 1.0f, 1.0f};
};

struct RenderComponent {
    Color tint {255, 255, 255, 255};
    bool drawCube = true;
    float cubeSize = 1.0f;
};

struct EntityRecord {
    Entity id = 0;
    TransformComponent transform {};
    RenderComponent render {};
    bool active = true;
};

class World {
public:
    void Clear();
    Entity CreateEntity();
    EntityRecord* FindEntity(Entity id);
    const std::vector<EntityRecord>& Entities() const { return m_entities; }
    std::vector<EntityRecord>& Entities() { return m_entities; }

private:
    Entity m_nextEntity = 1;
    std::vector<EntityRecord> m_entities;
};

} // namespace fw
