#include "game/content/ContentPipeline.h"

namespace fw {

bool ContentPipeline::LoadAll(const std::string& root) {
    bool ok = true;
    ok &= quests.LoadFromDirectory(root + "/quests");
    ok &= dialogues.LoadFromDirectory(root + "/dialogue");
    ok &= items.LoadFromDirectory(root + "/items");
    ok &= npcs.LoadFromDirectory(root + "/npcs");
    ok &= encounters.LoadFromDirectory(root + "/encounters");
    ok &= lootTables.LoadFromDirectory(root + "/loot");

    // Permanent fix: regions now load recursively from the full assets root so
    // content-pack JSON regions and legacy .region files flow through one path.
    ok &= regions.LoadFromDirectory(root);
    return ok;
}

} // namespace fw
