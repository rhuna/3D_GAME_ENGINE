#pragma once

#include <string>   // Used for slice and milestone text ids.
#include <vector>   // Used for the ordered milestone list.

namespace fw {      // Places the playable-slice roadmap types under the engine namespace.

struct SliceMilestone {
    std::string id;            // Stable milestone identifier.
    std::string description;   // Human-readable explanation of the milestone.
};

struct PlayableSliceRoadmap {
    std::string sliceId;                     // Names the slice this roadmap describes.
    std::vector<SliceMilestone> milestones;  // Ordered checklist of playable-slice milestones.
};

} // namespace fw  // End framework namespace.
