#pragma once

namespace fw {

struct RespawnComponent {
    float delaySeconds = 15.0f;
    float timer = 0.0f;
    bool pending = false;
};

} // namespace fw
