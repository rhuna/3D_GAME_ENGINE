
#include "game/content/NpcDatabase.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
namespace fw {
bool NpcDatabase::LoadFromDirectory(const std::string& dir){ m.clear(); if(!fs::exists(dir)) return false; for(auto& e: fs::directory_iterator(dir)){ if(!e.is_regular_file()) continue; std::ifstream in(e.path()); NpcDefinition d{}; std::string line; while(std::getline(in,line)){ auto p=line.find('='); if(p==std::string::npos) continue; auto k=line.substr(0,p); auto v=line.substr(p+1); if(k=="id") d.id=v; else if(k=="displayName") d.displayName=v; else if(k=="regionId") d.regionId=v; else if(k=="dialogueId") d.dialogueId=v; else if(k=="questId") d.questId=v; else if(k=="factionId") d.factionId=v; else if(k=="routineId") d.routineId=v; else if(k=="homeRegionId") d.homeRegionId=v;} if(!d.id.empty()) m[d.id]=d;} return true; }
const NpcDefinition* NpcDatabase::Find(const std::string& id) const { auto it=m.find(id); return it==m.end()?nullptr:&it->second; }
}
