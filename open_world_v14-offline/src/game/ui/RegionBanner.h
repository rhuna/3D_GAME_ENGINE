#pragma once

#include <string>

namespace fw {

class RegionBanner {
public:
    void Draw(const std::string& label, float alpha = 1.0f) const;
};

} // namespace fw
