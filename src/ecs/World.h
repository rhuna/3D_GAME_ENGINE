#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "raylib.h"

namespace fw {

using Entity = std::uint32_t;

struct TagComponent {
    std::string value;
};

struct TransformComponent {
    Vector3 position {0.0f, 0.0f, 0.0f};
    Vector3 rotationEuler {0.0f, 0.0f, 0.0f};
    Vector3 scale {1.0f, 1.0f, 1.0f};
};

struct RenderComponent {
    Color tint {255, 255, 255, 255};
    bool visible = true;
    bool drawCube = true;
    bool drawSphere = false;
    float cubeSize = 1.0f;
    float sphereRadius = 0.5f;
    bool useModel = false;
    std::string modelPath;
};

struct RigidbodyComponent {
    Vector3 velocity {0.0f, 0.0f, 0.0f};
    Vector3 acceleration {0.0f, 0.0f, 0.0f};
    float drag = 0.0f;
    bool useGravity = false;
    bool kinematic = false;
};

struct BoxColliderComponent {
    Vector3 halfExtents {0.5f, 0.5f, 0.5f};
    bool isTrigger = false;
    bool isStatic = false;
};

struct LifetimeComponent {
    float secondsRemaining = 0.0f;
};

struct EntityRecord {
    Entity id = 0;
    bool active = true;

    std::optional<TagComponent> tag;
    std::optional<TransformComponent> transform;
    std::optional<RenderComponent> render;
    std::optional<RigidbodyComponent> rigidbody;
    std::optional<BoxColliderComponent> collider;
    std::optional<LifetimeComponent> lifetime;
};

struct CollisionInfo {
    Entity a = 0;
    Entity b = 0;
};

class World {
public:
    void Clear();
    Entity CreateEntity();
    bool DestroyEntity(Entity id);
    EntityRecord* FindEntity(Entity id);
    const EntityRecord* FindEntity(Entity id) const;
    EntityRecord* FindByTag(const std::string& tag);
    const EntityRecord* FindByTag(const std::string& tag) const;

    void ClearTransientState();
    void AddCollision(CollisionInfo info);

    const std::vector<EntityRecord>& Entities() const { return m_entities; }
    std::vector<EntityRecord>& Entities() { return m_entities; }
    const std::vector<CollisionInfo>& Collisions() const { return m_collisions; }

private:
    Entity m_nextEntity = 1;
    std::vector<EntityRecord> m_entities;
    std::vector<CollisionInfo> m_collisions;
};

} // namespace fw
