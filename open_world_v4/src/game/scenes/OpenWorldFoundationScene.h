#pragma once

#include <string>

#include "game/state/OpenWorldGameState.h"
#include "game/ui/OpenWorldHud.h"
#include "game/world/RegionManager.h"
#include "scene/Scene.h"

namespace fw {

class OpenWorldFoundationScene : public Scene {
public:
    const char* Name() const override { return "OpenWorldFoundationScene"; }
    void OnEnter(Application& app) override;
    void OnExit(Application& app) override;
    void Update(Application& app, float deltaTime) override;
    void FixedUpdate(Application& app, float fixedDeltaTime) override;
    void Render(Application& app) override;
    void DrawUi(Application& app) override;

private:
    void LoadRegion(Application& app, const std::string& regionId, bool useSavedPosition);
    void BootstrapRegionGameplay(World& world);
    void TryTravel(Application& app);

    RegionManager m_regionManager;
    OpenWorldGameState m_state;
    OpenWorldHud m_hud;
};

} // namespace fw
