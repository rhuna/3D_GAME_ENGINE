#include "game/world/WorldStatePersistence.h"

#include <fstream>
#include <sstream>

namespace fw {

PersistentRegionState& WorldStatePersistence::GetOrCreate(const std::string& regionId) {
    return m_regions[regionId];
}

void WorldStatePersistence::ClearRegion(const std::string& regionId) {
    m_regions.erase(regionId);
}

void WorldStatePersistence::ResetAll() {
    m_regions.clear();
}

bool WorldStatePersistence::IsNodeHarvested(const std::string& regionId, const std::string& persistentId) const {
    const auto it = m_regions.find(regionId);
    if (it == m_regions.end()) {
        return false;
    }
    return it->second.harvestedNodeIds.find(persistentId) != it->second.harvestedNodeIds.end();
}

void WorldStatePersistence::MarkNodeHarvested(const std::string& regionId, const std::string& persistentId) {
    m_regions[regionId].harvestedNodeIds.insert(persistentId);
}

bool WorldStatePersistence::IsSavePointUsed(const std::string& regionId, const std::string& persistentId) const {
    const auto it = m_regions.find(regionId);
    if (it == m_regions.end()) {
        return false;
    }
    return it->second.usedSavePointIds.find(persistentId) != it->second.usedSavePointIds.end();
}

void WorldStatePersistence::MarkSavePointUsed(const std::string& regionId, const std::string& persistentId) {
    m_regions[regionId].usedSavePointIds.insert(persistentId);
}

bool WorldStatePersistence::SaveToFile(const std::string& path) const {
    std::ofstream out(path);
    if (!out.is_open()) {
        return false;
    }

    for (const auto& pair : m_regions) {
        const auto& regionId = pair.first;
        const auto& state = pair.second;

        out << "region=" << regionId << "\n";
        out << "initialized=" << (state.initialized ? 1 : 0) << "\n";
        out << "cleared=" << (state.encounterCleared ? 1 : 0) << "\n";

        for (const auto& enemy : state.enemies) {
            out << "enemy="
                << enemy.position.x << ','
                << enemy.position.y << ','
                << enemy.position.z << ','
                << enemy.type << ','
                << enemy.health << ','
                << (enemy.alive ? 1 : 0) << ','
                << enemy.attackCooldown << ','
                << enemy.hurtFlash << "\n";
        }

        for (const auto& gatherPoint : state.gatherPoints) {
            out << "gather="
                << gatherPoint.x << ','
                << gatherPoint.y << ','
                << gatherPoint.z << "\n";
        }

        for (const auto& lootDrop : state.lootDrops) {
            out << "loot="
                << lootDrop.position.x << ','
                << lootDrop.position.y << ','
                << lootDrop.position.z << ','
                << lootDrop.itemId << ','
                << lootDrop.gold << ','
                << (lootDrop.active ? 1 : 0) << "\n";
        }

        for (const auto& harvestedId : state.harvestedNodeIds) {
            out << "harvested=" << harvestedId << "\n";
        }

        for (const auto& savePointId : state.usedSavePointIds) {
            out << "savepoint=" << savePointId << "\n";
        }

        out << "endregion=1\n";
    }

    return true;
}

bool WorldStatePersistence::LoadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        return false;
    }

    m_regions.clear();

    std::string line;
    std::string currentRegion;

    while (std::getline(in, line)) {
        const auto pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }

        const std::string key = line.substr(0, pos);
        const std::string value = line.substr(pos + 1);

        if (key == "region") {
            currentRegion = value;
            m_regions[currentRegion] = PersistentRegionState{};
            continue;
        }

        if (currentRegion.empty()) {
            continue;
        }

        auto& state = m_regions[currentRegion];

        if (key == "initialized") {
            state.initialized = (value == "1");
        } else if (key == "cleared") {
            state.encounterCleared = (value == "1");
        } else if (key == "enemy") {
            std::stringstream ss(value);
            std::string token;
            EnemyInstance enemy{};

            if (std::getline(ss, token, ',')) enemy.position.x = std::stof(token);
            if (std::getline(ss, token, ',')) enemy.position.y = std::stof(token);
            if (std::getline(ss, token, ',')) enemy.position.z = std::stof(token);
            if (std::getline(ss, token, ',')) enemy.type = token;
            if (std::getline(ss, token, ',')) enemy.health = std::stoi(token);
            if (std::getline(ss, token, ',')) enemy.alive = (token == "1");
            if (std::getline(ss, token, ',')) enemy.attackCooldown = std::stof(token);
            if (std::getline(ss, token, ',')) enemy.hurtFlash = std::stof(token);

            state.enemies.push_back(enemy);
        } else if (key == "gather") {
            std::stringstream ss(value);
            std::string token;
            Vector3 gatherPoint{};

            if (std::getline(ss, token, ',')) gatherPoint.x = std::stof(token);
            if (std::getline(ss, token, ',')) gatherPoint.y = std::stof(token);
            if (std::getline(ss, token, ',')) gatherPoint.z = std::stof(token);

            state.gatherPoints.push_back(gatherPoint);
        } else if (key == "loot") {
            std::stringstream ss(value);
            std::string token;
            WorldLootDrop lootDrop{};

            if (std::getline(ss, token, ',')) lootDrop.position.x = std::stof(token);
            if (std::getline(ss, token, ',')) lootDrop.position.y = std::stof(token);
            if (std::getline(ss, token, ',')) lootDrop.position.z = std::stof(token);
            if (std::getline(ss, token, ',')) lootDrop.itemId = token;
            if (std::getline(ss, token, ',')) lootDrop.gold = std::stoi(token);
            if (std::getline(ss, token, ',')) lootDrop.active = (token == "1");

            state.lootDrops.push_back(lootDrop);
        } else if (key == "harvested") {
            state.harvestedNodeIds.insert(value);
        } else if (key == "savepoint") {
            state.usedSavePointIds.insert(value);
        } else if (key == "endregion") {
            currentRegion.clear();
        }
    }

    return true;
}

} // namespace fw
