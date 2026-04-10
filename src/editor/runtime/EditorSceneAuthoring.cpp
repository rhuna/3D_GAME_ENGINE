#include <algorithm>
#include "editor/runtime/EditorSceneAuthoring.h"

#include <array>

#include "core/Application.h"
#include "core/Logger.h"
#include "editor/runtime/EditorPicking.h"
#include "editor/serialization/SceneExporter.h"
#include "ecs/World.h"
#include "ecs/components/TransformComponent.h"
#include "editor/runtime/EditorSelection.h"
#include "gameplay/prefabs/PrefabLibrary.h"
#include "gameplay/prefabs/SpawnFactory.h"
#include "scene/data/SceneLibrary.h"

namespace fw {

void EditorSceneAuthoring::Update(Application& app, World& world, EditorSelection& selection, PrefabLibrary& prefabs, SceneLibrary& scenes) {
    static const std::array<const char*, 4> kVariants = {
        "crate_blue", "crate_red", "crate_tall", "crate_blue_large"
    };

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        const Entity picked = EditorPicking::PickEntity(world, app.GetCamera());
        if (picked != 0) selection.Select(picked);
    }

    if (IsKeyPressed(KEY_LEFT_BRACKET) || IsKeyPressed(KEY_RIGHT_BRACKET)) {
        auto it = std::find(kVariants.begin(), kVariants.end(), m_currentVariantName);
        std::size_t index = (it == kVariants.end()) ? 0 : static_cast<std::size_t>(std::distance(kVariants.begin(), it));
        if (IsKeyPressed(KEY_RIGHT_BRACKET)) index = (index + 1) % kVariants.size();
        else index = (index + kVariants.size() - 1) % kVariants.size();
        m_currentVariantName = kVariants[index];
        Logger::Info("Editor variant changed to: " + m_currentVariantName);
    }

    if (IsKeyPressed(KEY_INSERT) || IsKeyPressed(KEY_N)) {
        SpawnSelectedVariant(world, selection, prefabs);
    }

    const bool ctrlDown = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    if ((ctrlDown && IsKeyPressed(KEY_S)) || IsKeyPressed(KEY_F8)) {
        SaveNamedScene(world, prefabs, scenes, "sandbox_edited", "assets/scenes/sandbox_edited.scene");
    }
}

void EditorSceneAuthoring::SpawnSelectedVariant(World& world, EditorSelection& selection, PrefabLibrary& prefabs) {
    const Entity entity = spawn::SpawnFromVariant(world, prefabs, m_currentVariantName);
    if (entity == 0) {
        Logger::Warn("Failed to spawn editor variant: " + m_currentVariantName);
        return;
    }

    if (TransformComponent* transform = world.GetComponent<TransformComponent>(entity)) {
        transform->position = Vector3{0.0f, 2.0f, 0.0f};
        if (selection.HasSelection(world)) {
            if (const TransformComponent* selectedTransform = world.GetComponent<TransformComponent>(selection.Selected())) {
                transform->position.x = selectedTransform->position.x + 1.5f;
                transform->position.y = selectedTransform->position.y;
                transform->position.z = selectedTransform->position.z;
            }
        }
    }

    selection.Select(entity);
    Logger::Info("Spawned editor variant: " + m_currentVariantName);
}

bool EditorSceneAuthoring::SaveNamedScene(World& world, PrefabLibrary& prefabs, SceneLibrary& scenes, const std::string& sceneName, const std::string& path) {
    if (!SceneExporter::ExportWorldAsScene(world, prefabs, sceneName, path)) {
        Logger::Warn("Failed to save editor scene: " + path);
        return false;
    }

    scenes.LoadSceneFile(path);
    m_lastSavedScenePath = path;
    Logger::Info("Saved editor scene: " + path);
    return true;
}

} // namespace fw
