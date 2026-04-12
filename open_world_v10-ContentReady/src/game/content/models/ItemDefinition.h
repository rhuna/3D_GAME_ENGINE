#pragma once
#include <string>
namespace fw { struct ItemDefinition { std::string id, displayName, type, description, equipSlot; int value = 0; int attack = 0; int defense = 0; int heal = 0; }; }
