#pragma once

#include <string>

#include "ecs/World.h"

namespace fw::spawn {

Entity SpawnStaticBox(World& world, const std::string& tag, const Vector3& position,
                      const Vector3& scale, Color tint);
Entity SpawnDynamicBox(World& world, const std::string& tag, const Vector3& position,
                       const Vector3& scale, Color tint, bool useGravity = true);
Entity SpawnPlayer(World& world, const Vector3& position);
Entity SpawnProjectile(World& world, const Vector3& position, const Vector3& velocity);

} // namespace fw::spawn
