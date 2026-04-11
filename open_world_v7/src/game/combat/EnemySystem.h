#pragma once
#include <string>
#include <vector>
#include "raylib.h"
namespace fw { struct EnemyInstance { Vector3 position{0,1,0}; std::string type="raider"; int health=40; bool alive=true; float attackCooldown=0.0f; float hurtFlash=0.0f; }; class EnemySystem { public: void Update(std::vector<EnemyInstance>& enemies, const Vector3& playerPos, int& playerHealth, float dt) const; }; }
