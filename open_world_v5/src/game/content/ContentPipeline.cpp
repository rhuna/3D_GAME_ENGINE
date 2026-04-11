
#include "game/content/ContentPipeline.h"
namespace fw {
bool ContentPipeline::LoadAll(const std::string& root)
{
    bool ok = true;
    ok &= quests.LoadFromDirectory(root + "/quests");
    ok &= dialogues.LoadFromDirectory(root + "/dialogue");
    ok &= items.LoadFromDirectory(root + "/items");
    ok &= npcs.LoadFromDirectory(root + "/npcs");
    ok &= encounters.LoadFromDirectory(root + "/encounters");
    ok &= lootTables.LoadFromDirectory(root + "/loot");
    ok &= regions.LoadFromDirectory(root + "/regions");
    return ok;
}
}
