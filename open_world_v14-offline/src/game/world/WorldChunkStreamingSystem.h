#pragma once

#include <vector>
#include "game/world/WorldChunk.h"
#include "game/world/WorldChunkStreamingPolicy.h"

namespace fw {

class WorldChunkStreamingSystem {
public:
    void SetPolicy(const WorldChunkStreamingPolicy& policy);
    void SetChunks(std::vector<WorldChunk> chunks);

    const WorldChunkStreamingPolicy& Policy() const;
    const std::vector<WorldChunk>& Chunks() const;

    void UpdateForPlayerPosition(float x, float z);

private:
    WorldChunkStreamingPolicy m_policy{};
    std::vector<WorldChunk> m_chunks;
};

} // namespace fw
