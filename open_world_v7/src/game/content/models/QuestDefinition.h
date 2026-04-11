#pragma once
#include <string>
namespace fw { struct QuestDefinition { std::string id, title, description, giverNpcId, targetItemId, targetRegionId, targetEnemyType; int targetCount = 1; std::string rewardItemId; int rewardGold = 0; }; }
