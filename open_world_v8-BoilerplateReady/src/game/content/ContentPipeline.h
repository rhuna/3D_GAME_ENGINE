#pragma once
#include "game/content/QuestDatabase.h"
#include "game/content/DialogueDatabase.h"
#include "game/content/ItemDatabase.h"
#include "game/content/NpcDatabase.h"
#include "game/content/EncounterDatabase.h"
#include "game/content/LootTableDatabase.h"
#include "game/content/RegionDatabase.h"
namespace fw { class ContentPipeline { public: bool LoadAll(const std::string& assetsRoot); QuestDatabase quests; DialogueDatabase dialogues; ItemDatabase items; NpcDatabase npcs; EncounterDatabase encounters; LootTableDatabase lootTables; RegionDatabase regions; }; }
