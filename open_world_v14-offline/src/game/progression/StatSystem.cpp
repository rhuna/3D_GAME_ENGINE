#include "game/progression/StatSystem.h"
namespace fw { ComputedStats StatSystem::Compute(const SaveGameProfile& profile, const ItemDatabase& itemDb) const { ComputedStats out; if(const auto* w=itemDb.Find(profile.inventory.equippedWeaponId)) out.attack+=w->attack; if(const auto* a=itemDb.Find(profile.inventory.equippedArmorId)) out.defense+=a->defense; return out; } }
