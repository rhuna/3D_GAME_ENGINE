#include "game/world/WorldChunkStreamingSystem.h"

#include <cmath>

namespace fw {

void WorldChunkStreamingSystem::SetPolicy(const WorldChunkStreamingPolicy& policy) {
    m_policy = policy;
}

void WorldChunkStreamingSystem::SetChunks(std::vector<WorldChunk> chunks) {
    m_chunks = std::move(chunks);
}

const WorldChunkStreamingPolicy& WorldChunkStreamingSystem::Policy() const {
    return m_policy;
}

const std::vector<WorldChunk>& WorldChunkStreamingSystem::Chunks() const {
    return m_chunks;
}

void WorldChunkStreamingSystem::UpdateForPlayerPosition(float x, float z) {
    for (auto& chunk : m_chunks) {
        const float dx = chunk.centerX - x;
        const float dz = chunk.centerZ - z;
        const float distance = std::sqrt(dx * dx + dz * dz);

        if (distance <= m_policy.loadDistance) {
            chunk.state = WorldChunkState::Loaded;
        } else if (distance <= m_policy.preloadDistance) {
            chunk.state = WorldChunkState::Preloading;
        } else if (distance >= m_policy.unloadDistance) {
            chunk.state = WorldChunkState::Unloaded;
        } else if (chunk.state == WorldChunkState::Loaded) {
            chunk.state = WorldChunkState::Suspended;
        }
    }
}

} // namespace fw
