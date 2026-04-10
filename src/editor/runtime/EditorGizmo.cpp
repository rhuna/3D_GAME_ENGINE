#include "editor/runtime/EditorGizmo.h"

#include <cstdio>

#include "core/Application.h"
#include "ecs/World.h"
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
} // namespace

void EditorGizmo::Update(Application& app, World& world, EditorSelection& selection, float deltaTime) {
    (void)app;
    if (!selection.HasSelection(world)) return;

    TransformComponent* transform = world.GetComponent<TransformComponent>(selection.Selected());
    if (!transform) return;

    const float moveSpeed = kTranslateSpeed * deltaTime;
    const float rotateSpeed = kRotateSpeed * deltaTime;
    const float scaleSpeed = kScaleSpeed * deltaTime;

    if (IsKeyPressed(KEY_ONE)) m_mode = GizmoMode::Translate;
    if (IsKeyPressed(KEY_TWO)) m_mode = GizmoMode::Rotate;
    if (IsKeyPressed(KEY_THREE)) m_mode = GizmoMode::Scale;

    if (m_mode == GizmoMode::Translate) {
        if (IsKeyDown(KEY_J)) transform->position.x -= moveSpeed;
        if (IsKeyDown(KEY_L)) transform->position.x += moveSpeed;
        if (IsKeyDown(KEY_I)) transform->position.z -= moveSpeed;
        if (IsKeyDown(KEY_K)) transform->position.z += moveSpeed;
        if (IsKeyDown(KEY_U)) transform->position.y += moveSpeed;
        if (IsKeyDown(KEY_O)) transform->position.y -= moveSpeed;
    } else if (m_mode == GizmoMode::Rotate) {
        if (IsKeyDown(KEY_J)) transform->rotationEuler.y -= rotateSpeed;
        if (IsKeyDown(KEY_L)) transform->rotationEuler.y += rotateSpeed;
        if (IsKeyDown(KEY_I)) transform->rotationEuler.x -= rotateSpeed;
        if (IsKeyDown(KEY_K)) transform->rotationEuler.x += rotateSpeed;
        if (IsKeyDown(KEY_U)) transform->rotationEuler.z -= rotateSpeed;
        if (IsKeyDown(KEY_O)) transform->rotationEuler.z += rotateSpeed;
    } else {
        if (IsKeyDown(KEY_L)) transform->scale.x += scaleSpeed;
        if (IsKeyDown(KEY_J)) transform->scale.x = transform->scale.x > scaleSpeed ? transform->scale.x - scaleSpeed : 0.1f;
        if (IsKeyDown(KEY_U)) transform->scale.y += scaleSpeed;
        if (IsKeyDown(KEY_O)) transform->scale.y = transform->scale.y > scaleSpeed ? transform->scale.y - scaleSpeed : 0.1f;
        if (IsKeyDown(KEY_I)) transform->scale.z += scaleSpeed;
        if (IsKeyDown(KEY_K)) transform->scale.z = transform->scale.z > scaleSpeed ? transform->scale.z - scaleSpeed : 0.1f;
    }
}

void EditorGizmo::Draw(const World& world, const EditorSelection& selection) const {
    if (!selection.HasSelection(world)) return;

    const TransformComponent* transform = world.GetComponent<TransformComponent>(selection.Selected());
    if (!transform) return;

    const Vector3 center = transform->position;
    const float axisLength = 1.5f * (transform->scale.x + transform->scale.y + transform->scale.z) / 3.0f;

    DrawLine3D(center, Vector3Add(center, Vector3{axisLength, 0.0f, 0.0f}), RED);
    DrawLine3D(center, Vector3Add(center, Vector3{0.0f, axisLength, 0.0f}), GREEN);
    DrawLine3D(center, Vector3Add(center, Vector3{0.0f, 0.0f, axisLength}), BLUE);
    DrawSphereWires(center, 0.1f, 8, 8, YELLOW);

    DrawRectangle(10, 430, 420, 86, Color{0, 0, 0, 160});
    DrawText("Gizmo", 20, 440, 24, RAYWHITE);

    char buffer[256];
    std::snprintf(buffer, sizeof(buffer), "Mode: %s  (1/2/3)", GizmoModeName(m_mode));
    DrawText(buffer, 20, 470, 20, RAYWHITE);
    DrawText("Use J/L = X, I/K = Z or X-rot, U/O = Y/Z depending on mode.", 20, 494, 18, LIGHTGRAY);
}

} // namespace fw
