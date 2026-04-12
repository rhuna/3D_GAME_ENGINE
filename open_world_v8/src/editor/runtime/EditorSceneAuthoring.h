#pragma once

#include <string>

#include "editor/runtime/EditorGizmo.h"

namespace fw {
class Application;
class World;
class EditorSelection;
class PrefabLibrary;
class SceneLibrary;

class EditorSceneAuthoring {
public:
    void Update(Application& app, World& world, EditorSelection& selection, PrefabLibrary& prefabs, SceneLibrary& scenes);

    [[nodiscard]] const std::string& LastSavedScenePath() const { return m_lastSavedScenePath; }
    [[nodiscard]] const std::string& CurrentVariantName() const { return m_currentVariantName; }

private:
    void SpawnSelectedVariant(World& world, EditorSelection& selection, PrefabLibrary& prefabs);
    bool SaveNamedScene(World& world, PrefabLibrary& prefabs, SceneLibrary& scenes, const std::string& sceneName, const std::string& path);

    std::string m_currentVariantName = "crate_blue";
    std::string m_lastSavedScenePath;
};

} // namespace fw
