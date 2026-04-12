#pragma once

#include <string>
#include <unordered_map>

namespace fw {

struct ReputationState {
    std::unordered_map<std::string, int> standingByFaction;

    int GetStanding(const std::string& factionId) const {
        auto it = standingByFaction.find(factionId);
        return it != standingByFaction.end() ? it->second : 0;
    }

    void ModifyStanding(const std::string& factionId, int delta) {
        standingByFaction[factionId] += delta;
    }
};

} // namespace fw
