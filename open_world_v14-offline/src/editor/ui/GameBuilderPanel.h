#pragma once

#include <string>
#include <vector>

#include "raylib.h"

namespace fw {

class Application;

class GameBuilderPanel {
public:
    void Update(Application& app, float deltaTime);
    void Draw(const Application& app) const;

    [[nodiscard]] bool IsVisible() const { return m_visible; }
    void SetVisible(bool visible) { m_visible = visible; }
    void ToggleVisible() { m_visible = !m_visible; }

    [[nodiscard]] bool CapturesMouse(Vector2 mousePosition) const;

private:
    enum class Tab {
        Create = 0,
        Assets,
        Registry,
        Guide
    };

    struct AssetBrowserState {
        int categoryIndex = 0;
        int scrollOffset = 0;
        int selectedIndex = -1;
        std::vector<std::string> files;
        std::string selectedFile;
    };

    void RefreshAssetBrowser();
    void HandleTextInput();
    void StopEditing();
    bool HandleButton(Rectangle bounds, const char* label) const;
    bool HandleTextField(const std::string& fieldId, Rectangle bounds, std::string& value, int maxLength) const;
    void DrawTabs() const;
    void DrawCreateTab() const;
    void DrawAssetsTab() const;
    void DrawRegistryTab(const Application& app) const;
    void DrawGuideTab() const;
    void DrawStatusBar() const;

    void CreateScene(Application& app);
    void CreatePrefab(Application& app);
    void CreateNpc(Application& app);
    void CreateDialogue(Application& app);
    void CreateQuest(Application& app);
    void CreateItem(Application& app);
    void CreateStarterSlice(Application& app);
    void RefreshAuthoringContent(Application& app);

    std::string SanitizeId(const std::string& raw) const;
    std::string BuildStarterSceneText(const std::string& sceneId, const std::string& playerPrefabId, const std::string& npcPrefabId) const;

    bool m_visible = false;
    Tab m_tab = Tab::Create;
    mutable std::string m_activeFieldId;
    mutable float m_cursorBlinkTimer = 0.0f;

    std::string m_sceneId = "my_first_level";
    std::string m_playerPrefabId = "player";

    std::string m_prefabId = "starter_prop";
    std::string m_prefabTag = "prop";
    std::string m_prefabModelPath = "assets/models/props/starter_prop.glb";

    std::string m_npcId = "guide_mira";
    std::string m_npcDisplayName = "Guide Mira";
    std::string m_regionId = "starter_vale";
    std::string m_dialogueId = "guide_mira_intro";

    std::string m_questId = "first_steps";
    std::string m_questTitle = "First Steps";
    std::string m_itemId = "camp_supply";
    std::string m_itemDisplayName = "Camp Supply";
    mutable std::string m_registrySearch;
    mutable int m_registryKindIndex = 0;
    mutable int m_registryScrollOffset = 0;

    mutable std::string m_statusMessage = "V71 Game Builder ready. Press F10 to show or hide this panel.";
    AssetBrowserState m_assetBrowser {};
};

} // namespace fw
