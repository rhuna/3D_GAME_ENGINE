#pragma once

namespace fw {

struct ShopRuntimeState;
class MerchantInventory;

class ShopWindow {
public:
    void Draw(const ShopRuntimeState& runtimeState);
};

} // namespace fw
