#pragma once

#include <string>
#include "game/factions/ReputationState.h"
#include "game/factions/FactionStandingDatabase.h"

namespace fw {

enum class FactionReaction {
    Hostile,
    Unfriendly,
    Neutral,
    Friendly,
    Allied
};

class WorldReactionService {
public:
    static FactionReaction Evaluate(
        const ReputationState& reputation,
        const FactionStandingDatabase& database,
        const std::string& factionId)
    {
        const int standing = reputation.GetStanding(factionId);
        const FactionReactionRule* rule = database.FindRule(factionId);
        if (!rule) return FactionReaction::Neutral;

        if (standing <= rule->hostileAtOrBelow) return FactionReaction::Hostile;
        if (standing <= rule->unfriendlyAtOrBelow) return FactionReaction::Unfriendly;
        if (standing >= rule->alliedAtOrAbove) return FactionReaction::Allied;
        if (standing >= rule->friendlyAtOrAbove) return FactionReaction::Friendly;
        return FactionReaction::Neutral;
    }
};

} // namespace fw
