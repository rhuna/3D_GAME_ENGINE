#include "game/simulation/RoutineDatabase.h"
#include <filesystem>
#include <fstream>
#include <sstream>
namespace fs = std::filesystem;
namespace fw {
bool RoutineDatabase::LoadFromDirectory(const std::string& dir){ m_routines.clear(); if(!fs::exists(dir)) return false; for(const auto& entry: fs::directory_iterator(dir)){ if(!entry.is_regular_file()) continue; std::ifstream in(entry.path()); RoutineDefinition def{}; std::string line; while(std::getline(in,line)){ if(line.empty()||line[0]=='#') continue; auto pos=line.find('='); if(pos==std::string::npos) continue; std::string key=line.substr(0,pos), value=line.substr(pos+1); if(key=="id") def.id=value; else if(key=="step"){ std::stringstream ss(value); std::string tok; RoutineStep s; if(std::getline(ss,tok,',')) s.startHour=std::stof(tok); if(std::getline(ss,tok,',')) s.endHour=std::stof(tok); if(std::getline(ss,tok,',')) s.activity=tok; if(std::getline(ss,tok,',')) s.locationTag=tok; def.steps.push_back(s);} } if(!def.id.empty()) m_routines[def.id]=def; } return true; }
const RoutineDefinition* RoutineDatabase::Find(const std::string& id) const { auto it=m_routines.find(id); return it==m_routines.end()?nullptr:&it->second; }
}
