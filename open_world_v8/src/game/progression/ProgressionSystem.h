#pragma once
namespace fw { struct ProgressionState { int level = 1; int xp = 0; int xpToNext = 50; }; class ProgressionSystem { public: void AddXp(ProgressionState& state, int amount) const; }; }
