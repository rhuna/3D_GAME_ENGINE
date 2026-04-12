#pragma once

#include <optional>
#include <string>

namespace fw {

struct ShopRuntimeState {
    bool isOpen = false;
    std::optional<unsigned int> activeMerchantEntity;
    std::string activeMerchantId;
    std::string activeShopName;
    int selectedIndex = 0;
};

} // namespace fw
