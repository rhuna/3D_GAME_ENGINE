#include "game/save/SaveGameProfile.h"

#include <fstream>
#include <sstream>

namespace fw {

bool SaveGameProfile::SaveToFile(const std::string& path) const {
    std::ofstream out(path);
    if (!out.is_open()) {
        return false;
    }

    out << "region=" << currentRegion << "\n";
    out << "health=" << playerHealth << "\n";
    out << "maxHealth=" << playerMaxHealth << "\n";
    out << "pos=" << playerPosition.x << "," << playerPosition.y << "," << playerPosition.z << "\n";

    out << "gold=" << inventory.gold << "\n";
    out << "equippedWeapon=" << inventory.equippedWeaponId << "\n";
    out << "equippedArmor=" << inventory.equippedArmorId << "\n";
    out << "inventoryCount=" << inventory.items.size() << "\n";
    for (const auto& item : inventory.items) {
        out << "item=" << item.itemId << "," << item.displayName << "," << item.quantity << "\n";
    }

    out << "questCount=" << quests.size() << "\n";
    for (const auto& quest : quests) {
        out << "quest=" << quest.questId << "," << quest.title << "," << quest.description << "," << static_cast<int>(quest.status) << "\n";
        for (const auto& objective : quest.objectives) {
            out << "objective=" << objective.description << "," << (objective.complete ? 1 : 0) << "\n";
        }
        out << "questEnd\n";
    }

    return true;
}

bool SaveGameProfile::LoadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        ResetToDefaults();
        return false;
    }

    ResetToDefaults();

    std::string line;
    QuestEntry* currentQuest = nullptr;
    while (std::getline(in, line)) {
        if (line.rfind("region=", 0) == 0) {
            currentRegion = line.substr(7);
        } else if (line.rfind("health=", 0) == 0) {
            playerHealth = std::stoi(line.substr(7));
        } else if (line.rfind("maxHealth=", 0) == 0) {
            playerMaxHealth = std::stoi(line.substr(10));
        } else if (line.rfind("pos=", 0) == 0) {
            std::stringstream ss(line.substr(4));
            std::string part;
            if (std::getline(ss, part, ',')) playerPosition.x = std::stof(part);
            if (std::getline(ss, part, ',')) playerPosition.y = std::stof(part);
            if (std::getline(ss, part, ',')) playerPosition.z = std::stof(part);
        } else if (line.rfind("gold=", 0) == 0) {
            inventory.gold = std::stoi(line.substr(5));
        } else if (line.rfind("equippedWeapon=", 0) == 0) {
            inventory.equippedWeaponId = line.substr(15);
        } else if (line.rfind("equippedArmor=", 0) == 0) {
            inventory.equippedArmorId = line.substr(14);
        } else if (line.rfind("item=", 0) == 0) {
            InventoryEntry entry {};
            std::stringstream ss(line.substr(5));
            std::string part;
            if (std::getline(ss, part, ',')) entry.itemId = part;
            if (std::getline(ss, part, ',')) entry.displayName = part;
            if (std::getline(ss, part, ',')) entry.quantity = std::stoi(part);
            inventory.items.push_back(entry);
        } else if (line.rfind("quest=", 0) == 0) {
            QuestEntry quest {};
            std::stringstream ss(line.substr(6));
            std::string part;
            if (std::getline(ss, part, ',')) quest.questId = part;
            if (std::getline(ss, part, ',')) quest.title = part;
            if (std::getline(ss, part, ',')) quest.description = part;
            if (std::getline(ss, part, ',')) quest.status = static_cast<QuestStatus>(std::stoi(part));
            quests.push_back(quest);
            currentQuest = &quests.back();
        } else if (line.rfind("objective=", 0) == 0 && currentQuest != nullptr) {
            QuestObjective objective {};
            std::stringstream ss(line.substr(10));
            std::string part;
            if (std::getline(ss, part, ',')) objective.description = part;
            if (std::getline(ss, part, ',')) objective.complete = (std::stoi(part) != 0);
            currentQuest->objectives.push_back(objective);
        } else if (line == "questEnd") {
            currentQuest = nullptr;
        }
    }

    return true;
}

void SaveGameProfile::ResetToDefaults() {
    currentRegion = "village";
    playerHealth = 100;
    playerMaxHealth = 100;
    playerPosition = {0.0f, 0.0f, 0.0f};
    inventory = InventoryComponent{};
    quests.clear();
}

} // namespace fw
