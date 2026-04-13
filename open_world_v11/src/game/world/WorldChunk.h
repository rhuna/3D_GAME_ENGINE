#pragma once

#include <string>
#include <vector>

namespace fw {

enum class WorldChunkState {
    Unloaded,
    Preloading,
    Loaded,
    Suspended
};

struct WorldChunk {
    std::string id;
    std::string regionId;
    float centerX = 0.0f;
    float centerZ = 0.0f;
    float halfExtent = 0.0f;
    std::vector<std::string> sceneFiles;
    WorldChunkState state = WorldChunkState::Unloaded;
};

} // namespace fw
