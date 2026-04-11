#pragma once

#include <string>
#include "raylib.h"

namespace fw {

struct TravelTriggerComponent {
    std::string destinationRegion;
    Vector3 destinationSpawn {0.0f, 1.5f, 0.0f};
    float activationRadius = 2.5f;
    bool requiresInteraction = true;
};

} // namespace fw
