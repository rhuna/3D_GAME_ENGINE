#include "game/render/WorldRenderer.h"

namespace fw
{
    void WorldRenderer::DrawRegion(const RegionLayout& layout, const std::vector<Vector3>& npcPositions) const
    {
        for (const auto& p : layout.props)
        {
            if (p.type == LayoutPrimitiveType::Box)
            {
                DrawCube(p.position, p.size.x, p.size.y, p.size.z, p.color);
                DrawCubeWires(p.position, p.size.x, p.size.y, p.size.z, BLACK);
            }
            else
            {
                DrawCylinder(p.position, p.size.x, p.size.x, p.size.y, 12, p.color);
                DrawCylinderWires(p.position, p.size.x, p.size.x, p.size.y, 12, BLACK);
            }
        }

        for (const auto& pos : layout.gatherPoints)
        {
            DrawSphere(pos, 0.5f, GOLD);
            DrawSphereWires(pos, 0.5f, 8, 8, BROWN);
        }

        for (const auto& pos : layout.travelPoints)
        {
            DrawCube({pos.x, pos.y + 1.0f, pos.z}, 1.5f, 2.0f, 1.5f, SKYBLUE);
            DrawCubeWires({pos.x, pos.y + 1.0f, pos.z}, 1.5f, 2.0f, 1.5f, BLUE);
        }

        for (const auto& pos : npcPositions)
        {
            DrawCube({pos.x, pos.y + 1.0f, pos.z}, 1.0f, 2.0f, 1.0f, PURPLE);
            DrawCubeWires({pos.x, pos.y + 1.0f, pos.z}, 1.0f, 2.0f, 1.0f, BLACK);
        }
    }
}
