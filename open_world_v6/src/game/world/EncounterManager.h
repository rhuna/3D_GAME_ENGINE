#pragma once

#include <string>
#include <unordered_map>

namespace fw {

struct EncounterRecord {
    int defeatedCount = 0;
    bool cleared = false;
};

class EncounterManager {
public:
    EncounterRecord& Get(const std::string& id) { return m_records[id]; }
    const std::unordered_map<std::string, EncounterRecord>& Records() const { return m_records; }

private:
    std::unordered_map<std::string, EncounterRecord> m_records;
};

} // namespace fw
