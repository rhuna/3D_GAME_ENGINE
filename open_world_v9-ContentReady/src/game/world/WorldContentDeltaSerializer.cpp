#include "game/world/WorldContentDeltaSerializer.h"
#include "game/world/WorldContentDelta.h"

#include <fstream>

namespace fw {

bool WorldContentDeltaSerializer::Save(const WorldContentDelta& delta, const std::string& path) {
    std::ofstream out(path);
    if (!out) return false;

    for (const auto& id : delta.harvestedNodeIds) out << "HARVESTED " << id << "\n";
    for (const auto& id : delta.lootedContainerIds) out << "LOOTED " << id << "\n";
    for (const auto& id : delta.defeatedEnemyIds) out << "DEFEATED " << id << "\n";
    for (const auto& id : delta.activatedSavePointIds) out << "SAVEPOINT " << id << "\n";
    return true;
}

bool WorldContentDeltaSerializer::Load(const std::string& path, WorldContentDelta& outDelta) {
    std::ifstream in(path);
    if (!in) return false;

    outDelta = WorldContentDelta{};
    std::string type, id;
    while (in >> type >> id) {
        if (type == "HARVESTED") outDelta.harvestedNodeIds.insert(id);
        else if (type == "LOOTED") outDelta.lootedContainerIds.insert(id);
        else if (type == "DEFEATED") outDelta.defeatedEnemyIds.insert(id);
        else if (type == "SAVEPOINT") outDelta.activatedSavePointIds.insert(id);
    }
    return true;
}

} // namespace fw
