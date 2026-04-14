#pragma once

#include <string>
#include <vector>

#include "assets/AssetManager.h"
#include "core/EngineConfig.h"
#include "core/Input.h"
#include "core/Time.h"
#include "ecs/World.h"
#include "editor/runtime/EditorSelection.h"
#include "editor/runtime/EditorGizmo.h"
#include "editor/runtime/EditorSceneAuthoring.h"
#include "editor/runtime/InspectorPanel.h"
#include "editor/ui/VisualBuilderPanel.h"
#include "editor/validation/ContentValidator.h"
#include "gameplay/prefabs/PrefabLibrary.h"
#include "gameplay/registry/SystemRegistry.h"
#include "raylib.h"
#include "render/DebugOverlay.h"
#include "render/Renderer.h"
#include "scene/SceneManager.h"
#include "scene/data/SceneLibrary.h"

namespace fw {

class Application {
public:
    Application();
    ~Application();

    int Run();
    void RequestQuit() { m_isRunning = false; }
    void ReloadStartScene();
    void RunContentValidation();
    void ExportCurrentScene();

    [[nodiscard]] const EngineConfig& GetConfig() const { return m_config; }
    [[nodiscard]] Time& GetTime() { return m_time; }
    [[nodiscard]] const Time& GetTime() const { return m_time; }
    [[nodiscard]] Input& GetInput() { return m_input; }
    [[nodiscard]] Renderer& GetRenderer() { return m_renderer; }
    [[nodiscard]] const Renderer& GetRenderer() const { return m_renderer; }
    [[nodiscard]] World& GetWorld() { return m_world; }
    [[nodiscard]] const World& GetWorld() const { return m_world; }
    [[nodiscard]] AssetManager& GetAssets() { return m_assets; }
    [[nodiscard]] SceneManager& GetSceneManager() { return m_sceneManager; }
    [[nodiscard]] Camera3D& GetCamera() { return m_camera; }
    [[nodiscard]] const Camera3D& GetCamera() const { return m_camera; }
    [[nodiscard]] PrefabLibrary& GetPrefabLibrary() { return m_prefabs; }
    [[nodiscard]] const PrefabLibrary& GetPrefabLibrary() const { return m_prefabs; }
    [[nodiscard]] SceneLibrary& GetSceneLibrary() { return m_sceneLibrary; }
    [[nodiscard]] const SceneLibrary& GetSceneLibrary() const { return m_sceneLibrary; }
    [[nodiscard]] SystemRegistry& GetSystemRegistry() { return m_systemRegistry; }
    [[nodiscard]] const SystemRegistry& GetSystemRegistry() const { return m_systemRegistry; }
    [[nodiscard]] bool IsDebugOverlayEnabled() const { return m_showDebugOverlay; }
    [[nodiscard]] bool IsInspectorVisible() const { return m_showInspector; }
    [[nodiscard]] bool IsMouseLookActive() const { return m_mouseLookActive; }
    [[nodiscard]] const std::vector<ValidationMessage>& GetValidationMessages() const { return m_validationMessages; }
    [[nodiscard]] const std::string& GetLastExportPath() const { return m_lastExportPath; }
    [[nodiscard]] EditorSelection& GetEditorSelection() { return m_editorSelection; }
    [[nodiscard]] const EditorSelection& GetEditorSelection() const { return m_editorSelection; }
    [[nodiscard]] const EditorGizmo& GetEditorGizmo() const { return m_editorGizmo; }
    [[nodiscard]] const EditorSceneAuthoring& GetEditorSceneAuthoring() const { return m_editorAuthoring; }
    [[nodiscard]] const VisualBuilderPanel& GetVisualBuilderPanel() const { return m_visualBuilderPanel; }

private:
    void Initialize();
    void Shutdown();
    void UpdateCameraController(float deltaTime);

    EngineConfig m_config {};
    Time m_time {};
    Input m_input {};
    Renderer m_renderer {};
    DebugOverlay m_debugOverlay {};
    World m_world {};
    AssetManager m_assets {};
    SceneManager m_sceneManager {};
    PrefabLibrary m_prefabs {};
    SceneLibrary m_sceneLibrary {};
    SystemRegistry m_systemRegistry {};
    InspectorPanel m_inspectorPanel {};
    EditorSelection m_editorSelection {};
    EditorGizmo m_editorGizmo {};
    EditorSceneAuthoring m_editorAuthoring {};
    VisualBuilderPanel m_visualBuilderPanel {};
    std::vector<ValidationMessage> m_validationMessages {};
    std::string m_lastExportPath;
    Camera3D m_camera {};

    bool m_initialized = false;
    bool m_isRunning = true;
    bool m_showDebugOverlay = true;
    bool m_showInspector = true;
    bool m_mouseLookActive = false;
};

} // namespace fw
