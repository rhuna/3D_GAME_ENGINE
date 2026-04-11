#include "game/combat/CombatSystem.h"
#include "raymath.h"

namespace fw
{
    int CombatSystem::PlayerAttack(std::vector<EnemyInstance>& enemies, const Vector3& playerPos, const Vector3& facing, float range, int damage) const
    {
        int kills = 0;
        for (auto& e : enemies)
        {
            if (!e.alive) continue;
            Vector3 toEnemy = Vector3Subtract(e.position, playerPos);
            float dist = Vector3Length(toEnemy);
            if (dist > range || dist < 0.001f) continue;

            Vector3 dir = Vector3Normalize(toEnemy);
            float dot = Vector3DotProduct(dir, facing);
            if (dot < 0.35f) continue;

            e.health -= damage;
            if (e.health <= 0)
            {
                e.alive = false;
                ++kills;
            }
        }
        return kills;
    }
}
