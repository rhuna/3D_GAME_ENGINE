#include "gameplay/prefabs/SpawnFactory.h"

#include "ecs/components/EditorMetadataComponent.h"

namespace fw::spawn {

Entity SpawnFromPrefab(World& world, const PrefabDefinition& prefab) {
    const Entity id = world.CreateEntity();
    world.AddComponent<TagComponent>(id, prefab.tag);
    world.AddComponent<TransformComponent>(id, prefab.transform);
    world.AddComponent<RenderComponent>(id, prefab.render);

    if (prefab.hasCollider) {
        world.AddComponent<BoxColliderComponent>(id, prefab.collider);
    }
    if (prefab.hasRigidbody) {
        world.AddComponent<RigidbodyComponent>(id, prefab.rigidbody);
    }
    if (prefab.hasLifetime && prefab.lifetime.secondsRemaining > 0.0f) {
        world.AddComponent<LifetimeComponent>(id, prefab.lifetime);
    }

    EditorMetadataComponent meta;
    meta.sourcePrefab = prefab.name;
    world.AddComponent<EditorMetadataComponent>(id, meta);

    return id;
}

Entity SpawnFromPrefab(World& world, const PrefabLibrary& library, const std::string& prefabName) {
    const PrefabDefinition* prefab = library.Find(prefabName);
    if (!prefab) {
        return 0;
    }
    return SpawnFromPrefab(world, *prefab);
}

Entity SpawnFromVariant(World& world, const PrefabLibrary& library, const std::string& variantName) {
    PrefabDefinition prefab;
    if (!library.BuildPrefabFromVariant(variantName, prefab)) {
        return 0;
    }
    const Entity entity = SpawnFromPrefab(world, prefab);
    if (entity != 0) {
        if (EditorMetadataComponent* meta = world.GetComponent<EditorMetadataComponent>(entity)) {
            meta->sourceVariant = variantName;
        }
    }
    return entity;
}

Entity SpawnStaticBox(World& world, const std::string& tag, const Vector3& position,
                      const Vector3& scale, Color tint) {
    PrefabDefinition prefab;
    prefab.name = tag + "_static_box";
    prefab.tag.value = tag;
    prefab.transform.position = position;
    prefab.transform.scale = scale;
    prefab.render.tint = tint;
    prefab.render.drawCube = true;
    prefab.render.drawSphere = false;
    prefab.hasCollider = true;
    prefab.collider.halfExtents = Vector3{scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f};
    prefab.collider.isStatic = true;
    return SpawnFromPrefab(world, prefab);
}

Entity SpawnDynamicBox(World& world, const std::string& tag, const Vector3& position,
                       const Vector3& scale, Color tint, bool useGravity) {
    PrefabDefinition prefab;
    prefab.name = tag + "_dynamic_box";
    prefab.tag.value = tag;
    prefab.transform.position = position;
    prefab.transform.scale = scale;
    prefab.render.tint = tint;
    prefab.render.drawCube = true;
    prefab.render.drawSphere = false;
    prefab.hasCollider = true;
    prefab.collider.halfExtents = Vector3{scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f};
    prefab.hasRigidbody = true;
    prefab.rigidbody.drag = 1.5f;
    prefab.rigidbody.useGravity = useGravity;
    return SpawnFromPrefab(world, prefab);
}

Entity SpawnPlayer(World& world, const Vector3& position) {
    PrefabDefinition prefab;
    prefab.name = "player_runtime";
    prefab.tag.value = "player";
    prefab.transform.position = position;
    prefab.transform.scale = Vector3{0.8f, 1.8f, 0.8f};
    prefab.render.tint = Color{70, 170, 255, 255};
    prefab.hasCollider = true;
    prefab.collider.halfExtents = Vector3{0.4f, 0.9f, 0.4f};
    prefab.hasRigidbody = true;
    prefab.rigidbody.drag = 3.0f;
    prefab.rigidbody.useGravity = true;
    return SpawnFromPrefab(world, prefab);
}

Entity SpawnProjectile(World& world, const Vector3& position, const Vector3& velocity) {
    PrefabDefinition prefab;
    prefab.name = "projectile_runtime";
    prefab.tag.value = "projectile";
    prefab.transform.position = position;
    prefab.transform.scale = Vector3{0.35f, 0.35f, 0.35f};
    prefab.render.tint = Color{255, 220, 90, 255};
    prefab.render.drawCube = false;
    prefab.render.drawSphere = true;
    prefab.render.sphereRadius = 0.18f;
    prefab.hasCollider = true;
    prefab.collider.halfExtents = Vector3{0.18f, 0.18f, 0.18f};
    prefab.collider.isTrigger = true;
    prefab.hasRigidbody = true;
    prefab.rigidbody.velocity = velocity;
    prefab.rigidbody.drag = 0.1f;
    prefab.hasLifetime = true;
    prefab.lifetime.secondsRemaining = 4.0f;
    return SpawnFromPrefab(world, prefab);
}

} // namespace fw::spawn
