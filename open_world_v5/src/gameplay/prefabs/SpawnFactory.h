#pragma once

#include <string>

#include "ecs/World.h"
#include "gameplay/prefabs/PrefabDefinition.h"
#include "gameplay/prefabs/PrefabLibrary.h"

namespace fw::spawn {

Entity SpawnFromPrefab(World& world, const PrefabDefinition& prefab);
Entity SpawnFromPrefab(World& world, const PrefabLibrary& library, const std::string& prefabName);
Entity SpawnFromVariant(World& world, const PrefabLibrary& library, const std::string& variantName);

Entity SpawnStaticBox(World& world, const std::string& tag, const Vector3& position,
                      const Vector3& scale, Color tint);
Entity SpawnDynamicBox(World& world, const std::string& tag, const Vector3& position,
                       const Vector3& scale, Color tint, bool useGravity = true);
Entity SpawnPlayer(World& world, const Vector3& position);
Entity SpawnProjectile(World& world, const Vector3& position, const Vector3& velocity);

} // namespace fw::spawn
