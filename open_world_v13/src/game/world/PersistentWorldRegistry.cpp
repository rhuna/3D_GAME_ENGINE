#include "game/world/PersistentWorldRegistry.h"

namespace fw {

void PersistentWorldRegistry::Register(const std::string& persistentId, const std::string& category) {
    m_entries[persistentId] = category;
}

bool PersistentWorldRegistry::Has(const std::string& persistentId) const {
    return m_entries.find(persistentId) != m_entries.end();
}

std::string PersistentWorldRegistry::CategoryOf(const std::string& persistentId) const {
    const auto it = m_entries.find(persistentId);
    return it != m_entries.end() ? it->second : std::string{};
}

} // namespace fw
