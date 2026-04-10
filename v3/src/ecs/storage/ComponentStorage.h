#pragma once

#include <unordered_map>

#include "ecs/Entity.h"

namespace fw {

template <typename T>
class ComponentStorage {
public:
    bool Has(Entity entity) const {
        return m_components.find(entity) != m_components.end();
    }

    T& AddOrReplace(Entity entity, const T& value) {
        m_components[entity] = value;
        return m_components[entity];
    }

    void Remove(Entity entity) {
        m_components.erase(entity);
    }

    T* Get(Entity entity) {
        auto it = m_components.find(entity);
        return it != m_components.end() ? &it->second : nullptr;
    }

    const T* Get(Entity entity) const {
        auto it = m_components.find(entity);
        return it != m_components.end() ? &it->second : nullptr;
    }

    const std::unordered_map<Entity, T>& Raw() const { return m_components; }
    std::unordered_map<Entity, T>& Raw() { return m_components; }

    void Clear() {
        m_components.clear();
    }

private:
    std::unordered_map<Entity, T> m_components;
};

} // namespace fw
