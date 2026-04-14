#pragma once

#include <string>
#include <vector>

#include "ecs/Entity.h"
#include "ecs/components/RenderComponent.h"
#include "raylib.h"

namespace fw {
class Application;
class World;
class EditorSelection;
class PrefabLibrary;
class SceneLibrary;

class BuildAssetWorkshop {
public:
    void Update(Application& app, World& world, EditorSelection& selection, PrefabLibrary& prefabs, SceneLibrary& scenes);
    void Draw(const World& world, const EditorSelection& selection) const;

    [[nodiscard]] bool IsActive() const { return m_active; }
    [[nodiscard]] bool WantsMouseCapture(const Vector2& mousePosition) const;
    [[nodiscard]] const std::string& LastSavedKitPath() const { return m_lastSavedKitPath; }

private:
    Rectangle PanelBounds() const;
    void CleanupGroup(const World& world);
    void StampPrimitive(Application& app, World& world, EditorSelection& selection);
    void ToggleSelectedIntoGroup(const World& world, const EditorSelection& selection);
    void RemoveSelectedFromGroup(const EditorSelection& selection);
    void DuplicateSelected(World& world, EditorSelection& selection);
    void MirrorGroupAcrossX(World& world);
    bool SaveGroupAsKit(const World& world, const EditorSelection& selection, SceneLibrary& scenes);

    static bool TryPickGroundPoint(const Camera3D& camera, Vector3& outPoint);
    static const char* PrimitiveShapeLabel(PrimitiveShape shape);

    bool m_active = false;
    bool m_stampMode = false;
    bool m_autoAddStampedToGroup = true;
    bool m_mouseOverPanel = false;
    PrimitiveShape m_currentShape = PrimitiveShape::Cube;
    float m_snapStep = 0.5f;
    Color m_currentTint = Color{180, 180, 210, 255};
    char m_kitName[64] = "new_asset_kit";
    std::string m_statusText;
    std::string m_lastSavedKitPath;
    std::vector<Entity> m_group;
};

} // namespace fw
