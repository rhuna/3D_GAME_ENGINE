#pragma once

namespace fw {

class World;
struct CameraFrustum;

class VisibilityCullingSystem {
public:
    void Update(World& world);
};

} // namespace fw
