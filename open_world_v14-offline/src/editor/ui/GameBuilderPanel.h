#pragma once

#include <string>
#include <vector>

#include "raylib.h"

namespace fw {

class Application;
class ContentRegistry;

class GameBuilderPanel {
public:
    void ToggleVisible();
    void SetVisible(bool visible);
    [[nodiscard]] bool IsVisible() const;
    [[nodiscard]] bool IsMouseOverUi() const;

    void LoadSessionState();
    void SaveSessionState() const;

    void Update(Application& app, ContentRegistry& registry);
    void Draw(Application& app, const ContentRegistry& registry) const;

private:
    enum class Tab {
        Create,
        Story,
        Quest,
        Fight,
        Trade,
        World,
        Interior,
        Persist,
        Start,
        Audio,
        Review,
        Release,
        Templates,
        Ship,
        Complete,
        Registry
    };

    struct CreateDraft {
        std::string sceneId = "starter_slice";
        std::string prefabId = "starter_prop";
        std::string npcId = "guide_mira";
    };

    struct StoryDraft {
        std::string dialogueId = "guide_mira_intro";
        std::string npcId = "guide_mira";
        std::string openingText = "Welcome to the vale.";
        std::string choiceOne = "I need help.";
        std::string choiceTwo = "Just passing through.";
    };

    struct QuestDraft {
        std::string questId = "first_steps";
        std::string title = "First Steps";
        std::string objectiveType = "collect";
        std::string objectiveTarget = "forest_herb";
        std::string rewardItem = "healing_potion";
        std::string nextQuestId = "wolf_watch";
        int rewardGold = 25;
    };

    struct FightDraft {
        std::string enemyId = "wolf_raider";
        std::string displayName = "Wolf Raider";
        std::string prefabId = "encounter_wolf";
        std::string behavior = "roam";
        std::string encounterId = "starter_wolves";
        std::string lootTable = "wolf";
        int count = 3;
        float radius = 10.0f;
    };

    struct TradeDraft {
        std::string itemId = "healing_potion";
        std::string itemName = "Healing Potion";
        std::string itemType = "consumable";
        std::string merchantId = "starter_merchant";
        std::string lootId = "starter_field_loot";
        int value = 15;
        int healAmount = 30;
        int stockCount = 5;
        int stockPrice = 15;
        int dropWeight = 50;
    };

    struct WorldDraft {
        std::string regionId = "starter_village";
        std::string sceneId = "starter_village";
        std::string travelId = "starter_town_gate";
        std::string targetRegion = "western_forest";
        std::string bundleId = "village_rpg";
    };

    struct InteriorDraft {
        std::string buildingId = "starter_shop";
        std::string interiorId = "starter_shop";
        std::string destinationScene = "herbalist_hut_interior";
        std::string destinationSpawn = "entry";
        std::string roomPreset = "shop_basic";
        std::string containerSet = "starter_shop_containers";
    };

    struct PersistDraft {
        std::string profileId = "starter";
        std::string startScene = "starter_village";
        std::string startRegion = "starter_village";
        std::string startSpawn = "player_start";
        std::string startingQuest = "first_steps";
        std::string startingInventory = "healing_potion,wood_sword";
        std::string slotId = "slot_01";
        std::string worldStateId = "starter_persistence";
        int startingGold = 25;
        bool autosave = true;
        bool checkpoint = true;
    };

    struct StartDraft {
        std::string menuId = "main_menu";
        std::string gameTitle = "Starter Adventure";
        std::string hudId = "starter_hud";
        std::string startupId = "starter_startup";
        std::string profileId = "starter";
        std::string newGameScene = "starter_village";
        std::string continueSlot = "slot_01";
        std::string pauseMenuId = "pause_menu";
        bool showContinue = true;
        bool showOptions = true;
        bool showGold = true;
        bool showQuestTracker = true;
    };

    struct AudioDraft {
        std::string ambienceId = "starter_village";
        std::string regionId = "starter_village";
        std::string dayLoop = "audio/ambience/village_day.ogg";
        std::string nightLoop = "audio/ambience/village_night.ogg";
        std::string dayMusic = "audio/music/town_theme_day.ogg";
        std::string nightMusic = "audio/music/town_theme_night.ogg";
        std::string combatMusic = "audio/music/town_combat.ogg";
        std::string atmosphereId = "starter_village";
        int windLevel = 20;
        int wildlifeLevel = 35;
        int fireLevel = 10;
    };

