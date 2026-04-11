#pragma once
#include <vector>
#include "raylib.h"
#include "game/world/RegionLayoutDatabase.h"
namespace fw { class WorldRenderer { public: void DrawRegion(const RegionLayout& layout, const std::vector<Vector3>& npcPositions, const std::vector<Vector3>& enemyPositions) const; }; }
