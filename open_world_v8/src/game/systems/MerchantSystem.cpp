#include "game/systems/MerchantSystem.h"

namespace fw {

class World;
struct ShopRuntimeState;

void MerchantSystem::Update(World& world, ShopRuntimeState& runtimeState, float dt) {
    (void)world;
    (void)runtimeState;
    (void)dt;
    // v36 scaffold: merchant interaction and buy/sell flow hooks land here.
}

} // namespace fw
