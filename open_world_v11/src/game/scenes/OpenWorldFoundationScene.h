#pragma once
#include <vector>
#include <string>
#include "raylib.h"
#include "game/content/ContentPipeline.h"
#include "game/content/validation/ContentValidator.h"
#include "game/save/SaveGameProfile.h"
#include "game/simulation/WorldClock.h"
#include "game/simulation/RoutineDatabase.h"
#include "game/simulation/NpcRoutineSystem.h"
#include "game/world/RegionSimulationState.h"
#include "game/world/RegionLayoutDatabase.h"
#include "game/world/WorldStatePersistence.h"
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

namespace fw
{
    class OpenWorldFoundationScene
    {
    public:
        void Load();
        void Update(float dt);
        void Draw();
        void Unload();

    private:
        void RebuildRegionState();
        void EnsureCurrentRegionState();
        void ResetCurrentRegionStateFromLayout();

        ContentPipeline m_pipeline;
        ContentValidator m_validator;
        SaveGameProfile m_profile;
        WorldClock m_clock;
        RoutineDatabase m_routines;
        NpcRoutineSystem m_npcRoutineSystem;
        RegionSimulationState m_regionState;
        std::vector<ActiveNpcRoutineInfo> m_activeRoutines;
        RegionLayoutDatabase m_layouts;
        WorldStatePersistence m_worldState;
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
    };
}
