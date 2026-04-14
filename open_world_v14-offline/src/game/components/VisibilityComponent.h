#pragma once

namespace fw {

struct VisibilityComponent {
    float boundingRadius = 1.0f;
    bool visible = true;
    bool forceVisible = false;
    bool allowDistanceCull = true;
};

} // namespace fw
