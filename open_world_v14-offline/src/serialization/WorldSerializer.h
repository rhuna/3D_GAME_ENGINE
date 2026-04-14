#pragma once

#include <string>

#include "ecs/World.h"

namespace fw {

class WorldSerializer {
public:
    static bool SaveToFile(const World& world, const std::string& path);
    static bool LoadFromFile(World& world, const std::string& path);
};

} // namespace fw
