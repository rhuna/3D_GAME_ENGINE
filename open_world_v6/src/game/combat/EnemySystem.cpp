#include "game/combat/EnemySystem.h"
#include "raymath.h"

namespace fw
{
    void EnemySystem::ResetFromSpawns(const std::vector<Vector3>& spawns)
    {
        (void)spawns;
    }

    void EnemySystem::Update(std::vector<EnemyInstance>& enemies, const Vector3& playerPos, int& playerHealth, float dt) const
    {
        for (auto& e : enemies)
        {
            if (!e.alive) continue;
            Vector3 toPlayer = Vector3Subtract(playerPos, e.position);
            float dist = Vector3Length(toPlayer);
            if (dist > 0.001f && dist < 18.0f)
            {
                Vector3 dir = Vector3Scale(Vector3Normalize(toPlayer), 3.0f * dt);
                e.position = Vector3Add(e.position, {dir.x, 0.0f, dir.z});
            }

            if (e.attackCooldown > 0.0f) e.attackCooldown -= dt;
            if (dist < 1.75f && e.attackCooldown <= 0.0f)
            {
                playerHealth -= 8;
                e.attackCooldown = 1.0f;
            }
        }
    }
}
