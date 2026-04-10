#pragma once

#include "assets/AssetManager.h"
#include "core/EngineConfig.h"
#include "core/Input.h"
#include "core/Time.h"
#include "ecs/World.h"
#include "raylib.h"
#include "render/DebugOverlay.h"
#include "render/Renderer.h"
#include "scene/SceneManager.h"

namespace fw {

class Application {
public:
    Application();
    ~Application();

    int Run();
    void RequestQuit() { m_isRunning = false; }
    void ReloadStartScene();

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
    [[nodiscard]] bool IsDebugOverlayEnabled() const { return m_showDebugOverlay; }

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
    Camera3D m_camera {};

    bool m_initialized = false;
    bool m_isRunning = true;
    bool m_showDebugOverlay = true;
};

} // namespace fw
