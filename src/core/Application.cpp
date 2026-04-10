#include "core/Application.h"

#include <cmath>
#include <memory>

#include "raylib.h"
#include "raymath.h"

#include "core/Logger.h"
#include "editor/serialization/SceneExporter.h"
#include "scene/scenes/SandboxScene.h"
#include "serialization/WorldSerializer.h"

namespace fw {

Application::Application() = default;
Application::~Application() { Shutdown(); }

int Application::Run() {
    Initialize();
    float accumulator = 0.0f;

    while (m_isRunning && !WindowShouldClose()) {
        m_time.BeginFrame();
        m_input.BeginFrame();

        if (m_input.IsKeyPressed(KEY_ESCAPE)) m_isRunning = false;
        if (m_input.IsKeyPressed(KEY_F1)) m_showDebugOverlay = !m_showDebugOverlay;
        if (m_input.IsKeyPressed(KEY_F2)) m_editorSelection.SelectNext(m_world);
        if (m_input.IsKeyPressed(KEY_F3)) ExportCurrentScene();
        if (m_input.IsKeyPressed(KEY_F4)) RunContentValidation();
        if (m_input.IsKeyPressed(KEY_F5)) ReloadStartScene();
        if (m_input.IsKeyPressed(KEY_F6)) WorldSerializer::SaveToFile(m_world, "assets/saves/sandbox_world.txt");
        if (m_input.IsKeyPressed(KEY_F7)) WorldSerializer::LoadFromFile(m_world, "assets/saves/sandbox_world.txt");
        if (m_input.IsKeyPressed(KEY_TAB)) m_showInspector = !m_showInspector;

        UpdateCameraController(m_time.DeltaTime());
        m_sceneManager.Update(*this, m_time.DeltaTime());
        m_editorAuthoring.Update(*this, m_world, m_editorSelection, m_prefabs, m_sceneLibrary);
        m_editorGizmo.Update(*this, m_world, m_editorSelection, m_time.DeltaTime());
        if (m_showInspector) m_inspectorPanel.Update(*this, m_world, m_editorSelection, m_time.DeltaTime());

        accumulator += m_time.DeltaTime();
        while (accumulator >= m_config.fixedTimestep) {
            m_sceneManager.FixedUpdate(*this, m_config.fixedTimestep);
            accumulator -= m_config.fixedTimestep;
        }

        m_renderer.BeginFrame();
        m_renderer.Begin3D(m_camera);
        m_sceneManager.Render(*this);
        m_renderer.End3D();

        if (m_showDebugOverlay) {
            m_debugOverlay.Draw(m_time, m_world, m_camera, m_sceneManager.CurrentSceneName(), m_editorSelection.Selected(), m_validationMessages, m_lastExportPath, m_showInspector);
        }
        m_editorGizmo.Draw(m_world, m_editorSelection);
        if (m_showInspector) {
            m_inspectorPanel.Draw(m_world, m_editorSelection);
        }

        m_renderer.EndFrame();
    }

    return 0;
}

void Application::ReloadStartScene() {
    m_sceneManager.ChangeScene(*this, std::make_unique<SandboxScene>());
    m_editorSelection.Clear();
    Logger::Info("Reloaded start scene.");
}

void Application::RunContentValidation() {
    m_validationMessages = ContentValidator::ValidateAll(m_prefabs, m_sceneLibrary);
    Logger::Info("Content validation ran. Message count: " + std::to_string(m_validationMessages.size()));
}

void Application::ExportCurrentScene() {
    m_lastExportPath = "assets/editor/exported_scenes/runtime_export.scene";
    if (SceneExporter::ExportWorldAsScene(m_world, m_prefabs, "runtime_export", m_lastExportPath)) {
        Logger::Info("Exported scene to: " + m_lastExportPath);
    } else {
        Logger::Warn("Failed to export scene to: " + m_lastExportPath);
    }
}

void Application::Initialize() {
    if (m_initialized) return;

    m_config = EngineConfig::LoadFromFile("assets/engine.cfg");

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    if (m_config.vsync) SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(m_config.windowWidth, m_config.windowHeight, m_config.windowTitle.c_str());
    SetTargetFPS(m_config.targetFps);
    DisableCursor();

    m_camera.position = Vector3{5.0f, 6.0f, 10.0f};
    m_camera.target = Vector3{0.0f, 1.0f, 0.0f};
    m_camera.up = Vector3{0.0f, 1.0f, 0.0f};
    m_camera.fovy = 60.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;

    m_prefabs.LoadFromDirectory("assets/prefabs");
    m_prefabs.LoadVariantsFromDirectory("assets/prefab_variants");
    m_sceneLibrary.LoadFromDirectory("assets/scenes");
    RunContentValidation();

    ReloadStartScene();
    m_initialized = true;
    Logger::Info("Application initialized.");
}

void Application::Shutdown() {
    if (!m_initialized) return;
    m_systemRegistry.Clear();
    m_assets.UnloadAll();
    CloseWindow();
    m_initialized = false;
}

void Application::UpdateCameraController(float deltaTime) {
    const float distance = Vector3Distance(m_camera.position, m_camera.target);
    const float safeDistance = distance > 0.0001f ? distance : 0.0001f;

    float yaw = std::atan2f(m_camera.target.x - m_camera.position.x, m_camera.target.z - m_camera.position.z);
    float pitch = std::asinf((m_camera.target.y - m_camera.position.y) / safeDistance);

    if (m_input.IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        const Vector2 delta = GetMouseDelta();
        yaw -= delta.x * m_config.cameraLookSensitivity;
        pitch -= delta.y * m_config.cameraLookSensitivity;
        const float limit = 1.5f;
        if (pitch > limit) pitch = limit;
        if (pitch < -limit) pitch = -limit;
    }

    const Vector3 forward = Vector3Normalize(Vector3{
        std::sinf(yaw) * std::cosf(pitch),
        std::sinf(pitch),
        std::cosf(yaw) * std::cosf(pitch)
    });
    const Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, Vector3{0.0f, 1.0f, 0.0f}));
    const Vector3 up = Vector3{0.0f, 1.0f, 0.0f};

    Vector3 move{0.0f, 0.0f, 0.0f};
    if (m_input.IsKeyDown(KEY_W)) move = Vector3Add(move, forward);
    if (m_input.IsKeyDown(KEY_S)) move = Vector3Subtract(move, forward);
    if (m_input.IsKeyDown(KEY_D)) move = Vector3Add(move, right);
    if (m_input.IsKeyDown(KEY_A)) move = Vector3Subtract(move, right);
    if (m_input.IsKeyDown(KEY_E)) move = Vector3Add(move, up);
    if (m_input.IsKeyDown(KEY_Q)) move = Vector3Subtract(move, up);
    if (Vector3Length(move) > 0.0f) move = Vector3Normalize(move);

    const float speed = m_input.IsKeyDown(KEY_LEFT_SHIFT) ? m_config.cameraFastSpeed : m_config.cameraMoveSpeed;
    const Vector3 deltaMove = Vector3Scale(move, speed * deltaTime);
    m_camera.position = Vector3Add(m_camera.position, deltaMove);
    m_camera.target = Vector3Add(m_camera.position, forward);
}

} // namespace fw
