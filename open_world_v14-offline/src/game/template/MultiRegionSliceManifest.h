#pragma once
#include <string>   // Used for ids.
#include <vector>   // Used for groups of ids.

namespace fw {      // Framework namespace for reusable template types.

struct MultiRegionSliceManifest {
    std::string id;                    // Unique id for the multi-region slice.
    std::vector<std::string> regionIds; // Regions included in the slice.
    std::vector<std::string> questIds;  // Quests that define the slice progression.
    std::vector<std::string> linkIds;   // Travel or region-link definitions used by the slice.
};

} // namespace fw  // End of namespace.
