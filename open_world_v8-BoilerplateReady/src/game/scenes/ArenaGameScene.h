#pragma once

#include "game/progression/CampaignProfile.h"
#include "game/rules/WaveDirector.h"
#include "game/ui/GameHud.h"
#include "scene/Scene.h"

namespace fw {

class ArenaGameScene : public Scene {
public:
    void OnEnter(Application& app) override;
    void OnExit(Application& app) override;
    void Update(Application& app, float deltaTime) override;
    void FixedUpdate(Application& app, float fixedDeltaTime) override;
    void Render(Application& app) override;
    void DrawUi(Application& app) override;
    const char* Name() const override { return "ArenaGameScene"; }

private:
    const char* ResolveSceneName() const;
    void AdvanceArenaSelection(int delta);

    GameHud m_hud {};
    WaveDirector m_waveDirector {};
    CampaignProfile m_campaignProfile {};
};

} // namespace fw
