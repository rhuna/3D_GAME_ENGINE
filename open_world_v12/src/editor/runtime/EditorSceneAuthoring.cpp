#include <algorithm>
#include "editor/runtime/EditorSceneAuthoring.h"

#include <array>
#include <cmath>

#include "raylib.h"
#include "raymath.h"

#include "core/Application.h"
#include "core/Logger.h"
#include "ecs/World.h"
#include "ecs/components/EditorMetadataComponent.h"
#include "ecs/components/TransformComponent.h"
#include "editor/runtime/EditorPicking.h"
#include "editor/runtime/EditorSelection.h"
#include "editor/serialization/SceneExporter.h"
#include "gameplay/prefabs/PrefabLibrary.h"
#include "gameplay/prefabs/SpawnFactory.h"
#include "scene/data/SceneLibrary.h"

namespace fw {

void EditorSceneAuthoring::Update(Application& app, World& world, EditorSelection& selection, PrefabLibrary& prefabs, SceneLibrary& scenes) {
    static const std::array<const char*, 4> kVariants = {
        "crate_blue", "crate_red", "crate_tall", "crate_blue_large"
    };

    const bool mouseLookActive = app.IsMouseLookActive();
    const bool mouseOverEditorUi = false;

    if (!mouseLookActive && !mouseOverEditorUi) {
        selection.SetHovered(EditorPicking::PickEntity(world, app.GetCamera()));

        if (m_clickPlacementEnabled && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector3 placePosition{};
            if (TryBuildGroundPlacement(app, placePosition)) {
                SpawnSelectedPlacement(world, selection, prefabs, &placePosition);
            }
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            const Entity picked = selection.Hovered();
            if (picked != 0) selection.Select(picked);
            else selection.Clear();
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) && selection.HasSelection(world)) {
            if (TransformComponent* selectedTransform = world.GetComponent<TransformComponent>(selection.Selected())) {
                Camera3D& camera = app.GetCamera();
                const Vector3 offset = Vector3Subtract(camera.position, camera.target);
                camera.target = selectedTransform->position;
                camera.position = Vector3Add(camera.target, offset);
            }
        }
    } else {
        selection.SetHovered(0);
    }

    if (IsKeyPressed(KEY_LEFT_BRACKET) || IsKeyPressed(KEY_RIGHT_BRACKET)) {
        auto it = std::find(kVariants.begin(), kVariants.end(), m_currentVariantName);
        std::size_t index = (it == kVariants.end()) ? 0 : static_cast<std::size_t>(std::distance(kVariants.begin(), it));
        if (IsKeyPressed(KEY_RIGHT_BRACKET)) index = (index + 1) % kVariants.size();
        else index = (index + kVariants.size() - 1) % kVariants.size();
        m_currentVariantName = kVariants[index];
        m_placementMode = PlacementMode::Variant;
        Logger::Info("Editor variant changed to: " + m_currentVariantName);
    }

    if (IsKeyPressed(KEY_INSERT) || IsKeyPressed(KEY_N)) {
        SpawnSelectedPlacement(world, selection, prefabs);
    }

    if (IsKeyPressed(KEY_P)) {
        m_clickPlacementEnabled = !m_clickPlacementEnabled;
        Logger::Info(std::string("Click placement ") + (m_clickPlacementEnabled ? "enabled" : "disabled"));
    }

    const bool ctrlDown = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    if ((ctrlDown && IsKeyPressed(KEY_S)) || IsKeyPressed(KEY_F9)) {
        SaveQuickScene(world, prefabs, scenes);
    }
}

void EditorSceneAuthoring::SetCurrentVariantName(std::string variantName) {
    if (variantName.empty()) return;
    m_currentVariantName = std::move(variantName);
    m_placementMode = PlacementMode::Variant;
}

void EditorSceneAuthoring::SetCurrentPrefabName(std::string prefabName) {
    if (prefabName.empty()) return;
    m_currentPrefabName = std::move(prefabName);
    m_placementMode = PlacementMode::Prefab;
}

void EditorSceneAuthoring::SaveQuickScene(World& world, PrefabLibrary& prefabs, SceneLibrary& scenes) {
    SaveNamedScene(world, prefabs, scenes, "sandbox_edited", "assets/scenes/sandbox_edited.scene");
}

bool EditorSceneAuthoring::TryBuildGroundPlacement(Application& app, Vector3& outPosition) const {
    const Ray ray = GetMouseRay(GetMousePosition(), app.GetCamera());
    if (std::abs(ray.direction.y) < 0.0001f) {
        return false;
    }

    const float planeHeight = 0.0f;
    const float t = (planeHeight - ray.position.y) / ray.direction.y;
    if (t <= 0.0f) {
        return false;
    }

    outPosition = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
    return true;
}

void EditorSceneAuthoring::SpawnSelectedPlacement(World& world, EditorSelection& selection, PrefabLibrary& prefabs, const Vector3* desiredPosition) {
    Entity entity = 0;

    if (m_placementMode == PlacementMode::Prefab && !m_currentPrefabName.empty()) {
        entity = spawn::SpawnFromPrefab(world, prefabs, m_currentPrefabName);
    } else {
        entity = spawn::SpawnFromVariant(world, prefabs, m_currentVariantName);
    }

    if (entity == 0) {
        Logger::Warn("Failed to spawn editor placement.");
        return;
    }

    if (TransformComponent* transform = world.GetComponent<TransformComponent>(entity)) {
        if (desiredPosition) {
            transform->position = *desiredPosition;
        } else {
            transform->position = Vector3{0.0f, 2.0f, 0.0f};
            if (selection.HasSelection(world)) {
                if (const TransformComponent* selectedTransform = world.GetComponent<TransformComponent>(selection.Selected())) {
                    transform->position.x = selectedTransform->position.x + 1.5f;
                    transform->position.y = selectedTransform->position.y;
                    transform->position.z = selectedTransform->position.z;
                }
            }
        }
    }

    if (EditorMetadataComponent* meta = world.GetComponent<EditorMetadataComponent>(entity)) {
        if (m_placementMode == PlacementMode::Prefab) {
            meta->sourcePrefab = m_currentPrefabName;
            meta->sourceVariant.clear();
        } else {
            meta->sourceVariant = m_currentVariantName;
        }
    } else {
        EditorMetadataComponent metadata{};
        if (m_placementMode == PlacementMode::Prefab) metadata.sourcePrefab = m_currentPrefabName;
        else metadata.sourceVariant = m_currentVariantName;
        world.AddComponent<EditorMetadataComponent>(entity, metadata);
    }

    selection.Select(entity);
    Logger::Info(std::string("Spawned editor ") + (m_placementMode == PlacementMode::Prefab ? "prefab: " + m_currentPrefabName : "variant: " + m_currentVariantName));
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
