#pragma once

#include <string>
#include <unordered_map>

#include "game/content/models/RegionDefinition.h"

namespace fw {

class RegionDatabase {
public:
    bool LoadFromDirectory(const std::string& dir);
    const auto& GetAll() const { return m; }
    const RegionDefinition* Find(const std::string& id) const;

private:
    std::unordered_map<std::string, RegionDefinition> m;
};

} // namespace fw
