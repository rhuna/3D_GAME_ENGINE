#include "game/scenes/OpenWorldRuntimeScene.h"
#include "game/ui/OpenWorldHud.h"
#include "game/ui/QuestLogPanel.h"
#include "game/ui/EquipmentPanel.h"
#include "core/Application.h"
#include "ecs/World.h"
#include "game/components/PersistentIdComponent.h"
#include "game/components/SavePointComponent.h"
#include "game/world/TravelTriggerComponent.h"
#include "game/project/GameProjectLoader.h"
#include "game/project/GameStarterScaffold.h"
#include "render/Renderer.h"
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
            out.push_back(layout->npcSpawns[i].position);
        return out;
    }

    static int CountAlive(const std::vector<EnemyInstance>& enemies)
    {
        int alive = 0;
        for (const auto& e : enemies) if (e.alive) ++alive;
        return alive;
    }

    static std::vector<std::string> BuildRegionDebugOrder(const ContentPipeline& pipeline)
    {
        std::vector<std::string> ids;
        for (const auto& pair : pipeline.regions.GetAll())
        {
            ids.push_back(pair.first);
        }
        std::sort(ids.begin(), ids.end());
        return ids;
    }

    void OpenWorldRuntimeScene::ResetCurrentRegionStateFromLayout()
    {
        PersistentRegionState& state = m_worldState.GetOrCreate(m_profile.currentRegion);
        state.enemies.clear();
        state.gatherPoints.clear();
        state.lootDrops.clear();
        state.initialized = true;
        state.encounterCleared = false;

        if (const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion))
        {
            state.gatherPoints = layout->gatherPoints;
            for (const auto& spawn : layout->enemySpawns)
            {
                EnemyInstance e;
                e.position = spawn.position;
                e.type = spawn.type;
                e.health = spawn.type == "brute" ? 80 : (spawn.type == "wolf" ? 30 : 45);
                e.alive = true;
                e.attackCooldown = 0.0f;
                e.hurtFlash = 0.0f;
                state.enemies.push_back(e);
            }
        }
    }

    void OpenWorldRuntimeScene::EnsureCurrentRegionState()
    {
        PersistentRegionState& state = m_worldState.GetOrCreate(m_profile.currentRegion);
        if (!state.initialized) ResetCurrentRegionStateFromLayout();
    }

    void OpenWorldRuntimeScene::ApplyProjectBootstrap(const GameProjectDefinition& project)
    {
        m_profile.ResetToDefaults();
        m_profile.currentRegion = project.startRegion.empty() ? m_profile.currentRegion : project.startRegion;
        m_profile.inventory.gold = 25;

        if (m_pipeline.items.Find("iron_sword"))
        {
            m_profile.inventory.equippedWeaponId = "iron_sword";
        }
        if (m_pipeline.items.Find("leather_armor"))
        {
            m_profile.inventory.equippedArmorId = "leather_armor";
        }
        if (m_pipeline.items.Find("healing_potion"))
        {
            m_profile.inventory.items.push_back("healing_potion");
        }

        if (!m_pipeline.regions.Find(m_profile.currentRegion) && !m_regionDebugOrder.empty())
        {
            m_profile.currentRegion = m_regionDebugOrder.front();
        }
    }

    void OpenWorldRuntimeScene::OnEnter(Application& app)
    {
        (void)app;
        DisableCursor();

        GameProjectDefinition bootstrapProject;
        GameStarterScaffold::EnsureStarterProject("assets", bootstrapProject);
        m_project = GameProjectLoader::LoadFromFile("assets/game.project");
        const StarterScaffoldResult scaffold = GameStarterScaffold::EnsureStarterProject("assets", m_project);

        m_pipeline.LoadAll("assets");
        m_regionDebugOrder = BuildRegionDebugOrder(m_pipeline);
        m_routines.LoadFromDirectory("assets/routines");
        m_layouts.LoadFromDirectory("assets/regions");
        m_factionSystem.Reset();
        ApplyProjectBootstrap(m_project);

        if (scaffold.wroteAnyFiles)
        {
            m_statusText = "Starter content created in assets/. Edit START_HERE.txt and game.project.";
            m_statusTextTimer = 8.0f;
        }

        m_clock.Reset(8.0f);
        m_clock.SetTimeScale(0.35f);

        m_camera.position = {0.0f, 5.0f, -8.0f};
        m_camera.target = {0.0f, 1.5f, 0.0f};
        m_camera.up = {0.0f, 1.0f, 0.0f};
        m_camera.fovy = 70.0f;
        m_camera.projection = CAMERA_PERSPECTIVE;

        EnsureCurrentRegionState();
        RebuildRegionState(app);
        app.GetCamera() = m_camera;
    }

    void OpenWorldRuntimeScene::RebuildRegionState(Application& app, bool rebuildEcsWorld)
    {
        EnsureCurrentRegionState();
        PersistentRegionState& state = m_worldState.GetOrCreate(m_profile.currentRegion);
        m_activeRoutines = m_npcRoutineSystem.ResolveActiveRoutines(m_pipeline.npcs, m_routines, m_clock.GetHour(), m_profile.currentRegion);
        m_regionState.regionId = m_profile.currentRegion;
        const RegionDefinition* activeRegion = m_pipeline.regions.Find(m_profile.currentRegion);
        m_regionState.safeZone = activeRegion ? activeRegion->safeZone : (m_profile.currentRegion == "village_region");
        m_regionState.ambientPopulation = static_cast<int>(m_activeRoutines.size());
        m_regionState.activeEncounters = CountAlive(state.enemies);
        m_regionState.activeActivities.clear();
        for (const auto& r : m_activeRoutines) m_regionState.activeActivities.push_back(r.activity);

        const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion);
        if (layout)
        {
            m_activeNpcPositions = BuildNpcPositions(layout, m_activeRoutines);
            if (Vector3Length(m_profile.playerPosition) < 0.01f) m_profile.playerPosition = layout->playerSpawn;
        }
        else m_activeNpcPositions.clear();

        if (CountAlive(state.enemies) == 0 && state.initialized) state.encounterCleared = true;

        if (rebuildEcsWorld)
        {
            m_regionEcsLoader.Rebuild(app, m_pipeline, m_layouts, m_profile, m_profile.currentRegion);
        }
    }

    void OpenWorldRuntimeScene::Update(Application& app, float dt)
    {
        m_interactionPrompt.clear();
        if (m_statusTextTimer > 0.0f) { m_statusTextTimer -= dt; if (m_statusTextTimer <= 0.0f) { m_statusText.clear(); m_statusTextTimer = 0.0f; } }

        if (m_dialogue.active && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_E)))
        {
            m_dialogue.active = false;
            return;
        }

        if (IsKeyPressed(KEY_TAB)) m_showQuestLog = !m_showQuestLog;
        if (IsKeyPressed(KEY_I)) m_showEquipment = !m_showEquipment;

        if (IsKeyPressed(KEY_F5)) { m_profile.SaveToFile("savegame.profile"); m_statusText = "Profile saved."; m_statusTextTimer = 3.0f; }
        if (IsKeyPressed(KEY_F9)) { m_profile.LoadFromFile("savegame.profile"); EnsureCurrentRegionState(); RebuildRegionState(app); m_statusText = "Profile loaded."; m_statusTextTimer = 3.0f; }
        if (IsKeyPressed(KEY_O)) { m_worldState.SaveToFile("worldstate.snapshot"); m_statusText = "World state saved."; m_statusTextTimer = 3.0f; }
        if (IsKeyPressed(KEY_P)) { if(m_worldState.LoadFromFile("worldstate.snapshot")){ EnsureCurrentRegionState(); RebuildRegionState(app); m_statusText = "World state loaded."; } else { m_statusText = "No world state file found."; } m_statusTextTimer = 3.0f; }
        if (IsKeyPressed(KEY_K)) { m_worldState.ClearRegion(m_profile.currentRegion); EnsureCurrentRegionState(); RebuildRegionState(app); m_statusText = "Current region reset."; m_statusTextTimer = 3.0f; }

        if (IsKeyPressed(KEY_F6))
        {
            const StarterScaffoldResult scaffold = GameStarterScaffold::EnsureStarterProject("assets", m_project);
            if (scaffold.wroteAnyFiles)
            {
                m_pipeline.LoadAll("assets");
                m_regionDebugOrder = BuildRegionDebugOrder(m_pipeline);
                m_statusText = "Starter files restored. Check assets/START_HERE.txt.";
            }
            else
            {
                m_statusText = "Starter scaffold already present.";
            }
            m_statusTextTimer = 4.0f;
        }

        bool regionChanged = false;
        if (IsKeyPressed(KEY_ONE) && m_pipeline.regions.Find("starter_village")) { m_profile.currentRegion = "starter_village"; regionChanged = true; }
        if (IsKeyPressed(KEY_TWO) && m_pipeline.regions.Find("starter_forest")) { m_profile.currentRegion = "starter_forest"; regionChanged = true; }
        if (IsKeyPressed(KEY_THREE) && m_pipeline.regions.Find("ruins_region")) { m_profile.currentRegion = "ruins_region"; regionChanged = true; }

        if (regionChanged)
        {
            EnsureCurrentRegionState();
            if (const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion)) m_profile.playerPosition = layout->playerSpawn;
            RebuildRegionState(app);
        }

        Vector2 md = GetMouseDelta();
        m_cameraYaw += md.x * 0.003f;
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
        }

        if (m_grounded && IsKeyPressed(KEY_SPACE)) { m_verticalVelocity = 6.5f; m_grounded = false; }
        m_verticalVelocity -= 18.0f * dt;
        m_profile.playerPosition.y += m_verticalVelocity * dt;
        if (m_profile.playerPosition.y <= 1.0f) { m_profile.playerPosition.y = 1.0f; m_verticalVelocity = 0.0f; m_grounded = true; }

        Vector3 lookDir = {sinf(m_cameraYaw) * cosf(m_cameraPitch), sinf(m_cameraPitch), cosf(m_cameraYaw) * cosf(m_cameraPitch)};
        m_camera.target = Vector3Add(m_profile.playerPosition, {0.0f, 1.2f, 0.0f});
        m_camera.position = Vector3Subtract(m_camera.target, Vector3Scale(lookDir, 8.0f));
        if (m_camera.position.y < 2.0f) m_camera.position.y = 2.0f;

        PersistentRegionState& state = m_worldState.GetOrCreate(m_profile.currentRegion);
        ComputedStats stats = m_statSystem.Compute(m_profile, m_pipeline.items);

        // guards helpful or suspicious effect via reputation
        int guardRep = m_factionSystem.GetReputation("guards");
        int villagerRep = m_factionSystem.GetReputation("villagers");
        if (guardRep <= -10 && (m_profile.currentRegion == "village_region" || m_profile.currentRegion == "starter_village") && (GetRandomValue(0, 180) == 0))
        {
            m_profile.playerHealth -= 2;
            m_statusText = "The guards keep you under pressure.";
            m_statusTextTimer = 1.5f;
        }
        if ((m_profile.currentRegion == "forest_region" || m_profile.currentRegion == "starter_forest") && !state.encounterCleared && (GetRandomValue(0, 220) == 0))
        {
            EnemyInstance patrol;
            patrol.position = Vector3Add(m_profile.playerPosition, {6.0f, 0.0f, 6.0f});
            patrol.type = guardRep >= 5 ? "raider" : "wolf";
            patrol.health = patrol.type == "wolf" ? 30 : 45;
            patrol.alive = true;
            state.enemies.push_back(patrol);
            m_statusText = "Regional pressure spawned a new patrol.";
            m_statusTextTimer = 2.0f;
        }
        if (m_profile.currentRegion == "ruins_region" && villagerRep >= 10 && state.encounterCleared && GetRandomValue(0, 300) == 0)
        {
            state.lootDrops.push_back(m_lootSystem.MakeDrop("brute", Vector3Add(m_profile.playerPosition, {4.0f, 0.0f, -4.0f})));
            m_statusText = "A secured route produced a supply cache.";
            m_statusTextTimer = 2.0f;
        }

        m_enemySystem.Update(state.enemies, m_profile.playerPosition, m_profile.playerHealth, dt);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector3 facing = Vector3Normalize(Vector3{sinf(m_cameraYaw), 0.0f, cosf(m_cameraYaw)});
            std::vector<std::string> killedTypes;
            std::vector<Vector3> killedPositions;
            int kills = m_combatSystem.PlayerAttack(state.enemies, m_profile.playerPosition, facing, 3.5f, stats.attack, killedTypes, killedPositions);
            if (kills > 0)
            {
                for (size_t i = 0; i < killedTypes.size(); ++i)
                {
                    m_questGameplaySystem.RegisterEnemyKill(m_profile, killedTypes[i]);
                    m_progressionSystem.AddXp(m_progression, killedTypes[i] == "brute" ? 30 : (killedTypes[i] == "wolf" ? 12 : 20));
                    state.lootDrops.push_back(m_lootSystem.MakeDrop(killedTypes[i], killedPositions[i]));
                    if (killedTypes[i] == "raider") m_factionSystem.AddReputation("guards", 2);
                }
                m_profile.inventory.gold += kills * 5;
                m_statusText = TextFormat("Defeated %i enemy(s).", kills);
                m_statusTextTimer = 2.0f;
            }
            else
            {
                m_statusText = "Attack missed.";
                m_statusTextTimer = 1.0f;
            }
        }

        if (IsKeyPressed(KEY_F))
        {
            if (m_equipmentSystem.EquipFirstMatching(m_profile, m_pipeline.items, "weapon")) { m_statusText = "Equipped weapon."; m_statusTextTimer = 2.0f; }
        }
        if (IsKeyPressed(KEY_G))
        {
            if (m_equipmentSystem.UseFirstConsumable(m_profile, m_pipeline.items)) { m_statusText = "Used consumable."; m_statusTextTimer = 2.0f; }
        }

        if (m_profile.playerHealth <= 0)
        {
            if (const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion)) m_profile.playerPosition = layout->playerSpawn;
            m_profile.playerHealth = m_profile.playerMaxHealth;
            m_worldState.ClearRegion(m_profile.currentRegion);
            EnsureCurrentRegionState();
            RebuildRegionState(app);
            m_factionSystem.AddReputation("guards", -2);
            m_statusText = "You were defeated and the region reset.";
            m_statusTextTimer = 3.0f;
        }

        m_clock.Update(dt);
        RebuildRegionState(app, false);
        app.GetCamera() = m_camera;

        std::vector<InteractionCandidate> candidates;
        for (size_t i = 0; i < m_activeNpcPositions.size() && i < m_activeRoutines.size(); ++i)
        {
            float d = Vector3Distance(m_activeNpcPositions[i], m_profile.playerPosition);
            candidates.push_back({"npc", m_activeRoutines[i].npcId, m_activeNpcPositions[i], d, "Press E to talk"});
        }
        for (const auto& p : state.gatherPoints)
        {
            float d = Vector3Distance(p, m_profile.playerPosition);
            candidates.push_back({"gather", "herb", p, d, "Press E to gather herb"});
        }
        for (const Entity entity : app.GetWorld().Entities())
        {
            const auto* trigger = app.GetWorld().GetComponent<TravelTriggerComponent>(entity);
            const auto* transform = app.GetWorld().GetComponent<TransformComponent>(entity);
            if (!trigger || !transform) continue;
            float d = Vector3Distance(transform->position, m_profile.playerPosition);
            candidates.push_back({"travel", trigger->destinationRegion, transform->position, d, "Press E to travel"});
        }
        for (const Entity entity : app.GetWorld().Entities())
        {
            const auto* savePoint = app.GetWorld().GetComponent<SavePointComponent>(entity);
            const auto* persistentId = app.GetWorld().GetComponent<PersistentIdComponent>(entity);
            const auto* transform = app.GetWorld().GetComponent<TransformComponent>(entity);
            if (!savePoint || !transform) continue;
            float d = Vector3Distance(transform->position, m_profile.playerPosition);
            std::string prompt = "Press E to save";
            if (persistentId && m_worldState.IsSavePointUsed(m_profile.currentRegion, persistentId->value)) prompt = "Press E to save again";
            candidates.push_back({"save", persistentId ? persistentId->value : "", transform->position, d, prompt});
        }
        for (const auto& d : state.lootDrops)
        {
            if (!d.active) continue;
            float dist = Vector3Distance(d.position, m_profile.playerPosition);
            candidates.push_back({"loot", d.itemId, d.position, dist, "Press E to loot"});
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
                            m_dialogue = {true, npc->displayName, "I need proof of strength and supplies. Bring what I asked for."};
                        }
                        else if (m_questGameplaySystem.TryCompleteQuest(m_profile, m_pipeline, npc->questId))
                        {
                            m_factionSystem.AddReputation("villagers", 10);
                            m_factionSystem.AddReputation("guards", 4);
                            m_dialogue = {true, npc->displayName, "Excellent work. The village will remember this."};
                        }
                        else
                        {
                            m_dialogue = {true, npc->displayName, "You still need to finish the objectives."};
                        }
                    }
                    else
                    {
                        const auto* dlg = m_pipeline.dialogues.Find(npc->dialogueId);
                        std::string text = dlg ? dlg->text : "Hello.";
                        if (npc->factionId == "guards")
                        {
                            std::string standing = m_factionSystem.DescribeStanding("guards");
                            if (standing == "Trusted") text = "The guards welcome you like a proven ally.";
                            else if (standing == "Friendly") text = "The guards nod as you pass.";
                            else if (standing == "Distrusted") text = "The guards keep a close watch on you.";
                            else if (standing == "Hostile") text = "The guards are openly hostile toward you.";
                        }
                        m_dialogue = {true, dlg ? dlg->speakerName : npc->displayName, text};
                    }
                }
            }
            else if (best->kind == "gather")
            {
                int gathered = m_gatheringSystem.GatherAt(state.gatherPoints, m_profile.playerPosition, 3.0f);
                if (gathered > 0)
                {
                    m_profile.inventory.items.push_back("herb");
                    m_statusText = "Gathered herb.";
                    m_statusTextTimer = 2.0f;
                    for (auto& q : m_profile.quests)
                        if (q.state == QuestProgressState::Active)
                            for (auto& obj : q.objectives)
                                if (obj.id == "collect_herb") { obj.current = std::min(obj.required, obj.current + 1); obj.completed = (obj.current >= obj.required); }
                }
            }
            else if (best->kind == "travel")
            {
                m_profile.currentRegion = best->id;
                EnsureCurrentRegionState();
                if (const RegionLayout* newLayout = m_layouts.Find(m_profile.currentRegion)) m_profile.playerPosition = newLayout->playerSpawn;
                RebuildRegionState(app);
                m_statusText = "Travelled to " + m_profile.currentRegion;
                m_statusTextTimer = 2.0f;
            }
            else if (best->kind == "save")
            {
                m_profile.SaveToFile("savegame.profile");
                m_worldState.SaveToFile("worldstate.snapshot");
                if (!best->id.empty()) m_worldState.MarkSavePointUsed(m_profile.currentRegion, best->id);
                m_statusText = "Progress saved at checkpoint.";
                m_statusTextTimer = 2.0f;
            }
            else if (best->kind == "loot")
            {
                std::vector<std::string> items;
                int gold = 0;
                if (m_lootSystem.PickupNearest(state.lootDrops, m_profile.playerPosition, 3.0f, items, gold))
                {
                    for (const auto& item : items) m_profile.inventory.items.push_back(item);
                    m_profile.inventory.gold += gold;
                    m_statusText = "Loot collected.";
                    m_statusTextTimer = 2.0f;
                }
            }
        }
    }

    void OpenWorldRuntimeScene::FixedUpdate(Application& app, float fixedDeltaTime)
    {
        (void)app;
        (void)fixedDeltaTime;
    }

    void OpenWorldRuntimeScene::Render(Application& app)
    {
        PersistentRegionState& state = m_worldState.GetOrCreate(m_profile.currentRegion);
        const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion);

        app.GetRenderer().DrawGrid(40, 2.0f);
        app.GetRenderer().DrawWorld(app.GetWorld(), app.GetAssets());

        if (layout)
        {
            std::vector<Vector3> lootPositions;
            for (const auto& d : state.lootDrops) if (d.active) lootPositions.push_back(d.position);
            m_renderer.DrawRegion(*layout, m_activeNpcPositions, state.enemies, lootPositions);
        }

        DrawCapsule({m_profile.playerPosition.x, m_profile.playerPosition.y + 0.2f, m_profile.playerPosition.z},
                    {m_profile.playerPosition.x, m_profile.playerPosition.y + 1.8f, m_profile.playerPosition.z},
                    0.45f, 8, 8, RED);
    }

    void OpenWorldRuntimeScene::DrawUi(Application& app)
    {
        (void)app;
        PersistentRegionState& state = m_worldState.GetOrCreate(m_profile.currentRegion);
        ComputedStats stats = m_statSystem.Compute(m_profile, m_pipeline.items);

        DrawRectangle(0, 0, 900, 250, Fade(BLACK, 0.55f));
        DrawText("3D_GAME_ENGINE v29 Open World Runtime Merge", 20, 20, 28, WHITE);
        DrawText(TextFormat("Alive Enemies: %i", CountAlive(state.enemies)), 20, 60, 20, LIGHTGRAY);
        DrawText(TextFormat("Encounter Cleared: %s", state.encounterCleared ? "Yes" : "No"), 20, 85, 20, state.encounterCleared ? GREEN : ORANGE);
        DrawText(TextFormat("Villagers: %s | Guards: %s", m_factionSystem.DescribeStanding("villagers").c_str(), m_factionSystem.DescribeStanding("guards").c_str()), 20, 110, 20, SKYBLUE);
        DrawText(TextFormat("Level %i  XP %i/%i", m_progression.level, m_progression.xp, m_progression.xpToNext), 20, 135, 20, YELLOW);
        DrawText(TextFormat("Current Region: %s", m_profile.currentRegion.c_str()), 20, 160, 20, SKYBLUE);
        DrawText("This build routes open world through Application + SceneManager + ECS region spawning.", 20, 185, 20, SKYBLUE);

        std::string rep = "Villagers: " + m_factionSystem.DescribeStanding("villagers") + " | Guards: " + m_factionSystem.DescribeStanding("guards");
        const std::string formatted = m_clock.GetFormattedTime();
        DrawOpenWorldHud(m_profile, formatted.c_str(), m_regionState, m_activeRoutines, m_interactionPrompt.c_str(), m_statusText.c_str(), CountAlive(state.enemies), stats.attack, stats.defense, rep.c_str());
        DrawDialogueBox(m_dialogue);
        DrawQuestLogPanel(m_profile.quests, m_showQuestLog);
        DrawEquipmentPanel(m_profile, stats, m_progression, m_showEquipment);
    }

    void OpenWorldRuntimeScene::OnExit(Application& app)
    {
        app.GetWorld().Clear();
        EnableCursor();
    }
}
