#pragma once

#include <string>
#include <vector>

namespace fw {

struct GameProjectDefinition {
    std::string id = "default_project";
    std::string displayName = "3D Game Project";
    std::string startScene = "sandbox";
    std::string startRegion = "starter_village";
    std::string starterTemplate = "open_world_starter";
    bool createStarterContentIfMissing = true;
    std::vector<std::string> enabledContentPacks;
    std::vector<std::string> startupPreloadAssets;
};

} // namespace fw
