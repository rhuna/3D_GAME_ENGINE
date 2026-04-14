#include "gameplay/Spawn.h"

#include "ecs/components/BoxColliderComponent.h"
#include "ecs/components/LifetimeComponent.h"
#include "ecs/components/RenderComponent.h"
#include "ecs/components/RigidbodyComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"

namespace fw::spawn {

Entity SpawnStaticBox(World& world, const std::string& tag, const Vector3& position,
                      const Vector3& scale, Color tint) {
    const Entity id = world.CreateEntity();
    world.AddComponent<TagComponent>(id, TagComponent{tag});
    world.AddComponent<TransformComponent>(id, TransformComponent{position, Vector3{0.0f, 0.0f, 0.0f}, scale});
    RenderComponent render{};
    render.tint = tint;
    render.visible = true;
    render.drawCube = true;
    render.drawSphere = false;
    render.cubeSize = 1.0f;
    world.AddComponent<RenderComponent>(id, render);
    world.AddComponent<BoxColliderComponent>(id, BoxColliderComponent{Vector3{scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f}, false, true});
    return id;
}

Entity SpawnDynamicBox(World& world, const std::string& tag, const Vector3& position,
                       const Vector3& scale, Color tint, bool useGravity) {
    const Entity id = world.CreateEntity();
    world.AddComponent<TagComponent>(id, TagComponent{tag});
    world.AddComponent<TransformComponent>(id, TransformComponent{position, Vector3{0.0f, 0.0f, 0.0f}, scale});
    RenderComponent render{};
    render.tint = tint;
    render.visible = true;
    render.drawCube = true;
    render.drawSphere = false;
    render.cubeSize = 1.0f;
    world.AddComponent<RenderComponent>(id, render);
    world.AddComponent<BoxColliderComponent>(id, BoxColliderComponent{Vector3{scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f}, false, false});
    world.AddComponent<RigidbodyComponent>(id, RigidbodyComponent{Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 0.0f}, 1.5f, useGravity, false});
    return id;
}

Entity SpawnPlayer(World& world, const Vector3& position) {
    const Entity id = world.CreateEntity();
    world.AddComponent<TagComponent>(id, TagComponent{"player"});
    world.AddComponent<TransformComponent>(id, TransformComponent{position, Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.8f, 1.8f, 0.8f}});
    RenderComponent render{};
    render.tint = Color{70, 170, 255, 255};
    render.visible = true;
    render.drawCube = true;
    render.drawSphere = false;
    world.AddComponent<RenderComponent>(id, render);
    world.AddComponent<BoxColliderComponent>(id, BoxColliderComponent{Vector3{0.4f, 0.9f, 0.4f}, false, false});
    world.AddComponent<RigidbodyComponent>(id, RigidbodyComponent{Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 0.0f}, 3.0f, true, false});
    return id;
}

Entity SpawnProjectile(World& world, const Vector3& position, const Vector3& velocity) {
    const Entity id = world.CreateEntity();
    world.AddComponent<TagComponent>(id, TagComponent{"projectile"});
    world.AddComponent<TransformComponent>(id, TransformComponent{position, Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.35f, 0.35f, 0.35f}});
    RenderComponent render{};
    render.tint = Color{255, 220, 90, 255};
    render.visible = true;
    render.drawCube = false;
    render.drawSphere = true;
    render.sphereRadius = 0.18f;
    world.AddComponent<RenderComponent>(id, render);
    world.AddComponent<BoxColliderComponent>(id, BoxColliderComponent{Vector3{0.18f, 0.18f, 0.18f}, true, false});
    world.AddComponent<RigidbodyComponent>(id, RigidbodyComponent{velocity, Vector3{0.0f, 0.0f, 0.0f}, 0.1f, false, false});
    world.AddComponent<LifetimeComponent>(id, LifetimeComponent{4.0f});
    return id;
}

} // namespace fw::spawn
