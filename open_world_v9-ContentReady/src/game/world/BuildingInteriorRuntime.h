#pragma once

#include <string>
#include "raylib.h"

namespace fw {

struct BuildingInteriorLink {
    std::string exteriorRegionId;
    std::string exteriorSceneId;
    std::string exteriorAnchorId;
    std::string interiorRegionId;
    std::string interiorSceneId;
    std::string interiorAnchorId;
    std::string buildingId;
};

struct BuildingEnterTriggerComponent {
    std::string buildingId;
    std::string destinationSceneId;
    std::string destinationAnchorId;
    BoundingBox triggerBounds{};
};

struct BuildingExitTriggerComponent {
    std::string buildingId;
    std::string destinationSceneId;
    std::string destinationAnchorId;
    BoundingBox triggerBounds{};
};

struct InteriorSpawnAnchorComponent {
    std::string anchorId;
};

struct BuildingInteriorState {
    std::string activeBuildingId;
    std::string activeSceneId;
    std::string lastExteriorRegionId;
    std::string lastExteriorAnchorId;
    bool insideInterior = false;
};

} // namespace fw
