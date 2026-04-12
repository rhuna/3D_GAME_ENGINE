#pragma once

#include <string>

#include "ecs/World.h"
#include "gameplay/prefabs/PrefabDefinition.h"
#include "gameplay/prefabs/PrefabLibrary.h"
#include "raylib.h"

namespace fw::spawn {

inline Entity SpawnFromPrefab(World& world, const PrefabDefinition& prefab) {
    const Entity entity = world.CreateEntity();
    world.AddComponent<TagComponent>(entity, prefab.tag);
    world.AddComponent<TransformComponent>(entity, prefab.transform);
    world.AddComponent<RenderComponent>(entity, prefab.render);
    if (prefab.hasCollider) {
        world.AddComponent<BoxColliderComponent>(entity, prefab.collider);
    }
    if (prefab.hasRigidbody) {
        world.AddComponent<RigidbodyComponent>(entity, prefab.rigidbody);
    }
    if (prefab.hasLifetime) {
        world.AddComponent<LifetimeComponent>(entity, prefab.lifetime);
    }
    return entity;
}

inline Entity SpawnFromPrefab(World& world, const PrefabLibrary& library, const std::string& prefabName) {
    if (const PrefabDefinition* prefab = library.Find(prefabName)) {
        return SpawnFromPrefab(world, *prefab);
    }

    PrefabDefinition fallback{};
    fallback.name = prefabName;
    fallback.tag.value = prefabName;
    fallback.transform.position = Vector3{0.0f, 0.0f, 0.0f};
    //fallback.transform.rotation = Vector3{0.0f, 0.0f, 0.0f};
    //fallback.transform.scale = Vector3{1.0f, 1.0f, 1.0f};
    fallback.render.visible = true;
    fallback.render.drawCube = true;
    fallback.render.drawSphere = false;
    fallback.render.cubeSize = 1.0f;
    fallback.render.tint = WHITE;
    fallback.hasCollider = true;
    fallback.collider.halfExtents = Vector3{0.5f, 0.5f, 0.5f};
    fallback.collider.isTrigger = false;
    fallback.collider.isStatic = false;
    return SpawnFromPrefab(world, fallback);
}

inline Entity SpawnFromVariant(World& world, const PrefabLibrary& library, const std::string& variantName) {
    PrefabDefinition built{};
    if (library.BuildPrefabFromVariant(variantName, built)) {
        return SpawnFromPrefab(world, built);
    }
    return SpawnFromPrefab(world, library, variantName);
}

Entity SpawnStaticBox(World& world, const std::string& tag, const Vector3& position,
                      const Vector3& scale, Color tint);
Entity SpawnDynamicBox(World& world, const std::string& tag, const Vector3& position,
                       const Vector3& scale, Color tint, bool useGravity = true);
Entity SpawnPlayer(World& world, const Vector3& position);
Entity SpawnProjectile(World& world, const Vector3& position, const Vector3& velocity);

} // namespace fw::spawn
