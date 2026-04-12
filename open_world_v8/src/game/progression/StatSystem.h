#pragma once
#include "game/save/SaveGameProfile.h"
#include "game/content/ItemDatabase.h"
namespace fw { struct ComputedStats { int attack = 10; int defense = 0; }; class StatSystem { public: ComputedStats Compute(const SaveGameProfile& profile, const ItemDatabase& itemDb) const; }; }
