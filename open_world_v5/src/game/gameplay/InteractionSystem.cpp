#include "game/gameplay/InteractionSystem.h"

namespace fw
{
    const InteractionCandidate* InteractionSystem::FindBest(const std::vector<InteractionCandidate>& candidates, float maxDistance) const
    {
        const InteractionCandidate* best = nullptr;
        for (const auto& c : candidates)
        {
            if (c.distance > maxDistance) continue;
            if (!best || c.distance < best->distance) best = &c;
        }
        return best;
    }
}
