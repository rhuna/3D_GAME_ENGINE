#include "editor/runtime/EditorGizmo.h"

#include <cstdio>

#include "core/Application.h"
#include "core/Hotkeys.h"
#include "ecs/World.h"
#include "ecs/components/RenderComponent.h"
#include "ecs/components/TransformComponent.h"
#include "editor/runtime/EditorSelection.h"
#include "raylib.h"
#include "raymath.h"

namespace fw {

namespace {
constexpr float kTranslateSpeed = 4.0f;
constexpr float kRotateSpeed = 70.0f;
constexpr float kScaleSpeed = 1.5f;

const char* GizmoModeName(GizmoMode mode) {
    switch (mode) {
    case GizmoMode::Translate: return "Translate";
    case GizmoMode::Rotate: return "Rotate";
    case GizmoMode::Scale: return "Scale";
    }
    return "Unknown";
}

float SelectionRadius(const World& world, Entity entity) {
    const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
    const RenderComponent* render = world.GetComponent<RenderComponent>(entity);
    if (!transform) return 0.75f;
    if (render && render->drawSphere) return render->sphereRadius * transform->scale.x + 0.15f;
    const float avgScale = (transform->scale.x + transform->scale.y + transform->scale.z) / 3.0f;
    return avgScale * 0.75f + 0.15f;
}

Vector3 GroupPivot(const World& world, const EditorSelection& selection) {
    Vector3 pivot {0.0f, 0.0f, 0.0f};
    int count = 0;
    for (Entity entity : selection.SelectedEntities()) {
        const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        if (!transform) continue;
        pivot = Vector3Add(pivot, transform->position);
        ++count;
    }
    if (count == 0) return pivot;
    return Vector3Scale(pivot, 1.0f / static_cast<float>(count));
}
}

void EditorGizmo::Update(Application& app, World& world, EditorSelection& selection, float deltaTime) {
    (void)app;
    selection.PruneDead(world);
    if (!selection.HasSelection(world)) return;

    if (hotkeys::ToggleTranslateGizmoPressed()) m_mode = GizmoMode::Translate;
    if (hotkeys::ToggleRotateGizmoPressed()) m_mode = GizmoMode::Rotate;
    if (hotkeys::ToggleScaleGizmoPressed()) m_mode = GizmoMode::Scale;

    Vector3 translationDelta {0.0f, 0.0f, 0.0f};
    Vector3 rotationDelta {0.0f, 0.0f, 0.0f};
    Vector3 scaleDelta {0.0f, 0.0f, 0.0f};

    const float moveSpeed = kTranslateSpeed * deltaTime;
    const float rotateSpeed = kRotateSpeed * deltaTime;
    const float scaleSpeed = kScaleSpeed * deltaTime;

    if (m_mode == GizmoMode::Translate) {
        if (IsKeyDown(KEY_KP_4)) translationDelta.x -= moveSpeed;
        if (IsKeyDown(KEY_KP_6)) translationDelta.x += moveSpeed;
        if (IsKeyDown(KEY_KP_8)) translationDelta.z -= moveSpeed;
        if (IsKeyDown(KEY_KP_5)) translationDelta.z += moveSpeed;
        if (IsKeyDown(KEY_KP_9)) translationDelta.y += moveSpeed;
        if (IsKeyDown(KEY_KP_7)) translationDelta.y -= moveSpeed;
    } else if (m_mode == GizmoMode::Rotate) {
        if (IsKeyDown(KEY_KP_4)) rotationDelta.y -= rotateSpeed;
        if (IsKeyDown(KEY_KP_6)) rotationDelta.y += rotateSpeed;
        if (IsKeyDown(KEY_KP_8)) rotationDelta.x -= rotateSpeed;
        if (IsKeyDown(KEY_KP_5)) rotationDelta.x += rotateSpeed;
        if (IsKeyDown(KEY_KP_9)) rotationDelta.z -= rotateSpeed;
        if (IsKeyDown(KEY_KP_7)) rotationDelta.z += rotateSpeed;
    } else {
        if (IsKeyDown(KEY_KP_6)) scaleDelta.x += scaleSpeed;
        if (IsKeyDown(KEY_KP_4)) scaleDelta.x -= scaleSpeed;
        if (IsKeyDown(KEY_KP_9)) scaleDelta.y += scaleSpeed;
        if (IsKeyDown(KEY_KP_7)) scaleDelta.y -= scaleSpeed;
        if (IsKeyDown(KEY_KP_8)) scaleDelta.z += scaleSpeed;
        if (IsKeyDown(KEY_KP_5)) scaleDelta.z -= scaleSpeed;
    }

    const Vector3 pivot = GroupPivot(world, selection);

    for (Entity entity : selection.SelectedEntities()) {
        TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        if (!transform) continue;

        if (m_mode == GizmoMode::Translate) {
            transform->position = Vector3Add(transform->position, translationDelta);
        } else if (m_mode == GizmoMode::Rotate) {
            transform->rotationEuler = Vector3Add(transform->rotationEuler, rotationDelta);

            if (rotationDelta.y != 0.0f) {
                Vector3 relative = Vector3Subtract(transform->position, pivot);
                const float radians = rotationDelta.y * DEG2RAD;
                const float cs = std::cos(radians);
                const float sn = std::sin(radians);
                Vector3 rotated {
                    relative.x * cs - relative.z * sn,
                    relative.y,
                    relative.x * sn + relative.z * cs
                };
                transform->position = Vector3Add(pivot, rotated);
            }
        } else {
            transform->scale.x = (transform->scale.x + scaleDelta.x > 0.1f) ? transform->scale.x + scaleDelta.x : 0.1f;
            transform->scale.y = (transform->scale.y + scaleDelta.y > 0.1f) ? transform->scale.y + scaleDelta.y : 0.1f;
            transform->scale.z = (transform->scale.z + scaleDelta.z > 0.1f) ? transform->scale.z + scaleDelta.z : 0.1f;

            Vector3 relative = Vector3Subtract(transform->position, pivot);
            relative.x *= (scaleDelta.x >= 0.0f) ? 1.0f + (scaleDelta.x * 0.25f) : 1.0f / (1.0f + (-scaleDelta.x * 0.25f));
            relative.y *= (scaleDelta.y >= 0.0f) ? 1.0f + (scaleDelta.y * 0.25f) : 1.0f / (1.0f + (-scaleDelta.y * 0.25f));
            relative.z *= (scaleDelta.z >= 0.0f) ? 1.0f + (scaleDelta.z * 0.25f) : 1.0f / (1.0f + (-scaleDelta.z * 0.25f));
            transform->position = Vector3Add(pivot, relative);
        }
    }
}

void EditorGizmo::Draw(const World& world, const EditorSelection& selection) const {
    if (selection.HasHovered(world) && (!selection.HasSelection(world) || selection.Hovered() != selection.Selected())) {
        const TransformComponent* hoveredTransform = world.GetComponent<TransformComponent>(selection.Hovered());
        if (hoveredTransform) {
            DrawSphereWires(hoveredTransform->position, SelectionRadius(world, selection.Hovered()), 10, 10, SKYBLUE);
        }
    }

    if (!selection.HasSelection(world)) return;

    const Vector3 pivot = GroupPivot(world, selection);
    float avgRadius = 0.8f;
    int count = 0;
    for (Entity entity : selection.SelectedEntities()) {
        const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        if (!transform) continue;
        DrawSphereWires(transform->position, SelectionRadius(world, entity), 10, 10, YELLOW);
        avgRadius += SelectionRadius(world, entity);
        ++count;
    }
    if (count > 0) avgRadius /= static_cast<float>(count);

    const float axisLength = 1.8f + avgRadius;
    DrawLine3D(pivot, Vector3Add(pivot, Vector3{axisLength, 0.0f, 0.0f}), RED);
    DrawLine3D(pivot, Vector3Add(pivot, Vector3{0.0f, axisLength, 0.0f}), GREEN);
    DrawLine3D(pivot, Vector3Add(pivot, Vector3{0.0f, 0.0f, axisLength}), BLUE);
    DrawSphereWires(pivot, avgRadius * 0.55f, 12, 12, ORANGE);

    DrawRectangle(10, 430, 560, 112, Color{0, 0, 0, 160});
    DrawText("V68 Gizmo", 20, 440, 24, RAYWHITE);

    char buffer[256];
    std::snprintf(buffer, sizeof(buffer), "Mode: %s  (Alt+W / Alt+E / Alt+R)  Selected: %d", GizmoModeName(m_mode), static_cast<int>(selection.SelectedEntities().size()));
    DrawText(buffer, 20, 470, 20, RAYWHITE);
    DrawText("Ctrl+drag = box select | Ctrl+D = duplicate group | Ctrl+M = mirror X | Ctrl+Delete = remove", 20, 496, 18, LIGHTGRAY);
    DrawText("Numpad 4/6 = X axis | 8/5 = Z axis | 9/7 = Y axis", 20, 518, 18, LIGHTGRAY);
}

} // namespace fw
