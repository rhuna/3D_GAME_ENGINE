#include "gameplay/Spawn.h"

namespace fw::spawn {

Entity SpawnStaticBox(World& world, const std::string& tag, const Vector3& position,
                      const Vector3& scale, Color tint) {
    const Entity id = world.CreateEntity();
    EntityRecord* entity = world.FindEntity(id);
    if (!entity) {
        return 0;
    }

    entity->tag = TagComponent{tag};
    entity->transform = TransformComponent{position, Vector3{0.0f, 0.0f, 0.0f}, scale};
    entity->render = RenderComponent{tint, true, true, false, 1.0f, 0.5f, false, ""};
    entity->collider = BoxColliderComponent{Vector3{scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f}, false, true};
    entity->rigidbody.reset();
    return id;
}

Entity SpawnDynamicBox(World& world, const std::string& tag, const Vector3& position,
                       const Vector3& scale, Color tint, bool useGravity) {
    const Entity id = world.CreateEntity();
    EntityRecord* entity = world.FindEntity(id);
    if (!entity) {
        return 0;
    }

    entity->tag = TagComponent{tag};
    entity->transform = TransformComponent{position, Vector3{0.0f, 0.0f, 0.0f}, scale};
    entity->render = RenderComponent{tint, true, true, false, 1.0f, 0.5f, false, ""};
    entity->collider = BoxColliderComponent{Vector3{scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f}, false, false};
    entity->rigidbody = RigidbodyComponent{Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 0.0f}, 1.5f, useGravity, false};
    return id;
}

Entity SpawnPlayer(World& world, const Vector3& position) {
    const Entity id = world.CreateEntity();
    EntityRecord* entity = world.FindEntity(id);
    if (!entity) {
        return 0;
    }

    entity->tag = TagComponent{"player"};
    entity->transform = TransformComponent{position, Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.8f, 1.8f, 0.8f}};
    entity->render = RenderComponent{Color{70, 170, 255, 255}, true, true, false, 1.0f, 0.5f, false, ""};
    entity->collider = BoxColliderComponent{Vector3{0.4f, 0.9f, 0.4f}, false, false};
    entity->rigidbody = RigidbodyComponent{Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 0.0f}, 3.0f, true, false};
    return id;
}

Entity SpawnProjectile(World& world, const Vector3& position, const Vector3& velocity) {
    const Entity id = world.CreateEntity();
    EntityRecord* entity = world.FindEntity(id);
    if (!entity) {
        return 0;
    }

    entity->tag = TagComponent{"projectile"};
    entity->transform = TransformComponent{position, Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.35f, 0.35f, 0.35f}};
    entity->render = RenderComponent{Color{255, 220, 90, 255}, true, false, true, 1.0f, 0.18f, false, ""};
    entity->collider = BoxColliderComponent{Vector3{0.18f, 0.18f, 0.18f}, true, false};
    entity->rigidbody = RigidbodyComponent{velocity, Vector3{0.0f, 0.0f, 0.0f}, 0.1f, false, false};
    entity->lifetime = LifetimeComponent{4.0f};
    return id;
}

} // namespace fw::spawn
