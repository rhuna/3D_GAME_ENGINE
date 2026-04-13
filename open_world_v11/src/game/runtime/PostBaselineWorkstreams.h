#pragma once

#include <string>
#include <vector>

namespace fw {

struct WorkstreamEntry {
    std::string id;
    std::string objective;
};

struct PostBaselineWorkstreams {
    std::vector<WorkstreamEntry> entries;
};

} // namespace fw
