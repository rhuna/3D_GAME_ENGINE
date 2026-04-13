#pragma once

namespace fw {
class Renderer;
class Scene;
class World;

class ServiceLocator {
public:
    static void ProvideRenderer(Renderer* renderer);
    static void ProvideScene(Scene* scene);
    static void ProvideWorld(World* world);

    static Renderer& RendererRef();
    static Scene& SceneRef();
    static World& WorldRef();

private:
    static inline Renderer* s_renderer = nullptr;
    static inline Scene* s_scene = nullptr;
    static inline World* s_world = nullptr;
};
}  // namespace fw
