#pragma once

namespace fw {

class World;
struct ShopRuntimeState;

class MerchantSystem {
public:
    void Update(World& world, ShopRuntimeState& runtimeState, float dt);
};

} // namespace fw
