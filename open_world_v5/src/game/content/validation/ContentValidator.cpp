
#include "game/content/validation/ContentValidator.h"
namespace fw {
std::vector<ContentValidationMessage> ContentValidator::Validate(const ContentPipeline& p) const
{
    std::vector<ContentValidationMessage> out;
    if (p.regions.GetAll().empty()) out.push_back({"error", "No region definitions loaded."});
    if (p.quests.GetAll().empty()) out.push_back({"warning", "No quest definitions loaded."});
    if (p.dialogues.GetAll().empty()) out.push_back({"warning", "No dialogue definitions loaded."});
    if (p.items.GetAll().empty()) out.push_back({"warning", "No item definitions loaded."});
    if (p.npcs.GetAll().empty()) out.push_back({"warning", "No NPC definitions loaded."});
    if (out.empty()) out.push_back({"info", "Content validation passed."});
    return out;
}
}
