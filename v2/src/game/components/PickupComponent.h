#pragma once

namespace fw {

enum class PickupType {
    Health
};

struct PickupComponent {
    PickupType type = PickupType::Health;
    int value = 25;
    float bobTime = 0.0f;
    bool consumeOnTouch = true;
};

} // namespace fw
