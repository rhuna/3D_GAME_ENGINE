#pragma once

#include "ecs/Entity.h"

namespace fw {
struct CollisionInfo {
    Entity a = 0;
    Entity b = 0;
};
}
