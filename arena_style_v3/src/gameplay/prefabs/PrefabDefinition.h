#pragma once

#include <string>

#include "ecs/components/BoxColliderComponent.h"
#include "ecs/components/LifetimeComponent.h"
#include "ecs/components/RenderComponent.h"
#include "ecs/components/RigidbodyComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"

namespace fw {

struct PrefabDefinition {
    std::string name;
    TagComponent tag {};
    TransformComponent transform {};
    RenderComponent render {};

    bool hasCollider = false;
    BoxColliderComponent collider {};

    bool hasRigidbody = false;
    RigidbodyComponent rigidbody {};

    bool hasLifetime = false;
    LifetimeComponent lifetime {};
};

} // namespace fw
