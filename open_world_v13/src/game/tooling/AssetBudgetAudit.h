#pragma once

#include <string>
#include <vector>

namespace fw {

struct AssetBudgetAuditEntry {
    std::string assetId;
    std::string budgetTier;
    int textureResolution = 0;
    int materialSlots = 0;
};

class AssetBudgetAudit {
public:
    void Add(const AssetBudgetAuditEntry& entry);
    const std::vector<AssetBudgetAuditEntry>& Entries() const;

private:
    std::vector<AssetBudgetAuditEntry> m_entries;
};

} // namespace fw
