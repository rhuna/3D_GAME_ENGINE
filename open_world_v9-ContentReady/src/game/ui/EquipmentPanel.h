#pragma once
#include "game/save/SaveGameProfile.h"
namespace fw { struct ComputedStats; struct ProgressionState; void DrawEquipmentPanel(const SaveGameProfile& profile, const ComputedStats& stats, const ProgressionState& prog, bool visible); }
