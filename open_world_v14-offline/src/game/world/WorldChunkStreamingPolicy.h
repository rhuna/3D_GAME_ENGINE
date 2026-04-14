#pragma once

namespace fw {

struct WorldChunkStreamingPolicy {
    float preloadDistance = 80.0f;
    float loadDistance = 60.0f;
    float unloadDistance = 120.0f;
    bool keepNeighborChunksLoaded = true;
};

} // namespace fw
