
#include "game/content/EncounterDatabase.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
namespace fw {
bool EncounterDatabase::LoadFromDirectory(const std::string& dir){ m.clear(); if(!fs::exists(dir)) return false; for(auto& e: fs::directory_iterator(dir)){ if(!e.is_regular_file()) continue; std::ifstream in(e.path()); EncounterDefinition d{}; std::string line; while(std::getline(in,line)){ auto p=line.find('='); if(p==std::string::npos) continue; auto k=line.substr(0,p); auto v=line.substr(p+1); if(k=="id") d.id=v; else if(k=="regionId") d.regionId=v; else if(k=="lootTableId") d.lootTableId=v;} if(!d.id.empty()) m[d.id]=d;} return true; }
}
