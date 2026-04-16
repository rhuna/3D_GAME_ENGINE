#include "editor/runtime/BuildAssetWorkshop.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <sstream>

#include "core/Application.h"
#include "core/FileSystem.h"
#include "core/Logger.h"
#include "ecs/World.h"
#include "ecs/components/BoxColliderComponent.h"
#include "ecs/components/EditorMetadataComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "editor/runtime/EditorSelection.h"
#include "gameplay/prefabs/PrefabLibrary.h"
#include "scene/data/SceneLibrary.h"
#include "raymath.h"

namespace fw {

namespace {

float SnapValue(float value, float step) {
    if (step <= 0.0f) {
        return value;
    }
    return std::round(value / step) * step;
}

std::string ColorToString(Color color) {
    return std::to_string(static_cast<int>(color.r)) + "," +
           std::to_string(static_cast<int>(color.g)) + "," +
           std::to_string(static_cast<int>(color.b)) + "," +
           std::to_string(static_cast<int>(color.a));
}

std::string Vec3ToString(const Vector3& value) {
    std::ostringstream out;
    out << value.x << ',' << value.y << ',' << value.z;
    return out.str();
}

Vector3 ComputeGroupCentroid(const World& world, const std::vector<Entity>& entities) {
    Vector3 sum{0.0f, 0.0f, 0.0f};
    int count = 0;
    for (Entity entity : entities) {
        if (const TransformComponent* transform = world.GetComponent<TransformComponent>(entity)) {
            sum = Vector3Add(sum, transform->position);
            ++count;
        }
    }
    if (count == 0) {
        return Vector3Zero();
    }
    return Vector3Scale(sum, 1.0f / static_cast<float>(count));
}

} // namespace

Rectangle BuildAssetWorkshop::PanelBounds() const {
    return Rectangle{12.0f, 300.0f, 460.0f, 290.0f};
}

bool BuildAssetWorkshop::WantsMouseCapture(const Vector2& mousePosition) const {
    return m_active && CheckCollisionPointRec(mousePosition, PanelBounds());
}

const char* BuildAssetWorkshop::PrimitiveShapeLabel(PrimitiveShape shape) {
    switch (shape) {
    case PrimitiveShape::Cube: return "Cube";
    case PrimitiveShape::Sphere: return "Sphere";
    case PrimitiveShape::Cylinder: return "Cylinder";
    case PrimitiveShape::Plane: return "Plane";
    case PrimitiveShape::None:
    default: return "None";
    }
}

bool BuildAssetWorkshop::TryPickGroundPoint(const Camera3D& camera, Vector3& outPoint) {
    const Ray ray = GetMouseRay(GetMousePosition(), camera);
    if (std::fabs(ray.direction.y) < 0.0001f) {
        return false;
    }

    const float t = -ray.position.y / ray.direction.y;
    if (t < 0.0f) {
        return false;
    }

    outPoint = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
    return true;
}

void BuildAssetWorkshop::CleanupGroup(const World& world) {
    m_group.erase(std::remove_if(m_group.begin(), m_group.end(), [&](Entity entity) {
        return !world.IsAlive(entity);
    }), m_group.end());
}

void BuildAssetWorkshop::StampPrimitive(Application& app, World& world, EditorSelection& selection) {
    Vector3 hitPoint{};
    if (!TryPickGroundPoint(app.GetCamera(), hitPoint)) {
        return;
    }

    hitPoint.x = SnapValue(hitPoint.x, m_snapStep);
    hitPoint.y = SnapValue(hitPoint.y + 0.5f, m_snapStep);
    hitPoint.z = SnapValue(hitPoint.z, m_snapStep);

    const Entity entity = world.CreateEntity();
    world.AddComponent<TagComponent>(entity, TagComponent{"build_piece"});

    TransformComponent transform{};
    transform.position = hitPoint;
    transform.rotationEuler = Vector3Zero();
    transform.scale = Vector3{1.0f, 1.0f, 1.0f};
    world.AddComponent<TransformComponent>(entity, transform);

    RenderComponent render{};
    render.visible = true;
    render.tint = m_currentTint;
    render.primitiveShape = m_currentShape;
    render.drawCube = (m_currentShape == PrimitiveShape::Cube);
    render.drawSphere = (m_currentShape == PrimitiveShape::Sphere);
    render.cubeSize = 1.0f;
    render.sphereRadius = 0.5f;
    render.primitiveSize = 1.0f;
    render.primitiveRadius = 0.5f;
    render.primitiveHeight = 1.0f;
    world.AddComponent<RenderComponent>(entity, render);

    BoxColliderComponent collider{};
    collider.halfExtents = Vector3{0.5f, 0.5f, 0.5f};
    collider.isStatic = true;
    collider.isTrigger = false;
    world.AddComponent<BoxColliderComponent>(entity, collider);

    world.AddComponent<EditorMetadataComponent>(entity, EditorMetadataComponent{});
    selection.Select(entity);
    if (m_autoAddStampedToGroup) {
        m_group.push_back(entity);
    }
}

void BuildAssetWorkshop::ToggleSelectedIntoGroup(const World& world, const EditorSelection& selection) {
    if (!selection.HasSelection(world)) {
        return;
    }
    const Entity selected = selection.Selected();
    auto it = std::find(m_group.begin(), m_group.end(), selected);
    if (it == m_group.end()) {
        m_group.push_back(selected);
        m_statusText = "Added selected entity to group.";
    } else {
        m_group.erase(it);
        m_statusText = "Removed selected entity from group.";
    }
}

void BuildAssetWorkshop::RemoveSelectedFromGroup(const EditorSelection& selection) {
    const Entity selected = selection.Selected();
    m_group.erase(std::remove(m_group.begin(), m_group.end(), selected), m_group.end());
}

void BuildAssetWorkshop::DuplicateSelected(World& world, EditorSelection& selection) {
    if (!selection.HasSelection(world)) {
        return;
    }

    const Entity selected = selection.Selected();
    const TagComponent* tag = world.GetComponent<TagComponent>(selected);
    const TransformComponent* transform = world.GetComponent<TransformComponent>(selected);
    const RenderComponent* render = world.GetComponent<RenderComponent>(selected);
    if (!tag || !transform || !render) {
        return;
    }

    const Entity clone = world.CreateEntity();
    world.AddComponent<TagComponent>(clone, *tag);
    TransformComponent cloneTransform = *transform;
    cloneTransform.position.x += m_snapStep > 0.0f ? m_snapStep : 1.0f;
    world.AddComponent<TransformComponent>(clone, cloneTransform);
    world.AddComponent<RenderComponent>(clone, *render);
    if (const BoxColliderComponent* collider = world.GetComponent<BoxColliderComponent>(selected)) {
        world.AddComponent<BoxColliderComponent>(clone, *collider);
    }
    if (const EditorMetadataComponent* meta = world.GetComponent<EditorMetadataComponent>(selected)) {
        world.AddComponent<EditorMetadataComponent>(clone, *meta);
    }
    selection.Select(clone);

    if (std::find(m_group.begin(), m_group.end(), selected) != m_group.end()) {
        m_group.push_back(clone);
    }
    m_statusText = "Duplicated selected piece.";
}

void BuildAssetWorkshop::MirrorGroupAcrossX(World& world) {
    CleanupGroup(world);
    if (m_group.empty()) {
        return;
    }
    const Vector3 pivot = ComputeGroupCentroid(world, m_group);
    for (Entity entity : m_group) {
        if (TransformComponent* transform = world.GetComponent<TransformComponent>(entity)) {
            const float offset = transform->position.x - pivot.x;
            transform->position.x = pivot.x - offset;
            transform->rotationEuler.y = -transform->rotationEuler.y;
        }
    }
    m_statusText = "Mirrored grouped pieces across X.";
}

bool BuildAssetWorkshop::SaveGroupAsKit(const World& world, const EditorSelection& selection, SceneLibrary& scenes) {
    (void)selection;
    CleanupGroup(world);
    if (m_group.empty()) {
        m_statusText = "Group is empty.";
        return false;
    }

    std::string kitName = m_kitName;
    if (kitName.empty()) {
        m_statusText = "Kit name is empty.";
        return false;
    }

    const Vector3 pivot = ComputeGroupCentroid(world, m_group);
    std::ostringstream scene;
    scene << "scene=" << kitName << "\n";

    int generatedIndex = 0;
    for (Entity entity : m_group) {
        const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        const RenderComponent* render = world.GetComponent<RenderComponent>(entity);
        if (!transform || !render) {
            continue;
        }

        std::string prefabName;
        std::string variantName;
        if (const EditorMetadataComponent* meta = world.GetComponent<EditorMetadataComponent>(entity)) {
            prefabName = meta->sourcePrefab;
            variantName = meta->sourceVariant;
        }

        if (prefabName.empty() && variantName.empty()) {
            ++generatedIndex;
            char generatedName[128];
            std::snprintf(generatedName, sizeof(generatedName), "%s_piece_%03d", kitName.c_str(), generatedIndex);
            prefabName = generatedName;

            std::ostringstream prefab;
            prefab << "name=" << prefabName << "\n";
            prefab << "tag=build_piece\n";
            prefab << "position=0,0,0\n";
            prefab << "rotationEuler=0,0,0\n";
            prefab << "scale=1,1,1\n";
            prefab << "color=" << ColorToString(render->tint) << "\n";
            prefab << "visible=" << (render->visible ? "true" : "false") << "\n";
            prefab << "drawCube=" << (render->drawCube ? "true" : "false") << "\n";
            prefab << "drawSphere=" << (render->drawSphere ? "true" : "false") << "\n";
            prefab << "cubeSize=" << render->cubeSize << "\n";
            prefab << "sphereRadius=" << render->sphereRadius << "\n";
            prefab << "useModel=" << (render->useModel ? "true" : "false") << "\n";
            if (!render->modelPath.empty()) {
                prefab << "modelPath=" << render->modelPath << "\n";
            }
            prefab << "primitiveShape=";
            switch (render->primitiveShape) {
                case PrimitiveShape::Cube: prefab << "cube"; break;
                case PrimitiveShape::Sphere: prefab << "sphere"; break;
                case PrimitiveShape::Cylinder: prefab << "cylinder"; break;
                case PrimitiveShape::Plane: prefab << "plane"; break;
                case PrimitiveShape::None: default: prefab << "none"; break;
            }
            prefab << "\n";
            prefab << "primitiveSize=" << render->primitiveSize << "\n";
            prefab << "primitiveRadius=" << render->primitiveRadius << "\n";
            prefab << "primitiveHeight=" << render->primitiveHeight << "\n";
            if (const BoxColliderComponent* collider = world.GetComponent<BoxColliderComponent>(entity)) {
                prefab << "colliderHalfExtents=" << Vec3ToString(collider->halfExtents) << "\n";
                prefab << "colliderIsTrigger=" << (collider->isTrigger ? "true" : "false") << "\n";
                prefab << "colliderIsStatic=" << (collider->isStatic ? "true" : "false") << "\n";
            }

            const std::string prefabPath = "assets/prefabs/generated/" + prefabName + ".prefab";
            if (!FileSystem::WriteTextFile(prefabPath, prefab.str())) {
                m_statusText = "Failed to write prefab: " + prefabPath;
                return false;
            }
        }

        const Vector3 relative = Vector3Subtract(transform->position, pivot);
        scene << "spawn=";
        if (!variantName.empty()) {
            scene << "variant=" << variantName << ';';
        } else {
            scene << "prefab=" << prefabName << ';';
        }
        scene << "position=" << relative.x << ',' << relative.y << ',' << relative.z << ';';
        scene << "rotationEuler=" << transform->rotationEuler.x << ',' << transform->rotationEuler.y << ',' << transform->rotationEuler.z << ';';
        scene << "scale=" << transform->scale.x << ',' << transform->scale.y << ',' << transform->scale.z << ';';
        scene << "color=" << static_cast<int>(render->tint.r) << ',' << static_cast<int>(render->tint.g) << ','
              << static_cast<int>(render->tint.b) << ',' << static_cast<int>(render->tint.a) << "\n";
    }

    const std::string scenePath = "assets/scenes/generated/" + kitName + ".scene";
    if (!FileSystem::WriteTextFile(scenePath, scene.str())) {
        m_statusText = "Failed to write scene kit: " + scenePath;
        return false;
    }

    scenes.LoadFromDirectory("assets/scenes");
    m_lastSavedKitPath = scenePath;
    m_statusText = "Saved kit. Use spawn=kit=" + kitName + ";position=x,y,z in a scene.";
    Logger::Info("V66", "Saved asset kit: " + scenePath);
    return true;
}

void BuildAssetWorkshop::Update(Application& app, World& world, EditorSelection& selection, PrefabLibrary& prefabs, SceneLibrary& scenes) {
    (void)prefabs;
    if (IsKeyPressed(KEY_F9)) {
        m_active = !m_active;
        m_statusText = m_active ? "Build Asset Workshop opened." : "Build Asset Workshop hidden.";
    }

    if (!m_active) {
        return;
    }

    m_mouseOverPanel = WantsMouseCapture(GetMousePosition());
    CleanupGroup(world);
    int key = GetCharPressed();
    while (key > 0) {
        const std::size_t length = std::strlen(m_kitName);
        if ((key == '_' || key == '-' || (key >= '0' && key <= '9') || (key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z')) && length + 1 < sizeof(m_kitName)) {
            m_kitName[length] = static_cast<char>(key);
            m_kitName[length + 1] = '\0';
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        const std::size_t length = std::strlen(m_kitName);
        if (length > 0) {
            m_kitName[length - 1] = '\0';
        }
    }


    if (IsKeyPressed(KEY_ONE)) m_currentShape = PrimitiveShape::Cube;
    if (IsKeyPressed(KEY_TWO)) m_currentShape = PrimitiveShape::Sphere;
    if (IsKeyPressed(KEY_THREE)) m_currentShape = PrimitiveShape::Cylinder;
    if (IsKeyPressed(KEY_FOUR)) m_currentShape = PrimitiveShape::Plane;
    if (IsKeyPressed(KEY_B)) {
        m_stampMode = !m_stampMode;
        m_statusText = m_stampMode ? "Stamp mode enabled." : "Stamp mode disabled.";
    }
    if (IsKeyPressed(KEY_G)) {
        ToggleSelectedIntoGroup(world, selection);
    }
    if (IsKeyPressed(KEY_H)) {
        RemoveSelectedFromGroup(selection);
    }
    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_D)) {
        DuplicateSelected(world, selection);
    }
    if (IsKeyPressed(KEY_M)) {
        MirrorGroupAcrossX(world);
    }
    if (IsKeyPressed(KEY_LEFT_BRACKET)) {
        m_snapStep = std::max(0.25f, m_snapStep - 0.25f);
    }
    if (IsKeyPressed(KEY_RIGHT_BRACKET)) {
        m_snapStep = std::min(4.0f, m_snapStep + 0.25f);
    }
    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_P)) {
        SaveGroupAsKit(world, selection, scenes);
    }

