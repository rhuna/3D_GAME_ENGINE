#include "editor/runtime/InspectorPanel.h"

#include <cstdio>

#include "core/Application.h"
#include "ecs/World.h"
#include "ecs/components/EditorMetadataComponent.h"
#include "ecs/components/RenderComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "editor/runtime/EditorSelection.h"
#include "raylib.h"

namespace fw {

void InspectorPanel::Update(Application& app, World& world, EditorSelection& selection, float deltaTime) {
    if (!selection.HasSelection(world)) return;
    TransformComponent* transform = world.GetComponent<TransformComponent>(selection.Selected());
    if (!transform) return;

    const float moveSpeed = 4.0f * deltaTime;
    const float scaleSpeed = 1.5f * deltaTime;

    if (IsKeyDown(KEY_UP)) transform->position.z -= moveSpeed;
    if (IsKeyDown(KEY_DOWN)) transform->position.z += moveSpeed;
    if (IsKeyDown(KEY_LEFT)) transform->position.x -= moveSpeed;
    if (IsKeyDown(KEY_RIGHT)) transform->position.x += moveSpeed;
    if (IsKeyDown(KEY_PAGE_UP)) transform->position.y += moveSpeed;
    if (IsKeyDown(KEY_PAGE_DOWN)) transform->position.y -= moveSpeed;

    if (IsKeyDown(KEY_KP_ADD) || IsKeyDown(KEY_EQUAL)) {
        transform->scale.x += scaleSpeed;
        transform->scale.y += scaleSpeed;
        transform->scale.z += scaleSpeed;
    }
    if (IsKeyDown(KEY_KP_SUBTRACT) || IsKeyDown(KEY_MINUS)) {
        transform->scale.x = transform->scale.x > scaleSpeed ? transform->scale.x - scaleSpeed : 0.1f;
        transform->scale.y = transform->scale.y > scaleSpeed ? transform->scale.y - scaleSpeed : 0.1f;
        transform->scale.z = transform->scale.z > scaleSpeed ? transform->scale.z - scaleSpeed : 0.1f;
    }

    if (IsKeyPressed(KEY_DELETE)) {
        world.DestroyEntity(selection.Selected());
        selection.Clear();
    }

    (void)app;
}

void InspectorPanel::Draw(const World& world, const EditorSelection& selection) const {
    const int width = 420;
    const int x = GetScreenWidth() - width - 12;
    const int y = 12;
    const int height = std::min(300, GetScreenHeight() - 24);

    DrawRectangle(x, y, width, height, Color{0, 0, 0, 185});
    DrawRectangleLines(x, y, width, height, Fade(GREEN, 0.70f));
    DrawRectangle(x, y, width, 30, Color{18, 40, 18, 220});
    DrawText("Inspector", x + 12, y + 6, 20, RAYWHITE);

    if (!selection.HasSelection(world)) {
        DrawText("No entity selected.", x + 12, y + 42, 20, LIGHTGRAY);
        DrawText("LMB selects. MMB focuses camera.", x + 12, y + 70, 18, LIGHTGRAY);
        DrawText("Arrow keys/PageUp/PageDown move selection.", x + 12, y + 96, 18, LIGHTGRAY);
        DrawText("+/- scales. Delete removes. Tab hides inspector.", x + 12, y + 122, 18, LIGHTGRAY);
        return;
    }

    const Entity entity = selection.Selected();
    const TagComponent* tag = world.GetComponent<TagComponent>(entity);
    const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
    const RenderComponent* render = world.GetComponent<RenderComponent>(entity);
    const EditorMetadataComponent* meta = world.GetComponent<EditorMetadataComponent>(entity);

    char buffer[256];
    int drawY = y + 42;

    std::snprintf(buffer, sizeof(buffer), "Entity: %u", entity);
    DrawText(buffer, x + 12, drawY, 18, RAYWHITE);
    drawY += 26;

    std::snprintf(buffer, sizeof(buffer), "Tag: %s", tag ? tag->value.c_str() : "<none>");
    DrawText(buffer, x + 12, drawY, 18, RAYWHITE);
    drawY += 26;

    if (transform) {
        std::snprintf(buffer, sizeof(buffer), "Position: %.2f  %.2f  %.2f", transform->position.x, transform->position.y, transform->position.z);
        DrawText(buffer, x + 12, drawY, 18, RAYWHITE);
        drawY += 26;
        std::snprintf(buffer, sizeof(buffer), "Rotation: %.2f  %.2f  %.2f", transform->rotationEuler.x, transform->rotationEuler.y, transform->rotationEuler.z);
        DrawText(buffer, x + 12, drawY, 18, RAYWHITE);
        drawY += 26;
        std::snprintf(buffer, sizeof(buffer), "Scale: %.2f  %.2f  %.2f", transform->scale.x, transform->scale.y, transform->scale.z);
        DrawText(buffer, x + 12, drawY, 18, RAYWHITE);
        drawY += 26;
    }

    if (meta) {
        std::snprintf(buffer, sizeof(buffer), "Prefab: %s", meta->sourcePrefab.empty() ? "<runtime>" : meta->sourcePrefab.c_str());
        DrawText(buffer, x + 12, drawY, 18, RAYWHITE);
        drawY += 26;
        std::snprintf(buffer, sizeof(buffer), "Variant: %s", meta->sourceVariant.empty() ? "<none>" : meta->sourceVariant.c_str());
        DrawText(buffer, x + 12, drawY, 18, RAYWHITE);
        drawY += 26;
    } else if (render) {
        std::snprintf(buffer, sizeof(buffer), "Tint: %d, %d, %d, %d", render->tint.r, render->tint.g, render->tint.b, render->tint.a);
        DrawText(buffer, x + 12, drawY, 18, RAYWHITE);
        drawY += 26;
    }

    DrawText("Move: arrows / page up / page down", x + 12, y + height - 76, 18, LIGHTGRAY);
    DrawText("Scale: + / -", x + 12, y + height - 50, 18, LIGHTGRAY);
    DrawText("Delete: remove entity", x + 12, y + height - 24, 18, LIGHTGRAY);
}

} // namespace fw
