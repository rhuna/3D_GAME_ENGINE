#include "game/world/RegionLayoutDatabase.h"

namespace fw
{
    static Color MakeColor(unsigned char r, unsigned char g, unsigned char b) { return Color{r,g,b,255}; }

    void RegionLayoutDatabase::LoadDefaults()
    {
        m_layouts.clear();

        RegionLayout village{};
        village.regionId = "village_region";
        village.playerSpawn = {0, 1, 0};
        village.props.push_back({LayoutPrimitiveType::Box, {0, 0.5f, 0}, {50, 1, 50}, MakeColor(95, 125, 85)});
        village.props.push_back({LayoutPrimitiveType::Box, {-8, 2, -6}, {6, 4, 6}, MakeColor(150, 110, 70)});
        village.props.push_back({LayoutPrimitiveType::Box, {10, 2, 4}, {5, 4, 5}, MakeColor(140, 100, 60)});
        village.props.push_back({LayoutPrimitiveType::Cylinder, {4, 2, -10}, {1.5f, 4, 1.5f}, MakeColor(90, 150, 80)});
        village.npcSpawns = {{2,1,-2}, {-5,1,3}};
        village.gatherPoints = {{8,1,8}};
        village.travelPoints = {{20,1,0}, {-20,1,0}};
        m_layouts[village.regionId] = village;

        RegionLayout forest{};
        forest.regionId = "forest_region";
        forest.playerSpawn = {0, 1, 0};
        forest.props.push_back({LayoutPrimitiveType::Box, {0, 0.5f, 0}, {70, 1, 70}, MakeColor(55, 105, 60)});
        forest.props.push_back({LayoutPrimitiveType::Cylinder, {-10, 2, -10}, {1.2f, 4, 1.2f}, MakeColor(80, 130, 70)});
        forest.props.push_back({LayoutPrimitiveType::Cylinder, {6, 2, -14}, {1.6f, 4, 1.6f}, MakeColor(75, 140, 75)});
        forest.props.push_back({LayoutPrimitiveType::Cylinder, {14, 2, 8}, {1.3f, 4, 1.3f}, MakeColor(70, 135, 70)});
        forest.props.push_back({LayoutPrimitiveType::Box, {-4, 0.6f, 12}, {4, 1.2f, 3}, MakeColor(120, 90, 60)});
        forest.npcSpawns = {{-6,1,4}};
        forest.gatherPoints = {{3,1,6}, {10,1,-2}, {-12,1,-4}};
        forest.travelPoints = {{-25,1,0}, {25,1,0}};
        m_layouts[forest.regionId] = forest;

        RegionLayout ruins{};
        ruins.regionId = "ruins_region";
        ruins.playerSpawn = {0, 1, 0};
        ruins.props.push_back({LayoutPrimitiveType::Box, {0, 0.5f, 0}, {60, 1, 60}, MakeColor(90, 90, 95)});
        ruins.props.push_back({LayoutPrimitiveType::Box, {-10, 2, 0}, {4, 4, 10}, MakeColor(115, 115, 125)});
        ruins.props.push_back({LayoutPrimitiveType::Box, {10, 2, -4}, {6, 4, 4}, MakeColor(130, 130, 140)});
        ruins.props.push_back({LayoutPrimitiveType::Box, {0, 1.5f, 10}, {8, 3, 2}, MakeColor(120, 120, 130)});
        ruins.npcSpawns = {};
        ruins.gatherPoints = {{2,1,-6}};
        ruins.travelPoints = {{0,1,-22}};
        m_layouts[ruins.regionId] = ruins;
    }

    const RegionLayout* RegionLayoutDatabase::Find(const std::string& regionId) const
    {
        auto it = m_layouts.find(regionId);
        return it == m_layouts.end() ? nullptr : &it->second;
    }
}
