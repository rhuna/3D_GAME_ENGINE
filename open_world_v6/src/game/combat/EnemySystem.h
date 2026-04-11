#pragma once
#include <vector>
#include "raylib.h"

namespace fw
{
    struct EnemyInstance
    {
        Vector3 position{0,1,0};
        int health = 40;
        bool alive = true;
        float attackCooldown = 0.0f;
    };

    class EnemySystem
    {
    public:
        void ResetFromSpawns(const std::vector<Vector3>& spawns);
        void Update(std::vector<EnemyInstance>& enemies, const Vector3& playerPos, int& playerHealth, float dt) const;
    };
}
