#pragma once
#include <vector>
#include "raylib.h"
#include "game/combat/EnemySystem.h"
namespace fw { class CombatSystem { public: int PlayerAttack(std::vector<EnemyInstance>& enemies, const Vector3& playerPos, const Vector3& facing, float range, int damage, std::vector<std::string>& killedTypes) const; }; }
