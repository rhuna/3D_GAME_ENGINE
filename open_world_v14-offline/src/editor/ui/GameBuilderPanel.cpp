#include "editor/ui/GameBuilderPanel.h"

#include <algorithm>
#include <array>
#include <filesystem>
#include <sstream>

#include "core/Application.h"
#include "content/ContentRegistry.h"
#include "core/FileSystem.h"
#include "core/Logger.h"
#include "util/StringUtil.h"

namespace fw {

namespace {

constexpr Rectangle kPanelBounds {16.0f, 16.0f, 500.0f, 688.0f};
constexpr Rectangle kAssetsBounds {532.0f, 16.0f, 520.0f, 688.0f};
constexpr int kLineHeight = 24;
constexpr int kFieldHeight = 30;
constexpr int kButtonHeight = 32;
constexpr int kPadding = 12;

Color PanelColor() { return Color{22, 27, 35, 235}; }
Color AccentColor() { return Color{64, 156, 255, 255}; }
Color SoftAccentColor() { return Color{64, 156, 255, 80}; }
Color BorderColor() { return Color{90, 100, 120, 255}; }
Color InputColor() { return Color{35, 42, 55, 255}; }
Color TextColor() { return Color{235, 240, 248, 255}; }
Color MutedTextColor() { return Color{170, 180, 195, 255}; }
Color SuccessColor() { return Color{100, 205, 120, 255}; }
Color DangerColor() { return Color{215, 90, 90, 255}; }

void DrawPanelBox(Rectangle bounds, const char* title, const char* subtitle) {
    DrawRectangleRounded(bounds, 0.08f, 8, PanelColor());
    DrawRectangleRoundedLinesEx(bounds, 0.08f, 8, 1.5f, BorderColor());
    DrawText(title, static_cast<int>(bounds.x) + kPadding, static_cast<int>(bounds.y) + 10, 26, TextColor());
    DrawText(subtitle, static_cast<int>(bounds.x) + kPadding, static_cast<int>(bounds.y) + 40, 18, MutedTextColor());
}

bool PointInRect(Vector2 p, Rectangle r) {
    return CheckCollisionPointRec(p, r);
}

std::string MakeRelative(const std::string& path) {
    std::error_code ec;
    const auto current = std::filesystem::current_path(ec);
    if (ec) {
        return path;
    }
    const std::filesystem::path input(path);
    const auto relative = std::filesystem::relative(input, current, ec);
    return ec ? input.generic_string() : relative.generic_string();
}

} // namespace

void GameBuilderPanel::Update(Application& app, float deltaTime) {
    if (!m_visible) {
        StopEditing();
        return;
    }

    m_cursorBlinkTimer += deltaTime;
    if (m_cursorBlinkTimer > 1.0f) {
        m_cursorBlinkTimer -= 1.0f;
    }

    const Vector2 mouse = GetMousePosition();
    const bool mouseLeftPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (mouseLeftPressed) {
        const Rectangle createTabButton {kPanelBounds.x + 12.0f, kPanelBounds.y + 74.0f, 118.0f, 32.0f};
        const Rectangle assetsTabButton {kPanelBounds.x + 138.0f, kPanelBounds.y + 74.0f, 118.0f, 32.0f};
        const Rectangle registryTabButton {kPanelBounds.x + 264.0f, kPanelBounds.y + 74.0f, 118.0f, 32.0f};
        const Rectangle guideTabButton {kPanelBounds.x + 390.0f, kPanelBounds.y + 74.0f, 118.0f, 32.0f};
        if (PointInRect(mouse, createTabButton)) m_tab = Tab::Create;
        if (PointInRect(mouse, assetsTabButton)) m_tab = Tab::Assets;
        if (PointInRect(mouse, registryTabButton)) m_tab = Tab::Registry;
        if (PointInRect(mouse, guideTabButton)) m_tab = Tab::Guide;
    }

    if (m_tab == Tab::Create) {
        float y = kPanelBounds.y + 122.0f;
        const float labelX = kPanelBounds.x + 18.0f;
        const float fieldX = kPanelBounds.x + 180.0f;
        const float fieldWidth = kPanelBounds.width - 198.0f;

        HandleTextField("sceneId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_sceneId, 64); y += 36.0f;
        HandleTextField("playerPrefabId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_playerPrefabId, 64); y += 46.0f;

        if (mouseLeftPressed && PointInRect(mouse, {labelX, y, 145.0f, static_cast<float>(kButtonHeight)})) {
            CreateScene(app);
        }
        if (mouseLeftPressed && PointInRect(mouse, {labelX + 160.0f, y, 160.0f, static_cast<float>(kButtonHeight)})) {
            CreateStarterSlice(app);
        }
        y += 52.0f;

        HandleTextField("prefabId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_prefabId, 64); y += 36.0f;
        HandleTextField("prefabTag", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_prefabTag, 64); y += 36.0f;
        HandleTextField("prefabModelPath", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_prefabModelPath, 180); y += 44.0f;
        if (mouseLeftPressed && PointInRect(mouse, {labelX, y, 145.0f, static_cast<float>(kButtonHeight)})) {
            CreatePrefab(app);
        }
        y += 52.0f;

        HandleTextField("npcId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_npcId, 64); y += 36.0f;
        HandleTextField("npcDisplayName", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_npcDisplayName, 64); y += 36.0f;
        HandleTextField("dialogueId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_dialogueId, 64); y += 36.0f;
        HandleTextField("regionId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_regionId, 64); y += 44.0f;
        if (mouseLeftPressed && PointInRect(mouse, {labelX, y, 145.0f, static_cast<float>(kButtonHeight)})) {
            CreateNpc(app);
        }
        if (mouseLeftPressed && PointInRect(mouse, {labelX + 160.0f, y, 145.0f, static_cast<float>(kButtonHeight)})) {
            CreateDialogue(app);
        }
        y += 52.0f;

        HandleTextField("questId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_questId, 64); y += 36.0f;
        HandleTextField("questTitle", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_questTitle, 64); y += 44.0f;
        if (mouseLeftPressed && PointInRect(mouse, {labelX, y, 145.0f, static_cast<float>(kButtonHeight)})) {
            CreateQuest(app);
        }
        y += 52.0f;

        HandleTextField("itemId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_itemId, 64); y += 36.0f;
        HandleTextField("itemDisplayName", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, m_itemDisplayName, 64); y += 44.0f;
        if (mouseLeftPressed && PointInRect(mouse, {labelX, y, 145.0f, static_cast<float>(kButtonHeight)})) {
            CreateItem(app);
        }

        HandleTextInput();
    } else if (m_tab == Tab::Assets) {
        if (m_assetBrowser.files.empty()) {
            RefreshAssetBrowser();
        }

        const Rectangle refreshButton {kPanelBounds.x + 18.0f, kPanelBounds.y + 122.0f, 130.0f, static_cast<float>(kButtonHeight)};
        const Rectangle useButton {kPanelBounds.x + 160.0f, kPanelBounds.y + 122.0f, 168.0f, static_cast<float>(kButtonHeight)};
        const Rectangle modelsButton {kPanelBounds.x + 18.0f, kPanelBounds.y + 168.0f, 108.0f, static_cast<float>(kButtonHeight)};
        const Rectangle texturesButton {kPanelBounds.x + 136.0f, kPanelBounds.y + 168.0f, 108.0f, static_cast<float>(kButtonHeight)};
        const Rectangle prefabsButton {kPanelBounds.x + 254.0f, kPanelBounds.y + 168.0f, 108.0f, static_cast<float>(kButtonHeight)};
        const Rectangle scenesButton {kPanelBounds.x + 372.0f, kPanelBounds.y + 168.0f, 108.0f, static_cast<float>(kButtonHeight)};

        if (mouseLeftPressed && PointInRect(mouse, refreshButton)) {
            RefreshAssetBrowser();
        }
        if (mouseLeftPressed && PointInRect(mouse, useButton) && !m_assetBrowser.selectedFile.empty()) {
            m_prefabModelPath = MakeRelative(m_assetBrowser.selectedFile);
            m_tab = Tab::Create;
            m_statusMessage = "Assigned selected asset to prefab model path.";
        }
        if (mouseLeftPressed && PointInRect(mouse, modelsButton)) { m_assetBrowser.categoryIndex = 0; RefreshAssetBrowser(); }
        if (mouseLeftPressed && PointInRect(mouse, texturesButton)) { m_assetBrowser.categoryIndex = 1; RefreshAssetBrowser(); }
        if (mouseLeftPressed && PointInRect(mouse, prefabsButton)) { m_assetBrowser.categoryIndex = 2; RefreshAssetBrowser(); }
        if (mouseLeftPressed && PointInRect(mouse, scenesButton)) { m_assetBrowser.categoryIndex = 3; RefreshAssetBrowser(); }

        const Rectangle listBounds {kPanelBounds.x + 18.0f, kPanelBounds.y + 214.0f, kPanelBounds.width - 36.0f, 422.0f};
        if (PointInRect(mouse, listBounds)) {
            const float wheel = GetMouseWheelMove();
            if (wheel != 0.0f) {
                m_assetBrowser.scrollOffset -= static_cast<int>(wheel * 3.0f);
                if (m_assetBrowser.scrollOffset < 0) m_assetBrowser.scrollOffset = 0;
            }
            if (mouseLeftPressed) {
                const int visibleRows = static_cast<int>(listBounds.height / 28.0f);
                for (int i = 0; i < visibleRows; ++i) {
                    const int index = m_assetBrowser.scrollOffset + i;
                    if (index >= static_cast<int>(m_assetBrowser.files.size())) break;
                    Rectangle row {listBounds.x + 6.0f, listBounds.y + 6.0f + i * 28.0f, listBounds.width - 12.0f, 24.0f};
                    if (PointInRect(mouse, row)) {
                        m_assetBrowser.selectedIndex = index;
                        m_assetBrowser.selectedFile = m_assetBrowser.files[index];
                        m_statusMessage = "Selected asset: " + MakeRelative(m_assetBrowser.selectedFile);
                        break;
                    }
                }
            }
        }
    } else if (m_tab == Tab::Registry) {
        const Rectangle rebuildButton {kPanelBounds.x + 18.0f, kPanelBounds.y + 122.0f, 160.0f, static_cast<float>(kButtonHeight)};
        const Rectangle exportButton {kPanelBounds.x + 190.0f, kPanelBounds.y + 122.0f, 180.0f, static_cast<float>(kButtonHeight)};
        const Rectangle allButton {kPanelBounds.x + 18.0f, kPanelBounds.y + 166.0f, 92.0f, static_cast<float>(kButtonHeight)};
        const Rectangle prefabButton {kPanelBounds.x + 120.0f, kPanelBounds.y + 166.0f, 92.0f, static_cast<float>(kButtonHeight)};
        const Rectangle sceneButton {kPanelBounds.x + 222.0f, kPanelBounds.y + 166.0f, 92.0f, static_cast<float>(kButtonHeight)};
        const Rectangle kitButton {kPanelBounds.x + 324.0f, kPanelBounds.y + 166.0f, 92.0f, static_cast<float>(kButtonHeight)};
        const Rectangle dataButton {kPanelBounds.x + 426.0f, kPanelBounds.y + 166.0f, 92.0f, static_cast<float>(kButtonHeight)};
        const Rectangle searchField {kPanelBounds.x + 18.0f, kPanelBounds.y + 210.0f, 260.0f, static_cast<float>(kFieldHeight)};
        HandleTextField("registrySearch", searchField, m_registrySearch, 64);
        if (mouseLeftPressed && PointInRect(mouse, rebuildButton)) {
            app.RefreshContentRegistry();
            m_statusMessage = "Rebuilt content registry from local project files.";
        }
        if (mouseLeftPressed && PointInRect(mouse, exportButton)) {
            if (app.GetContentRegistry().SaveToFile("assets/registry/content_registry.txt")) m_statusMessage = "Exported registry snapshot to assets/registry/content_registry.txt";
            else m_statusMessage = "Failed to export registry snapshot.";
        }
        if (mouseLeftPressed && PointInRect(mouse, allButton)) m_registryKindIndex = 0;
        if (mouseLeftPressed && PointInRect(mouse, prefabButton)) m_registryKindIndex = 1;
        if (mouseLeftPressed && PointInRect(mouse, sceneButton)) m_registryKindIndex = 2;
        if (mouseLeftPressed && PointInRect(mouse, kitButton)) m_registryKindIndex = 3;
        if (mouseLeftPressed && PointInRect(mouse, dataButton)) m_registryKindIndex = 4;

        const Rectangle listBounds {kPanelBounds.x + 18.0f, kPanelBounds.y + 254.0f, kPanelBounds.width - 36.0f, 382.0f};
        if (PointInRect(mouse, listBounds)) {
            const float wheel = GetMouseWheelMove();
            if (wheel != 0.0f) {
                m_registryScrollOffset -= static_cast<int>(wheel * 3.0f);
                if (m_registryScrollOffset < 0) m_registryScrollOffset = 0;
            }
        }

        HandleTextInput();
    } else {
        StopEditing();
    }
}

void GameBuilderPanel::Draw(const Application& app) const {
    if (!m_visible) {
        return;
    }

    DrawPanelBox(kPanelBounds, "V71 Game Builder", "Offline content tools + local registry browser.");
    DrawTabs();

    switch (m_tab) {
        case Tab::Create: DrawCreateTab(); break;
        case Tab::Assets: DrawAssetsTab(); break;
        case Tab::Registry: DrawRegistryTab(app); break;
        case Tab::Guide: DrawGuideTab(); break;
    }

    DrawStatusBar();
}

bool GameBuilderPanel::CapturesMouse(Vector2 mousePosition) const {
    return m_visible && (PointInRect(mousePosition, kPanelBounds) || PointInRect(mousePosition, kAssetsBounds));
}

void GameBuilderPanel::RefreshAssetBrowser() {
    static const std::array<std::pair<std::string, std::vector<std::string>>, 4> categories {{
        {"assets/models", {".glb", ".gltf", ".obj", ".fbx", ".iqm"}},
        {"assets/textures", {".png", ".jpg", ".jpeg", ".bmp", ".tga", ".webp"}},
        {"assets/prefabs", {".prefab"}},
        {"assets/scenes", {".scene"}}
    }};

    const auto& category = categories[std::clamp(m_assetBrowser.categoryIndex, 0, static_cast<int>(categories.size() - 1))];
    m_assetBrowser.files = FileSystem::ListFilesRecursive(category.first, category.second);
    m_assetBrowser.selectedIndex = -1;
    m_assetBrowser.selectedFile.clear();
    m_assetBrowser.scrollOffset = 0;
    m_statusMessage = "Scanned " + category.first + " and found " + std::to_string(m_assetBrowser.files.size()) + " file(s).";
}

void GameBuilderPanel::HandleTextInput() {
    if (m_activeFieldId.empty()) {
        return;
    }

    std::string* target = nullptr;
    if (m_activeFieldId == "sceneId") target = &m_sceneId;
    else if (m_activeFieldId == "playerPrefabId") target = &m_playerPrefabId;
    else if (m_activeFieldId == "prefabId") target = &m_prefabId;
    else if (m_activeFieldId == "prefabTag") target = &m_prefabTag;
    else if (m_activeFieldId == "prefabModelPath") target = &m_prefabModelPath;
    else if (m_activeFieldId == "npcId") target = &m_npcId;
    else if (m_activeFieldId == "npcDisplayName") target = &m_npcDisplayName;
    else if (m_activeFieldId == "regionId") target = &m_regionId;
    else if (m_activeFieldId == "dialogueId") target = &m_dialogueId;
    else if (m_activeFieldId == "questId") target = &m_questId;
    else if (m_activeFieldId == "questTitle") target = &m_questTitle;
    else if (m_activeFieldId == "itemId") target = &m_itemId;
    else if (m_activeFieldId == "itemDisplayName") target = &m_itemDisplayName;
    else if (m_activeFieldId == "registrySearch") target = &m_registrySearch;

    if (!target) {
        return;
    }

    int codepoint = GetCharPressed();
    while (codepoint > 0) {
        if (codepoint >= 32 && codepoint < 127) {
            target->push_back(static_cast<char>(codepoint));
        }
        codepoint = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !target->empty()) {
        target->pop_back();
    }

    if (IsKeyPressed(KEY_ENTER)) {
        StopEditing();
    }
}

void GameBuilderPanel::StopEditing() {
    m_activeFieldId.clear();
}

bool GameBuilderPanel::HandleButton(Rectangle bounds, const char* label) const {
    const Vector2 mouse = GetMousePosition();
    const bool hovered = PointInRect(mouse, bounds);
    const Color fill = hovered ? AccentColor() : Color{52, 64, 82, 255};
    DrawRectangleRounded(bounds, 0.18f, 8, fill);
    DrawRectangleRoundedLinesEx(bounds, 0.18f, 8, 1.5f, hovered ? WHITE : BorderColor());
    const int textWidth = MeasureText(label, 18);
    DrawText(label, static_cast<int>(bounds.x + (bounds.width - textWidth) * 0.5f), static_cast<int>(bounds.y + 7.0f), 18, WHITE);
    return hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

bool GameBuilderPanel::HandleTextField(const std::string& fieldId, Rectangle bounds, std::string& value, int maxLength) const {
    const Vector2 mouse = GetMousePosition();
    const bool hovered = PointInRect(mouse, bounds);
    const bool active = (m_activeFieldId == fieldId);

    if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_activeFieldId = fieldId;
    }

    DrawRectangleRounded(bounds, 0.12f, 8, InputColor());
    DrawRectangleRoundedLinesEx(bounds, 0.12f, 8, 1.5f, active ? AccentColor() : hovered ? WHITE : BorderColor());

    std::string display = value;
    if (static_cast<int>(display.size()) > maxLength) {
        display.resize(maxLength);
        value = display;
    }

    const bool showCursor = active && m_cursorBlinkTimer < 0.5f;
    if (showCursor) {
        display += '|';
    }

    DrawText(display.c_str(), static_cast<int>(bounds.x) + 10, static_cast<int>(bounds.y) + 7, 18, TextColor());
    return active;
}

void GameBuilderPanel::DrawTabs() const {
    const Rectangle createTabButton {kPanelBounds.x + 12.0f, kPanelBounds.y + 74.0f, 118.0f, 32.0f};
    const Rectangle assetsTabButton {kPanelBounds.x + 138.0f, kPanelBounds.y + 74.0f, 118.0f, 32.0f};
    const Rectangle registryTabButton {kPanelBounds.x + 264.0f, kPanelBounds.y + 74.0f, 118.0f, 32.0f};
    const Rectangle guideTabButton {kPanelBounds.x + 390.0f, kPanelBounds.y + 74.0f, 118.0f, 32.0f};

    auto drawTab = [&](Rectangle bounds, const char* label, bool selected) {
        DrawRectangleRounded(bounds, 0.18f, 8, selected ? AccentColor() : Color{40, 48, 62, 255});
        DrawRectangleRoundedLinesEx(bounds, 0.18f, 8, 1.5f, selected ? WHITE : BorderColor());
        const int textWidth = MeasureText(label, 18);
        DrawText(label, static_cast<int>(bounds.x + (bounds.width - textWidth) * 0.5f), static_cast<int>(bounds.y + 7.0f), 18, WHITE);
    };

    drawTab(createTabButton, "Create", m_tab == Tab::Create);
    drawTab(assetsTabButton, "Assets", m_tab == Tab::Assets);
    drawTab(registryTabButton, "Registry", m_tab == Tab::Registry);
    drawTab(guideTabButton, "Guide", m_tab == Tab::Guide);
}

void GameBuilderPanel::DrawCreateTab() const {
    float y = kPanelBounds.y + 122.0f;
    const float labelX = kPanelBounds.x + 18.0f;
    const float fieldX = kPanelBounds.x + 180.0f;
    const float fieldWidth = kPanelBounds.width - 198.0f;

    auto label = [&](const char* text, float labelY) {
        DrawText(text, static_cast<int>(labelX), static_cast<int>(labelY) + 7, 18, TextColor());
    };

    label("Scene Id", y); HandleTextField("sceneId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_sceneId), 64); y += 36.0f;
    label("Player Prefab", y); HandleTextField("playerPrefabId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_playerPrefabId), 64); y += 46.0f;
    HandleButton({labelX, y, 145.0f, static_cast<float>(kButtonHeight)}, "Create Scene");
    HandleButton({labelX + 160.0f, y, 160.0f, static_cast<float>(kButtonHeight)}, "Starter Slice");
    y += 52.0f;

    label("Prefab Id", y); HandleTextField("prefabId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_prefabId), 64); y += 36.0f;
    label("Prefab Tag", y); HandleTextField("prefabTag", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_prefabTag), 64); y += 36.0f;
    label("Model Path", y); HandleTextField("prefabModelPath", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_prefabModelPath), 180); y += 44.0f;
    HandleButton({labelX, y, 145.0f, static_cast<float>(kButtonHeight)}, "Create Prefab");
    y += 52.0f;

    label("NPC Id", y); HandleTextField("npcId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_npcId), 64); y += 36.0f;
    label("NPC Name", y); HandleTextField("npcDisplayName", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_npcDisplayName), 64); y += 36.0f;
    label("Dialogue Id", y); HandleTextField("dialogueId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_dialogueId), 64); y += 36.0f;
    label("Region Id", y); HandleTextField("regionId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_regionId), 64); y += 44.0f;
    HandleButton({labelX, y, 145.0f, static_cast<float>(kButtonHeight)}, "Create NPC");
    HandleButton({labelX + 160.0f, y, 145.0f, static_cast<float>(kButtonHeight)}, "Dialogue File");
    y += 52.0f;

    label("Quest Id", y); HandleTextField("questId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_questId), 64); y += 36.0f;
    label("Quest Title", y); HandleTextField("questTitle", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_questTitle), 64); y += 44.0f;
    HandleButton({labelX, y, 145.0f, static_cast<float>(kButtonHeight)}, "Create Quest");
    y += 52.0f;

    label("Item Id", y); HandleTextField("itemId", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_itemId), 64); y += 36.0f;
    label("Item Name", y); HandleTextField("itemDisplayName", {fieldX, y, fieldWidth, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_itemDisplayName), 64); y += 44.0f;
    HandleButton({labelX, y, 145.0f, static_cast<float>(kButtonHeight)}, "Create Item");
}

void GameBuilderPanel::DrawAssetsTab() const {
    DrawText("Browse assets and push a selected file back into the prefab form.", static_cast<int>(kPanelBounds.x) + 18, static_cast<int>(kPanelBounds.y) + 126, 18, TextColor());
    HandleButton({kPanelBounds.x + 18.0f, kPanelBounds.y + 154.0f, 130.0f, static_cast<float>(kButtonHeight)}, "Refresh");
    HandleButton({kPanelBounds.x + 160.0f, kPanelBounds.y + 154.0f, 168.0f, static_cast<float>(kButtonHeight)}, "Use In Prefab");
    HandleButton({kPanelBounds.x + 18.0f, kPanelBounds.y + 196.0f, 108.0f, static_cast<float>(kButtonHeight)}, "Models");
    HandleButton({kPanelBounds.x + 136.0f, kPanelBounds.y + 196.0f, 108.0f, static_cast<float>(kButtonHeight)}, "Textures");
    HandleButton({kPanelBounds.x + 254.0f, kPanelBounds.y + 196.0f, 108.0f, static_cast<float>(kButtonHeight)}, "Prefabs");
    HandleButton({kPanelBounds.x + 372.0f, kPanelBounds.y + 196.0f, 108.0f, static_cast<float>(kButtonHeight)}, "Scenes");

    const Rectangle listBounds {kPanelBounds.x + 18.0f, kPanelBounds.y + 242.0f, kPanelBounds.width - 36.0f, 394.0f};
    DrawRectangleRounded(listBounds, 0.03f, 6, InputColor());
    DrawRectangleRoundedLinesEx(listBounds, 0.03f, 6, 1.5f, BorderColor());

    const int visibleRows = static_cast<int>(listBounds.height / 28.0f);
    for (int i = 0; i < visibleRows; ++i) {
        const int index = m_assetBrowser.scrollOffset + i;
        if (index >= static_cast<int>(m_assetBrowser.files.size())) {
            break;
        }
        Rectangle row {listBounds.x + 6.0f, listBounds.y + 6.0f + i * 28.0f, listBounds.width - 12.0f, 24.0f};
        const bool selected = (index == m_assetBrowser.selectedIndex);
        if (selected) {
            DrawRectangleRounded(row, 0.12f, 4, SoftAccentColor());
            DrawRectangleRoundedLinesEx(row, 0.12f, 4, 1.0f, AccentColor());
        }
        const std::string relative = MakeRelative(m_assetBrowser.files[index]);
        DrawText(relative.c_str(), static_cast<int>(row.x) + 8, static_cast<int>(row.y) + 4, 17, selected ? WHITE : TextColor());
    }

    if (m_assetBrowser.files.empty()) {
        DrawText("No files found in the selected asset category yet.", static_cast<int>(listBounds.x) + 12, static_cast<int>(listBounds.y) + 12, 18, MutedTextColor());
    }

    if (!m_assetBrowser.selectedFile.empty()) {
        DrawText("Selected", static_cast<int>(kPanelBounds.x) + 18, static_cast<int>(kPanelBounds.y) + 648, 18, SuccessColor());
        DrawText(MakeRelative(m_assetBrowser.selectedFile).c_str(), static_cast<int>(kPanelBounds.x) + 94, static_cast<int>(kPanelBounds.y) + 648, 18, TextColor());
    }
}

void GameBuilderPanel::DrawRegistryTab(const Application& app) const {
    const auto& registry = app.GetContentRegistry();
    DrawText("Browse the V71 local content index. This is the offline backbone for kits, scenes, templates, and assets.", static_cast<int>(kPanelBounds.x) + 18, static_cast<int>(kPanelBounds.y) + 126, 18, TextColor());
    HandleButton({kPanelBounds.x + 18.0f, kPanelBounds.y + 154.0f, 160.0f, static_cast<float>(kButtonHeight)}, "Rebuild Registry");
    HandleButton({kPanelBounds.x + 190.0f, kPanelBounds.y + 154.0f, 180.0f, static_cast<float>(kButtonHeight)}, "Export Snapshot");
    HandleButton({kPanelBounds.x + 18.0f, kPanelBounds.y + 196.0f, 92.0f, static_cast<float>(kButtonHeight)}, "All");
    HandleButton({kPanelBounds.x + 120.0f, kPanelBounds.y + 196.0f, 92.0f, static_cast<float>(kButtonHeight)}, "Prefabs");
    HandleButton({kPanelBounds.x + 222.0f, kPanelBounds.y + 196.0f, 92.0f, static_cast<float>(kButtonHeight)}, "Scenes");
    HandleButton({kPanelBounds.x + 324.0f, kPanelBounds.y + 196.0f, 92.0f, static_cast<float>(kButtonHeight)}, "Kits");
    HandleButton({kPanelBounds.x + 426.0f, kPanelBounds.y + 196.0f, 92.0f, static_cast<float>(kButtonHeight)}, "Data");
    HandleTextField("registrySearch", {kPanelBounds.x + 18.0f, kPanelBounds.y + 238.0f, 260.0f, static_cast<float>(kFieldHeight)}, const_cast<std::string&>(m_registrySearch), 64);

    const Rectangle listBounds {kPanelBounds.x + 18.0f, kPanelBounds.y + 282.0f, kPanelBounds.width - 36.0f, 354.0f};
    DrawRectangleRounded(listBounds, 0.03f, 6, InputColor());
    DrawRectangleRoundedLinesEx(listBounds, 0.03f, 6, 1.5f, BorderColor());

    std::vector<ContentEntry> entries;
    if (m_registrySearch.empty()) entries = registry.Entries();
    else entries = registry.Search(m_registrySearch);

    auto matchesKind = [&](const ContentEntry& entry) {
        switch (m_registryKindIndex) {
            case 1: return entry.kind == ContentKind::Prefab || entry.kind == ContentKind::Variant;
            case 2: return entry.kind == ContentKind::Scene;
            case 3: return entry.kind == ContentKind::Kit;
            case 4: return entry.kind == ContentKind::Template || entry.kind == ContentKind::Project;
            default: return true;
        }
    };
    std::vector<ContentEntry> filtered;
    for (const auto& entry : entries) if (matchesKind(entry)) filtered.push_back(entry);

    const int visibleRows = static_cast<int>(listBounds.height / 28.0f);
    const int startIndex = std::min(m_registryScrollOffset, std::max(0, static_cast<int>(filtered.size()) - visibleRows));
    for (int i = 0; i < visibleRows; ++i) {
        const int index = startIndex + i;
        if (index >= static_cast<int>(filtered.size())) break;
        const auto& entry = filtered[index];
        Rectangle row {listBounds.x + 6.0f, listBounds.y + 6.0f + i * 28.0f, listBounds.width - 12.0f, 24.0f};
        DrawText(entry.id.c_str(), static_cast<int>(row.x) + 8, static_cast<int>(row.y) + 4, 17, TextColor());
        const std::string right = std::string(ContentRegistry::KindToString(entry.kind)) + "  |  " + MakeRelative(entry.path);
        DrawText(right.c_str(), static_cast<int>(row.x) + 250, static_cast<int>(row.y) + 4, 17, MutedTextColor());
    }

    DrawText(TextFormat("Entries: %i  Prefabs: %i  Scenes: %i  Kits: %i",
             static_cast<int>(registry.Count()),
             static_cast<int>(registry.CountByKind(ContentKind::Prefab) + registry.CountByKind(ContentKind::Variant)),
             static_cast<int>(registry.CountByKind(ContentKind::Scene)),
             static_cast<int>(registry.CountByKind(ContentKind::Kit))),
             static_cast<int>(kPanelBounds.x) + 18, static_cast<int>(kPanelBounds.y) + 648, 18, SuccessColor());
}

void GameBuilderPanel::DrawGuideTab() const {
    const int x = static_cast<int>(kPanelBounds.x) + 18;
    int y = static_cast<int>(kPanelBounds.y) + 124;
    auto line = [&](const char* text, Color color = TextColor()) {
        DrawText(text, x, y, 19, color);
        y += 30;
    };

    line("V71 adds a local content registry for fully offline authoring.");
    line("1. Fill out a form in the Create tab.");
    line("2. Click a create button.");
    line("3. The panel writes starter files into assets/.");
    line("4. Prefabs and scenes reload automatically.");
    y += 6;
    line("Good first workflow", SuccessColor());
    line("- Create a prefab shell.");
    line("- Open Assets and assign a model path.");
    line("- Create a scene or starter slice.");
    line("- Press F5 to reload the scene if needed.");
    y += 6;
    line("Keyboard", AccentColor());
    line("F10 Toggle Game Builder panel");
    line("F5  Reload start scene");
    line("F4  Run validation");
    line("TAB Toggle old inspector panel");
    y += 6;
    line("Near-term V72 goal", SuccessColor());
    line("Offline import pipeline + asset thumbnails.");
}

void GameBuilderPanel::DrawStatusBar() const {
    const Rectangle bar {kPanelBounds.x, kPanelBounds.y + kPanelBounds.height + 8.0f, 1036.0f, 36.0f};
    DrawRectangleRounded(bar, 0.18f, 8, Color{15, 20, 28, 230});
    DrawRectangleRoundedLinesEx(bar, 0.18f, 8, 1.5f, BorderColor());
    DrawText(m_statusMessage.c_str(), static_cast<int>(bar.x) + 12, static_cast<int>(bar.y) + 8, 18, TextColor());
}

void GameBuilderPanel::CreateScene(Application& app) {
    const std::string sceneId = SanitizeId(m_sceneId);
    if (sceneId.empty()) {
        m_statusMessage = "Scene id cannot be empty.";
        return;
    }

    const std::string npcPrefabId = SanitizeId(m_npcId) + "_npc";
    const std::string path = "assets/scenes/" + sceneId + ".scene";
    if (!FileSystem::WriteTextFile(path, BuildStarterSceneText(sceneId, SanitizeId(m_playerPrefabId), npcPrefabId))) {
        m_statusMessage = "Failed to write scene file: " + path;
        return;
    }

    RefreshAuthoringContent(app);
    m_statusMessage = "Created scene: " + path;
}

void GameBuilderPanel::CreatePrefab(Application& app) {
    const std::string prefabId = SanitizeId(m_prefabId);
    if (prefabId.empty()) {
        m_statusMessage = "Prefab id cannot be empty.";
        return;
    }

    std::ostringstream out;
    out << "name=" << prefabId << "\n";
    out << "tag=" << SanitizeId(m_prefabTag.empty() ? prefabId : m_prefabTag) << "\n";
    out << "position=0,0,0\n";
    out << "rotationEuler=0,0,0\n";
    out << "scale=1,1,1\n";
    out << "color=255,255,255,255\n";
    out << "visible=true\n";
    out << "drawCube=false\n";
    out << "drawSphere=false\n";
    out << "useModel=true\n";
    out << "modelPath=" << StringUtil::Trim(m_prefabModelPath) << "\n";
    out << "colliderHalfExtents=0.5,0.5,0.5\n";
    out << "colliderIsTrigger=false\n";
    out << "colliderIsStatic=true\n";

    const std::string path = "assets/prefabs/" + prefabId + ".prefab";
    if (!FileSystem::WriteTextFile(path, out.str())) {
        m_statusMessage = "Failed to write prefab file: " + path;
        return;
    }

    RefreshAuthoringContent(app);
    m_statusMessage = "Created prefab: " + path;
}

void GameBuilderPanel::CreateNpc(Application& app) {
    const std::string npcId = SanitizeId(m_npcId);
    if (npcId.empty()) {
        m_statusMessage = "NPC id cannot be empty.";
        return;
    }

    std::ostringstream npcOut;
    npcOut << "id=" << npcId << "\n";
    npcOut << "displayName=" << m_npcDisplayName << "\n";
    npcOut << "regionId=" << SanitizeId(m_regionId) << "\n";
    npcOut << "dialogueId=" << SanitizeId(m_dialogueId) << "\n";
    npcOut << "questId=" << SanitizeId(m_questId) << "\n";
    npcOut << "homeRegionId=" << SanitizeId(m_regionId) << "\n";

    const std::string npcPath = "assets/content/npcs/" + npcId + ".npc";
    if (!FileSystem::WriteTextFile(npcPath, npcOut.str())) {
        m_statusMessage = "Failed to write NPC file: " + npcPath;
        return;
    }

    const std::string prefabId = npcId + "_npc";
    std::ostringstream prefabOut;
    prefabOut << "name=" << prefabId << "\n";
    prefabOut << "tag=npc\n";
    prefabOut << "position=0,0,0\n";
    prefabOut << "rotationEuler=0,0,0\n";
    prefabOut << "scale=1,1,1\n";
    prefabOut << "color=200,180,120,255\n";
    prefabOut << "visible=true\n";
    prefabOut << "drawCube=true\n";
    prefabOut << "cubeSize=1.0\n";
    prefabOut << "useModel=false\n";
    prefabOut << "colliderHalfExtents=0.4,1.0,0.4\n";
    prefabOut << "colliderIsTrigger=false\n";
    prefabOut << "colliderIsStatic=true\n";

    FileSystem::WriteTextFile("assets/prefabs/" + prefabId + ".prefab", prefabOut.str());
    RefreshAuthoringContent(app);
    m_statusMessage = "Created NPC data + prefab shell for: " + npcId;
}

void GameBuilderPanel::CreateDialogue(Application& app) {
    const std::string dialogueId = SanitizeId(m_dialogueId);
    if (dialogueId.empty()) {
        m_statusMessage = "Dialogue id cannot be empty.";
        return;
    }

    std::ostringstream out;
    out << "id=" << dialogueId << "\n";
    out << "speakerName=" << m_npcDisplayName << "\n";
    out << "text=Welcome to the builder. Replace this line with real dialogue later.\n";

    const std::string path = "assets/content/dialogue/" + dialogueId + ".dialogue";
    if (!FileSystem::WriteTextFile(path, out.str())) {
        m_statusMessage = "Failed to write dialogue file: " + path;
        return;
    }

    RefreshAuthoringContent(app);
    m_statusMessage = "Created dialogue: " + path;
}

void GameBuilderPanel::CreateQuest(Application& app) {
    const std::string questId = SanitizeId(m_questId);
    if (questId.empty()) {
        m_statusMessage = "Quest id cannot be empty.";
        return;
    }

    std::ostringstream out;
    out << "id=" << questId << "\n";
    out << "title=" << m_questTitle << "\n";
    out << "description=Replace this with the player-facing quest description.\n";
    out << "giverNpcId=" << SanitizeId(m_npcId) << "\n";
    out << "targetItemId=" << SanitizeId(m_itemId) << "\n";
    out << "targetRegionId=" << SanitizeId(m_regionId) << "\n";
    out << "targetCount=1\n";
    out << "rewardItemId=" << SanitizeId(m_itemId) << "\n";
    out << "rewardGold=25\n";

    const std::string path = "assets/content/quests/" + questId + ".quest";
    if (!FileSystem::WriteTextFile(path, out.str())) {
        m_statusMessage = "Failed to write quest file: " + path;
        return;
    }

    RefreshAuthoringContent(app);
    m_statusMessage = "Created quest: " + path;
}

void GameBuilderPanel::CreateItem(Application& app) {
    const std::string itemId = SanitizeId(m_itemId);
    if (itemId.empty()) {
        m_statusMessage = "Item id cannot be empty.";
        return;
    }

    std::ostringstream out;
    out << "id=" << itemId << "\n";
    out << "displayName=" << m_itemDisplayName << "\n";
    out << "type=quest\n";
    out << "description=Replace this with an item description.\n";
    out << "value=10\n";
    out << "attack=0\n";
    out << "defense=0\n";
    out << "heal=0\n";

    const std::string path = "assets/content/items/" + itemId + ".item";
    if (!FileSystem::WriteTextFile(path, out.str())) {
        m_statusMessage = "Failed to write item file: " + path;
        return;
    }

    RefreshAuthoringContent(app);
    m_statusMessage = "Created item: " + path;
}

void GameBuilderPanel::CreateStarterSlice(Application& app) {
    CreatePrefab(app);
    CreateDialogue(app);
    CreateItem(app);
    CreateQuest(app);
    CreateNpc(app);
    CreateScene(app);
    m_statusMessage = "Created a starter playable slice: scene + prefab + NPC + dialogue + quest + item.";
}

void GameBuilderPanel::RefreshAuthoringContent(Application& app) {
    app.GetPrefabLibrary().LoadFromDirectory("assets/prefabs");
    app.GetPrefabLibrary().LoadVariantsFromDirectory("assets/prefab_variants");
    app.GetSceneLibrary().LoadFromDirectory("assets/scenes");
    app.RunContentValidation();
    app.RefreshContentRegistry();
}

std::string GameBuilderPanel::SanitizeId(const std::string& raw) const {
    std::string out;
    out.reserve(raw.size());
    for (char c : raw) {
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            out.push_back(c);
        } else if (c >= 'A' && c <= 'Z') {
            out.push_back(static_cast<char>(c - 'A' + 'a'));
        } else if (c == ' ' || c == '-' || c == '_') {
            if (out.empty() || out.back() == '_') continue;
            out.push_back('_');
        }
    }
    while (!out.empty() && out.back() == '_') {
        out.pop_back();
    }
    return out;
}

std::string GameBuilderPanel::BuildStarterSceneText(const std::string& sceneId, const std::string& playerPrefabId, const std::string& npcPrefabId) const {
    std::ostringstream out;
    out << "scene=" << sceneId << "\n";
    out << "spawn=prefab=ground;position=0,-0.5,0;scale=30,1,30\n";
    if (!playerPrefabId.empty()) {
        out << "spawn=prefab=" << playerPrefabId << ";position=0,1,0\n";
    }
    if (!npcPrefabId.empty()) {
        out << "spawn=prefab=" << npcPrefabId << ";position=4,1,3\n";
    }
    if (!SanitizeId(m_prefabId).empty()) {
        out << "spawn=prefab=" << SanitizeId(m_prefabId) << ";position=-3,1,2\n";
    }
    return out.str();
}

} // namespace fw
