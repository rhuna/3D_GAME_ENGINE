#pragma once

#include <string>

#include "ecs/components/BoxColliderComponent.h"
#include "ecs/components/LifetimeComponent.h"
#include "ecs/components/RenderComponent.h"
#include "ecs/components/RigidbodyComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"

namespace fw {

struct PrefabVariantDefinition {
    std::string name;
    std::string basePrefab;
    std::string baseVariant;

    bool overrideTag = false;
    TagComponent tag {};

    bool overridePosition = false;
    bool overrideRotation = false;
    bool overrideScale = false;
    TransformComponent transform {};

    bool overrideRender = false;
    RenderComponent render {};

    bool overrideCollider = false;
    BoxColliderComponent collider {};

    bool overrideRigidbody = false;
    RigidbodyComponent rigidbody {};

    bool overrideLifetime = false;
    LifetimeComponent lifetime {};
};

} // namespace fw