    if (!m_mouseOverPanel && !app.IsMouseLookActive() && m_stampMode && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        StampPrimitive(app, world, selection);
    }
}

void BuildAssetWorkshop::Draw(const World& world, const EditorSelection& selection) const {
    if (!m_active) {
        return;
    }

    const Rectangle panel = PanelBounds();
    DrawRectangleRec(panel, Color{0, 0, 0, 190});
    DrawRectangleLinesEx(panel, 2.0f, Fade(ORANGE, 0.80f));
    DrawText("V66 Build Asset Workshop", static_cast<int>(panel.x) + 12, static_cast<int>(panel.y) + 10, 22, RAYWHITE);

    int y = static_cast<int>(panel.y) + 42;
    DrawText(TextFormat("F9 toggle | B stamp | 1-4 shape | Ctrl+P save kit"), static_cast<int>(panel.x) + 12, y, 18, LIGHTGRAY); y += 26;
    DrawText(TextFormat("Current shape: %s", PrimitiveShapeLabel(m_currentShape)), static_cast<int>(panel.x) + 12, y, 20, RAYWHITE); y += 24;
    DrawText(TextFormat("Stamp mode: %s", m_stampMode ? "ON" : "OFF"), static_cast<int>(panel.x) + 12, y, 20, m_stampMode ? GREEN : LIGHTGRAY); y += 24;
    DrawText(TextFormat("Snap step: %.2f", m_snapStep), static_cast<int>(panel.x) + 12, y, 20, RAYWHITE); y += 24;
    DrawText(TextFormat("Grouped entities: %i", static_cast<int>(m_group.size())), static_cast<int>(panel.x) + 12, y, 20, RAYWHITE); y += 24;
    DrawText(TextFormat("Selected entity: %u", selection.Selected()), static_cast<int>(panel.x) + 12, y, 20, RAYWHITE); y += 30;

    DrawText(TextFormat("Kit name: %s", m_kitName), static_cast<int>(panel.x) + 12, y, 20, YELLOW); y += 26;
    DrawText("Actions: G add/remove selected, H remove, Ctrl+D duplicate, M mirror group", static_cast<int>(panel.x) + 12, y, 18, LIGHTGRAY); y += 24;
    DrawText("Save result: assets/scenes/generated/<kit>.scene", static_cast<int>(panel.x) + 12, y, 18, LIGHTGRAY); y += 24;
    DrawText("Then use spawn=kit=<kit>;position=x,y,z in any scene.", static_cast<int>(panel.x) + 12, y, 18, LIGHTGRAY); y += 28;

    Color statusColor = world.Entities().empty() ? LIGHTGRAY : SKYBLUE;
    DrawText(m_statusText.empty() ? "Ready." : m_statusText.c_str(), static_cast<int>(panel.x) + 12, y, 18, statusColor);
}

} // namespace fw
