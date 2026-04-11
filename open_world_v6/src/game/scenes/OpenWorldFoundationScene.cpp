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

    static int CountAlive(const std::vector<EnemyInstance>& enemies)
    {
        int alive = 0;
        for (const auto& e : enemies) if (e.alive) ++alive;
        return alive;
    }

    void OpenWorldFoundationScene::ResetEnemiesForRegion()
    {
        m_enemies.clear();
        if (const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion))
        {
            for (const auto& p : layout->enemySpawns)
                m_enemies.push_back({p, 40, true, 0.0f});
        }
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

        m_camera.position = {0.0f, 5.0f, -8.0f};
        m_camera.target = {0.0f, 1.5f, 0.0f};
        m_camera.up = {0.0f, 1.0f, 0.0f};
        m_camera.fovy = 70.0f;
        m_camera.projection = CAMERA_PERSPECTIVE;

        RebuildRegionState();
        ResetEnemiesForRegion();
    }

    void OpenWorldFoundationScene::RebuildRegionState()
    {
        m_activeRoutines = m_npcRoutineSystem.ResolveActiveRoutines(
            m_pipeline.npcs, m_routines, m_clock.GetHour(), m_profile.currentRegion);

        m_regionState.regionId = m_profile.currentRegion;
        m_regionState.safeZone = (m_profile.currentRegion == "village_region");
        m_regionState.ambientPopulation = static_cast<int>(m_activeRoutines.size());
        m_regionState.activeEncounters = CountAlive(m_enemies);
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
        m_interactionPrompt.clear();

if (m_statusTextTimer > 0.0f)
{
    m_statusTextTimer -= dt;
    if (m_statusTextTimer <= 0.0f)
    {
        m_statusText.clear();
        m_statusTextTimer = 0.0f;
    }
}

        if (IsKeyPressed(KEY_F5)) { m_profile.SaveToFile("savegame.profile"); m_statusText = "Profile saved."; m_statusTextTimer = m_statusTextDuration; }
        if (IsKeyPressed(KEY_F9)) { m_profile.LoadFromFile("savegame.profile"); m_statusText = "Profile loaded."; }

        bool regionChanged = false;
        if (IsKeyPressed(KEY_ONE)) { m_profile.currentRegion = "village_region"; regionChanged = true; }
        if (IsKeyPressed(KEY_TWO)) { m_profile.currentRegion = "forest_region"; regionChanged = true; }
        if (IsKeyPressed(KEY_THREE)) { m_profile.currentRegion = "ruins_region"; regionChanged = true; }

        if (regionChanged)
        {
            if (const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion))
                m_profile.playerPosition = layout->playerSpawn;
            ResetEnemiesForRegion();
            RebuildRegionState();
        }

        Vector2 md = GetMouseDelta();
        m_cameraYaw -= md.x * 0.003f;
        m_cameraPitch -= md.y * 0.003f;
        m_cameraPitch = std::clamp(m_cameraPitch, -0.6f, 1.0f);

        Vector3 camForward = Vector3Normalize({sinf(m_cameraYaw) * cosf(m_cameraPitch), 0.0f, cosf(m_cameraYaw) * cosf(m_cameraPitch)});
        Vector3 camRight = Vector3Normalize(Vector3CrossProduct(camForward, {0,1,0}));

        Vector3 move{};
        if (IsKeyDown(KEY_W)) move = Vector3Add(move, camForward);
        if (IsKeyDown(KEY_S)) move = Vector3Subtract(move, camForward);
        if (IsKeyDown(KEY_A)) move = Vector3Subtract(move, camRight);
        if (IsKeyDown(KEY_D)) move = Vector3Add(move, camRight);

        if (Vector3Length(move) > 0.001f)
        {
            move = Vector3Normalize(move);
            float speed = IsKeyDown(KEY_LEFT_SHIFT) ? 8.5f : 5.5f;
            m_profile.playerPosition = Vector3Add(m_profile.playerPosition, Vector3Scale({move.x, 0.0f, move.z}, speed * dt));
            m_playerYaw = atan2f(move.x, move.z);
        }

        if (m_grounded && IsKeyPressed(KEY_SPACE))
        {
            m_verticalVelocity = 6.5f;
            m_grounded = false;
        }
        m_verticalVelocity -= 18.0f * dt;
        m_profile.playerPosition.y += m_verticalVelocity * dt;
        if (m_profile.playerPosition.y <= 1.0f)
        {
            m_profile.playerPosition.y = 1.0f;
            m_verticalVelocity = 0.0f;
            m_grounded = true;
        }

        // Third-person camera
        Vector3 lookDir = {sinf(m_cameraYaw) * cosf(m_cameraPitch), sinf(m_cameraPitch), cosf(m_cameraYaw) * cosf(m_cameraPitch)};
        m_camera.target = Vector3Add(m_profile.playerPosition, {0.0f, 1.2f, 0.0f});
        m_camera.position = Vector3Subtract(m_camera.target, Vector3Scale(lookDir, 8.0f));
        if (m_camera.position.y < 2.0f) m_camera.position.y = 2.0f;

        // Combat update
        m_enemySystem.Update(m_enemies, m_profile.playerPosition, m_profile.playerHealth, dt);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector3 facing = {sinf(m_playerYaw), 0.0f, cosf(m_playerYaw)};
            int kills = m_combatSystem.PlayerAttack(m_enemies, m_profile.playerPosition, facing, 2.6f, 20);
            if (kills > 0)
            {
                m_profile.inventory.gold += kills * 5;
                m_statusText = TextFormat("You defeated %i enemy(s).", kills);
            }
        }

        if (m_profile.playerHealth <= 0)
        {
            if (const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion))
                m_profile.playerPosition = layout->playerSpawn;
            m_profile.playerHealth = m_profile.playerMaxHealth;
            ResetEnemiesForRegion();
            m_statusText = "You were defeated and respawned.";
        }

        m_clock.Update(dt);

        // refresh sim state without resetting gatherables
        m_activeRoutines = m_npcRoutineSystem.ResolveActiveRoutines(
            m_pipeline.npcs, m_routines, m_clock.GetHour(), m_profile.currentRegion);
        m_regionState.regionId = m_profile.currentRegion;
        m_regionState.safeZone = (m_profile.currentRegion == "village_region");
        m_regionState.ambientPopulation = static_cast<int>(m_activeRoutines.size());
        m_regionState.activeEncounters = CountAlive(m_enemies);
        m_regionState.activeActivities.clear();
        for (const auto& r : m_activeRoutines) m_regionState.activeActivities.push_back(r.activity);
        const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion);
        m_activeNpcPositions = BuildNpcPositions(layout, m_activeRoutines);

        std::vector<InteractionCandidate> candidates;

        for (size_t i = 0; i < m_activeNpcPositions.size() && i < m_activeRoutines.size(); ++i)
        {
            float d = Vector3Distance(m_activeNpcPositions[i], m_profile.playerPosition);
            candidates.push_back({"npc", m_activeRoutines[i].npcId, m_activeNpcPositions[i], d, "Press E to talk"});
        }
        for (const auto& p : m_activeGatherPoints)
        {
            float d = Vector3Distance(p, m_profile.playerPosition);
            candidates.push_back({"gather", "herb", p, d, "Press E to gather herb"});
        }
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
                        m_statusTextTimer = m_statusTextDuration;
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
                ResetEnemiesForRegion();
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
            std::vector<Vector3> enemyPositions;
            for (const auto& e : m_enemies) if (e.alive) enemyPositions.push_back(e.position);
            m_renderer.DrawRegion(*layout, m_activeNpcPositions, enemyPositions);

            for (const auto& p : m_activeGatherPoints)
            {
                DrawSphere(p, 0.45f, YELLOW);
                DrawSphereWires(p, 0.45f, 8, 8, ORANGE);
            }
        }

        DrawCapsule({m_profile.playerPosition.x, m_profile.playerPosition.y + 0.2f, m_profile.playerPosition.z},
                    {m_profile.playerPosition.x, m_profile.playerPosition.y + 1.8f, m_profile.playerPosition.z},
                    0.45f, 8, 8, RED);
        EndMode3D();

        DrawRectangle(0, 0, 620, 230, Fade(BLACK, 0.55f));
        DrawText("3D_GAME_ENGINE v22 Third-Person Exploration Combat", 20, 20, 28, WHITE);
        DrawText(TextFormat("Regions Loaded: %i", (int)m_pipeline.regions.GetAll().size()), 20, 60, 20, LIGHTGRAY);
        DrawText(TextFormat("NPCs Loaded: %i", (int)m_pipeline.npcs.GetAll().size()), 20, 85, 20, LIGHTGRAY);
        DrawText(TextFormat("Alive Enemies: %i", CountAlive(m_enemies)), 20, 110, 20, LIGHTGRAY);
        DrawText("WASD move | Mouse camera | Shift sprint | Space jump", 20, 140, 20, SKYBLUE);
        DrawText("LMB attack | E interact | 1/2/3 region switch | F5/F9 save/load", 20, 165, 20, SKYBLUE);

        const std::string formatted = m_clock.GetFormattedTime();
        DrawOpenWorldHud(m_profile, formatted.c_str(), m_regionState, m_activeRoutines,
                         m_interactionPrompt.c_str(), m_statusText.c_str(), CountAlive(m_enemies));
    }

    void OpenWorldFoundationScene::Unload()
    {
        EnableCursor();
    }
}