    struct ReviewDraft {
        std::string playtestId = "starter_slice";
        std::string targetScene = "starter_village";
        std::string targetRegion = "starter_village";
        std::string reviewId = "starter_slice";
        std::string notes = "Review encounter rewards before ship.";
    };

    struct ReleaseDraft {
        std::string buildId = "starter_release";
        std::string gameTitle = "Starter Adventure";
        std::string version = "0.1.0";
        std::string startScene = "starter_village";
        std::string startRegion = "starter_village";
        std::string profileId = "starter";
        std::string menuId = "main_menu";
        std::string hudId = "starter_hud";
        std::string outputFolder = "exports/starter_build";
        std::string exportPresetId = "starter_release";
        bool includeSaves = false;
        bool compressExport = true;
        bool zipOutput = true;
    };

    struct TemplateDraft {
        std::string templateId = "village_rpg";
        std::string displayName = "Village RPG";
        std::string projectId = "village_rpg_project";
        std::string startScene = "starter_village";
        std::string startRegion = "starter_village";
        std::string profileId = "starter";
        std::string menuId = "main_menu";
        std::string hudId = "starter_hud";
        std::string regions = "starter_village,western_forest";
        std::string quests = "first_steps";
        std::string dialogue = "guide_mira_intro";
        std::string encounters = "starter_wolves";
        std::string economy = "starter_market";
    };

    struct ShipDraft {
        std::string shipModeId = "starter_release";
        std::string buildId = "starter_release";
        std::string contentLockId = "starter_release";
        std::string releaseGateId = "starter_release";
        bool contentLock = true;
        bool allowMissingAssets = false;
        bool allowBrokenLinks = false;
        bool allowMissingStartup = false;
    };

    struct CompleteDraft {
        std::string foundationId = "v90";
        std::string milestoneLabel = "completion_baseline";
        std::string defaultTemplate = "village_rpg";
        std::string defaultProfile = "starter";
        std::string defaultBuild = "starter_release";
        std::string releaseProfileId = "v90_completion";
        std::string creatorMode = "creator_first_coder_capable";
    };

    [[nodiscard]] bool Button(const Rectangle& rect, const char* label) const;
    [[nodiscard]] bool PointInRect(const Rectangle& rect, Vector2 point) const;
    void DrawHeaderTab(const Rectangle& rect, const char* label, Tab tab) const;
    void DrawTextField(const Rectangle& rect, const char* label, const std::string& value, int fieldId) const;
    void HandleTextInput(std::string& target, std::size_t maxLength = 96);
    void UpdateTabClicks();
    void DrawStatusBar() const;
    [[nodiscard]] Rectangle CloseButtonRect() const;

    void HandleCreateTab(ContentRegistry& registry);
    void HandleStoryTab(ContentRegistry& registry);
    void HandleQuestTab(ContentRegistry& registry);
    void HandleFightTab(ContentRegistry& registry);
    void HandleTradeTab(ContentRegistry& registry);
    void HandleWorldTab(ContentRegistry& registry);
    void HandleInteriorTab(ContentRegistry& registry);
    void HandlePersistTab(ContentRegistry& registry);
    void HandleStartTab(ContentRegistry& registry);
    void HandleAudioTab(ContentRegistry& registry);
    void HandleReviewTab(ContentRegistry& registry);
    void HandleReleaseTab(Application& app, ContentRegistry& registry);
    void HandleTemplatesTab(ContentRegistry& registry);
    void HandleShipTab(ContentRegistry& registry);
    void HandleCompleteTab(ContentRegistry& registry);
    void HandleRegistryTab(ContentRegistry& registry);

    bool RefreshRegistry(ContentRegistry& registry);
    void WriteVersionMetadata() const;
    [[nodiscard]] static const char* TabToString(Tab tab);
    [[nodiscard]] static Tab TabFromString(const std::string& value);

    Rectangle m_bounds {18.0f, 18.0f, 560.0f, 680.0f};
    bool m_visible = false;
    Tab m_activeTab = Tab::Create;
    int m_activeTextField = 0;
    std::string m_status = "Ready";
    std::string m_registrySearch;

    CreateDraft m_create {};
    StoryDraft m_story {};
    QuestDraft m_quest {};
    FightDraft m_fight {};
    TradeDraft m_trade {};
    WorldDraft m_world {};
    InteriorDraft m_interior {};
    PersistDraft m_persist {};
    StartDraft m_start {};
    AudioDraft m_audio {};
    ReviewDraft m_review {};
    ReleaseDraft m_release {};
    TemplateDraft m_template {};
    ShipDraft m_ship {};
    CompleteDraft m_complete {};
};

} // namespace fw
