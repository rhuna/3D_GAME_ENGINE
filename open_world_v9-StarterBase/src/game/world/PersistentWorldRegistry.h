#pragma once

#include <string>
#include <unordered_map>

namespace fw {

class PersistentWorldRegistry {
public:
    void Register(const std::string& persistentId, const std::string& category);
    bool Has(const std::string& persistentId) const;
    std::string CategoryOf(const std::string& persistentId) const;

private:
    std::unordered_map<std::string, std::string> m_entries;
};

} // namespace fw
