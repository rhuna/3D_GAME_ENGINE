#pragma once

#include <string>
#include <vector>

#include "raylib.h"

namespace fw {
class Application;
class World;
class EditorSelection;
class PrefabLibrary;
class SceneLibrary;

class VisualBuilderPanel {
public:
    struct PrimitiveDraft {
        std::string prefabName = "new_prop";
        std::string tag = "prop";
        std::string primitiveShape = "cube";
        Vector3 scale {1.0f, 1.0f, 1.0f};
        Color color {200, 200, 200, 255};
        bool addCollider = true;
        bool saveAsVariant = false;
        std::string variantBase = "ground";
    };

    void ToggleVisible() { m_visible = !m_visible; }
    [[nodiscard]] bool IsVisible() const { return m_visible; }
    [[nodiscard]] bool IsMouseOverUi() const;

    void Update(Application& app, World& world, EditorSelection& selection, PrefabLibrary& prefabs, SceneLibrary& scenes);
    void Draw(Application& app, const PrefabLibrary& prefabs, const SceneLibrary& scenes) const;

private:
    enum class Tab {
        Place,
        Create
    };

    void RefreshCaches(const PrefabLibrary& prefabs, const SceneLibrary& scenes);
    void HandleTextInput(std::string& target, std::size_t maxLength);
    bool Button(const Rectangle& rect, const char* label) const;
    void DrawHeaderTab(const Rectangle& rect, const char* label, Tab tab) const;
    void HandlePlaceTab(Application& app, World& world, EditorSelection& selection, PrefabLibrary& prefabs, SceneLibrary& scenes);
    void HandleCreateTab(Application& app, PrefabLibrary& prefabs);
    bool SavePrimitiveAsset(PrefabLibrary& prefabs) const;
    bool SavePrimitiveVariant(PrefabLibrary& prefabs) const;
    void SpawnPlacement(World& world, EditorSelection& selection, PrefabLibrary& prefabs, const Vector3& position) const;

    Rectangle m_bounds {20.0f, 20.0f, 420.0f, 520.0f};
    bool m_visible = false;
    bool m_clickPlaceEnabled = false;
    bool m_useVariantPlacement = false;
    bool m_needsRefresh = true;
    Tab m_activeTab = Tab::Place;

    std::vector<std::string> m_prefabNames;
    std::vector<std::string> m_variantNames;
    std::vector<std::string> m_sceneNames;
    std::size_t m_selectedPrefabIndex = 0;
    std::size_t m_selectedVariantIndex = 0;
    std::size_t m_selectedSceneIndex = 0;

    PrimitiveDraft m_draft {};
    int m_activeTextField = 0;
    std::string m_statusMessage = "Ready";
};

} // namespace fw
