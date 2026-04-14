#pragma once

#include <string>

#include "raylib.h"
#include "ecs/Entity.h"

namespace fw {
class Application;
class World;
class EditorSelection;
class PrefabLibrary;
class SceneLibrary;

class EditorSceneAuthoring {
public:
    enum class PlacementMode {
        Variant,
        Prefab
    };

    void Update(Application& app, World& world, EditorSelection& selection, PrefabLibrary& prefabs, SceneLibrary& scenes);

    [[nodiscard]] const std::string& LastSavedScenePath() const { return m_lastSavedScenePath; }
    [[nodiscard]] const std::string& CurrentVariantName() const { return m_currentVariantName; }
    [[nodiscard]] const std::string& CurrentPrefabName() const { return m_currentPrefabName; }
    [[nodiscard]] PlacementMode CurrentPlacementMode() const { return m_placementMode; }
    [[nodiscard]] bool ClickPlacementEnabled() const { return m_clickPlacementEnabled; }

    void SetCurrentVariantName(std::string variantName);
    void SetCurrentPrefabName(std::string prefabName);
    void SetPlacementMode(PlacementMode mode) { m_placementMode = mode; }
    void SetClickPlacementEnabled(bool enabled) { m_clickPlacementEnabled = enabled; }
    void SaveQuickScene(World& world, PrefabLibrary& prefabs, SceneLibrary& scenes);

private:
    Entity SpawnSelectedVariant(World& world, PrefabLibrary& prefabs);
    bool TryBuildGroundPlacement(Application& app, Vector3& outPosition) const;
    void SpawnSelectedPlacement(World& world, EditorSelection& selection, PrefabLibrary& prefabs, const Vector3* desiredPosition = nullptr);
    bool SaveNamedScene(World& world, PrefabLibrary& prefabs, SceneLibrary& scenes, const std::string& sceneName, const std::string& path);

    std::string m_currentVariantName = "crate_blue";
    std::string m_currentPrefabName = "";
    std::string m_lastSavedScenePath;
    PlacementMode m_placementMode = PlacementMode::Variant;
    bool m_clickPlacementEnabled = false;
};

} // namespace fw
