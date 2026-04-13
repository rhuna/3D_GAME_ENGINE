#include "scene/spawning/SceneSpawner.h"

#include "core/Logger.h"
#include "gameplay/prefabs/SpawnFactory.h"
#include "ecs/components/EditorMetadataComponent.h"

namespace fw {

bool SceneSpawner::Spawn(World& world, const PrefabLibrary& prefabs, const SceneDefinition& scene) {
    bool spawnedAny = false;

    for (const SceneSpawnEntry& entry : scene.entries) {
        PrefabDefinition instance;
        bool resolved = false;

        if (!entry.variantName.empty()) {
            resolved = prefabs.BuildPrefabFromVariant(entry.variantName, instance);
        } else if (!entry.prefabName.empty()) {
            if (const PrefabDefinition* prefab = prefabs.Find(entry.prefabName)) {
                instance = *prefab;
                resolved = true;
            }
        }

        if (!resolved) {
            Logger::Warn("SceneSpawner could not resolve entry in scene: " + scene.name);
            continue;
        }

        if (entry.hasPosition) {
            instance.transform.position = entry.transform.position;
        }
        if (entry.hasRotation) {
            instance.transform.rotationEuler = entry.transform.rotationEuler;
        }
        if (entry.hasScale) {
            instance.transform.scale = entry.transform.scale;
            if (instance.hasCollider) {
                instance.collider.halfExtents = Vector3{
                    instance.transform.scale.x * 0.5f,
                    instance.transform.scale.y * 0.5f,
                    instance.transform.scale.z * 0.5f
                };
            }
        }
        if (!entry.tagOverride.empty()) {
            instance.tag.value = entry.tagOverride;
        }
        if (entry.hasTint) {
            instance.render.tint = entry.tint;
        }

        const Entity entity = spawn::SpawnFromPrefab(world, instance);
        if (entity != 0) {
            if (EditorMetadataComponent* meta = world.GetComponent<EditorMetadataComponent>(entity)) {
                meta->sourcePrefab = entry.prefabName.empty() ? instance.name : entry.prefabName;
                meta->sourceVariant = entry.variantName;
            }
            spawnedAny = true;
        }
    }

    return spawnedAny;
}

} // namespace fw
