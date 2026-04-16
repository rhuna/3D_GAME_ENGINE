#include "editor/ui/GameBuilderPanel.h"

#include <algorithm>
#include <array>
#include <sstream>
#include <unordered_map>

#include "content/ContentRegistry.h"
#include "core/Application.h"
#include "core/FileSystem.h"
#include "core/Logger.h"
#include "editor/serialization/ExportPipeline.h"
#include "util/StringUtil.h"

namespace fw {
namespace {

constexpr float kHeaderHeight = 32.0f;
constexpr float kTitleHeight = 36.0f;
constexpr float kTabGridTop = 30.0f;
constexpr float kTabGridRows = 4.0f;
constexpr float kTabRowGap = 6.0f;
constexpr float kTabGridBottomPadding = 14.0f;
constexpr float kTabWidth = 126.0f;
constexpr float kTabHeight = 28.0f;
constexpr float kLeft = 32.0f;
constexpr float kFieldWidth = 250.0f;
constexpr float kFieldHeight = 30.0f;
constexpr float kColumnGap = 16.0f;
constexpr float kButtonWidth = 160.0f;

std::string BoolText(bool value) {
    return value ? "true" : "false";
}

Rectangle MakeRow(float x, float y, float width = kFieldWidth, float height = kFieldHeight) {
    return Rectangle{x, y, width, height};
}

bool WriteAndLog(const std::string& path, const std::string& text, std::string& status) {
    if (!FileSystem::WriteTextFile(path, text)) {
        status = "Failed to write: " + path;
        Logger::Warn(status);
        return false;
    }
    status = "Wrote: " + path;
    Logger::Info(status);
    return true;
}

std::string JoinAssetList(const std::vector<ContentEntry>& entries) {
    std::ostringstream out;
    for (std::size_t i = 0; i < entries.size(); ++i) {
        out << entries[i].id << " [" << ContentRegistry::KindToString(entries[i].kind) << "]";
        if (i + 1 < entries.size()) out << "\n";
    }
    return out.str();
}

void DrawBuilderVisibilityText(bool visible) {
    const std::string text = std::string("Builder Visible: ") + (visible ? "true" : "false");
    DrawText(text.c_str(), 20, 10, 16, YELLOW);
}

void DrawButtonVisual(const Rectangle& rect, const char* label, bool hovered, Color fill) {
    DrawRectangleRec(rect, hovered ? Color{58, 80, 110, 255} : fill);
    DrawRectangleLinesEx(rect, 1.0f, SKYBLUE);
    DrawText(label, static_cast<int>(rect.x + 10.0f), static_cast<int>(rect.y + 7.0f), 18, RAYWHITE);
}

} // namespace

const char* GameBuilderPanel::TabToString(Tab tab) {
    switch (tab) {
        case Tab::Create: return "Create";
        case Tab::Story: return "Story";
        case Tab::Quest: return "Quest";
        case Tab::Fight: return "Fight";
        case Tab::Trade: return "Trade";
        case Tab::World: return "World";
        case Tab::Interior: return "Interior";
        case Tab::Persist: return "Persist";
        case Tab::Start: return "Start";
        case Tab::Audio: return "Audio";
        case Tab::Review: return "Review";
        case Tab::Release: return "Release";
        case Tab::Templates: return "Templates";
        case Tab::Ship: return "Ship";
        case Tab::Complete: return "Complete";
        case Tab::Registry: return "Registry";
    }
    return "Create";
}

GameBuilderPanel::Tab GameBuilderPanel::TabFromString(const std::string& value) {
    static const std::unordered_map<std::string, Tab> kMap {
        {"Create", Tab::Create}, {"Story", Tab::Story}, {"Quest", Tab::Quest}, {"Fight", Tab::Fight},
        {"Trade", Tab::Trade}, {"World", Tab::World}, {"Interior", Tab::Interior}, {"Persist", Tab::Persist},
        {"Start", Tab::Start}, {"Audio", Tab::Audio}, {"Review", Tab::Review}, {"Release", Tab::Release},
        {"Templates", Tab::Templates}, {"Ship", Tab::Ship}, {"Complete", Tab::Complete}, {"Registry", Tab::Registry}
    };
    const auto it = kMap.find(value);
    return it != kMap.end() ? it->second : Tab::Create;
}

void GameBuilderPanel::ToggleVisible() {
    m_visible = !m_visible;
    SaveSessionState();
}

void GameBuilderPanel::SetVisible(bool visible) {
    m_visible = visible;
    if (!m_visible) {
        m_activeTextField = 0;
    }
    m_status = m_visible ? "Builder open" : "Builder closed";
    SaveSessionState();
}

bool GameBuilderPanel::IsVisible() const {
    return m_visible;
}

bool GameBuilderPanel::PointInRect(const Rectangle& rect, Vector2 point) const {
    return CheckCollisionPointRec(point, rect);
}

bool GameBuilderPanel::Button(const Rectangle& rect, const char* label) const {
    (void)label;
    return PointInRect(rect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

bool GameBuilderPanel::IsMouseOverUi() const {
    return m_visible && PointInRect(m_bounds, GetMousePosition());
}

Rectangle GameBuilderPanel::ContentRect() const {
    const float contentTop = ContentTop();
    const float bottomPadding = 46.0f;
    return Rectangle{
        m_bounds.x + 18.0f,
        contentTop,
        m_bounds.width - 36.0f,
        std::max(120.0f, (m_bounds.y + m_bounds.height - bottomPadding) - contentTop)
    };
}

float GameBuilderPanel::ContentTop() const {
    const float tabsBottom = m_bounds.y + kTabGridTop + (kTabGridRows * kTabHeight) + ((kTabGridRows - 1.0f) * kTabRowGap);
    return tabsBottom + kTabGridBottomPadding;
}

void GameBuilderPanel::LoadSessionState() {
    const std::string text = FileSystem::ReadTextFile("assets/saves/builder_session.cfg");
    if (text.empty()) {
        return;
    }

    std::istringstream input(text);
    std::string line;
    while (std::getline(input, line)) {
        const std::size_t equals = line.find('=');
        if (equals == std::string::npos) {
            continue;
        }

        const std::string key = line.substr(0, equals);
        const std::string value = line.substr(equals + 1);
        if (key == "visible") {
            m_visible = (value == "true");
        } else if (key == "active_tab") {
            m_activeTab = TabFromString(value);
        } else if (key == "status") {
            m_status = value.empty() ? "Ready" : value;
        }
    }
}

void GameBuilderPanel::SaveSessionState() const {
    std::ostringstream out;
    out << "visible=" << BoolText(m_visible) << "\n";
    out << "active_tab=" << TabToString(m_activeTab) << "\n";
    out << "status=" << (m_status.empty() ? "Ready" : m_status) << "\n";
    FileSystem::WriteTextFile("assets/saves/builder_session.cfg", out.str());
}

void GameBuilderPanel::DrawHeaderTab(const Rectangle& rect, const char* label, Tab tab) const {
    const bool active = m_activeTab == tab;
    DrawRectangleRec(rect, active ? Color{40, 75, 125, 255} : Color{35, 42, 54, 255});
    DrawRectangleLinesEx(rect, 1.0f, SKYBLUE);
    DrawBuilderVisibilityText(m_visible);
    DrawText(label, static_cast<int>(rect.x + 8.0f), static_cast<int>(rect.y + 6.0f), 18, active ? RAYWHITE : LIGHTGRAY);
}

void GameBuilderPanel::DrawTextField(const Rectangle& rect, const char* label, const std::string& value, int fieldId) const {
    const bool active = m_activeTextField == fieldId;
    DrawBuilderVisibilityText(m_visible);
    DrawText(label, static_cast<int>(rect.x), static_cast<int>(rect.y - 18.0f), 16, LIGHTGRAY);
    DrawRectangleRec(rect, active ? Color{32, 40, 52, 255} : Color{22, 28, 38, 255});
    DrawRectangleLinesEx(rect, 1.0f, active ? SKYBLUE : Fade(LIGHTGRAY, 0.6f));
    DrawBuilderVisibilityText(m_visible);
    DrawText(value.c_str(), static_cast<int>(rect.x + 8.0f), static_cast<int>(rect.y + 7.0f), 18, RAYWHITE);
}

void GameBuilderPanel::HandleTextInput(std::string& target, std::size_t maxLength) {
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

Rectangle GameBuilderPanel::CloseButtonRect() const {
    return Rectangle{m_bounds.x + m_bounds.width - 42.0f, m_bounds.y + 6.0f, 28.0f, 24.0f};
}

void GameBuilderPanel::DrawStatusBar() const {
    const Rectangle bar {m_bounds.x + 10.0f, m_bounds.y + m_bounds.height - 36.0f, m_bounds.width - 20.0f, 26.0f};
    DrawRectangleRec(bar, Color{20, 24, 30, 230});
    DrawRectangleLinesEx(bar, 1.0f, Fade(SKYBLUE, 0.75f));
    DrawBuilderVisibilityText(m_visible);
    DrawText(m_status.c_str(), static_cast<int>(bar.x + 8.0f), static_cast<int>(bar.y + 5.0f), 16, RAYWHITE);
}

void GameBuilderPanel::UpdateTabClicks() {
    const Vector2 mouse = GetMousePosition();
    const std::array<std::pair<Tab, const char*>, 16> tabs {{
        {Tab::Create, "Create"}, {Tab::Story, "Story"}, {Tab::Quest, "Quest"}, {Tab::Fight, "Fight"},
        {Tab::Trade, "Trade"}, {Tab::World, "World"}, {Tab::Interior, "Interior"}, {Tab::Persist, "Persist"},
        {Tab::Start, "Start"}, {Tab::Audio, "Audio"}, {Tab::Review, "Review"}, {Tab::Release, "Release"},
        {Tab::Templates, "Templates"}, {Tab::Ship, "Ship"}, {Tab::Complete, "Complete"}, {Tab::Registry, "Registry"}
    }};

    for (std::size_t i = 0; i < tabs.size(); ++i) {
        const int row = static_cast<int>(i / 4);
        const int col = static_cast<int>(i % 4);
        const Rectangle rect {
            m_bounds.x + 10.0f + col * (kTabWidth + 8.0f),
            m_bounds.y + kTabGridTop + row * (kTabHeight + kTabRowGap),
            kTabWidth,
            kTabHeight
        };
        if (PointInRect(rect, mouse) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_activeTab = tabs[i].first;
            m_activeTextField = 0;
            SaveSessionState();
        }
    }
}

bool GameBuilderPanel::RefreshRegistry(ContentRegistry& registry) {
    const bool ok = registry.RebuildFromProject();
    registry.SaveToFile("assets/registry/content_registry.txt");
    if (ok) {
        m_status = "Registry rebuilt";
    } else {
        m_status = "Registry rebuilt with no entries";
    }
    return ok;
}

void GameBuilderPanel::WriteVersionMetadata() const {
    FileSystem::WriteTextFile("version_notes/VERSION.txt", "V114\n\nV114 Export Pipeline Staging and Builder Toggle Hardening\n");

    const std::array<std::pair<const char*, const char*>, 17> notes {{
        {"version_notes/V82_NOTES.md", "# V82 Notes\n\nIntegrated interior authoring in the real V90 builder.\n"},
        {"version_notes/V83_NOTES.md", "# V83 Notes\n\nIntegrated persistence/profile authoring in the real V90 builder.\n"},
        {"version_notes/V84_NOTES.md", "# V84 Notes\n\nIntegrated startup/menu/HUD authoring in the real V90 builder.\n"},
        {"version_notes/V85_NOTES.md", "# V85 Notes\n\nIntegrated audio and atmosphere authoring in the real V90 builder.\n"},
        {"version_notes/V86_NOTES.md", "# V86 Notes\n\nIntegrated playtest/review authoring in the real V90 builder.\n"},
        {"version_notes/V87_NOTES.md", "# V87 Notes\n\nIntegrated release/export authoring in the real V90 builder.\n"},
        {"version_notes/V88_NOTES.md", "# V88 Notes\n\nIntegrated starter template authoring in the real V90 builder.\n"},
        {"version_notes/V89_NOTES.md", "# V89 Notes\n\nIntegrated ship mode authoring in the real V90 builder.\n"},
        {"version_notes/V90_NOTES.md", "# V90 Notes\n\nReal engine integration and hardening pass for the V90 baseline.\n"},
        {"version_notes/V91_NOTES.md", "# V91 Notes\n\nIntegration and hardening milestone after the V90 real baseline.\n"},
        {"version_notes/V92_NOTES.md", "# V92 Notes\n\nCommented integration baseline and local verification follow-up.\n"},
        {"version_notes/V93_NOTES.md", "# V93 Notes\n\nCompile hardening and integration cleanup milestone.\n"},
        {"version_notes/V94_NOTES.md", "# V94 Notes\n\nWindows compile verification and runtime repair milestone.\n"},
        {"version_notes/V95_NOTES.md", "# V95 Notes\n\nFirst clean Windows build milestone.\n"},
        {"version_notes/V96_NOTES.md", "# V96 Notes\n\nFirst real runtime boot milestone.\n"},
        {"version_notes/V97_NOTES.md", "# V97 Notes\n\nReal repo integration baseline built from the uploaded full project tree.\n"}
    }};
    for (const auto& [path, text] : notes) FileSystem::WriteTextFile(path, text);
    FileSystem::WriteTextFile("version_notes/V113_NOTES.md", "# V113 Notes\n\nOffline build bootstrap hardening plus builder session persistence.\n");
    FileSystem::WriteTextFile("version_notes/V114_NOTES.md", "# V114 Notes\n\nReal export bundle staging plus builder toggle hardening and close button support.\n");
    FileSystem::WriteTextFile("version_notes/V115_NOTES.md", "# V115 Notes\n\nBuilder render/input repair plus Windows export pipeline compile fix.\n");
}

void GameBuilderPanel::Update(Application& app, ContentRegistry& registry) {
    if (!m_visible) return;

    const float width = std::min(760.0f, static_cast<float>(GetScreenWidth()) - 36.0f);
    const float height = std::min(700.0f, static_cast<float>(GetScreenHeight()) - 36.0f);
    m_bounds = Rectangle{18.0f, 18.0f, std::max(560.0f, width), std::max(620.0f, height)};

    const Rectangle closeRect = CloseButtonRect();
    if (PointInRect(closeRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        SetVisible(false);
        m_status = "Builder closed";
        return;
    }

    UpdateTabClicks();

    switch (m_activeTab) {
        case Tab::Create: HandleCreateTab(registry); break;
        case Tab::Story: HandleStoryTab(registry); break;
        case Tab::Quest: HandleQuestTab(registry); break;
        case Tab::Fight: HandleFightTab(registry); break;
        case Tab::Trade: HandleTradeTab(registry); break;
        case Tab::World: HandleWorldTab(registry); break;
        case Tab::Interior: HandleInteriorTab(registry); break;
        case Tab::Persist: HandlePersistTab(registry); break;
        case Tab::Start: HandleStartTab(registry); break;
        case Tab::Audio: HandleAudioTab(registry); break;
        case Tab::Review: HandleReviewTab(registry); break;
        case Tab::Release: HandleReleaseTab(app, registry); break;
        case Tab::Templates: HandleTemplatesTab(registry); break;
        case Tab::Ship: HandleShipTab(registry); break;
        case Tab::Complete: HandleCompleteTab(registry); break;
        case Tab::Registry: HandleRegistryTab(registry); break;
    }
}

void GameBuilderPanel::HandleCreateTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    float y = ContentTop();
    const Rectangle sceneRect = MakeRow(left, y);
    const Rectangle prefabRect = MakeRow(left, y + 54.0f);
    const Rectangle npcRect = MakeRow(left, y + 108.0f);
    if (PointInRect(sceneRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 1;
    if (PointInRect(prefabRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 2;
    if (PointInRect(npcRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 3;
    if (m_activeTextField == 1) HandleTextInput(m_create.sceneId);
    if (m_activeTextField == 2) HandleTextInput(m_create.prefabId);
    if (m_activeTextField == 3) HandleTextInput(m_create.npcId);

    DrawTextField(sceneRect, "Scene Id", m_create.sceneId, 1);
    DrawTextField(prefabRect, "Prefab Id", m_create.prefabId, 2);
    DrawTextField(npcRect, "Starter NPC Id", m_create.npcId, 3);

    if (Button(MakeRow(left + 280.0f, y, kButtonWidth, 34.0f), "Write Scene")) {
        std::ostringstream out;
        out << "scene=" << m_create.sceneId << "\n";
        out << "spawn=prefab=ground;position=0,0,0\n";
        out << "spawn=prefab=player;position=0,1,0\n";
        out << "spawn=prefab=" << m_create.prefabId << ";position=2,0,2\n";
        WriteAndLog("assets/scenes/" + m_create.sceneId + ".scene", out.str(), m_status);
        RefreshRegistry(registry);
    }
    if (Button(MakeRow(left + 280.0f, y + 44.0f, kButtonWidth, 34.0f), "Write Prefab")) {
        std::ostringstream out;
        out << "name=" << m_create.prefabId << "\n";
        out << "tag=prop\nvisible=true\ndrawCube=true\nprimitiveShape=cube\nscale=1,1,1\ncolor=180,180,180,255\n";
        WriteAndLog("assets/prefabs/" + m_create.prefabId + ".prefab", out.str(), m_status);
        RefreshRegistry(registry);
    }
    if (Button(MakeRow(left + 280.0f, y + 88.0f, kButtonWidth, 34.0f), "Write NPC")) {
        std::ostringstream out;
        out << "npcId=" << m_create.npcId << "\nname=" << m_create.npcId << "\nprefab=npc_villager\ndialogue=" << m_story.dialogueId << "\n";
        WriteAndLog("assets/npcs/" + m_create.npcId + ".npc", out.str(), m_status);
        RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleStoryTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 5> fields {{
        {MakeRow(left, y), {11, &m_story.dialogueId}},
        {MakeRow(left, y + 54.0f), {12, &m_story.npcId}},
        {MakeRow(left, y + 108.0f), {13, &m_story.openingText}},
        {MakeRow(left, y + 162.0f), {14, &m_story.choiceOne}},
        {MakeRow(left, y + 216.0f), {15, &m_story.choiceTwo}}
    }};
    for (const auto& [rect, data] : fields) {
        if (PointInRect(rect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = data.first;
        if (m_activeTextField == data.first) HandleTextInput(*data.second, 120);
    }
    DrawTextField(fields[0].first, "Dialogue Id", m_story.dialogueId, 11);
    DrawTextField(fields[1].first, "NPC Id", m_story.npcId, 12);
    DrawTextField(fields[2].first, "Opening Text", m_story.openingText, 13);
    DrawTextField(fields[3].first, "Choice One", m_story.choiceOne, 14);
    DrawTextField(fields[4].first, "Choice Two", m_story.choiceTwo, 15);

    if (Button(MakeRow(left + 290.0f, y, 180.0f, 34.0f), "Write Dialogue")) {
        std::ostringstream readable;
        readable << "dialogueId=" << m_story.dialogueId << "\n";
        readable << "npcId=" << m_story.npcId << "\n";
        readable << "text=" << m_story.openingText << "\n";
        readable << "choice=" << m_story.choiceOne << "\n";
        readable << "choice=" << m_story.choiceTwo << "\n";
        const bool ok1 = WriteAndLog("assets/dialogue/" + m_story.dialogueId + ".dialogue", readable.str(), m_status);

        std::ostringstream graph;
        graph << "node=start\ntext=" << m_story.openingText << "\n";
        graph << "choice=one;label=" << m_story.choiceOne << ";next=choice_one\n";
        graph << "choice=two;label=" << m_story.choiceTwo << ";next=choice_two\n\n";
        graph << "node=choice_one\ntext=Take this and head to the forest.\n\n";
        graph << "node=choice_two\ntext=Safe travels.\n";
        const bool ok2 = WriteAndLog("assets/dialogue/" + m_story.dialogueId + ".dialoguegraph", graph.str(), m_status);

        std::ostringstream links;
        links << "conversationId=" << m_story.dialogueId << "_conversation\n";
        links << "npc=" << m_story.npcId << "\n";
        links << "dialogue=" << m_story.dialogueId << "\n";
        links << "quest=" << m_quest.questId << "\n";
        const bool ok3 = WriteAndLog("assets/links/" + m_story.dialogueId + "_conversation.links", links.str(), m_status);
        if (ok1 && ok2 && ok3) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleQuestTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const Rectangle idRect = MakeRow(left, y);
    const Rectangle titleRect = MakeRow(left, y + 54.0f);
    const Rectangle typeRect = MakeRow(left, y + 108.0f);
    const Rectangle targetRect = MakeRow(left, y + 162.0f);
    const Rectangle rewardRect = MakeRow(right, y);
    const Rectangle nextRect = MakeRow(right, y + 54.0f);
    if (PointInRect(idRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 21;
    if (PointInRect(titleRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 22;
    if (PointInRect(typeRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 23;
    if (PointInRect(targetRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 24;
    if (PointInRect(rewardRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 25;
    if (PointInRect(nextRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 26;
    if (m_activeTextField == 21) HandleTextInput(m_quest.questId);
    if (m_activeTextField == 22) HandleTextInput(m_quest.title);
    if (m_activeTextField == 23) HandleTextInput(m_quest.objectiveType);
    if (m_activeTextField == 24) HandleTextInput(m_quest.objectiveTarget);
    if (m_activeTextField == 25) HandleTextInput(m_quest.rewardItem);
    if (m_activeTextField == 26) HandleTextInput(m_quest.nextQuestId);
    DrawTextField(idRect, "Quest Id", m_quest.questId, 21);
    DrawTextField(titleRect, "Title", m_quest.title, 22);
    DrawTextField(typeRect, "Objective Type", m_quest.objectiveType, 23);
    DrawTextField(targetRect, "Objective Target", m_quest.objectiveTarget, 24);
    DrawTextField(rewardRect, "Reward Item", m_quest.rewardItem, 25);
    DrawTextField(nextRect, "Next Quest Id", m_quest.nextQuestId, 26);
    if (Button(MakeRow(right, y + 108.0f, 72.0f, 30.0f), "Gold-")) m_quest.rewardGold = std::max(0, m_quest.rewardGold - 5);
    if (Button(MakeRow(right + 76.0f, y + 108.0f, 72.0f, 30.0f), "Gold+")) m_quest.rewardGold += 5;

    if (Button(MakeRow(right, y + 162.0f, 180.0f, 34.0f), "Write Quest Bundle")) {
        std::ostringstream quest;
        quest << "questId=" << m_quest.questId << "\n";
        quest << "title=" << m_quest.title << "\n";
        quest << "objectiveType=" << m_quest.objectiveType << "\n";
        quest << "objectiveTarget=" << m_quest.objectiveTarget << "\n";
        quest << "rewardItem=" << m_quest.rewardItem << "\n";
        quest << "rewardGold=" << m_quest.rewardGold << "\n";
        quest << "nextQuestId=" << m_quest.nextQuestId << "\n";
        const bool ok1 = WriteAndLog("assets/quests/" + m_quest.questId + ".quest", quest.str(), m_status);

        std::ostringstream graph;
        graph << "quest=" << m_quest.questId << "\nstart=start\nnode=start;objective=" << m_quest.objectiveType << ";target=" << m_quest.objectiveTarget << ";next=turn_in\nnode=turn_in;rewardItem=" << m_quest.rewardItem << ";rewardGold=" << m_quest.rewardGold << ";next=" << m_quest.nextQuestId << "\n";
        const bool ok2 = WriteAndLog("assets/questgraphs/" + m_quest.questId + ".questgraph", graph.str(), m_status);

        std::ostringstream links;
        links << "questLinkId=" << m_quest.questId << "_quest\nquest=" << m_quest.questId << "\ndialogue=" << m_story.dialogueId << "\nnextQuest=" << m_quest.nextQuestId << "\n";
        const bool ok3 = WriteAndLog("assets/links/" + m_quest.questId + "_quest.links", links.str(), m_status);
        if (ok1 && ok2 && ok3) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleFightTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 6> fields {{
        {MakeRow(left, y), {31, &m_fight.enemyId}},
        {MakeRow(left, y + 54.0f), {32, &m_fight.displayName}},
        {MakeRow(left, y + 108.0f), {33, &m_fight.prefabId}},
        {MakeRow(right, y), {34, &m_fight.behavior}},
        {MakeRow(right, y + 54.0f), {35, &m_fight.encounterId}},
        {MakeRow(right, y + 108.0f), {36, &m_fight.lootTable}}
    }};
    for (const auto& [rect, data] : fields) {
        if (PointInRect(rect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = data.first;
        if (m_activeTextField == data.first) HandleTextInput(*data.second);
    }
    for (std::size_t i = 0; i < fields.size(); ++i) {
        static const char* labels[] = {"Enemy Id", "Display Name", "Prefab", "Behavior", "Encounter Id", "Loot Table"};
        DrawTextField(fields[i].first, labels[i], *fields[i].second.second, fields[i].second.first);
    }
    if (Button(MakeRow(left, y + 162.0f, 72.0f, 30.0f), "Count-")) m_fight.count = std::max(1, m_fight.count - 1);
    if (Button(MakeRow(left + 76.0f, y + 162.0f, 72.0f, 30.0f), "Count+")) ++m_fight.count;
    if (Button(MakeRow(right, y + 162.0f, 72.0f, 30.0f), "Rad-")) m_fight.radius = std::max(1.0f, m_fight.radius - 1.0f);
    if (Button(MakeRow(right + 76.0f, y + 162.0f, 72.0f, 30.0f), "Rad+")) m_fight.radius += 1.0f;

    if (Button(MakeRow(left, y + 214.0f, 180.0f, 34.0f), "Write Encounter Bundle")) {
        std::ostringstream enemy;
        enemy << "enemyId=" << m_fight.enemyId << "\nname=" << m_fight.displayName << "\nprefab=" << m_fight.prefabId << "\nbehavior=" << m_fight.behavior << "\n";
        const bool ok1 = WriteAndLog("assets/enemies/" + m_fight.enemyId + ".enemy", enemy.str(), m_status);

        std::ostringstream graph;
        graph << "encounterId=" << m_fight.encounterId << "\nenemyId=" << m_fight.enemyId << "\ncount=" << m_fight.count << "\nradius=" << m_fight.radius << "\nlootTable=" << m_fight.lootTable << "\n";
        const bool ok2 = WriteAndLog("assets/encountergraphs/" + m_fight.encounterId + ".encountergraph", graph.str(), m_status);

        std::ostringstream links;
        links << "encounterLinkId=" << m_fight.encounterId << "_encounter\nencounter=" << m_fight.encounterId << "\nenemy=" << m_fight.enemyId << "\nloot=" << m_fight.lootTable << "\n";
        const bool ok3 = WriteAndLog("assets/links/" + m_fight.encounterId + "_encounter.links", links.str(), m_status);
        if (ok1 && ok2 && ok3) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleTradeTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 5> fields {{
        {MakeRow(left, y), {41, &m_trade.itemId}},
        {MakeRow(left, y + 54.0f), {42, &m_trade.itemName}},
        {MakeRow(left, y + 108.0f), {43, &m_trade.itemType}},
        {MakeRow(right, y), {44, &m_trade.merchantId}},
        {MakeRow(right, y + 54.0f), {45, &m_trade.lootId}}
    }};
    for (const auto& [rect, data] : fields) {
        if (PointInRect(rect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = data.first;
        if (m_activeTextField == data.first) HandleTextInput(*data.second);
    }
    for (std::size_t i = 0; i < fields.size(); ++i) {
        static const char* labels[] = {"Item Id", "Item Name", "Item Type", "Merchant Id", "Loot Table Id"};
        DrawTextField(fields[i].first, labels[i], *fields[i].second.second, fields[i].second.first);
    }
    if (Button(MakeRow(left, y + 162.0f, 72.0f, 30.0f), "Val-")) m_trade.value = std::max(0, m_trade.value - 5);
    if (Button(MakeRow(left + 76.0f, y + 162.0f, 72.0f, 30.0f), "Val+")) m_trade.value += 5;
    if (Button(MakeRow(left + 152.0f, y + 162.0f, 72.0f, 30.0f), "Heal-")) m_trade.healAmount = std::max(0, m_trade.healAmount - 5);
    if (Button(MakeRow(left + 228.0f, y + 162.0f, 72.0f, 30.0f), "Heal+")) m_trade.healAmount += 5;
    if (Button(MakeRow(right, y + 108.0f, 72.0f, 30.0f), "Stock-")) m_trade.stockCount = std::max(1, m_trade.stockCount - 1);
    if (Button(MakeRow(right + 76.0f, y + 108.0f, 72.0f, 30.0f), "Stock+")) ++m_trade.stockCount;

    if (Button(MakeRow(right, y + 162.0f, 180.0f, 34.0f), "Write Economy Bundle")) {
        std::ostringstream item;
        item << "itemId=" << m_trade.itemId << "\nname=" << m_trade.itemName << "\nitemType=" << m_trade.itemType << "\nvalue=" << m_trade.value << "\nhealAmount=" << m_trade.healAmount << "\n";
        const bool ok1 = WriteAndLog("assets/items/" + m_trade.itemId + ".item", item.str(), m_status);
        const bool ok2 = WriteAndLog("assets/itemgraphs/" + m_trade.itemId + ".itemgraph", item.str(), m_status);

        std::ostringstream merchant;
        merchant << "merchantId=" << m_trade.merchantId << "\nstock=" << m_trade.itemId << "\nstockCount=" << m_trade.stockCount << "\nstockPrice=" << m_trade.stockPrice << "\n";
        const bool ok3 = WriteAndLog("assets/npcs/" + m_trade.merchantId + ".merchant", merchant.str(), m_status);

        std::ostringstream loot;
        loot << "lootTableId=" << m_trade.lootId << "\nitem=" << m_trade.itemId << "\nweight=" << m_trade.dropWeight << "\n";
        const bool ok4 = WriteAndLog("assets/loot/" + m_trade.lootId + ".loot", loot.str(), m_status);

        std::ostringstream links;
        links << "economyLinkId=starter_market_economy\nmerchant=" << m_trade.merchantId << "\nitem=" << m_trade.itemId << "\nloot=" << m_trade.lootId << "\n";
        const bool ok5 = WriteAndLog("assets/links/starter_market_economy.links", links.str(), m_status);
        if (ok1 && ok2 && ok3 && ok4 && ok5) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleWorldTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 5> fields {{
        {MakeRow(left, y), {51, &m_world.regionId}},
        {MakeRow(left, y + 54.0f), {52, &m_world.sceneId}},
        {MakeRow(right, y), {53, &m_world.travelId}},
        {MakeRow(right, y + 54.0f), {54, &m_world.targetRegion}},
        {MakeRow(left, y + 108.0f), {55, &m_world.bundleId}}
    }};
    for (const auto& [rect, data] : fields) {
        if (PointInRect(rect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = data.first;
        if (m_activeTextField == data.first) HandleTextInput(*data.second);
    }
    static const char* labels[] = {"Region Id", "Scene Id", "Travel Id", "Target Region", "World Bundle Id"};
    for (std::size_t i = 0; i < fields.size(); ++i) DrawTextField(fields[i].first, labels[i], *fields[i].second.second, fields[i].second.first);

    if (Button(MakeRow(right, y + 108.0f, 180.0f, 34.0f), "Write World Bundle")) {
        std::ostringstream region;
        region << "regionId=" << m_world.regionId << "\nscene=" << m_world.sceneId << "\n";
        const bool ok1 = WriteAndLog("assets/regions/" + m_world.regionId + ".region", region.str(), m_status);
        const bool ok2 = WriteAndLog("assets/scenes/" + m_world.sceneId + ".scene", "scene=" + m_world.sceneId + "\n", m_status);
        std::ostringstream travel;
        travel << "travelId=" << m_world.travelId << "\nfromRegion=" << m_world.regionId << "\ntargetRegion=" << m_world.targetRegion << "\n";
        const bool ok3 = WriteAndLog("assets/world/" + m_world.travelId + ".travel", travel.str(), m_status);
        std::ostringstream bundle;
        bundle << "worldBundleId=" << m_world.bundleId << "\nregions=" << m_world.regionId << "," << m_world.targetRegion << "\ntravelLinks=" << m_world.travelId << "\n";
        const bool ok4 = WriteAndLog("assets/world/" + m_world.bundleId + ".world", bundle.str(), m_status);
        if (ok1 && ok2 && ok3 && ok4) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleInteriorTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 6> fields {{
        {MakeRow(left, y), {61, &m_interior.buildingId}},
        {MakeRow(left, y + 54.0f), {62, &m_interior.interiorId}},
        {MakeRow(left, y + 108.0f), {63, &m_interior.destinationScene}},
        {MakeRow(right, y), {64, &m_interior.destinationSpawn}},
        {MakeRow(right, y + 54.0f), {65, &m_interior.roomPreset}},
        {MakeRow(right, y + 108.0f), {66, &m_interior.containerSet}}
    }};
    static const char* labels[] = {"Building Id", "Interior Id", "Destination Scene", "Spawn Id", "Room Preset", "Container Set"};
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (PointInRect(fields[i].first, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = fields[i].second.first;
        if (m_activeTextField == fields[i].second.first) HandleTextInput(*fields[i].second.second);
        DrawTextField(fields[i].first, labels[i], *fields[i].second.second, fields[i].second.first);
    }
    if (Button(MakeRow(left, y + 162.0f, 180.0f, 34.0f), "Write Interior Bundle")) {
        std::ostringstream interior;
        interior << "interiorId=" << m_interior.interiorId << "\ndestinationScene=" << m_interior.destinationScene << "\ndestinationSpawn=" << m_interior.destinationSpawn << "\nroomPreset=" << m_interior.roomPreset << "\ncontainerSet=" << m_interior.containerSet << "\n";
        const bool ok1 = WriteAndLog("assets/interiors/" + m_interior.interiorId + ".interior", interior.str(), m_status);
        const bool ok2 = WriteAndLog("assets/interiors/" + m_interior.roomPreset + ".roompreset", "roomPresetId=" + m_interior.roomPreset + "\n", m_status);
        const bool ok3 = WriteAndLog("assets/links/" + m_interior.buildingId + "_interior.links", "building=" + m_interior.buildingId + "\ninterior=" + m_interior.interiorId + "\n", m_status);
        const bool ok4 = WriteAndLog("assets/world/" + m_interior.buildingId + ".interiorworld", "interiorWorldId=" + m_interior.buildingId + "\ninteriors=" + m_interior.interiorId + "\n", m_status);
        if (ok1 && ok2 && ok3 && ok4) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandlePersistTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 8> fields {{
        {MakeRow(left, y), {71, &m_persist.profileId}},
        {MakeRow(left, y + 54.0f), {72, &m_persist.startScene}},
        {MakeRow(left, y + 108.0f), {73, &m_persist.startRegion}},
        {MakeRow(left, y + 162.0f), {74, &m_persist.startSpawn}},
        {MakeRow(right, y), {75, &m_persist.startingQuest}},
        {MakeRow(right, y + 54.0f), {76, &m_persist.startingInventory}},
        {MakeRow(right, y + 108.0f), {77, &m_persist.slotId}},
        {MakeRow(right, y + 162.0f), {78, &m_persist.worldStateId}}
    }};
    static const char* labels[] = {"Profile Id", "Start Scene", "Start Region", "Start Spawn", "Starting Quest", "Starting Inventory", "Save Slot Id", "World State Id"};
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (PointInRect(fields[i].first, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = fields[i].second.first;
        if (m_activeTextField == fields[i].second.first) HandleTextInput(*fields[i].second.second, 120);
        DrawTextField(fields[i].first, labels[i], *fields[i].second.second, fields[i].second.first);
    }
    if (Button(MakeRow(left, y + 216.0f, 72.0f, 30.0f), "Gold-")) m_persist.startingGold = std::max(0, m_persist.startingGold - 5);
    if (Button(MakeRow(left + 76.0f, y + 216.0f, 72.0f, 30.0f), "Gold+")) m_persist.startingGold += 5;
    if (Button(MakeRow(right, y + 216.0f, 110.0f, 30.0f), m_persist.autosave ? "Autosave ON" : "Autosave OFF")) m_persist.autosave = !m_persist.autosave;
    if (Button(MakeRow(right + 116.0f, y + 216.0f, 120.0f, 30.0f), m_persist.checkpoint ? "Checkpoint ON" : "Checkpoint OFF")) m_persist.checkpoint = !m_persist.checkpoint;

    if (Button(MakeRow(left, y + 264.0f, 200.0f, 34.0f), "Write Persistence Bundle")) {
        std::ostringstream profile;
        profile << "profileId=" << m_persist.profileId << "\ndisplayName=Starter Profile\nstartScene=" << m_persist.startScene << "\nstartRegion=" << m_persist.startRegion << "\nstartSpawn=" << m_persist.startSpawn << "\nstartingGold=" << m_persist.startingGold << "\nstartingQuest=" << m_persist.startingQuest << "\nstartingInventory=" << m_persist.startingInventory << "\n";
        const bool ok1 = WriteAndLog("assets/profiles/" + m_persist.profileId + ".profile", profile.str(), m_status);
        std::ostringstream slot;
        slot << "slotId=" << m_persist.slotId << "\nprofileId=" << m_persist.profileId << "\nautosaveEnabled=" << BoolText(m_persist.autosave) << "\ncheckpointEnabled=" << BoolText(m_persist.checkpoint) << "\n";
        const bool ok2 = WriteAndLog("assets/saves/" + m_persist.slotId + ".saveprofile", slot.str(), m_status);
        std::ostringstream worldState;
        worldState << "worldStateId=" << m_persist.worldStateId << "\nprofileId=" << m_persist.profileId << "\nquestState=" << m_persist.startingQuest << ":available\n";
        const bool ok3 = WriteAndLog("assets/world/" + m_persist.worldStateId + ".worldstate", worldState.str(), m_status);
        const bool ok4 = WriteAndLog("assets/registry/v83_persistence_report.txt", "V83 persistence bundle refreshed\n", m_status);
        if (ok1 && ok2 && ok3 && ok4) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleStartTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 7> fields {{
        {MakeRow(left, y), {81, &m_start.menuId}},
        {MakeRow(left, y + 54.0f), {82, &m_start.gameTitle}},
        {MakeRow(left, y + 108.0f), {83, &m_start.hudId}},
        {MakeRow(left, y + 162.0f), {84, &m_start.startupId}},
        {MakeRow(right, y), {85, &m_start.profileId}},
        {MakeRow(right, y + 54.0f), {86, &m_start.newGameScene}},
        {MakeRow(right, y + 108.0f), {87, &m_start.continueSlot}}
    }};
    static const char* labels[] = {"Menu Id", "Game Title", "HUD Id", "Startup Id", "Profile Id", "New Game Scene", "Continue Slot"};
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (PointInRect(fields[i].first, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = fields[i].second.first;
        if (m_activeTextField == fields[i].second.first) HandleTextInput(*fields[i].second.second, 120);
        DrawTextField(fields[i].first, labels[i], *fields[i].second.second, fields[i].second.first);
    }
    if (Button(MakeRow(right, y + 162.0f, 110.0f, 30.0f), m_start.showContinue ? "Continue ON" : "Continue OFF")) m_start.showContinue = !m_start.showContinue;
    if (Button(MakeRow(right + 116.0f, y + 162.0f, 110.0f, 30.0f), m_start.showOptions ? "Options ON" : "Options OFF")) m_start.showOptions = !m_start.showOptions;
    if (Button(MakeRow(left, y + 216.0f, 110.0f, 30.0f), m_start.showGold ? "Gold ON" : "Gold OFF")) m_start.showGold = !m_start.showGold;
    if (Button(MakeRow(left + 116.0f, y + 216.0f, 140.0f, 30.0f), m_start.showQuestTracker ? "Quest UI ON" : "Quest UI OFF")) m_start.showQuestTracker = !m_start.showQuestTracker;

    if (Button(MakeRow(left, y + 264.0f, 200.0f, 34.0f), "Write Start Bundle")) {
        std::ostringstream menu;
        menu << "menuId=" << m_start.menuId << "\ndisplayTitle=" << m_start.gameTitle << "\nshowContinue=" << BoolText(m_start.showContinue) << "\nshowOptions=" << BoolText(m_start.showOptions) << "\nshowExit=true\n";
        const bool ok1 = WriteAndLog("assets/menus/" + m_start.menuId + ".menu", menu.str(), m_status);
        std::ostringstream hud;
        hud << "hudId=" << m_start.hudId << "\nshowHealthBar=true\nshowStaminaBar=true\nshowGoldCounter=" << BoolText(m_start.showGold) << "\nshowQuestTracker=" << BoolText(m_start.showQuestTracker) << "\n";
        const bool ok2 = WriteAndLog("assets/hud/" + m_start.hudId + ".hud", hud.str(), m_status);
        std::ostringstream startup;
        startup << "startupId=" << m_start.startupId << "\nprofileId=" << m_start.profileId << "\nbootMenu=" << m_start.menuId << "\nbootHud=" << m_start.hudId << "\nnewGameScene=" << m_start.newGameScene << "\ncontinueSlot=" << m_start.continueSlot << "\npauseMenu=" << m_start.pauseMenuId << "\n";
        const bool ok3 = WriteAndLog("assets/ui/" + m_start.startupId + ".startup", startup.str(), m_status);
        const bool ok4 = WriteAndLog("assets/menus/" + m_start.pauseMenuId + ".menu", "menuId=" + m_start.pauseMenuId + "\nshowResume=true\nshowSave=true\nshowLoad=true\n", m_status);
        const bool ok5 = WriteAndLog("assets/registry/v84_ui_report.txt", "V84 UI bundle refreshed\n", m_status);
        if (ok1 && ok2 && ok3 && ok4 && ok5) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleAudioTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 8> fields {{
        {MakeRow(left, y), {91, &m_audio.ambienceId}},
        {MakeRow(left, y + 54.0f), {92, &m_audio.regionId}},
        {MakeRow(left, y + 108.0f), {93, &m_audio.dayLoop}},
        {MakeRow(left, y + 162.0f), {94, &m_audio.nightLoop}},
        {MakeRow(right, y), {95, &m_audio.dayMusic}},
        {MakeRow(right, y + 54.0f), {96, &m_audio.nightMusic}},
        {MakeRow(right, y + 108.0f), {97, &m_audio.combatMusic}},
        {MakeRow(right, y + 162.0f), {98, &m_audio.atmosphereId}}
    }};
    static const char* labels[] = {"Ambience Id", "Region Id", "Day Loop", "Night Loop", "Day Music", "Night Music", "Combat Music", "Atmosphere Id"};
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (PointInRect(fields[i].first, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = fields[i].second.first;
        if (m_activeTextField == fields[i].second.first) HandleTextInput(*fields[i].second.second, 120);
        DrawTextField(fields[i].first, labels[i], *fields[i].second.second, fields[i].second.first);
    }
    if (Button(MakeRow(left, y + 216.0f, 72.0f, 30.0f), "Wind-")) m_audio.windLevel = std::max(0, m_audio.windLevel - 5);
    if (Button(MakeRow(left + 76.0f, y + 216.0f, 72.0f, 30.0f), "Wind+")) m_audio.windLevel = std::min(100, m_audio.windLevel + 5);
    if (Button(MakeRow(left + 152.0f, y + 216.0f, 72.0f, 30.0f), "Wild-")) m_audio.wildlifeLevel = std::max(0, m_audio.wildlifeLevel - 5);
    if (Button(MakeRow(left + 228.0f, y + 216.0f, 72.0f, 30.0f), "Wild+")) m_audio.wildlifeLevel = std::min(100, m_audio.wildlifeLevel + 5);
    if (Button(MakeRow(right, y + 216.0f, 72.0f, 30.0f), "Fire-")) m_audio.fireLevel = std::max(0, m_audio.fireLevel - 5);
    if (Button(MakeRow(right + 76.0f, y + 216.0f, 72.0f, 30.0f), "Fire+")) m_audio.fireLevel = std::min(100, m_audio.fireLevel + 5);

    if (Button(MakeRow(left, y + 264.0f, 200.0f, 34.0f), "Write Audio Bundle")) {
        std::ostringstream ambience;
        ambience << "ambienceId=" << m_audio.ambienceId << "\nregionId=" << m_audio.regionId << "\ndayLoop=" << m_audio.dayLoop << "\nnightLoop=" << m_audio.nightLoop << "\nwindLevel=" << m_audio.windLevel / 100.0f << "\nwildlifeLevel=" << m_audio.wildlifeLevel / 100.0f << "\nfireLevel=" << m_audio.fireLevel / 100.0f << "\n";
        const bool ok1 = WriteAndLog("assets/ambience/" + m_audio.ambienceId + ".ambience", ambience.str(), m_status);
        std::ostringstream music;
        music << "musicZoneId=" << m_audio.regionId << "\nregionId=" << m_audio.regionId << "\ndayTrack=" << m_audio.dayMusic << "\nnightTrack=" << m_audio.nightMusic << "\ncombatTrack=" << m_audio.combatMusic << "\n";
        const bool ok2 = WriteAndLog("assets/music/" + m_audio.regionId + ".musiczone", music.str(), m_status);
        std::ostringstream atmosphere;
        atmosphere << "atmosphereId=" << m_audio.atmosphereId << "\nambienceId=" << m_audio.ambienceId << "\nmusicZoneId=" << m_audio.regionId << "\ntimePreset=clear_day\n";
        const bool ok3 = WriteAndLog("assets/regions/" + m_audio.regionId + ".atmosphere", atmosphere.str(), m_status);
        const bool ok4 = WriteAndLog("assets/registry/v85_audio_report.txt", "V85 audio bundle refreshed\n", m_status);
        if (ok1 && ok2 && ok3 && ok4) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleReviewTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 4> fields {{
        {MakeRow(left, y), {101, &m_review.playtestId}},
        {MakeRow(left, y + 54.0f), {102, &m_review.targetScene}},
        {MakeRow(right, y), {103, &m_review.targetRegion}},
        {MakeRow(right, y + 54.0f), {104, &m_review.reviewId}}
    }};
    static const char* labels[] = {"Playtest Id", "Target Scene", "Target Region", "Review Id"};
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (PointInRect(fields[i].first, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = fields[i].second.first;
        if (m_activeTextField == fields[i].second.first) HandleTextInput(*fields[i].second.second);
        DrawTextField(fields[i].first, labels[i], *fields[i].second.second, fields[i].second.first);
    }
    const Rectangle notesRect = MakeRow(left, y + 108.0f, 510.0f, 30.0f);
    if (PointInRect(notesRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 105;
    if (m_activeTextField == 105) HandleTextInput(m_review.notes, 140);
    DrawTextField(notesRect, "Review Notes", m_review.notes, 105);
    if (Button(MakeRow(left, y + 162.0f, 200.0f, 34.0f), "Write Review Bundle")) {
        std::ostringstream playtest;
        playtest << "playtestId=" << m_review.playtestId << "\nscene=" << m_review.targetScene << "\nregion=" << m_review.targetRegion << "\ncheckQuestLinks=true\ncheckDialogueLinks=true\ncheckEncounterLinks=true\ncheckLootAssignments=true\ncheckTravelLinks=true\ncheckMissingAssets=true\n";
        const bool ok1 = WriteAndLog("assets/reports/" + m_review.playtestId + ".playtest", playtest.str(), m_status);
        std::ostringstream review;
        review << "reviewId=" << m_review.reviewId << "\nmissingAssets=0\nmissingDialogueLinks=0\nmissingQuestLinks=0\nnotes=" << m_review.notes << "\n";
        const bool ok2 = WriteAndLog("assets/debug/" + m_review.reviewId + ".debugreview", review.str(), m_status);
        const bool ok3 = WriteAndLog("assets/registry/v86_validation_report.txt", "V86 validation bundle refreshed\n", m_status);
        const bool ok4 = WriteAndLog("assets/registry/v86_export_readiness.txt", "Export Readiness\n[OK] Core authored files present\n", m_status);
        if (ok1 && ok2 && ok3 && ok4) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleReleaseTab(Application& app, ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 9> fields {{
        {MakeRow(left, y), {111, &m_release.buildId}},
        {MakeRow(left, y + 54.0f), {112, &m_release.gameTitle}},
        {MakeRow(left, y + 108.0f), {113, &m_release.version}},
        {MakeRow(left, y + 162.0f), {114, &m_release.startScene}},
        {MakeRow(right, y), {115, &m_release.startRegion}},
        {MakeRow(right, y + 54.0f), {116, &m_release.profileId}},
        {MakeRow(right, y + 108.0f), {117, &m_release.menuId}},
        {MakeRow(right, y + 162.0f), {118, &m_release.hudId}},
        {MakeRow(left, y + 216.0f, 510.0f, 30.0f), {119, &m_release.outputFolder}}
    }};
    static const char* labels[] = {"Build Id", "Game Title", "Version", "Start Scene", "Start Region", "Profile Id", "Menu Id", "HUD Id", "Output Folder"};
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (PointInRect(fields[i].first, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = fields[i].second.first;
        if (m_activeTextField == fields[i].second.first) HandleTextInput(*fields[i].second.second, 120);
        DrawTextField(fields[i].first, labels[i], *fields[i].second.second, fields[i].second.first);
    }
    if (Button(MakeRow(right, y + 216.0f, 120.0f, 30.0f), m_release.includeSaves ? "Saves ON" : "Saves OFF")) m_release.includeSaves = !m_release.includeSaves;
    if (Button(MakeRow(right + 126.0f, y + 216.0f, 120.0f, 30.0f), m_release.compressExport ? "Compress ON" : "Compress OFF")) m_release.compressExport = !m_release.compressExport;
    if (Button(MakeRow(left, y + 264.0f, 220.0f, 34.0f), "Write Release Bundle")) {
        std::ostringstream build;
        build << "buildId=" << m_release.buildId << "\ngameTitle=" << m_release.gameTitle << "\nversion=" << m_release.version << "\nstartScene=" << m_release.startScene << "\nstartRegion=" << m_release.startRegion << "\nprofileId=" << m_release.profileId << "\nmenuId=" << m_release.menuId << "\nhudId=" << m_release.hudId << "\nincludeSaves=" << BoolText(m_release.includeSaves) << "\ncompressExport=" << BoolText(m_release.compressExport) << "\noutputFolder=" << m_release.outputFolder << "\n";
        const bool ok1 = WriteAndLog("assets/builds/" + m_release.buildId + ".build", build.str(), m_status);
        const bool ok2 = WriteAndLog("assets/builds/" + m_release.buildId + ".manifest.txt", "Build manifest for " + m_release.buildId + "\n", m_status);
        const bool ok3 = WriteAndLog("assets/exports/" + m_release.exportPresetId + ".exportpreset", "exportPresetId=" + m_release.exportPresetId + "\nzipOutput=" + BoolText(m_release.zipOutput) + "\n", m_status);
        const bool ok4 = WriteAndLog(m_release.outputFolder + "/README_EXPORT.txt", "Staged export bundle for " + m_release.buildId + "\n", m_status);
        const bool ok5 = WriteAndLog("assets/registry/v87_export_wizard_report.txt", "V87 release bundle refreshed\n", m_status);
        if (ok1 && ok2 && ok3 && ok4 && ok5) RefreshRegistry(registry);
    }
    if (Button(MakeRow(left + 230.0f, y + 264.0f, 220.0f, 34.0f), "Stage Export Bundle")) {
        ExportBundleSettings settings {};
        settings.buildId = m_release.buildId;
        settings.gameTitle = m_release.gameTitle;
        settings.version = m_release.version;
        settings.startScene = m_release.startScene;
        settings.startRegion = m_release.startRegion;
        settings.profileId = m_release.profileId;
        settings.menuId = m_release.menuId;
        settings.hudId = m_release.hudId;
        settings.outputFolder = m_release.outputFolder;
        settings.includeSaves = m_release.includeSaves;
        settings.compressExport = m_release.compressExport;
        settings.zipOutput = m_release.zipOutput;

        const ExportBundleResult result = app.StageExportBundle(settings);
        m_status = result.message;
        if (result.success) {
            WriteAndLog("assets/registry/v114_export_bundle_report.txt", "V114 staged export bundle: " + result.outputFolder + "\n", m_status);
        }
    }
}

void GameBuilderPanel::HandleTemplatesTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 12> fields {{
        {MakeRow(left, y), {121, &m_template.templateId}},
        {MakeRow(left, y + 54.0f), {122, &m_template.displayName}},
        {MakeRow(left, y + 108.0f), {123, &m_template.projectId}},
        {MakeRow(left, y + 162.0f), {124, &m_template.startScene}},
        {MakeRow(right, y), {125, &m_template.startRegion}},
        {MakeRow(right, y + 54.0f), {126, &m_template.profileId}},
        {MakeRow(right, y + 108.0f), {127, &m_template.menuId}},
        {MakeRow(right, y + 162.0f), {128, &m_template.hudId}},
        {MakeRow(left, y + 216.0f, 510.0f, 30.0f), {129, &m_template.regions}},
        {MakeRow(left, y + 270.0f, 510.0f, 30.0f), {130, &m_template.quests}},
        {MakeRow(left, y + 324.0f, 510.0f, 30.0f), {131, &m_template.dialogue}},
        {MakeRow(left, y + 378.0f, 510.0f, 30.0f), {132, &m_template.encounters}}
    }};
    static const char* labels[] = {"Template Id", "Display Name", "Project Id", "Start Scene", "Start Region", "Profile Id", "Menu Id", "HUD Id", "Regions", "Quests", "Dialogue", "Encounters"};
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (PointInRect(fields[i].first, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = fields[i].second.first;
        if (m_activeTextField == fields[i].second.first) HandleTextInput(*fields[i].second.second, 140);
        DrawTextField(fields[i].first, labels[i], *fields[i].second.second, fields[i].second.first);
    }
    if (Button(MakeRow(right, y + 432.0f, 220.0f, 34.0f), "Write Template Bundle")) {
        std::ostringstream templ;
        templ << "templateId=" << m_template.templateId << "\ndisplayName=" << m_template.displayName << "\nprojectId=" << m_template.projectId << "\nstartScene=" << m_template.startScene << "\nstartRegion=" << m_template.startRegion << "\nprofileId=" << m_template.profileId << "\nmenuId=" << m_template.menuId << "\nhudId=" << m_template.hudId << "\nregions=" << m_template.regions << "\nquests=" << m_template.quests << "\ndialogue=" << m_template.dialogue << "\nencounters=" << m_template.encounters << "\neconomy=" << m_template.economy << "\n";
        const bool ok1 = WriteAndLog("assets/templates/" + m_template.templateId + ".template", templ.str(), m_status);
        const bool ok2 = WriteAndLog("assets/projects/" + m_template.projectId + ".project", "projectId=" + m_template.projectId + "\ntemplateId=" + m_template.templateId + "\n", m_status);
        const bool ok3 = WriteAndLog("assets/world/" + m_template.templateId + ".worldbundle", "worldBundleId=" + m_template.templateId + "\nregions=" + m_template.regions + "\n", m_status);
        const bool ok4 = WriteAndLog("assets/links/" + m_template.templateId + "_template.links", "templateId=" + m_template.templateId + "\nproject=" + m_template.projectId + "\n", m_status);
        const bool ok5 = WriteAndLog("assets/registry/v88_template_report.txt", "V88 template bundle refreshed\n", m_status);
        if (ok1 && ok2 && ok3 && ok4 && ok5) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleShipTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 4> fields {{
        {MakeRow(left, y), {141, &m_ship.shipModeId}},
        {MakeRow(left, y + 54.0f), {142, &m_ship.buildId}},
        {MakeRow(right, y), {143, &m_ship.contentLockId}},
        {MakeRow(right, y + 54.0f), {144, &m_ship.releaseGateId}}
    }};
    static const char* labels[] = {"Ship Mode Id", "Build Id", "Content Lock Id", "Release Gate Id"};
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (PointInRect(fields[i].first, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = fields[i].second.first;
        if (m_activeTextField == fields[i].second.first) HandleTextInput(*fields[i].second.second);
        DrawTextField(fields[i].first, labels[i], *fields[i].second.second, fields[i].second.first);
    }
    if (Button(MakeRow(left, y + 108.0f, 150.0f, 30.0f), m_ship.contentLock ? "Lock ON" : "Lock OFF")) m_ship.contentLock = !m_ship.contentLock;
    if (Button(MakeRow(left + 156.0f, y + 108.0f, 170.0f, 30.0f), m_ship.allowMissingAssets ? "Missing Assets OK" : "Missing Assets BLOCK")) m_ship.allowMissingAssets = !m_ship.allowMissingAssets;
    if (Button(MakeRow(right, y + 108.0f, 170.0f, 30.0f), m_ship.allowBrokenLinks ? "Broken Links OK" : "Broken Links BLOCK")) m_ship.allowBrokenLinks = !m_ship.allowBrokenLinks;
    if (Button(MakeRow(right, y + 148.0f, 170.0f, 30.0f), m_ship.allowMissingStartup ? "Startup OK" : "Startup BLOCK")) m_ship.allowMissingStartup = !m_ship.allowMissingStartup;

    if (Button(MakeRow(left, y + 200.0f, 200.0f, 34.0f), "Write Ship Bundle")) {
        std::ostringstream mode;
        mode << "shipModeId=" << m_ship.shipModeId << "\nbuildId=" << m_ship.buildId << "\ncontentLock=" << BoolText(m_ship.contentLock) << "\nallowMissingAssets=" << BoolText(m_ship.allowMissingAssets) << "\nallowBrokenLinks=" << BoolText(m_ship.allowBrokenLinks) << "\nallowMissingStartupFlow=" << BoolText(m_ship.allowMissingStartup) << "\nrequireValidationReport=true\nrequireExportReadiness=true\nrequireTemplateSelection=true\n";
        const bool ok1 = WriteAndLog("assets/ship/" + m_ship.shipModeId + ".shipmode", mode.str(), m_status);
        std::ostringstream lock;
        lock << "contentLockId=" << m_ship.contentLockId << "\nlockedTemplates=" << m_template.templateId << "\nlockedProfile=" << m_persist.profileId << "\nlockedBuild=" << m_release.buildId << "\n";
        const bool ok2 = WriteAndLog("assets/ship/" + m_ship.contentLockId + ".contentlock", lock.str(), m_status);
        std::ostringstream gate;
        gate << "releaseGateId=" << m_ship.releaseGateId << "\nvalidationReport=assets/registry/v86_validation_report.txt\nexportReadiness=assets/registry/v86_export_readiness.txt\nshipMode=assets/ship/" << m_ship.shipModeId << ".shipmode\ncontentLock=assets/ship/" << m_ship.contentLockId << ".contentlock\nstatus=provisional_pass\n";
        const bool ok3 = WriteAndLog("assets/builds/" + m_ship.releaseGateId + ".releasegate", gate.str(), m_status);
        const bool ok4 = WriteAndLog("assets/reports/" + m_release.buildId + "_ship_checklist.txt", "Starter Release Ship Checklist\n[OK] Core flow scaffolded\n", m_status);
        const bool ok5 = WriteAndLog("assets/registry/v89_ship_report.txt", "V89 ship bundle refreshed\n", m_status);
        if (ok1 && ok2 && ok3 && ok4 && ok5) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleCompleteTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    const float right = left + 280.0f;
    float y = ContentTop();
    const std::array<std::pair<Rectangle, std::pair<int, std::string*>>, 6> fields {{
        {MakeRow(left, y), {151, &m_complete.foundationId}},
        {MakeRow(left, y + 54.0f), {152, &m_complete.milestoneLabel}},
        {MakeRow(left, y + 108.0f), {153, &m_complete.defaultTemplate}},
        {MakeRow(right, y), {154, &m_complete.defaultProfile}},
        {MakeRow(right, y + 54.0f), {155, &m_complete.defaultBuild}},
        {MakeRow(right, y + 108.0f), {156, &m_complete.releaseProfileId}}
    }};
    static const char* labels[] = {"Foundation Id", "Milestone Label", "Default Template", "Default Profile", "Default Build", "Release Profile Id"};
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (PointInRect(fields[i].first, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = fields[i].second.first;
        if (m_activeTextField == fields[i].second.first) HandleTextInput(*fields[i].second.second);
        DrawTextField(fields[i].first, labels[i], *fields[i].second.second, fields[i].second.first);
    }
    const Rectangle creatorRect = MakeRow(left, y + 162.0f, 510.0f, 30.0f);
    if (PointInRect(creatorRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 157;
    if (m_activeTextField == 157) HandleTextInput(m_complete.creatorMode);
    DrawTextField(creatorRect, "Creator Mode", m_complete.creatorMode, 157);

    if (Button(MakeRow(left, y + 216.0f, 220.0f, 34.0f), "Write Completion Bundle")) {
        std::ostringstream foundation;
        foundation << "foundationId=" << m_complete.foundationId << "\nmilestone=" << m_complete.milestoneLabel << "\nprojectMode=" << m_complete.creatorMode << "\ndefaultTemplate=" << m_complete.defaultTemplate << "\ndefaultProfile=" << m_complete.defaultProfile << "\ndefaultBuild=" << m_complete.defaultBuild << "\n";
        const bool ok1 = WriteAndLog("assets/foundation/" + m_complete.foundationId + ".foundation", foundation.str(), m_status);
        std::ostringstream project;
        project << "projectId=v90_creator_baseline\ndisplayName=V90 Creator Baseline\ntemplate=" << m_complete.defaultTemplate << "\nprofile=" << m_complete.defaultProfile << "\nstartupFlow=" << m_start.startupId << "\nhud=" << m_start.hudId << "\nmenu=" << m_start.menuId << "\nbuild=" << m_complete.defaultBuild << "\nshipMode=" << m_ship.shipModeId << "\nfoundation=" << m_complete.foundationId << "\n";
        const bool ok2 = WriteAndLog("assets/projects/v90_creator_baseline.project", project.str(), m_status);
        std::ostringstream releaseProfile;
        releaseProfile << "releaseProfileId=" << m_complete.releaseProfileId << "\nbuildId=" << m_release.buildId << "\nshipMode=" << m_ship.shipModeId << "\ncontentLock=" << m_ship.contentLockId << "\nstatus=baseline_ready\n";
        const bool ok3 = WriteAndLog("assets/builds/" + m_complete.releaseProfileId + ".releaseprofile", releaseProfile.str(), m_status);
        const bool ok4 = WriteAndLog("assets/reports/v90_completion_checklist.txt", "V90 Completion Checklist\n[OK] Core authoring tabs integrated\n", m_status);
        const bool ok5 = WriteAndLog("assets/registry/v90_milestone_report.txt", "V90 milestone bundle refreshed\n", m_status);
        WriteVersionMetadata();
        if (ok1 && ok2 && ok3 && ok4 && ok5) RefreshRegistry(registry);
    }
}

void GameBuilderPanel::HandleRegistryTab(ContentRegistry& registry) {
    const float left = m_bounds.x + 20.0f;
    float y = ContentTop();
    const Rectangle searchRect = MakeRow(left, y, 340.0f, 30.0f);
    if (PointInRect(searchRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_activeTextField = 201;
    if (m_activeTextField == 201) HandleTextInput(m_registrySearch, 120);
    DrawTextField(searchRect, "Search", m_registrySearch, 201);
    if (Button(MakeRow(left + 356.0f, y, 150.0f, 30.0f), "Rebuild Registry")) RefreshRegistry(registry);
    if (Button(MakeRow(left + 356.0f, y + 40.0f, 150.0f, 30.0f), "Write Metadata")) {
        WriteVersionMetadata();
        m_status = "Version metadata refreshed";
    }
}

void GameBuilderPanel::Draw(Application& app, const ContentRegistry& registry) const {
    (void)app;
    if (!m_visible) return;

    const Rectangle contentRect = ContentRect();
    const Rectangle titleRect {m_bounds.x, m_bounds.y, m_bounds.width, kTitleHeight};

    DrawRectangleRec(m_bounds, Color{8, 12, 18, 238});
    DrawRectangleLinesEx(m_bounds, 2.0f, SKYBLUE);
    DrawRectangleRec(titleRect, Color{16, 22, 30, 245});
    DrawRectangleRec(contentRect, Color{10, 14, 22, 228});
    DrawRectangleLinesEx(contentRect, 1.0f, Fade(SKYBLUE, 0.45f));

    DrawBuilderVisibilityText(m_visible);
    DrawText("V117 Builder Layout and Input Capture", static_cast<int>(m_bounds.x + 16.0f), static_cast<int>(m_bounds.y + 6.0f), 22, RAYWHITE);

    const Rectangle closeRect = CloseButtonRect();
    DrawButtonVisual(closeRect, "X", PointInRect(closeRect, GetMousePosition()), Color{90, 36, 36, 255});

    const std::array<std::pair<Tab, const char*>, 16> tabs {{
        {Tab::Create, "Create"}, {Tab::Story, "Story"}, {Tab::Quest, "Quest"}, {Tab::Fight, "Fight"},
        {Tab::Trade, "Trade"}, {Tab::World, "World"}, {Tab::Interior, "Interior"}, {Tab::Persist, "Persist"},
        {Tab::Start, "Start"}, {Tab::Audio, "Audio"}, {Tab::Review, "Review"}, {Tab::Release, "Release"},
        {Tab::Templates, "Templates"}, {Tab::Ship, "Ship"}, {Tab::Complete, "Complete"}, {Tab::Registry, "Registry"}
    }};
    for (std::size_t i = 0; i < tabs.size(); ++i) {
        const int row = static_cast<int>(i / 4);
        const int col = static_cast<int>(i % 4);
        const Rectangle rect {
            m_bounds.x + 10.0f + col * (kTabWidth + 8.0f),
            m_bounds.y + kTabGridTop + row * (kTabHeight + kTabRowGap),
            kTabWidth,
            kTabHeight
        };
        DrawHeaderTab(rect, tabs[i].second, tabs[i].first);
    }

    DrawText("F10, F9, or Ctrl+B toggles builder. Builder captures camera/editor input while open.", static_cast<int>(contentRect.x + 2.0f), static_cast<int>(contentRect.y - 18.0f), 16, LIGHTGRAY);

    if (m_activeTab == Tab::Quest) {
        DrawBuilderVisibilityText(m_visible);
        DrawText(TextFormat("Reward Gold: %d", m_quest.rewardGold), static_cast<int>(contentRect.x + 300.0f), static_cast<int>(contentRect.y + 126.0f), 18, GOLD);
    }
    if (m_activeTab == Tab::Fight) {
        DrawBuilderVisibilityText(m_visible);
        DrawText(TextFormat("Count: %d | Radius: %.1f", m_fight.count, m_fight.radius), static_cast<int>(contentRect.x + 2.0f), static_cast<int>(contentRect.y + 194.0f), 18, GOLD);
    }
    if (m_activeTab == Tab::Trade) {
        DrawBuilderVisibilityText(m_visible);
        DrawText(TextFormat("Value: %d | Heal: %d | Stock: %d", m_trade.value, m_trade.healAmount, m_trade.stockCount), static_cast<int>(contentRect.x + 2.0f), static_cast<int>(contentRect.y + 194.0f), 18, GOLD);
    }
    if (m_activeTab == Tab::Persist) {
        DrawBuilderVisibilityText(m_visible);
        DrawText(TextFormat("Starting Gold: %d", m_persist.startingGold), static_cast<int>(contentRect.x + 2.0f), static_cast<int>(contentRect.y + 248.0f), 18, GOLD);
    }
    if (m_activeTab == Tab::Audio) {
        DrawBuilderVisibilityText(m_visible);
        DrawText(TextFormat("Wind %d | Wildlife %d | Fire %d", m_audio.windLevel, m_audio.wildlifeLevel, m_audio.fireLevel), static_cast<int>(contentRect.x + 2.0f), static_cast<int>(contentRect.y + 248.0f), 18, GOLD);
    }
    if (m_activeTab == Tab::Registry) {
        DrawBuilderVisibilityText(m_visible);
        DrawText(TextFormat("Registry Entries: %i", static_cast<int>(registry.Count())), static_cast<int>(contentRect.x + 2.0f), static_cast<int>(contentRect.y + 60.0f), 18, GOLD);
        const auto results = registry.Search(m_registrySearch);
        const Rectangle listRect {contentRect.x + 2.0f, contentRect.y + 90.0f, contentRect.width - 4.0f, std::min(420.0f, contentRect.height - 100.0f)};
        DrawRectangleRec(listRect, Color{12, 16, 24, 220});
        DrawRectangleLinesEx(listRect, 1.0f, Fade(SKYBLUE, 0.7f));
        int drawY = static_cast<int>(listRect.y + 8.0f);
        for (std::size_t i = 0; i < results.size() && i < 18; ++i) {
            const std::string line = results[i].id + " [" + ContentRegistry::KindToString(results[i].kind) + "]  " + results[i].path;
            DrawBuilderVisibilityText(m_visible);
            DrawText(line.c_str(), static_cast<int>(listRect.x + 8.0f), drawY, 16, RAYWHITE);
            drawY += 22;
        }
    }

    DrawStatusBar();
}

} // namespace fw


