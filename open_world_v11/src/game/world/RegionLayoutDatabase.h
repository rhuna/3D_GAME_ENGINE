#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "raylib.h"
namespace fw { enum class LayoutPrimitiveType { Box, Cylinder }; struct LayoutPrimitive { LayoutPrimitiveType type = LayoutPrimitiveType::Box; Vector3 position{0,0,0}; Vector3 size{1,1,1}; Color color{200,200,200,255}; }; struct EnemySpawn { Vector3 position{0,1,0}; std::string type; }; struct RegionLayout { std::string regionId; Vector3 playerSpawn{0,1,0}; std::vector<LayoutPrimitive> props; std::vector<Vector3> npcSpawns; std::vector<Vector3> gatherPoints; std::vector<Vector3> travelPoints; std::vector<EnemySpawn> enemySpawns; }; class RegionLayoutDatabase { public: void LoadDefaults(); const RegionLayout* Find(const std::string& regionId) const; private: std::unordered_map<std::string, RegionLayout> m_layouts; }; }
