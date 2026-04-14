#pragma once

#include <string>

namespace fw {

struct ObjectiveMarkerComponent {
    std::string objectiveId;
    std::string label;
    bool visible = true;
};

} // namespace fw
