#include "gameplay/registry/SystemRegistry.h"

#include "core/Application.h"
#include "ecs/World.h"

namespace fw {

void SystemRegistry::Clear() {
    m_updateSystems.clear();
    m_fixedSystems.clear();
    m_renderSystems.clear();
}

void SystemRegistry::RegisterUpdate(std::unique_ptr<IUpdateSystem> system) {
    if (system) {
        m_updateSystems.push_back(std::move(system));
    }
}

void SystemRegistry::RegisterFixed(std::unique_ptr<IFixedUpdateSystem> system) {
    if (system) {
        m_fixedSystems.push_back(std::move(system));
    }
}

void SystemRegistry::RegisterRender(std::unique_ptr<IRenderSystem> system) {
    if (system) {
        m_renderSystems.push_back(std::move(system));
    }
}

void SystemRegistry::UpdateAll(Application& app, World& world, float deltaTime) {
    for (auto& system : m_updateSystems) {
        system->Update(app, world, deltaTime);
    }
}

void SystemRegistry::FixedUpdateAll(Application& app, World& world, float fixedDeltaTime) {
    for (auto& system : m_fixedSystems) {
        system->FixedUpdate(app, world, fixedDeltaTime);
    }
}

void SystemRegistry::RenderAll(Application& app, World& world) {
    for (auto& system : m_renderSystems) {
        system->Render(app, world);
    }
}

} // namespace fw
