#include "core/Application.h"

#include <cmath>
#include <memory>

#include "raymath.h"

#include "core/Logger.h"
#include "editor/serialization/SceneExporter.h"
#include "editor/serialization/ExportPipeline.h"
#include "game/scenes/OpenWorldRuntimeScene.h"
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

        if (m_input.IsKeyPressed(KEY_ESCAPE)) {
    if (m_gameBuilderPanel.IsVisible()) {
        m_gameBuilderPanel.SetVisible(false);
        Logger::Info("Builder closed with Escape.");
    } else {
        m_isRunning = false;
    }
}
        if (m_input.IsKeyPressed(KEY_F1)) m_showDebugOverlay = !m_showDebugOverlay;
        if (m_input.IsKeyPressed(KEY_F2)) m_editorSelection.SelectNext(m_world);
        m_editorSelection.PruneDead(m_world);
        if (m_input.IsKeyPressed(KEY_F3)) ExportCurrentScene();
        if (m_input.IsKeyPressed(KEY_F4)) RunContentValidation();
        if (m_input.IsKeyPressed(KEY_F5)) ReloadStartScene();
        if (m_input.IsKeyPressed(KEY_F6)) WorldSerializer::SaveToFile(m_world, "assets/saves/open_world_snapshot.txt");
        if (m_input.IsKeyPressed(KEY_F7)) WorldSerializer::LoadFromFile(m_world, "assets/saves/open_world_snapshot.txt");
// Use one authoritative builder toggle path so the panel state is not being
// changed by competing code paths in the same frame.
const bool builderToggleRequested =
    ::IsKeyPressed(KEY_F10) ||
    ::IsKeyPressed(KEY_F9) ||
    ((::IsKeyDown(KEY_LEFT_CONTROL) || ::IsKeyDown(KEY_RIGHT_CONTROL)) && ::IsKeyPressed(KEY_B));

if (builderToggleRequested) {
    const bool newState = !m_gameBuilderPanel.IsVisible();
    m_gameBuilderPanel.SetVisible(newState);
    if (newState && m_mouseLookActive) {
        EnableCursor();
        m_mouseLookActive = false;
    }
    Logger::Info(std::string("Builder visibility set to: ") + (newState ? "true" : "false"));
}
if (m_input.IsKeyPressed(KEY_TAB)) m_showInspector = !m_showInspector;
        if (m_input.IsKeyPressed(KEY_F11)) ToggleFullscreen();

        const bool ctrlDown = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
        const bool mouseOverBuilder = m_gameBuilderPanel.IsMouseOverUi();
        const bool builderVisibleForInput = m_gameBuilderPanel.IsVisible();
        if (!m_mouseLookActive && !builderVisibleForInput && !mouseOverBuilder && ctrlDown && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_editorSelection.BeginBoxSelect(GetMousePosition());
        }
        if (m_editorSelection.IsBoxSelecting()) {
            m_editorSelection.UpdateBoxSelect(GetMousePosition());
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                m_editorSelection.EndBoxSelect(m_world, m_camera, ctrlDown);
            }
        }

        if (!builderVisibleForInput) {
            if (ctrlDown && m_input.IsKeyPressed(KEY_D)) DuplicateSelectionGroup();
            if (m_input.IsKeyPressed(KEY_M)) MirrorSelectionGroupX();
            if (m_input.IsKeyPressed(KEY_DELETE)) DeleteSelectionGroup();
        }

        const bool builderVisible = m_gameBuilderPanel.IsVisible();

        UpdateCameraController(m_time.DeltaTime());
        m_sceneManager.Update(*this, m_time.DeltaTime());
        if (!builderVisible) {
            m_editorAuthoring.Update(*this, m_world, m_editorSelection, m_prefabs, m_sceneLibrary);
            m_editorGizmo.Update(*this, m_world, m_editorSelection, m_time.DeltaTime());
            if (m_showInspector) {
                m_inspectorPanel.Update(*this, m_world, m_editorSelection, m_time.DeltaTime());
            }
        }

        accumulator += m_time.DeltaTime();
        while (accumulator >= m_config.fixedTimestep) {
            m_sceneManager.FixedUpdate(*this, m_config.fixedTimestep);
            accumulator -= m_config.fixedTimestep;
        }

        m_renderer.BeginFrame();
        m_renderer.Begin3D(m_camera);
        m_sceneManager.Render(*this);
        m_renderer.End3D();
        m_sceneManager.DrawUi(*this);

        if (m_showDebugOverlay) {
            m_debugOverlay.Draw(m_time, m_world, m_camera, m_sceneManager.CurrentSceneName(), m_editorSelection.Selected(), m_validationMessages, m_lastExportPath, m_showInspector);
        }
        m_editorGizmo.Draw(m_world, m_editorSelection);
        if (m_editorSelection.IsBoxSelecting()) {
            const Rectangle rect = m_editorSelection.BoxSelectRect();
            DrawRectangleLinesEx(rect, 2.0f, SKYBLUE);
            DrawRectangleRec(rect, Fade(SKYBLUE, 0.18f));
        }
        if (m_showInspector) {
            m_inspectorPanel.Draw(m_world, m_editorSelection);
        }

        // The builder tab body uses immediate-mode raylib drawing inside the
        // panel handlers, so update it only after the active drawing frame has started.
        m_gameBuilderPanel.Update(*this, m_contentRegistry);
        m_gameBuilderPanel.Draw(*this, m_contentRegistry);

        if (!m_mouseLookActive) {
            const Vector2 mousePos = GetMousePosition();
            DrawCircleV(mousePos, 4.0f, SKYBLUE);
            DrawCircleLines(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y), 10.0f, Fade(SKYBLUE, 0.75f));
        }

        m_renderer.EndFrame();
    }

    return 0;
}

