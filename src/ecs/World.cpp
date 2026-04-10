#include "ecs/World.h"

namespace fw {

void World::Clear() {
    m_entities.clear();
    m_collisions.clear();
    m_nextEntity = 1;
    m_tags.Clear();
    m_transforms.Clear();
    m_renders.Clear();
    m_rigidbodies.Clear();
    m_colliders.Clear();
    m_lifetimes.Clear();
    m_editorMetadata.Clear();
}

Entity World::CreateEntity() {
    const Entity entity = m_nextEntity++;
    m_entities.push_back(entity);
    return entity;
}

bool World::IsAlive(Entity entity) const {
    return std::find(m_entities.begin(), m_entities.end(), entity) != m_entities.end();
}

bool World::DestroyEntity(Entity entity) {
    if (!IsAlive(entity)) {
        return false;
    }

    RemoveComponent<TagComponent>(entity);
    RemoveComponent<TransformComponent>(entity);
    RemoveComponent<RenderComponent>(entity);
    RemoveComponent<RigidbodyComponent>(entity);
    RemoveComponent<BoxColliderComponent>(entity);
    RemoveComponent<LifetimeComponent>(entity);
    RemoveComponent<EditorMetadataComponent>(entity);

    m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
    m_collisions.erase(std::remove_if(m_collisions.begin(), m_collisions.end(), [entity](const CollisionInfo& info) {
        return info.a == entity || info.b == entity;
    }), m_collisions.end());
    return true;
}

void World::CollectGarbage() {
    std::vector<Entity> toRemove;
    for (const Entity entity : m_entities) {
        const LifetimeComponent* life = GetComponent<LifetimeComponent>(entity);
        if (life && life->secondsRemaining <= 0.0f) {
            toRemove.push_back(entity);
        }
    }
    for (const Entity entity : toRemove) {
        DestroyEntity(entity);
    }
}

Entity World::FindByTag(const std::string& tagValue) const {
    for (const Entity entity : m_entities) {
        const TagComponent* tag = GetComponent<TagComponent>(entity);
        if (tag && tag->value == tagValue) {
            return entity;
        }
    }
    return 0;
}

void World::ClearTransientState() {
    m_collisions.clear();
}

void World::AddCollision(CollisionInfo info) {
    m_collisions.push_back(info);
}

} // namespace fw
