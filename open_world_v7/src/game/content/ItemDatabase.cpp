#include "game/content/ItemDatabase.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem; namespace fw {
bool ItemDatabase::LoadFromDirectory(const std::string& dir){ m.clear(); if(!fs::exists(dir)) return false; for(auto& e:fs::directory_iterator(dir)){ if(!e.is_regular_file()) continue; std::ifstream in(e.path()); ItemDefinition d{}; std::string line; while(std::getline(in,line)){ auto p=line.find('='); if(p==std::string::npos) continue; auto k=line.substr(0,p), val=line.substr(p+1); if(k=="id") d.id=val; else if(k=="displayName") d.displayName=val; else if(k=="type") d.type=val; else if(k=="description") d.description=val; else if(k=="equipSlot") d.equipSlot=val; else if(k=="value") d.value=std::stoi(val); else if(k=="attack") d.attack=std::stoi(val); else if(k=="defense") d.defense=std::stoi(val); else if(k=="heal") d.heal=std::stoi(val); } if(!d.id.empty()) m[d.id]=d; } return true; }
const ItemDefinition* ItemDatabase::Find(const std::string& id) const { auto it=m.find(id); return it==m.end()?nullptr:&it->second; } }
