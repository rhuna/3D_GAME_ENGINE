#pragma once

#include <string>

namespace fw {

struct MerchantComponent {
    std::string merchantId;
    std::string shopName;
    bool canBuyFromPlayer = true;
    bool canSellToPlayer = true;
};

} // namespace fw
