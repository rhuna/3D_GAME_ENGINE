#pragma once

#include <string>
#include <unordered_map>
#include "game/factions/FactionReactionRule.h"

namespace fw {

class FactionStandingDatabase {
public:
    void SetRule(const FactionReactionRule& rule) {
        m_rules[rule.factionId] = rule;
    }

    const FactionReactionRule* FindRule(const std::string& factionId) const {
        auto it = m_rules.find(factionId);
        return it != m_rules.end() ? &it->second : nullptr;
    }

private:
    std::unordered_map<std::string, FactionReactionRule> m_rules;
};

} // namespace fw
