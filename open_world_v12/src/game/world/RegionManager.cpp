#include "game/world/RegionManager.h"

#include <utility>

#include "core/Logger.h"

namespace fw {

void RegionManager::Register(RegionDescriptor descriptor) {
    if (descriptor.regionId.empty()) {
        Logger::Warn("REGION", "Attempted to register region with empty regionId.");
        return;
    }

    if (m_currentRegionId.empty()) {
        m_currentRegionId = descriptor.regionId;
        Logger::Info("REGION", "Initial current region set to: " + m_currentRegionId);
    }

    Logger::Info(
        "REGION",
        "Registering region id=" + descriptor.regionId +
        " name=" + descriptor.displayName
    );

    m_regions[descriptor.regionId] = std::move(descriptor);
}

void RegionManager::RegisterDefaults() {
    Logger::Warn("REGION", "RegisterDefaults is now fallback-only. Prefer InitializeFromDatabase().");
    Register({"village_region", "village_region", "Rivermoor Village", {"forest_region"}});
    Register({"forest_region", "forest_region", "Pinewatch Forest", {"village_region", "ruins_region"}});
    Register({"ruins_region", "ruins_region", "Oldwatch Ruins", {"forest_region"}});
}

void RegionManager::InitializeFromDatabase(const RegionDatabase& database) {
    m_regions.clear();
    m_currentRegionId.clear();

    for (const auto& pair : database.GetAll()) {
        const RegionDefinition& region = pair.second;
        RegionDescriptor descriptor;
        descriptor.regionId = region.id;
        descriptor.sceneName = region.sceneFile;
        descriptor.displayName = region.displayName.empty() ? region.id : region.displayName;
        for (const RegionLinkDefinition& link : region.links) {
            if (!link.targetRegion.empty()) {
                descriptor.neighbors.push_back(link.targetRegion);
            }
        }
        Register(std::move(descriptor));
    }

    if (m_regions.empty()) {
        RegisterDefaults();
    }
}

const RegionDescriptor* RegionManager::Find(const std::string& regionId) const {
    const auto it = m_regions.find(regionId);
    if (it == m_regions.end()) {
        Logger::Warn("REGION", "Region not found: " + regionId);
        return nullptr;
    }
    return &it->second;
}

const RegionDescriptor* RegionManager::Current() const {
    return Find(m_currentRegionId);
}

bool RegionManager::SetCurrent(const std::string& regionId) {
    if (!Find(regionId)) {
        Logger::Error("REGION", "Failed to set current region. Unknown region: " + regionId);
        return false;
    }

    Logger::Info("REGION", "Switching current region from " + m_currentRegionId + " to " + regionId);
    m_currentRegionId = regionId;
    return true;
}

} // namespace fw
