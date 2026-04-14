#include "editor/ui/VisualBuilderPanel.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <sstream>

#include "core/Application.h"
#include "core/FileSystem.h"
#include "core/Logger.h"
#include "editor/runtime/EditorSelection.h"
#include "editor/serialization/SceneExporter.h"
#include "ecs/World.h"
#include "ecs/components/TransformComponent.h"
#include "gameplay/prefabs/PrefabLibrary.h"
#include "gameplay/prefabs/SpawnFactory.h"
#include "scene/data/SceneLibrary.h"
#include "util/StringUtil.h"
#include "raymath.h"

namespace fw {

namespace {

bool PointInRect(const Rectangle& rect, Vector2 point) {
    return CheckCollisionPointRec(point, rect);
}

Rectangle MakeRow(float x, float y, float width, float height) {
    return Rectangle{x, y, width, height};
}

std::string StripExtension(const std::string& filePath) {
    return std::filesystem::path(filePath).stem().string();
}

Vector3 MouseToGround(const Camera3D& camera, float groundY, bool& hit) {
    const Ray ray = GetMouseRay(GetMousePosition(), camera);
    const float dy = ray.direction.y;
    if (std::abs(dy) < 0.0001f) {
        hit = false;
        return Vector3{};
    }

    const float t = (groundY - ray.position.y) / dy;
    if (t < 0.0f) {
        hit = false;
        return Vector3{};
    }

    hit = true;
    return Vector3Add(ray.position, Vector3Scale(ray.direction, t));
}

std::string PrimitivePrefabText(const VisualBuilderPanel::PrimitiveDraft& draft) {
    std::ostringstream out;
    out << "name=" << draft.prefabName << "\n";
    out << "tag=" << draft.tag << "\n";
    out << "visible=true\n";
    out << "drawCube=" << (draft.primitiveShape == "cube" ? "true" : "false") << "\n";
    out << "drawSphere=" << (draft.primitiveShape == "sphere" ? "true" : "false") << "\n";
    out << "primitiveShape=" << draft.primitiveShape << "\n";
    out << "scale=" << draft.scale.x << "," << draft.scale.y << "," << draft.scale.z << "\n";
    out << "cubeSize=1.0\n";
    out << "sphereRadius=0.5\n";
    out << "color="
        << static_cast<int>(draft.color.r) << ","
        << static_cast<int>(draft.color.g) << ","
        << static_cast<int>(draft.color.b) << ","
        << static_cast<int>(draft.color.a) << "\n";
    if (draft.addCollider) {
        out << "colliderHalfExtents=" << draft.scale.x * 0.5f << "," << draft.scale.y * 0.5f << "," << draft.scale.z * 0.5f << "\n";
        out << "colliderIsTrigger=false\n";
        out << "colliderIsStatic=true\n";
    }
    return out.str();
}

std::string PrimitiveVariantText(const VisualBuilderPanel::PrimitiveDraft& draft) {
    std::ostringstream out;
    out << "name=" << draft.prefabName << "\n";
    out << "base=" << draft.variantBase << "\n";
    out << "tag=" << draft.tag << "\n";
    out << "drawCube=" << (draft.primitiveShape == "cube" ? "true" : "false") << "\n";
    out << "drawSphere=" << (draft.primitiveShape == "sphere" ? "true" : "false") << "\n";
    out << "primitiveShape=" << draft.primitiveShape << "\n";
    out << "scale=" << draft.scale.x << "," << draft.scale.y << "," << draft.scale.z << "\n";
    out << "color="
        << static_cast<int>(draft.color.r) << ","
        << static_cast<int>(draft.color.g) << ","
        << static_cast<int>(draft.color.b) << ","
        << static_cast<int>(draft.color.a) << "\n";
    return out.str();
}

} // namespace

bool VisualBuilderPanel::IsMouseOverUi() const {
    return m_visible && PointInRect(m_bounds, GetMousePosition());
}

void VisualBuilderPanel::RefreshCaches(const PrefabLibrary& prefabs, const SceneLibrary& scenes) {
    m_prefabNames.clear();
    m_variantNames.clear();
    m_sceneNames.clear();

    for (const auto& [name, _] : prefabs.Prefabs()) m_prefabNames.push_back(name);
    for (const auto& [name, _] : prefabs.Variants()) m_variantNames.push_back(name);
    for (const auto& [name, _] : scenes.Scenes()) m_sceneNames.push_back(name);

    std::sort(m_prefabNames.begin(), m_prefabNames.end());
    std::sort(m_variantNames.begin(), m_variantNames.end());
    std::sort(m_sceneNames.begin(), m_sceneNames.end());

    if (m_selectedPrefabIndex >= m_prefabNames.size()) m_selectedPrefabIndex = 0;
    if (m_selectedVariantIndex >= m_variantNames.size()) m_selectedVariantIndex = 0;
    if (m_selectedSceneIndex >= m_sceneNames.size()) m_selectedSceneIndex = 0;

    m_needsRefresh = false;
}

void VisualBuilderPanel::HandleTextInput(std::string& target, std::size_t maxLength) {
    int codepoint = GetCharPressed();
    while (codepoint > 0) {
        if ((codepoint >= 32 && codepoint <= 126) && target.size() < maxLength) {
            target.push_back(static_cast<char>(codepoint));
        }
        codepoint = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !target.empty()) {
        target.pop_back();
    }
}

bool VisualBuilderPanel::Button(const Rectangle& rect, const char* label) const {
    (void)label;
    return PointInRect(rect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void VisualBuilderPanel::DrawHeaderTab(const Rectangle& rect, const char* label, Tab tab) const {
    const bool active = m_activeTab == tab;
    DrawRectangleRec(rect, active ? Color{40, 75, 125, 255} : Color{35, 42, 54, 255});
    DrawRectangleLinesEx(rect, 1.0f, SKYBLUE);
    DrawText(label, static_cast<int>(rect.x + 12.0f), static_cast<int>(rect.y + 8.0f), 20, active ? RAYWHITE : LIGHTGRAY);
}

void VisualBuilderPanel::Update(Application& app, World& world, EditorSelection& selection, PrefabLibrary& prefabs, SceneLibrary& scenes) {
    if (!m_visible) return;

    if (m_needsRefresh) {
        RefreshCaches(prefabs, scenes);
    }

    const Rectangle placeTabRect {m_bounds.x + 10.0f, m_bounds.y + 10.0f, 110.0f, 36.0f};
    const Rectangle createTabRect {m_bounds.x + 126.0f, m_bounds.y + 10.0f, 110.0f, 36.0f};
    if (PointInRect(placeTabRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTab = Tab::Place;
    if (PointInRect(createTabRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTab = Tab::Create;

    if (m_activeTab == Tab::Place) {
        HandlePlaceTab(app, world, selection, prefabs, scenes);
    } else {
        HandleCreateTab(app, prefabs);
    }
}

void VisualBuilderPanel::HandlePlaceTab(Application& app, World& world, EditorSelection& selection, PrefabLibrary& prefabs, SceneLibrary& scenes) {
    const float left = m_bounds.x + 14.0f;
    float y = m_bounds.y + 62.0f;

    const Rectangle modePrefabRect = MakeRow(left, y, 110.0f, 30.0f);
    const Rectangle modeVariantRect = MakeRow(left + 116.0f, y, 110.0f, 30.0f);
    if (Button(modePrefabRect, m_useVariantPlacement ? "Use Prefab" : "Prefab Mode")) {
        m_useVariantPlacement = false;
    }
    if (Button(modeVariantRect, m_useVariantPlacement ? "Variant Mode" : "Use Variant")) {
        m_useVariantPlacement = true;
    }

    y += 42.0f;
    if (!m_useVariantPlacement) {
        if (!m_prefabNames.empty()) {
            if (Button(MakeRow(left, y, 36.0f, 28.0f), "<") && m_selectedPrefabIndex > 0) --m_selectedPrefabIndex;
            if (Button(MakeRow(left + 300.0f, y, 36.0f, 28.0f), ">") && (m_selectedPrefabIndex + 1) < m_prefabNames.size()) ++m_selectedPrefabIndex;
        }
    } else {
        if (!m_variantNames.empty()) {
            if (Button(MakeRow(left, y, 36.0f, 28.0f), "<") && m_selectedVariantIndex > 0) --m_selectedVariantIndex;
            if (Button(MakeRow(left + 300.0f, y, 36.0f, 28.0f), ">") && (m_selectedVariantIndex + 1) < m_variantNames.size()) ++m_selectedVariantIndex;
        }
    }

    if (Button(MakeRow(left, y + 40.0f, 150.0f, 32.0f), m_clickPlaceEnabled ? "Placement: ON" : "Placement: OFF")) {
        m_clickPlaceEnabled = !m_clickPlaceEnabled;
    }
    if (Button(MakeRow(left + 160.0f, y + 40.0f, 150.0f, 32.0f), "Refresh Lists")) {
        m_needsRefresh = true;
        m_statusMessage = "Refreshed content lists";
    }

    if (Button(MakeRow(left, y + 84.0f, 150.0f, 32.0f), "Save Scene As")) {
        const std::string sceneName = !m_sceneNames.empty() ? m_sceneNames[m_selectedSceneIndex] + "_edited" : "sandbox_edited";
        const std::string scenePath = "assets/scenes/" + sceneName + ".scene";
        if (SceneExporter::ExportWorldAsScene(world, prefabs, sceneName, scenePath)) {
            scenes.LoadSceneFile(scenePath);
            m_statusMessage = "Saved scene: " + sceneName;
            m_needsRefresh = true;
        } else {
            m_statusMessage = "Scene save failed";
        }
    }

    const bool canPlace = m_clickPlaceEnabled && !app.IsMouseLookActive() && !IsMouseOverUi();
    if (canPlace && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        bool hit = false;
        const Vector3 position = MouseToGround(app.GetCamera(), 0.0f, hit);
        if (hit) {
            SpawnPlacement(world, selection, prefabs, position);
        }
    }
}

void VisualBuilderPanel::HandleCreateTab(Application& app, PrefabLibrary& prefabs) {
    (void)app;
    if (m_activeTextField == 1) HandleTextInput(m_draft.prefabName, 32);
    if (m_activeTextField == 2) HandleTextInput(m_draft.tag, 32);

    const float left = m_bounds.x + 14.0f;
    float y = m_bounds.y + 74.0f;

    const Rectangle prefabField {left, y, 260.0f, 30.0f};
    const Rectangle tagField {left, y + 44.0f, 260.0f, 30.0f};
    if (PointInRect(prefabField, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 1;
    if (PointInRect(tagField, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 2;

    if (Button(MakeRow(left + 276.0f, y, 110.0f, 30.0f), "Shape")) {
        static const std::vector<std::string> shapes {"cube", "sphere", "cylinder", "plane"};
        auto it = std::find(shapes.begin(), shapes.end(), m_draft.primitiveShape);
        std::size_t index = it == shapes.end() ? 0 : static_cast<std::size_t>(std::distance(shapes.begin(), it));
        index = (index + 1) % shapes.size();
        m_draft.primitiveShape = shapes[index];
    }

    y += 96.0f;
    if (Button(MakeRow(left, y, 36.0f, 28.0f), "-") && m_draft.scale.x > 0.25f) m_draft.scale.x -= 0.25f;
    if (Button(MakeRow(left + 40.0f, y, 36.0f, 28.0f), "+")) m_draft.scale.x += 0.25f;
    if (Button(MakeRow(left + 100.0f, y, 36.0f, 28.0f), "-") && m_draft.scale.y > 0.25f) m_draft.scale.y -= 0.25f;
    if (Button(MakeRow(left + 140.0f, y, 36.0f, 28.0f), "+")) m_draft.scale.y += 0.25f;
    if (Button(MakeRow(left + 200.0f, y, 36.0f, 28.0f), "-") && m_draft.scale.z > 0.25f) m_draft.scale.z -= 0.25f;
    if (Button(MakeRow(left + 240.0f, y, 36.0f, 28.0f), "+")) m_draft.scale.z += 0.25f;

    y += 42.0f;
    if (Button(MakeRow(left, y, 70.0f, 28.0f), "R-") && m_draft.color.r >= 10) m_draft.color.r -= 10;
    if (Button(MakeRow(left + 74.0f, y, 70.0f, 28.0f), "R+") && m_draft.color.r <= 245) m_draft.color.r += 10;
    if (Button(MakeRow(left + 148.0f, y, 70.0f, 28.0f), "G-") && m_draft.color.g >= 10) m_draft.color.g -= 10;
    if (Button(MakeRow(left + 222.0f, y, 70.0f, 28.0f), "G+") && m_draft.color.g <= 245) m_draft.color.g += 10;
    if (Button(MakeRow(left + 296.0f, y, 70.0f, 28.0f), "B+") && m_draft.color.b <= 245) m_draft.color.b += 10;

    y += 44.0f;
    if (Button(MakeRow(left, y, 180.0f, 30.0f), m_draft.addCollider ? "Collider: ON" : "Collider: OFF")) {
        m_draft.addCollider = !m_draft.addCollider;
    }
    if (Button(MakeRow(left + 190.0f, y, 180.0f, 30.0f), m_draft.saveAsVariant ? "Save As Variant" : "Save As Prefab")) {
        m_draft.saveAsVariant = !m_draft.saveAsVariant;
    }

    y += 46.0f;
    if (Button(MakeRow(left, y, 180.0f, 34.0f), "Write Asset File")) {
        const bool ok = m_draft.saveAsVariant ? SavePrimitiveVariant(prefabs) : SavePrimitiveAsset(prefabs);
        m_statusMessage = ok ? "Saved asset authoring file" : "Failed to save asset file";
        m_needsRefresh = true;
    }
    if (Button(MakeRow(left + 190.0f, y, 180.0f, 34.0f), "Spawn Draft")) {
        PrefabDefinition draftPrefab{};
        draftPrefab.name = m_draft.prefabName;
        draftPrefab.tag.value = m_draft.tag;
        draftPrefab.render.visible = true;
        draftPrefab.render.drawCube = m_draft.primitiveShape == "cube";
        draftPrefab.render.drawSphere = m_draft.primitiveShape == "sphere";
        draftPrefab.render.primitiveShape = m_draft.primitiveShape;
        draftPrefab.render.tint = m_draft.color;
        draftPrefab.transform.scale = m_draft.scale;
        if (m_draft.addCollider) {
            draftPrefab.hasCollider = true;
            draftPrefab.collider.halfExtents = Vector3Scale(m_draft.scale, 0.5f);
            draftPrefab.collider.isStatic = true;
        }
        prefabs.Register(draftPrefab);
        m_statusMessage = "Draft registered in memory";
        m_needsRefresh = true;
    }
}

bool VisualBuilderPanel::SavePrimitiveAsset(PrefabLibrary& prefabs) const {
    const std::string path = "assets/prefabs/" + m_draft.prefabName + ".prefab";
    if (!FileSystem::WriteTextFile(path, PrimitivePrefabText(m_draft))) {
        Logger::Warn("Failed to write prefab: " + path);
        return false;
    }
    prefabs.LoadFromDirectory("assets/prefabs");
    return true;
}

bool VisualBuilderPanel::SavePrimitiveVariant(PrefabLibrary& prefabs) const {
    const std::string path = "assets/prefab_variants/" + m_draft.prefabName + ".variant";
    if (!FileSystem::WriteTextFile(path, PrimitiveVariantText(m_draft))) {
        Logger::Warn("Failed to write variant: " + path);
        return false;
    }
    prefabs.LoadVariantsFromDirectory("assets/prefab_variants");
    return true;
}

void VisualBuilderPanel::SpawnPlacement(World& world, EditorSelection& selection, PrefabLibrary& prefabs, const Vector3& position) const {
    Entity entity = 0;
    if (!m_useVariantPlacement && !m_prefabNames.empty()) {
        entity = spawn::SpawnFromPrefab(world, prefabs, m_prefabNames[m_selectedPrefabIndex]);
    } else if (m_useVariantPlacement && !m_variantNames.empty()) {
        entity = spawn::SpawnFromVariant(world, prefabs, m_variantNames[m_selectedVariantIndex]);
    }

    if (entity == 0) return;
    if (TransformComponent* transform = world.GetComponent<TransformComponent>(entity)) {
        transform->position = position;
        if (transform->position.y == 0.0f) {
            transform->position.y = transform->scale.y * 0.5f;
        }
    }
    selection.Select(entity);
}

void VisualBuilderPanel::Draw(Application& app, const PrefabLibrary& prefabs, const SceneLibrary& scenes) const {
    (void)app;
    (void)prefabs;
    (void)scenes;
    if (!m_visible) return;

    DrawRectangleRounded(m_bounds, 0.02f, 8, Color{18, 24, 34, 240});
    DrawRectangleRoundedLinesEx(m_bounds, 0.02f, 8, 1.0f, SKYBLUE);
    DrawText("V64 Visual Builder", static_cast<int>(m_bounds.x + 14.0f), static_cast<int>(m_bounds.y + 14.0f), 22, RAYWHITE);

    const Rectangle placeTabRect {m_bounds.x + 10.0f, m_bounds.y + 40.0f, 110.0f, 36.0f};
    const Rectangle createTabRect {m_bounds.x + 126.0f, m_bounds.y + 40.0f, 110.0f, 36.0f};
    DrawHeaderTab(placeTabRect, "Place", Tab::Place);
    DrawHeaderTab(createTabRect, "Create", Tab::Create);

    const float left = m_bounds.x + 14.0f;
    if (m_activeTab == Tab::Place) {
        DrawText("Placement mode", static_cast<int>(left), static_cast<int>(m_bounds.y + 88.0f), 18, LIGHTGRAY);
        const std::string currentName = !m_useVariantPlacement
            ? (m_prefabNames.empty() ? std::string("<no prefabs>") : m_prefabNames[m_selectedPrefabIndex])
            : (m_variantNames.empty() ? std::string("<no variants>") : m_variantNames[m_selectedVariantIndex]);
        DrawText(currentName.c_str(), static_cast<int>(left + 50.0f), static_cast<int>(m_bounds.y + 140.0f), 20, GOLD);
        DrawText(m_clickPlaceEnabled ? "Left-click ground to place" : "Enable placement to stamp into the world", static_cast<int>(left), static_cast<int>(m_bounds.y + 190.0f), 18, LIGHTGRAY);
        DrawText("Use F8 to open/close this panel.", static_cast<int>(left), static_cast<int>(m_bounds.y + 228.0f), 18, LIGHTGRAY);
        DrawText(m_statusMessage.c_str(), static_cast<int>(left), static_cast<int>(m_bounds.y + 494.0f), 18, SKYBLUE);
    } else {
        DrawText("Primitive asset workshop", static_cast<int>(left), static_cast<int>(m_bounds.y + 88.0f), 18, LIGHTGRAY);
        const Rectangle prefabField {left, m_bounds.y + 108.0f, 260.0f, 30.0f};
        const Rectangle tagField {left, m_bounds.y + 152.0f, 260.0f, 30.0f};
        DrawRectangleRec(prefabField, m_activeTextField == 1 ? Color{42, 61, 88, 255} : Color{28, 34, 45, 255});
        DrawRectangleLinesEx(prefabField, 1.0f, SKYBLUE);
        DrawText(m_draft.prefabName.c_str(), static_cast<int>(prefabField.x + 8.0f), static_cast<int>(prefabField.y + 7.0f), 18, RAYWHITE);
        DrawRectangleRec(tagField, m_activeTextField == 2 ? Color{42, 61, 88, 255} : Color{28, 34, 45, 255});
        DrawRectangleLinesEx(tagField, 1.0f, SKYBLUE);
        DrawText(m_draft.tag.c_str(), static_cast<int>(tagField.x + 8.0f), static_cast<int>(tagField.y + 7.0f), 18, RAYWHITE);
        DrawText(TextFormat("Shape: %s", m_draft.primitiveShape.c_str()), static_cast<int>(left + 280.0f), static_cast<int>(m_bounds.y + 115.0f), 18, GOLD);
        DrawText(TextFormat("Scale: %.2f %.2f %.2f", m_draft.scale.x, m_draft.scale.y, m_draft.scale.z), static_cast<int>(left), static_cast<int>(m_bounds.y + 214.0f), 18, LIGHTGRAY);
        DrawRectangle(left, static_cast<int>(m_bounds.y + 258.0f), 110, 42, m_draft.color);
        DrawRectangleLines(static_cast<int>(left), static_cast<int>(m_bounds.y + 258.0f), 110, 42, WHITE);
        DrawText("Prefab name", static_cast<int>(left), static_cast<int>(m_bounds.y + 92.0f), 16, LIGHTGRAY);
        DrawText("Tag", static_cast<int>(left), static_cast<int>(m_bounds.y + 136.0f), 16, LIGHTGRAY);
        DrawText(m_statusMessage.c_str(), static_cast<int>(left), static_cast<int>(m_bounds.y + 494.0f), 18, SKYBLUE);
    }
}

} // namespace fw
