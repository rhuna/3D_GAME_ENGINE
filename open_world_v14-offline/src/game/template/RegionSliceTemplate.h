#pragma once
#include <string>   // Used for ids and labels.
#include <vector>   // Used for lists of authored content ids.

namespace fw {

struct RegionSliceTemplate {
    std::string id;                        // Unique region-slice id.
    std::string regionId;                  // Region that owns this slice.
    std::vector<std::string> encounterIds; // Encounters authored for the slice.
    std::vector<std::string> merchantIds;  // Merchants that appear in the slice.
    std::vector<std::string> questIds;     // Quests that drive the slice.
};

} // namespace fw
