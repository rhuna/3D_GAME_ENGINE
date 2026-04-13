#pragma once

#include <string>

namespace fw {

struct WorldContentDelta;

class WorldContentDeltaSerializer {
public:
    static bool Save(const WorldContentDelta& delta, const std::string& path);
    static bool Load(const std::string& path, WorldContentDelta& outDelta);
};

} // namespace fw
