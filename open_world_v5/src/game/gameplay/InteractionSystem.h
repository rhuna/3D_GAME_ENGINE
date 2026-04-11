#pragma once
#include <string>
#include <vector>
#include "raylib.h"

namespace fw
{
    struct InteractionCandidate
    {
        std::string kind;
        std::string id;
        Vector3 position{0,0,0};
        float distance = 0.0f;
        std::string prompt;
    };

    class InteractionSystem
    {
    public:
        const InteractionCandidate* FindBest(const std::vector<InteractionCandidate>& candidates, float maxDistance) const;
    };
}
