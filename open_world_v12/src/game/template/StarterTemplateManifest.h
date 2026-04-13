#pragma once

#include <string>
#include <vector>

namespace fw {

struct StarterTemplateManifest {
    std::string starterRegionId = "starter_vale";
    std::string interiorSceneId = "starter_hut_interior";
    std::string merchantPrefabId = "starter_merchant";
    std::string gatherPrefabId = "starter_herb_node";
    std::string savePointPrefabId = "starter_save_point";
    std::vector<std::string> starterQuestIds{"welcome_to_vale"};
};

} // namespace fw
