#pragma once

#include <string>

namespace fw {
class Application;
class ContentPipeline;
class RegionLayoutDatabase;
struct SaveGameProfile;

class RegionEcsLoader {
public:
    void Rebuild(Application& app,
                 const ContentPipeline& pipeline,
                 const RegionLayoutDatabase& layouts,
                 const SaveGameProfile& profile,
                 const std::string& regionId) const;
};

} // namespace fw
