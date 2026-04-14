#include "editor/runtime/KitPlacement.h"

#include <functional>
#include <vector>

#include "editor/runtime/EditorSelection.h"
#include "ecs/World.h"
#include "ecs/components/EditorMetadataComponent.h"
#include "ecs/components/TransformComponent.h"
#include "gameplay/prefabs/PrefabLibrary.h"
#include "gameplay/prefabs/SpawnFactory.h"
#include "scene/data/SceneDefinition.h"
#include "scene/data/SceneLibrary.h"
#include "raymath.h"

namespace fw {
namespace {
Entity SpawnEntry(World& world,
                  PrefabLibrary& prefabs,
                  const SceneSpawnEntry& entry,
                  const Vector3& rootPosition) {
    Entity entity = 0;
    if (!entry.prefabName.empty()) {
        entity = spawn::SpawnFromPrefab(world, prefabs, entry.prefabName);
    } else if (!entry.variantName.empty()) {
        entity = spawn::SpawnFromVariant(world, prefabs, entry.variantName);
    }
    if (entity == 0) return 0;

    if (TransformComponent* transform = world.GetComponent<TransformComponent>(entity)) {
        if (entry.hasPosition) transform->position = Vector3Add(rootPosition, entry.transform.position);
        else transform->position = rootPosition;
        if (entry.hasRotation) transform->rotationEuler = entry.transform.rotationEuler;
        if (entry.hasScale) transform->scale = entry.transform.scale;
        if (transform->position.y == 0.0f && transform->scale.y > 0.0f) {
            transform->position.y = transform->scale.y * 0.5f;
        }
    }

    if (RenderComponent* render = world.GetComponent<RenderComponent>(entity)) {
        if (entry.hasTint) render->tint = entry.tint;
    }

    EditorMetadataComponent meta{};
    meta.sourcePrefab = entry.prefabName;
    meta.sourceVariant = entry.variantName;
    if (const EditorMetadataComponent* existing = world.GetComponent<EditorMetadataComponent>(entity)) {
        meta.locked = existing->locked;
        meta.hiddenInEditor = existing->hiddenInEditor;
    }
    world.AddComponent<EditorMetadataComponent>(entity, meta);
    return entity;
}
}

int KitPlacement::PlaceKitAt(World& world,
                             EditorSelection& selection,
                             PrefabLibrary& prefabs,
                             const SceneLibrary& scenes,
                             const std::string& kitName,
                             const Vector3& rootPosition,
                             bool replaceSelection) {
    std::vector<Entity> placed;

    std::function<void(const std::string&, const Vector3&, int)> placeRecursive;
    placeRecursive = [&](const std::string& name, const Vector3& origin, int depth) {
        if (depth > 8) return;
        const SceneDefinition definition = scenes.BuildResolvedScene(name);
        if (definition.name.empty()) return;

        for (const SceneSpawnEntry& entry : definition.entries) {
            const Vector3 nestedOrigin = entry.hasPosition ? Vector3Add(origin, entry.transform.position) : origin;
            if (!entry.kitName.empty()) {
                placeRecursive(entry.kitName, nestedOrigin, depth + 1);
                continue;
            }
            const Entity entity = SpawnEntry(world, prefabs, entry, origin);
            if (entity != 0) placed.push_back(entity);
        }
    };

    placeRecursive(kitName, rootPosition, 0);

    if (placed.empty()) return 0;
    if (replaceSelection) selection.Clear();
    for (Entity entity : placed) selection.ToggleSelection(entity);
    if (!placed.empty()) selection.SetPrimary(placed.front());
    return static_cast<int>(placed.size());
}

} // namespace fw
