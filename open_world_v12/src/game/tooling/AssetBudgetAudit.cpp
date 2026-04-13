#include "game/tooling/AssetBudgetAudit.h"

namespace fw {

void AssetBudgetAudit::Add(const AssetBudgetAuditEntry& entry) {
    m_entries.push_back(entry);
}

const std::vector<AssetBudgetAuditEntry>& AssetBudgetAudit::Entries() const {
    return m_entries;
}

} // namespace fw
