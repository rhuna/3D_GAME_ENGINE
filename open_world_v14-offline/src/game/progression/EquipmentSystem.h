#pragma once
#include <string>
#include "game/save/SaveGameProfile.h"
#include "game/content/ItemDatabase.h"
namespace fw { class EquipmentSystem { public: bool EquipFirstMatching(SaveGameProfile& profile, const ItemDatabase& itemDb, const std::string& slot) const; bool UseFirstConsumable(SaveGameProfile& profile, const ItemDatabase& itemDb) const; }; }
