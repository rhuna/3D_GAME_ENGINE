#pragma once

#include <memory>
#include <string>

namespace fw {

class Application;
class Scene;

class SceneManager {
public:
    void ChangeScene(Application& app, std::unique_ptr<Scene> nextScene);
    void Update(Application& app, float deltaTime);
    void FixedUpdate(Application& app, float fixedDeltaTime);
    void Render(Application& app);
    void DrawUi(Application& app);

    [[nodiscard]] Scene* CurrentScene() const { return m_currentScene.get(); }
    [[nodiscard]] std::string CurrentSceneName() const;

private:
    std::unique_ptr<Scene> m_currentScene;
};

} // namespace fw
