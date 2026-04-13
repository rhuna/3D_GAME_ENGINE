#include "scene/SceneManager.h"

#include "core/Application.h"
#include "scene/Scene.h"

namespace fw {

void SceneManager::ChangeScene(Application& app, std::unique_ptr<Scene> nextScene) {
    if (m_currentScene) {
        m_currentScene->OnExit(app);
    }

    m_currentScene = std::move(nextScene);

    if (m_currentScene) {
        m_currentScene->OnEnter(app);
    }
}

void SceneManager::Update(Application& app, float deltaTime) {
    if (m_currentScene) {
        m_currentScene->Update(app, deltaTime);
    }
}

void SceneManager::FixedUpdate(Application& app, float fixedDeltaTime) {
    if (m_currentScene) {
        m_currentScene->FixedUpdate(app, fixedDeltaTime);
    }
}

void SceneManager::Render(Application& app) {
    if (m_currentScene) {
        m_currentScene->Render(app);
    }
}

void SceneManager::DrawUi(Application& app) {
    if (m_currentScene) {
        m_currentScene->DrawUi(app);
    }
}

std::string SceneManager::CurrentSceneName() const {
    return m_currentScene ? m_currentScene->Name() : "<none>";
}

} // namespace fw
