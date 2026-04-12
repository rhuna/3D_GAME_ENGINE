#pragma once
#include <string>
#include <vector>
#include "raylib.h"
namespace fw { struct WorldLootDrop { Vector3 position{0,1,0}; std::string itemId; int gold = 0; bool active = true; }; class LootSystem { public: WorldLootDrop MakeDrop(const std::string& enemyType, const Vector3& pos) const; bool PickupNearest(std::vector<WorldLootDrop>& drops, const Vector3& playerPos, float maxDistance, std::vector<std::string>& outItems, int& outGold) const; }; }
