#include "game/scenes/OpenWorldFoundationScene.h"
#include "game/ui/OpenWorldHud.h"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>

namespace fw
{
    static std::vector<Vector3> BuildNpcPositions(const RegionLayout* layout, const std::vector<ActiveNpcRoutineInfo>& active)
    {
        std::vector<Vector3> out;
        if (!layout) return out;
        for (size_t i = 0; i < active.size() && i < layout->npcSpawns.size(); ++i)
            out.push_back(layout->npcSpawns[i]);
        return out;
    }

    void OpenWorldFoundationScene::Load()
    {
        DisableCursor();
        m_pipeline.LoadAll("assets");
        m_routines.LoadFromDirectory("assets/routines");
        m_layouts.LoadDefaults();

        m_profile.ResetToDefaults();
        m_profile.inventory.gold = 25;
        m_profile.inventory.equippedWeaponId = "iron_sword";

        m_clock.Reset(8.0f);
        m_clock.SetTimeScale(0.35f);

        m_camera.position = {0.0f, 2.0f, -6.0f};
        m_camera.target = {0.0f, 2.0f, 0.0f};
        m_camera.up = {0.0f, 1.0f, 0.0f};
        m_camera.fovy = 70.0f;
        m_camera.projection = CAMERA_PERSPECTIVE;

        RebuildRegionState();
    }

