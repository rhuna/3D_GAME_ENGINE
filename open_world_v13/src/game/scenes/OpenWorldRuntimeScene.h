#pragma once

#include <string>
#include <vector>

#include "raylib.h"
#include "scene/Scene.h"
#include "game/content/ContentPipeline.h"
#include "game/save/SaveGameProfile.h"
#include "game/simulation/WorldClock.h"
#include "game/simulation/RoutineDatabase.h"
#include "game/simulation/NpcRoutineSystem.h"
#include "game/world/RegionSimulationState.h"
#include "game/world/RegionLayoutDatabase.h"
#include "game/world/WorldStatePersistence.h"
#include "game/world/RegionEcsLoader.h"
#include "game/render/WorldRenderer.h"
#include "game/gameplay/InteractionSystem.h"
#include "game/gameplay/QuestGameplaySystem.h"
#include "game/gameplay/GatheringSystem.h"
#include "game/combat/EnemySystem.h"
#include "game/combat/CombatSystem.h"
#include "game/progression/EquipmentSystem.h"
#include "game/progression/StatSystem.h"
#include "game/progression/ProgressionSystem.h"
#include "game/loot/LootSystem.h"
#include "game/faction/FactionSystem.h"
#include "game/ui/DialogueBox.h"
#include "game/project/GameProjectDefinition.h"

namespace fw {

class OpenWorldRuntimeScene : public Scene {
public:
    const char* Name() const override { return "OpenWorldRuntimeScene"; }
    void OnEnter(Application& app) override;
    void OnExit(Application& app) override;
    void Update(Application& app, float dt) override;
    void FixedUpdate(Application& app, float fixedDeltaTime) override;
    void Render(Application& app) override;
    void DrawUi(Application& app) override;

private:
    void RebuildRegionState(Application& app, bool rebuildEcsWorld = true);
    void EnsureCurrentRegionState();
    void ResetCurrentRegionStateFromLayout();
    void ApplyProjectBootstrap(const GameProjectDefinition& project);

    ContentPipeline m_pipeline;
    SaveGameProfile m_profile;
    WorldClock m_clock;
    RoutineDatabase m_routines;
    NpcRoutineSystem m_npcRoutineSystem;
    RegionSimulationState m_regionState;
    std::vector<ActiveNpcRoutineInfo> m_activeRoutines;
    RegionLayoutDatabase m_layouts;
    WorldStatePersistence m_worldState;
    RegionEcsLoader m_regionEcsLoader;
    WorldRenderer m_renderer;
    Camera3D m_camera{};
    float m_cameraYaw = 0.0f;
    float m_cameraPitch = 0.35f;
    float m_verticalVelocity = 0.0f;
    bool m_grounded = true;
    std::vector<Vector3> m_activeNpcPositions;
    InteractionSystem m_interactionSystem;
    QuestGameplaySystem m_questGameplaySystem;
    GatheringSystem m_gatheringSystem;
    EnemySystem m_enemySystem;
    CombatSystem m_combatSystem;
    EquipmentSystem m_equipmentSystem;
    StatSystem m_statSystem;
    ProgressionState m_progression;
    ProgressionSystem m_progressionSystem;
    LootSystem m_lootSystem;
    FactionSystem m_factionSystem;
    DialogueBoxState m_dialogue;
    std::string m_interactionPrompt;
    std::string m_statusText;
    float m_statusTextTimer = 0.0f;
    bool m_showQuestLog = true;
    bool m_showEquipment = true;
    GameProjectDefinition m_project;
    std::vector<std::string> m_regionDebugOrder;
};

} // namespace fw
