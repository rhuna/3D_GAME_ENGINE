#include "game/gameplay/GatheringSystem.h"
#include "raymath.h"

namespace fw
{
    int GatheringSystem::GatherAt(std::vector<Vector3>& gatherPoints, const Vector3& playerPos, float maxDistance) const
    {
        for (int i = 0; i < static_cast<int>(gatherPoints.size()); ++i)
        {
            if (Vector3Distance(gatherPoints[i], playerPos) <= maxDistance)
            {
                gatherPoints.erase(gatherPoints.begin() + i);
                return 1;
            }
        }
        return 0;
    }
}
