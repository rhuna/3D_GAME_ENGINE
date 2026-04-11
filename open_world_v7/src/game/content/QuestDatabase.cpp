#include "game/content/QuestDatabase.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem; namespace fw {
bool QuestDatabase::LoadFromDirectory(const std::string& dir){ m.clear(); if(!fs::exists(dir)) return false; for(auto& e:fs::directory_iterator(dir)){ if(!e.is_regular_file()) continue; std::ifstream in(e.path()); QuestDefinition d{}; std::string line; while(std::getline(in,line)){ auto p=line.find('='); if(p==std::string::npos) continue; auto k=line.substr(0,p), val=line.substr(p+1); if(k=="id") d.id=val; else if(k=="title") d.title=val; else if(k=="description") d.description=val; else if(k=="giverNpcId") d.giverNpcId=val; else if(k=="targetItemId") d.targetItemId=val; else if(k=="targetRegionId") d.targetRegionId=val; else if(k=="targetEnemyType") d.targetEnemyType=val; else if(k=="targetCount") d.targetCount=std::stoi(val); else if(k=="rewardItemId") d.rewardItemId=val; else if(k=="rewardGold") d.rewardGold=std::stoi(val); } if(!d.id.empty()) m[d.id]=d; } return true; }
const QuestDefinition* QuestDatabase::Find(const std::string& id) const { auto it=m.find(id); return it==m.end()?nullptr:&it->second; } }
