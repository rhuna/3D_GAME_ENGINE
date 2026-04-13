#include "editor/validation/ContentValidator.h"

#include "game/project/GameProjectDefinition.h"
#include "gameplay/prefabs/PrefabLibrary.h"
#include "scene/data/SceneDefinition.h"
#include "scene/data/SceneLibrary.h"

namespace fw {

std::vector<ValidationMessage> ContentValidator::ValidateAll(const PrefabLibrary& prefabs,
                                                             const SceneLibrary& scenes,
                                                             const GameProjectDefinition* project) {
    std::vector<ValidationMessage> result;

    for (const auto& [name, prefab] : prefabs.Prefabs()) {
        if (prefab.tag.value.empty()) {
            result.push_back({"warning", "Prefab missing tag: " + name});
        }
        if (prefab.render.useModel && prefab.render.modelPath.empty()) {
            result.push_back({"warning", "Prefab uses model without modelPath: " + name});
        }
    }

    for (const auto& [name, variant] : prefabs.Variants()) {
        if (variant.basePrefab.empty() && variant.baseVariant.empty()) {
            result.push_back({"error", "Variant missing basePrefab/baseVariant: " + name});
        }
        if (!variant.basePrefab.empty() && prefabs.Find(variant.basePrefab) == nullptr) {
            result.push_back({"error", "Variant base prefab not found: " + name + " -> " + variant.basePrefab});
        }
        if (!variant.baseVariant.empty() && prefabs.FindVariant(variant.baseVariant) == nullptr) {
            result.push_back({"error", "Variant base variant not found: " + name + " -> " + variant.baseVariant});
        }
    }

    for (const auto& [name, scene] : scenes.Scenes()) {
        for (const std::string& includeName : scene.includeScenes) {
            if (scenes.Find(includeName) == nullptr) {
                result.push_back({"error", "Scene include missing: " + name + " -> " + includeName});
            }
        }
        if (scene.autoSpawnPlayer && !scene.playerPrefab.empty() && prefabs.Find(scene.playerPrefab) == nullptr) {
            result.push_back({"warning", "Scene auto-spawns missing player prefab: " + name + " -> " + scene.playerPrefab});
        }
        for (std::size_t i = 0; i < scene.entries.size(); ++i) {
            const auto& entry = scene.entries[i];
            if (entry.prefabName.empty() && entry.variantName.empty()) {
                result.push_back({"error", "Scene entry missing prefab/variant: " + name + " index " + std::to_string(i)});
            }
            if (!entry.prefabName.empty() && prefabs.Find(entry.prefabName) == nullptr) {
                result.push_back({"error", "Scene prefab missing: " + name + " -> " + entry.prefabName});
            }
            if (!entry.variantName.empty() && prefabs.FindVariant(entry.variantName) == nullptr) {
                result.push_back({"error", "Scene variant missing: " + name + " -> " + entry.variantName});
            }
        }
    }

    if (project) {
        if (!project->startScene.empty() && scenes.Find(project->startScene) == nullptr) {
            result.push_back({"warning", "Project start scene not found in scene library: " + project->startScene});
        }
    }

    if (result.empty()) {
        result.push_back({"info", "Content validation passed with no issues."});
    }
    return result;
}

} // namespace fw
