#pragma once

#include <string>
#include <vector>

namespace fw {

struct DeprecatedPathEntry {
    std::string path;
    std::string reason;
    std::string replacement;
};

struct DeprecatedPathRegistry {
    std::vector<DeprecatedPathEntry> entries;
};

} // namespace fw