    void OpenWorldFoundationScene::RebuildRegionState()
    {
        m_activeRoutines = m_npcRoutineSystem.ResolveActiveRoutines(
            m_pipeline.npcs, m_routines, m_clock.GetHour(), m_profile.currentRegion);

        m_regionState.regionId = m_profile.currentRegion;
        m_regionState.safeZone = (m_profile.currentRegion == "village_region");
        m_regionState.ambientPopulation = static_cast<int>(m_activeRoutines.size());
        m_regionState.activeEncounters = (m_profile.currentRegion == "forest_region") ? 2 : (m_profile.currentRegion == "ruins_region" ? 1 : 0);
        m_regionState.activeActivities.clear();
        for (const auto& r : m_activeRoutines)
            m_regionState.activeActivities.push_back(r.activity);

        const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion);
        if (layout)
        {
            m_activeNpcPositions = BuildNpcPositions(layout, m_activeRoutines);
            m_activeGatherPoints = layout->gatherPoints;
            if (Vector3Length(m_profile.playerPosition) < 0.01f)
                m_profile.playerPosition = layout->playerSpawn;
        }
        else
        {
            m_activeNpcPositions.clear();
            m_activeGatherPoints.clear();
        }
    }

    void OpenWorldFoundationScene::Update(float dt)
    {
        m_statusText.clear();
        m_interactionPrompt.clear();

        if (IsKeyPressed(KEY_F5)) { m_profile.SaveToFile("savegame.profile"); m_statusText = "Profile saved."; }
        if (IsKeyPressed(KEY_F9)) { m_profile.LoadFromFile("savegame.profile"); m_statusText = "Profile loaded."; }

        bool regionChanged = false;
        if (IsKeyPressed(KEY_ONE)) { m_profile.currentRegion = "village_region"; regionChanged = true; }
        if (IsKeyPressed(KEY_TWO)) { m_profile.currentRegion = "forest_region"; regionChanged = true; }
        if (IsKeyPressed(KEY_THREE)) { m_profile.currentRegion = "ruins_region"; regionChanged = true; }

        if (regionChanged)
        {
            if (const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion))
                m_profile.playerPosition = layout->playerSpawn;
            RebuildRegionState();
        }

        Vector2 md = GetMouseDelta();
        m_yaw -= md.x * 0.003f;
        m_pitch += md.y * 0.003f;
        m_pitch = std::clamp(m_pitch, -1.2f, 1.2f);

        Vector3 forward = { sinf(m_yaw) * cosf(m_pitch), 0.0f, cosf(m_yaw) * cosf(m_pitch) };
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, {0,1,0}));

        float speed = 10.0f * dt;
        if (IsKeyDown(KEY_W)) m_profile.playerPosition = Vector3Add(m_profile.playerPosition, Vector3Scale(forward, speed));
        if (IsKeyDown(KEY_S)) m_profile.playerPosition = Vector3Subtract(m_profile.playerPosition, Vector3Scale(forward, speed));
        if (IsKeyDown(KEY_A)) m_profile.playerPosition = Vector3Subtract(m_profile.playerPosition, Vector3Scale(right, speed));
        if (IsKeyDown(KEY_D)) m_profile.playerPosition = Vector3Add(m_profile.playerPosition, Vector3Scale(right, speed));
        if (IsKeyDown(KEY_SPACE)) m_profile.playerPosition.y += speed;
        if (IsKeyDown(KEY_LEFT_CONTROL)) m_profile.playerPosition.y -= speed;
        if (m_profile.playerPosition.y < 1.0f) m_profile.playerPosition.y = 1.0f;

        m_camera.position = Vector3Add(m_profile.playerPosition, {0.0f, 2.0f, 0.0f});
        m_camera.target = Vector3Add(m_camera.position, {sinf(m_yaw)*cosf(m_pitch), sinf(-m_pitch), cosf(m_yaw)*cosf(m_pitch)});

        m_clock.Update(dt);

        // rebuild routines without resetting gatherables
        m_activeRoutines = m_npcRoutineSystem.ResolveActiveRoutines(
            m_pipeline.npcs, m_routines, m_clock.GetHour(), m_profile.currentRegion);
        m_regionState.regionId = m_profile.currentRegion;
        m_regionState.safeZone = (m_profile.currentRegion == "village_region");
        m_regionState.ambientPopulation = static_cast<int>(m_activeRoutines.size());
        m_regionState.activeEncounters = (m_profile.currentRegion == "forest_region") ? 2 : (m_profile.currentRegion == "ruins_region" ? 1 : 0);
        m_regionState.activeActivities.clear();
        for (const auto& r : m_activeRoutines) m_regionState.activeActivities.push_back(r.activity);
        const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion);
        m_activeNpcPositions = BuildNpcPositions(layout, m_activeRoutines);

        std::vector<InteractionCandidate> candidates;

        // NPC interaction
        for (size_t i = 0; i < m_activeNpcPositions.size() && i < m_activeRoutines.size(); ++i)
        {
            float d = Vector3Distance(m_activeNpcPositions[i], m_profile.playerPosition);
            candidates.push_back({"npc", m_activeRoutines[i].npcId, m_activeNpcPositions[i], d, "Press E to talk"});
        }
        // gatherables
        for (const auto& p : m_activeGatherPoints)
        {
            float d = Vector3Distance(p, m_profile.playerPosition);
            candidates.push_back({"gather", "herb", p, d, "Press E to gather herb"});
        }
        // travel points
        if (layout)
        {
            for (size_t i = 0; i < layout->travelPoints.size(); ++i)
            {
                float d = Vector3Distance(layout->travelPoints[i], m_profile.playerPosition);
                candidates.push_back({"travel", std::to_string((int)i), layout->travelPoints[i], d, "Press E to travel"});
            }
        }

        const InteractionCandidate* best = m_interactionSystem.FindBest(candidates, 3.0f);
        if (best) m_interactionPrompt = best->prompt;

        if (best && IsKeyPressed(KEY_E))
        {
            if (best->kind == "npc")
            {
                const auto* npc = m_pipeline.npcs.Find(best->id);
                if (npc)
                {
                    if (!npc->questId.empty())
                    {
                        if (!m_questGameplaySystem.HasQuest(m_profile, npc->questId))
                        {
                            m_questGameplaySystem.AcceptQuest(m_profile, m_pipeline, npc->questId);
                            m_statusText = "Quest accepted: " + npc->questId;
                        }
                        else if (m_questGameplaySystem.TryCompleteQuest(m_profile, m_pipeline, npc->questId))
                        {
                            m_statusText = "Quest completed: " + npc->questId;
                        }
                        else
                        {
                            m_statusText = npc->displayName + ": You still need the quest items.";
                        }
                    }
                    else
                    {
                        const auto* dlg = m_pipeline.dialogues.Find(npc->dialogueId);
                        m_statusText = dlg ? (dlg->speakerName + ": " + dlg->text) : (npc->displayName + ": Hello.");
                    }
                }
            }
            else if (best->kind == "gather")
            {
                int gathered = m_gatheringSystem.GatherAt(m_activeGatherPoints, m_profile.playerPosition, 3.0f);
                if (gathered > 0)
                {
                    m_profile.inventory.items.push_back("herb");
                    m_statusText = "Gathered herb.";
                    for (auto& q : m_profile.quests)
                    {
                        if (q.state == QuestProgressState::Active)
                        {
                            for (auto& obj : q.objectives)
                            {
                                if (obj.id == "collect_herb")
                                {
                                    obj.current = std::min(obj.required, obj.current + 1);
                                    obj.completed = (obj.current >= obj.required);
                                }
                            }
                        }
                    }
                }
            }
            else if (best->kind == "travel")
            {
                if (m_profile.currentRegion == "village_region") m_profile.currentRegion = "forest_region";
                else if (m_profile.currentRegion == "forest_region") m_profile.currentRegion = "ruins_region";
                else m_profile.currentRegion = "village_region";

                if (const RegionLayout* newLayout = m_layouts.Find(m_profile.currentRegion))
                    m_profile.playerPosition = newLayout->playerSpawn;
                RebuildRegionState();
                m_statusText = "Travelled to " + m_profile.currentRegion;
            }
        }
    }

    void OpenWorldFoundationScene::Draw()
    {
        const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion);

        BeginMode3D(m_camera);
        DrawGrid(40, 2.0f);
        if (layout)
        {
            m_renderer.DrawRegion(*layout, m_activeNpcPositions);
            for (const auto& p : m_activeGatherPoints)
            {
                DrawSphere(p, 0.45f, YELLOW);
                DrawSphereWires(p, 0.45f, 8, 8, ORANGE);
            }
        }
        DrawCube({m_profile.playerPosition.x, m_profile.playerPosition.y + 1.0f, m_profile.playerPosition.z}, 1.0f, 2.0f, 1.0f, RED);
        DrawCubeWires({m_profile.playerPosition.x, m_profile.playerPosition.y + 1.0f, m_profile.playerPosition.z}, 1.0f, 2.0f, 1.0f, BLACK);
        EndMode3D();

        DrawRectangle(0, 0, 560, 230, Fade(BLACK, 0.55f));
        DrawText("3D_GAME_ENGINE v21 Real Exploration Gameplay", 20, 20, 28, WHITE);
        DrawText(TextFormat("Regions Loaded: %i", (int)m_pipeline.regions.GetAll().size()), 20, 60, 20, LIGHTGRAY);
        DrawText(TextFormat("NPCs Loaded: %i", (int)m_pipeline.npcs.GetAll().size()), 20, 85, 20, LIGHTGRAY);
        DrawText(TextFormat("Routines Loaded: %i", (int)m_routines.GetAll().size()), 20, 110, 20, LIGHTGRAY);
        DrawText("WASD move | Mouse look | E interact | F5/F9 save/load", 20, 140, 20, SKYBLUE);
        DrawText("Quest loop: talk to Alric -> gather herbs in forest -> return", 20, 165, 20, SKYBLUE);

        const std::string formatted = m_clock.GetFormattedTime();
        DrawOpenWorldHud(m_profile, formatted.c_str(), m_regionState, m_activeRoutines, m_interactionPrompt.c_str(), m_statusText.c_str());
    }

    void OpenWorldFoundationScene::Unload()
    {
        EnableCursor();
    }
}
