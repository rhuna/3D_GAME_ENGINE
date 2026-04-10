#include "ecs/World.h"

namespace fw {

void World::Clear() {
    m_entities.clear();
    m_collisions.clear();
    m_nextEntity = 1;
}

Entity World::CreateEntity() {
    EntityRecord record;
    record.id = m_nextEntity++;
    record.transform = TransformComponent{};
    record.render = RenderComponent{};
    m_entities.push_back(record);
    return record.id;
}

bool World::DestroyEntity(Entity id) {
    for (auto& entity : m_entities) {
        if (entity.id == id) {
            entity.active = false;
            return true;
        }
    }
    return false;
}

EntityRecord* World::FindEntity(Entity id) {
    for (auto& entity : m_entities) {
        if (entity.id == id) {
            return &entity;
        }
    }
    return nullptr;
}

const EntityRecord* World::FindEntity(Entity id) const {
    for (const auto& entity : m_entities) {
        if (entity.id == id) {
            return &entity;
        }
    }
    return nullptr;
}

EntityRecord* World::FindByTag(const std::string& tagValue) {
    for (auto& entity : m_entities) {
        if (entity.tag && entity.tag->value == tagValue) {
            return &entity;
        }
    }
    return nullptr;
}

const EntityRecord* World::FindByTag(const std::string& tagValue) const {
    for (const auto& entity : m_entities) {
        if (entity.tag && entity.tag->value == tagValue) {
            return &entity;
        }
    }
    return nullptr;
}

void World::ClearTransientState() {
    m_collisions.clear();
}

void World::AddCollision(CollisionInfo info) {
    m_collisions.push_back(info);
}

} // namespace fw
