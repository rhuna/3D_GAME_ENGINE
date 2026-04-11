#pragma once
#include <vector>
#include "raylib.h"

namespace fw
{
    class GatheringSystem
    {
    public:
        int GatherAt(std::vector<Vector3>& gatherPoints, const Vector3& playerPos, float maxDistance) const;
    };
}
