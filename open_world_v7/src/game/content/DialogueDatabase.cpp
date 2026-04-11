#include "game/content/DialogueDatabase.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem; namespace fw {
bool DialogueDatabase::LoadFromDirectory(const std::string& dir){ m.clear(); if(!fs::exists(dir)) return false; for(auto& e:fs::directory_iterator(dir)){ if(!e.is_regular_file()) continue; std::ifstream in(e.path()); DialogueDefinition d{}; std::string line; while(std::getline(in,line)){ auto p=line.find('='); if(p==std::string::npos) continue; auto k=line.substr(0,p), val=line.substr(p+1); if(k=="id") d.id=val; else if(k=="speakerName") d.speakerName=val; else if(k=="text") d.text=val; } if(!d.id.empty()) m[d.id]=d; } return true; }
const DialogueDefinition* DialogueDatabase::Find(const std::string& id) const { auto it=m.find(id); return it==m.end()?nullptr:&it->second; } }
