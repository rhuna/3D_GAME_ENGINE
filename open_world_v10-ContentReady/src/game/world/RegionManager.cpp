#include "game/world/RegionManager.h"

namespace fw {

void RegionManager::Register(RegionDescriptor descriptor) {
    if (descriptor.regionId.empty()) {
        return;
    }
    if (m_currentRegionId.empty()) {
        m_currentRegionId = descriptor.regionId;
    }
    m_regions[descriptor.regionId] = std::move(descriptor);
}

void RegionManager::RegisterDefaults() {
    Register({"village", "village_region", "Rivermoor Village", {"forest"}});
    Register({"forest", "forest_region", "Pinewatch Forest", {"village", "ruins"}});
    Register({"ruins", "ruins_region", "Oldwatch Ruins", {"forest"}});
}

const RegionDescriptor* RegionManager::Find(const std::string& regionId) const {
    const auto it = m_regions.find(regionId);
    return it != m_regions.end() ? &it->second : nullptr;
}

const RegionDescriptor* RegionManager::Current() const {
    return Find(m_currentRegionId);
}

bool RegionManager::SetCurrent(const std::string& regionId) {
    if (!Find(regionId)) {
        return false;
    }
    m_currentRegionId = regionId;
    return true;
}

} // namespace fw
