#pragma once

namespace fw {

class World;

class AnimationSystem {
public:
    void Update(World& world, float dt);
};

} // namespace fw
