#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace fw {

struct RegionDescriptor {
    std::string regionId;
    std::string sceneName;
    std::string displayName;
    std::vector<std::string> neighbors;
};

class RegionManager {
public:
    void Register(RegionDescriptor descriptor);
    void RegisterDefaults();

    const RegionDescriptor* Find(const std::string& regionId) const;
    const RegionDescriptor* Current() const;
    bool SetCurrent(const std::string& regionId);

    const std::unordered_map<std::string, RegionDescriptor>& Regions() const { return m_regions; }
    const std::string& CurrentRegionId() const { return m_currentRegionId; }

private:
    std::unordered_map<std::string, RegionDescriptor> m_regions;
    std::string m_currentRegionId;
};

} // namespace fw
