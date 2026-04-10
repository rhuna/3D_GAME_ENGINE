#include "editor/serialization/SceneExporter.h"

#include <fstream>

#include "ecs/World.h"
#include "ecs/components/EditorMetadataComponent.h"
#include "ecs/components/RenderComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "gameplay/prefabs/PrefabLibrary.h"

namespace fw {

namespace {

std::string ResolvePrefabName(const World& world, const PrefabLibrary& prefabs, Entity entity) {
    if (const auto* meta = world.GetComponent<EditorMetadataComponent>(entity)) {
        if (!meta->sourceVariant.empty()) return {};
        if (!meta->sourcePrefab.empty()) return meta->sourcePrefab;
    }
    if (const auto* tag = world.GetComponent<TagComponent>(entity)) {
        if (prefabs.Find(tag->value)) return tag->value;
    }
    return {};
}

std::string ResolveVariantName(const World& world, Entity entity) {
    if (const auto* meta = world.GetComponent<EditorMetadataComponent>(entity)) {
        return meta->sourceVariant;
    }
    return {};
}

} // namespace

bool SceneExporter::ExportWorldAsScene(const World& world, const PrefabLibrary& prefabs, const std::string& sceneName, const std::string& outputPath) {
    std::ofstream file(outputPath);
    if (!file.is_open()) return false;

    file << "scene=" << sceneName << "
";

    for (Entity entity : world.Entities()) {
        const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        const RenderComponent* render = world.GetComponent<RenderComponent>(entity);
        if (!transform || !render) continue;

        const std::string variantName = ResolveVariantName(world, entity);
        const std::string prefabName = variantName.empty() ? ResolvePrefabName(world, prefabs, entity) : std::string{};

        file << "spawn=";
        if (!variantName.empty()) file << "variant=" << variantName << ';';
        else if (!prefabName.empty()) file << "prefab=" << prefabName << ';';
        else file << "prefab=dynamic_crate;";

        if (const auto* tag = world.GetComponent<TagComponent>(entity)) {
            file << "tag=" << tag->value << ';';
        }
        file << "position=" << transform->position.x << ',' << transform->position.y << ',' << transform->position.z << ';';
        file << "rotationEuler=" << transform->rotationEuler.x << ',' << transform->rotationEuler.y << ',' << transform->rotationEuler.z << ';';
        file << "scale=" << transform->scale.x << ',' << transform->scale.y << ',' << transform->scale.z << ';';
        file << "color=" << static_cast<int>(render->tint.r) << ',' << static_cast<int>(render->tint.g) << ','
             << static_cast<int>(render->tint.b) << ',' << static_cast<int>(render->tint.a) << "
";
    }

    return true;
}

} // namespace fw
