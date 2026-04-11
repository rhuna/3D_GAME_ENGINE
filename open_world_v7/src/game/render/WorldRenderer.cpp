#include "game/render/WorldRenderer.h"
#include "game/combat/EnemySystem.h"
namespace fw {
void WorldRenderer::DrawRegion(const RegionLayout& layout, const std::vector<Vector3>& npcPositions, const std::vector<EnemyInstance>& enemies, const std::vector<Vector3>& lootDrops) const
{
    for(const auto& p: layout.props){
        if(p.type==LayoutPrimitiveType::Box){ DrawCube(p.position,p.size.x,p.size.y,p.size.z,p.color); DrawCubeWires(p.position,p.size.x,p.size.y,p.size.z,BLACK);}
        else { DrawCylinder(p.position,p.size.x,p.size.x,p.size.y,12,p.color); DrawCylinderWires(p.position,p.size.x,p.size.x,p.size.y,12,BLACK);}
    }
    for(const auto& pos: layout.gatherPoints){ DrawSphere(pos,0.5f,GOLD); DrawSphereWires(pos,0.5f,8,8,BROWN); }
    for(const auto& pos: layout.travelPoints){ DrawCube({pos.x,pos.y+1.0f,pos.z},1.5f,2.0f,1.5f,SKYBLUE); DrawCubeWires({pos.x,pos.y+1.0f,pos.z},1.5f,2.0f,1.5f,BLUE); }
    for(const auto& pos: npcPositions){ DrawCube({pos.x,pos.y+1.0f,pos.z},1.0f,2.0f,1.0f,PURPLE); DrawCubeWires({pos.x,pos.y+1.0f,pos.z},1.0f,2.0f,1.0f,BLACK); }
    for(const auto& e: enemies){ if(!e.alive) continue; Color c = e.hurtFlash > 0.0f ? ORANGE : (e.type=="brute" ? DARKBROWN : (e.type=="wolf" ? BROWN : MAROON)); DrawCube({e.position.x,e.position.y+0.9f,e.position.z},1.2f,1.8f,1.2f,c); DrawCubeWires({e.position.x,e.position.y+0.9f,e.position.z},1.2f,1.8f,1.2f,BLACK); }
    for(const auto& pos: lootDrops){ DrawCube({pos.x,pos.y+0.35f,pos.z},0.7f,0.7f,0.7f,YELLOW); DrawCubeWires({pos.x,pos.y+0.35f,pos.z},0.7f,0.7f,0.7f,BLACK); }
}
}
