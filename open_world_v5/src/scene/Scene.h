#pragma once

#include <string>

namespace fw {

class Application;

class Scene {
public:
    virtual ~Scene() = default;

    virtual const char* Name() const = 0;
    virtual void OnEnter(Application& app) = 0;
    virtual void OnExit(Application& app) = 0;
    virtual void Update(Application& app, float deltaTime) = 0;
    virtual void FixedUpdate(Application& app, float fixedDeltaTime) = 0;
    virtual void Render(Application& app) = 0;
    virtual void DrawUi(Application& app) { (void)app; }
};

} // namespace fw
