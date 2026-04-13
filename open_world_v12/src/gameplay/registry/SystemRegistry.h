#pragma once

#include <memory>
#include <string>
#include <vector>

namespace fw {

class Application;
class World;

class IUpdateSystem {
public:
    virtual ~IUpdateSystem() = default;
    virtual const char* Name() const = 0;
    virtual void Update(Application& app, World& world, float deltaTime) = 0;
};

class IFixedUpdateSystem {
public:
    virtual ~IFixedUpdateSystem() = default;
    virtual const char* Name() const = 0;
    virtual void FixedUpdate(Application& app, World& world, float fixedDeltaTime) = 0;
};

class IRenderSystem {
public:
    virtual ~IRenderSystem() = default;
    virtual const char* Name() const = 0;
    virtual void Render(Application& app, World& world) = 0;
};

class SystemRegistry {
public:
    void Clear();

    void RegisterUpdate(std::unique_ptr<IUpdateSystem> system);
    void RegisterFixed(std::unique_ptr<IFixedUpdateSystem> system);
    void RegisterRender(std::unique_ptr<IRenderSystem> system);

    void UpdateAll(Application& app, World& world, float deltaTime);
    void FixedUpdateAll(Application& app, World& world, float fixedDeltaTime);
    void RenderAll(Application& app, World& world);

    std::size_t UpdateSystemCount() const { return m_updateSystems.size(); }
    std::size_t FixedSystemCount() const { return m_fixedSystems.size(); }
    std::size_t RenderSystemCount() const { return m_renderSystems.size(); }

private:
    std::vector<std::unique_ptr<IUpdateSystem>> m_updateSystems;
    std::vector<std::unique_ptr<IFixedUpdateSystem>> m_fixedSystems;
    std::vector<std::unique_ptr<IRenderSystem>> m_renderSystems;
};

} // namespace fw
