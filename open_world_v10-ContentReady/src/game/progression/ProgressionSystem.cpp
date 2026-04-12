#include "game/progression/ProgressionSystem.h"
namespace fw { void ProgressionSystem::AddXp(ProgressionState& state, int amount) const { state.xp+=amount; while(state.xp>=state.xpToNext){ state.xp-=state.xpToNext; ++state.level; state.xpToNext+=25; } } }
