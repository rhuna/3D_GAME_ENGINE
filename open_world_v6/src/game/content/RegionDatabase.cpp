
#include "game/content/RegionDatabase.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
namespace fw {
bool RegionDatabase::LoadFromDirectory(const std::string& dir){ m.clear(); if(!fs::exists(dir)) return false; for(auto& e: fs::directory_iterator(dir)){ if(!e.is_regular_file()) continue; std::ifstream in(e.path()); RegionDefinition d{}; std::string line; while(std::getline(in,line)){ auto p=line.find('='); if(p==std::string::npos) continue; auto k=line.substr(0,p); auto v=line.substr(p+1); if(k=="id") d.id=v; else if(k=="displayName") d.displayName=v; else if(k=="sceneFile") d.sceneFile=v; else if(k=="ambientMusicId") d.ambientMusicId=v; else if(k=="safeZone") d.safeZone=(v=="true");} if(!d.id.empty()) m[d.id]=d;} return true; }
const RegionDefinition* RegionDatabase::Find(const std::string& id) const { auto it=m.find(id); return it==m.end()?nullptr:&it->second; }
}
