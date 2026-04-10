#include "editor/validation/ContentValidator.h"

#include <unordered_set>

#include "gameplay/prefabs/PrefabLibrary.h"
#include "scene/data/SceneLibrary.h"

namespace fw {

std::vector<ValidationMessage> ContentValidator::ValidateAll(const PrefabLibrary& prefabs, const SceneLibrary& scenes) {
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

    if (result.empty()) {
        result.push_back({"info", "Content validation passed with no issues."});
    }
    return result;
}

} // namespace fw
