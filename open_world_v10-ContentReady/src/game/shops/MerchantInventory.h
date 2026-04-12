#pragma once

#include <string>
#include <vector>

namespace fw {

struct MerchantStockEntry {
    std::string itemId;
    std::string displayName;
    int buyPrice = 0;
    int sellPrice = 0;
    int quantity = -1; // -1 means infinite stock
};

struct MerchantInventory {
    std::string merchantId;
    std::string shopName;
    std::vector<MerchantStockEntry> stock;
};

} // namespace fw
