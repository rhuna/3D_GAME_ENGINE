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
#include "game/render/WorldRenderer.h"
#include "game/gameplay/InteractionSystem.h"
#include "game/gameplay/QuestGameplaySystem.h"
#include "game/gameplay/GatheringSystem.h"

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

        ContentPipeline m_pipeline;
        ContentValidator m_validator;
        SaveGameProfile m_profile;
        WorldClock m_clock;
        RoutineDatabase m_routines;
        NpcRoutineSystem m_npcRoutineSystem;
        RegionSimulationState m_regionState;
        std::vector<ActiveNpcRoutineInfo> m_activeRoutines;
        RegionLayoutDatabase m_layouts;
        WorldRenderer m_renderer;
        Camera3D m_camera{};
        float m_yaw = 0.0f;
        float m_pitch = 0.0f;
        std::vector<Vector3> m_activeNpcPositions;
        std::vector<Vector3> m_activeGatherPoints;
        InteractionSystem m_interactionSystem;
        QuestGameplaySystem m_questGameplaySystem;
        GatheringSystem m_gatheringSystem;
        std::string m_interactionPrompt;
        std::string m_statusText;
    };
}
