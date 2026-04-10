#pragma once

#include <algorithm>
#include <string>
#include <type_traits>
#include <vector>

#include "ecs/Entity.h"
#include "ecs/components/BoxColliderComponent.h"
#include "ecs/components/EditorMetadataComponent.h"
#include "ecs/components/LifetimeComponent.h"
#include "ecs/components/RenderComponent.h"
#include "ecs/components/RigidbodyComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "ecs/events/CollisionInfo.h"
#include "ecs/storage/ComponentStorage.h"
#include "game/components/EnemyComponent.h"
#include "game/components/HealthComponent.h"
#include "game/components/PlayerComponent.h"
#include "game/components/ProjectileComponent.h"
#include "game/components/SpawnerComponent.h"
#include "game/components/TeamComponent.h"

namespace fw {

class World {
public:
    void Clear();
    Entity CreateEntity();
    bool IsAlive(Entity entity) const;
    bool DestroyEntity(Entity entity);
    void CollectGarbage();

    const std::vector<Entity>& Entities() const { return m_entities; }

    Entity FindByTag(const std::string& tag) const;

    void ClearTransientState();
    void AddCollision(CollisionInfo info);
    const std::vector<CollisionInfo>& Collisions() const { return m_collisions; }

    template <typename T>
    T& AddComponent(Entity entity, const T& component);

    template <typename T>
    bool HasComponent(Entity entity) const;

    template <typename T>
    T* GetComponent(Entity entity);

    template <typename T>
    const T* GetComponent(Entity entity) const;

    template <typename T>
    void RemoveComponent(Entity entity);

private:
    template <typename T>
    ComponentStorage<T>& Storage();

    template <typename T>
    const ComponentStorage<T>& Storage() const;

    Entity m_nextEntity = 1;
    std::vector<Entity> m_entities;
    std::vector<CollisionInfo> m_collisions;

    ComponentStorage<TagComponent> m_tags;
    ComponentStorage<TransformComponent> m_transforms;
    ComponentStorage<RenderComponent> m_renders;
    ComponentStorage<RigidbodyComponent> m_rigidbodies;
    ComponentStorage<BoxColliderComponent> m_colliders;
    ComponentStorage<LifetimeComponent> m_lifetimes;
    ComponentStorage<EditorMetadataComponent> m_editorMetadata;

    ComponentStorage<PlayerComponent> m_players;
    ComponentStorage<HealthComponent> m_health;
    ComponentStorage<EnemyComponent> m_enemies;
    ComponentStorage<ProjectileComponent> m_projectiles;
    ComponentStorage<TeamComponent> m_teams;
    ComponentStorage<SpawnerComponent> m_spawners;
};

// ----- template implementation -----

template <typename T>
T& World::AddComponent(Entity entity, const T& component) {
    return Storage<T>().AddOrReplace(entity, component);
}

template <typename T>
bool World::HasComponent(Entity entity) const {
    return Storage<T>().Has(entity);
}

template <typename T>
T* World::GetComponent(Entity entity) {
    return Storage<T>().Get(entity);
}

template <typename T>
const T* World::GetComponent(Entity entity) const {
    return Storage<T>().Get(entity);
}

template <typename T>
void World::RemoveComponent(Entity entity) {
    Storage<T>().Remove(entity);
}

template <typename T>
ComponentStorage<T>& World::Storage() {
    if constexpr (std::is_same_v<T, TagComponent>) return m_tags;
    else if constexpr (std::is_same_v<T, TransformComponent>) return m_transforms;
    else if constexpr (std::is_same_v<T, RenderComponent>) return m_renders;
    else if constexpr (std::is_same_v<T, RigidbodyComponent>) return m_rigidbodies;
    else if constexpr (std::is_same_v<T, BoxColliderComponent>) return m_colliders;
    else if constexpr (std::is_same_v<T, LifetimeComponent>) return m_lifetimes;
    else if constexpr (std::is_same_v<T, EditorMetadataComponent>) return m_editorMetadata;
    else if constexpr (std::is_same_v<T, PlayerComponent>) return m_players;
    else if constexpr (std::is_same_v<T, HealthComponent>) return m_health;
    else if constexpr (std::is_same_v<T, EnemyComponent>) return m_enemies;
    else if constexpr (std::is_same_v<T, ProjectileComponent>) return m_projectiles;
    else if constexpr (std::is_same_v<T, TeamComponent>) return m_teams;
    else if constexpr (std::is_same_v<T, SpawnerComponent>) return m_spawners;
    else {
        static_assert(sizeof(T) == 0, "Unsupported component type");
    }
}

template <typename T>
const ComponentStorage<T>& World::Storage() const {
    if constexpr (std::is_same_v<T, TagComponent>) return m_tags;
    else if constexpr (std::is_same_v<T, TransformComponent>) return m_transforms;
    else if constexpr (std::is_same_v<T, RenderComponent>) return m_renders;
    else if constexpr (std::is_same_v<T, RigidbodyComponent>) return m_rigidbodies;
    else if constexpr (std::is_same_v<T, BoxColliderComponent>) return m_colliders;
    else if constexpr (std::is_same_v<T, LifetimeComponent>) return m_lifetimes;
    else if constexpr (std::is_same_v<T, EditorMetadataComponent>) return m_editorMetadata;
    else if constexpr (std::is_same_v<T, PlayerComponent>) return m_players;
    else if constexpr (std::is_same_v<T, HealthComponent>) return m_health;
    else if constexpr (std::is_same_v<T, EnemyComponent>) return m_enemies;
    else if constexpr (std::is_same_v<T, ProjectileComponent>) return m_projectiles;
    else if constexpr (std::is_same_v<T, TeamComponent>) return m_teams;
    else if constexpr (std::is_same_v<T, SpawnerComponent>) return m_spawners;
    else {
        static_assert(sizeof(T) == 0, "Unsupported component type");
    }
}

} // namespace fw
