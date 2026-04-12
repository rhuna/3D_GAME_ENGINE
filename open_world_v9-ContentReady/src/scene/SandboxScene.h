#pragma once

#include "scene/Scene.h"

namespace fw {

class SandboxScene final : public Scene {
public:
    const char* Name() const override { return "SandboxScene"; }
    void OnEnter(Application& app) override;
    void OnExit(Application& app) override;
    void Update(Application& app, float deltaTime) override;
    void FixedUpdate(Application& app, float fixedDeltaTime) override;
    void Render(Application& app) override;
};

} // namespace fw
