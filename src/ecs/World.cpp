#include "ecs/World.h"

namespace fw {

void World::Clear() {
    m_entities.clear();
    m_nextEntity = 1;
}

Entity World::CreateEntity() {
    EntityRecord record;
    record.id = m_nextEntity++;
    m_entities.push_back(record);
    return record.id;
}

EntityRecord* World::FindEntity(Entity id) {
    for (auto& entity : m_entities) {
        if (entity.id == id) {
            return &entity;
        }
    }
    return nullptr;
}

} // namespace fw
