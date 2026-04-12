#pragma once

#include "scene/Scene.h"

namespace fw {

class SandboxScene : public Scene {
public:
    void OnEnter(Application& app) override;
    void OnExit(Application& app) override;
    void Update(Application& app, float deltaTime) override;
    void FixedUpdate(Application& app, float fixedDeltaTime) override;
    void Render(Application& app) override;
    const char* Name() const override { return "SandboxScene"; }
};

} // namespace fw
