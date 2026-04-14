#pragma once

namespace fw {

class Application;
class World;
struct BuildingInteriorState;

class BuildingInteriorSystem {
public:
    void Update(Application& app, World& world, BuildingInteriorState& state, float dt);
};

} // namespace fw
