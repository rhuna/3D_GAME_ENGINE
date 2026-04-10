#include "ServiceLocator.h"

#include <cassert>

namespace fw {
class Renderer;
class Scene;
class World;

void ServiceLocator::ProvideRenderer(Renderer* renderer) { s_renderer = renderer; }
void ServiceLocator::ProvideScene(Scene* scene) { s_scene = scene; }
void ServiceLocator::ProvideWorld(World* world) { s_world = world; }

Renderer& ServiceLocator::RendererRef() {
    assert(s_renderer && "Renderer service not provided.");
    return *s_renderer;
}

Scene& ServiceLocator::SceneRef() {
    assert(s_scene && "Scene service not provided.");
    return *s_scene;
}

World& ServiceLocator::WorldRef() {
    assert(s_world && "World service not provided.");
    return *s_world;
}
}  // namespace fw
