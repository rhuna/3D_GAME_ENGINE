#include "game/content/NpcDatabase.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem; namespace fw {
bool NpcDatabase::LoadFromDirectory(const std::string& dir){ m.clear(); if(!fs::exists(dir)) return false; for(auto& e:fs::directory_iterator(dir)){ if(!e.is_regular_file()) continue; std::ifstream in(e.path()); NpcDefinition d{}; std::string line; while(std::getline(in,line)){ auto p=line.find('='); if(p==std::string::npos) continue; auto k=line.substr(0,p), val=line.substr(p+1); if(k=="id") d.id=val; else if(k=="displayName") d.displayName=val; else if(k=="regionId") d.regionId=val; else if(k=="dialogueId") d.dialogueId=val; else if(k=="questId") d.questId=val; else if(k=="factionId") d.factionId=val; else if(k=="routineId") d.routineId=val; else if(k=="homeRegionId") d.homeRegionId=val; } if(!d.id.empty()) m[d.id]=d; } return true; }
const NpcDefinition* NpcDatabase::Find(const std::string& id) const { auto it=m.find(id); return it==m.end()?nullptr:&it->second; } }
