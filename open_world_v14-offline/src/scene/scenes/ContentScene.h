#pragma once

#include <string>

#include "scene/Scene.h"

namespace fw {

class ContentScene : public Scene {
public:
    explicit ContentScene(std::string sceneName) : m_sceneName(std::move(sceneName)) {}

    void OnEnter(Application& app) override;
    void OnExit(Application& app) override;
    void Update(Application& app, float deltaTime) override;
    void FixedUpdate(Application& app, float fixedDeltaTime) override;
    void Render(Application& app) override;
    void DrawUi(Application& app) override;
    const char* Name() const override { return "ContentScene"; }

private:
    std::string m_sceneName;
    std::string m_displayName;
};

} // namespace fw