void Application::ReloadStartScene() {
    m_sceneManager.ChangeScene(*this, std::make_unique<OpenWorldRuntimeScene>());
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

ExportBundleResult Application::StageExportBundle(const ExportBundleSettings& settings) {
    ExportBundleResult result = ExportPipeline::StageBundle(settings, m_world, m_prefabs);
    if (result.success) {
        m_lastExportPath = result.outputFolder;
        Logger::Info(result.message);
    } else {
        Logger::Warn(result.message);
    }
    return result;
}

void Application::DuplicateSelectionGroup() {
    const auto selected = m_editorSelection.SelectedEntities();
    if (selected.empty()) return;

    std::vector<Entity> duplicates;
    for (Entity entity : selected) {
        if (!m_world.IsAlive(entity)) continue;
        const Entity clone = m_world.CreateEntity();

        if (const auto* tag = m_world.GetComponent<TagComponent>(entity)) m_world.AddComponent<TagComponent>(clone, *tag);
        if (const auto* transform = m_world.GetComponent<TransformComponent>(entity)) {
            TransformComponent copy = *transform;
            copy.position.x += 1.5f;
            copy.position.z += 1.5f;
            m_world.AddComponent<TransformComponent>(clone, copy);
        }
        if (const auto* render = m_world.GetComponent<RenderComponent>(entity)) m_world.AddComponent<RenderComponent>(clone, *render);
        if (const auto* collider = m_world.GetComponent<BoxColliderComponent>(entity)) m_world.AddComponent<BoxColliderComponent>(clone, *collider);
        if (const auto* meta = m_world.GetComponent<EditorMetadataComponent>(entity)) m_world.AddComponent<EditorMetadataComponent>(clone, *meta);
        duplicates.push_back(clone);
    }

    m_editorSelection.Clear();
    for (Entity entity : duplicates) m_editorSelection.ToggleSelection(entity);
}

void Application::MirrorSelectionGroupX() {
    const auto selected = m_editorSelection.SelectedEntities();
    if (selected.empty()) return;

    Vector3 pivot {0.0f, 0.0f, 0.0f};
    int count = 0;
    for (Entity entity : selected) {
        if (const auto* transform = m_world.GetComponent<TransformComponent>(entity)) {
            pivot = Vector3Add(pivot, transform->position);
            ++count;
        }
    }
    if (count == 0) return;
    pivot = Vector3Scale(pivot, 1.0f / static_cast<float>(count));

    for (Entity entity : selected) {
        if (auto* transform = m_world.GetComponent<TransformComponent>(entity)) {
            const float dx = transform->position.x - pivot.x;
            transform->position.x = pivot.x - dx;
            transform->rotationEuler.y = -transform->rotationEuler.y;
        }
    }
}

void Application::DeleteSelectionGroup() {
    const auto selected = m_editorSelection.SelectedEntities();
    if (selected.empty()) return;

    for (Entity entity : selected) {
        m_world.DestroyEntity(entity);
    }
    m_editorSelection.Clear();
}

void Application::Initialize() {
    if (m_initialized) return;

    m_config = EngineConfig::LoadFromFile("assets/engine.cfg");

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (m_config.vsync) SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(m_config.windowWidth, m_config.windowHeight, m_config.windowTitle.c_str());
    SetWindowMinSize(1280, 720);
    SetTargetFPS(m_config.targetFps);
    EnableCursor();

    m_camera.position = Vector3{0.0f, 8.0f, 12.0f};
    m_camera.target = Vector3{0.0f, 1.0f, 0.0f};
    m_camera.up = Vector3{0.0f, 1.0f, 0.0f};
    m_camera.fovy = 60.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;

    m_prefabs.LoadFromDirectory("assets/prefabs");
    m_prefabs.LoadVariantsFromDirectory("assets/prefab_variants");
    m_sceneLibrary.LoadFromDirectory("assets/scenes");
    m_contentRegistry.RebuildFromProject();
    m_contentRegistry.SaveToFile("assets/registry/content_registry.txt");
    RunContentValidation();
    m_gameBuilderPanel.LoadSessionState();

    ReloadStartScene();
    m_initialized = true;
    Logger::Info("Application initialized.");
}

void Application::Shutdown() {
    if (!m_initialized) return;
    m_gameBuilderPanel.SaveSessionState();
    m_systemRegistry.Clear();
    m_assets.UnloadAll();
    CloseWindow();
    m_initialized = false;
}

void Application::UpdateCameraController(float deltaTime) {
    const bool builderBlockingLook = m_gameBuilderPanel.IsVisible();
    const bool activateLook = !builderBlockingLook && m_input.IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
    if (activateLook && !m_mouseLookActive) {
        DisableCursor();
        m_mouseLookActive = true;
    } else if (!activateLook && m_mouseLookActive) {
        EnableCursor();
        m_mouseLookActive = false;
    }

    const float distance = Vector3Distance(m_camera.position, m_camera.target);
    const float safeDistance = distance > 0.0001f ? distance : 0.0001f;

    float yaw = std::atan2f(m_camera.target.x - m_camera.position.x, m_camera.target.z - m_camera.position.z);
    float pitch = std::asinf((m_camera.target.y - m_camera.position.y) / safeDistance);

    if (m_mouseLookActive) {
        const Vector2 delta = GetMouseDelta();
        yaw -= delta.x * m_config.cameraLookSensitivity;
        pitch += delta.y * m_config.cameraLookSensitivity;
        const float limit = 1.2f;
        if (pitch > limit) pitch = limit;
        if (pitch < -limit) pitch = -limit;
    }

    const Vector3 forward = Vector3Normalize(Vector3{
        std::sinf(yaw) * std::cosf(pitch),
        std::sinf(pitch),
        std::cosf(yaw) * std::cosf(pitch)
    });
    m_camera.target = Vector3Add(m_camera.position, forward);

    if (m_mouseLookActive && m_input.IsKeyDown(KEY_E)) {
        const Vector3 up = Vector3{0.0f, 1.0f, 0.0f};
        const Vector3 deltaMove = Vector3Scale(up, m_config.cameraMoveSpeed * deltaTime);
        m_camera.position = Vector3Add(m_camera.position, deltaMove);
        m_camera.target = Vector3Add(m_camera.target, deltaMove);
    }

    if (m_mouseLookActive && m_input.IsKeyDown(KEY_Q)) {
        const Vector3 up = Vector3{0.0f, -1.0f, 0.0f};
        const Vector3 deltaMove = Vector3Scale(up, m_config.cameraMoveSpeed * deltaTime);
        m_camera.position = Vector3Add(m_camera.position, deltaMove);
        m_camera.target = Vector3Add(m_camera.target, deltaMove);
    }
}

} // namespace fw


