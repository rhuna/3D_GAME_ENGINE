#pragma once

#include <string>

namespace fw {

struct AnimationClipState {
    std::string clipId;
    float timeSeconds = 0.0f;
    float playbackSpeed = 1.0f;
    bool looping = true;
};

} // namespace fw
