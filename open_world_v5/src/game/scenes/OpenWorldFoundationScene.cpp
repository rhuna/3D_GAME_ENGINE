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
        m_profile.inventory.items.push_back("herb");

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
            if (Vector3Length(m_profile.playerPosition) < 0.01f)
                m_profile.playerPosition = layout->playerSpawn;
        }
        else
        {
            m_activeNpcPositions.clear();
        }
    }

    void OpenWorldFoundationScene::Update(float dt)
    {
        if (IsKeyPressed(KEY_F5)) m_profile.SaveToFile("savegame.profile");
        if (IsKeyPressed(KEY_F9)) m_profile.LoadFromFile("savegame.profile");

        bool regionChanged = false;
        if (IsKeyPressed(KEY_ONE)) { m_profile.currentRegion = "village_region"; regionChanged = true; }
        if (IsKeyPressed(KEY_TWO)) { m_profile.currentRegion = "forest_region"; regionChanged = true; }
        if (IsKeyPressed(KEY_THREE)) { m_profile.currentRegion = "ruins_region"; regionChanged = true; }

        if (regionChanged)
        {
            if (const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion))
                m_profile.playerPosition = layout->playerSpawn;
        }

        Vector2 md = GetMouseDelta();
        m_yaw += md.x * 0.003f;
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
        RebuildRegionState();
    }

    void OpenWorldFoundationScene::Draw()
    {
        const RegionLayout* layout = m_layouts.Find(m_profile.currentRegion);

        BeginMode3D(m_camera);
        DrawGrid(40, 2.0f);
        if (layout)
            m_renderer.DrawRegion(*layout, m_activeNpcPositions);
        DrawCube({m_profile.playerPosition.x, m_profile.playerPosition.y + 1.0f, m_profile.playerPosition.z}, 1.0f, 2.0f, 1.0f, RED);
        DrawCubeWires({m_profile.playerPosition.x, m_profile.playerPosition.y + 1.0f, m_profile.playerPosition.z}, 1.0f, 2.0f, 1.0f, BLACK);
        EndMode3D();

        DrawRectangle(0, 0, 520, 240, Fade(BLACK, 0.55f));
        DrawText("3D_GAME_ENGINE v20 Visible World Rendering", 20, 20, 28, WHITE);
        DrawText(TextFormat("Regions Loaded: %i", (int)m_pipeline.regions.GetAll().size()), 20, 60, 20, LIGHTGRAY);
        DrawText(TextFormat("NPCs Loaded: %i", (int)m_pipeline.npcs.GetAll().size()), 20, 85, 20, LIGHTGRAY);
        DrawText(TextFormat("Routines Loaded: %i", (int)m_routines.GetAll().size()), 20, 110, 20, LIGHTGRAY);
        DrawText("WASD move | Mouse look | Space/Ctrl up-down", 20, 140, 20, SKYBLUE);
        DrawText("1/2/3 switch regions | F5 save | F9 load", 20, 165, 20, SKYBLUE);

        const std::string formatted = m_clock.GetFormattedTime();
        DrawOpenWorldHud(m_profile, formatted.c_str(), m_regionState, m_activeRoutines);
    }

    void OpenWorldFoundationScene::Unload()
    {
        EnableCursor();
    }
}
