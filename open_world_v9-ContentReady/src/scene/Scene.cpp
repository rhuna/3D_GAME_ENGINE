#include "scene/Scene.h"
#include "raylib.h"
#include "raymath.h"
#include "core/Input.h"
#include "ecs/World.h"
#include <algorithm>
#include <cmath>

namespace fw {
namespace {
float ToRadians(float degrees) {
    return degrees * 3.1415926535f / 180.0f;
}
}  // namespace

Scene::Scene(World& world) : m_world(world) {
    m_camera.position = {0.0f, 2.0f, 6.0f};
    m_camera.target = {0.0f, 1.0f, 0.0f};
    m_camera.up = {0.0f, 1.0f, 0.0f};
    m_camera.fovy = 60.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void Scene::LoadDefaultScene() {
    Entity& floor = m_world.CreateEntity("Floor");
    floor.hasMesh = true;
    floor.mesh.primitive = MeshComponent::Primitive::Plane;
    floor.mesh.size = {20.0f, 1.0f, 20.0f};
    floor.mesh.tint = DARKGREEN;

    Entity& cube = m_world.CreateEntity("CenterCube");
    cube.hasMesh = true;
    cube.mesh.primitive = MeshComponent::Primitive::Cube;
    cube.mesh.size = {1.5f, 1.5f, 1.5f};
    cube.mesh.tint = ORANGE;
    cube.transform.position = {0.0f, 0.75f, 0.0f};
    cube.hasRotator = true;
    cube.rotator.degreesPerSecond = {0.0f, 45.0f, 0.0f};

    Entity& sphere = m_world.CreateEntity("MarkerSphere");
    sphere.hasMesh = true;
    sphere.mesh.primitive = MeshComponent::Primitive::Sphere;
    sphere.mesh.size = {0.75f, 0.75f, 0.75f};
    sphere.mesh.tint = SKYBLUE;
    sphere.transform.position = {3.0f, 1.0f, 0.0f};
}

void Scene::Update(float dt) {
    UpdateCamera(dt);
}

Camera3D& Scene::MainCamera() { return m_camera; }
const Camera3D& Scene::MainCamera() const { return m_camera; }

void Scene::UpdateCamera(float dt) {
    constexpr float moveSpeed = 6.0f;
    constexpr float mouseSensitivity = 0.12f;
    constexpr float maxPitch = 89.0f;

    if (Input::IsLookActive()) {
        const Vector2 delta = Input::MouseDelta();
        m_yaw += delta.x * mouseSensitivity;
        m_pitch -= delta.y * mouseSensitivity;
        m_pitch = std::clamp(m_pitch, -maxPitch, maxPitch);
    }

    const float yawRad = ToRadians(m_yaw);
    const float pitchRad = ToRadians(m_pitch);

    Vector3 forward = {
        std::cosf(pitchRad) * std::sinf(yawRad),
        std::sinf(pitchRad),
        std::cosf(pitchRad) * std::cosf(yawRad),
    };
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, {0.0f, 1.0f, 0.0f}));

    if (Input::IsMoveForward()) {
        m_camera.position = Vector3Add(m_camera.position, Vector3Scale(forward, moveSpeed * dt));
    }
    if (Input::IsMoveBackward()) {
        m_camera.position = Vector3Subtract(m_camera.position, Vector3Scale(forward, moveSpeed * dt));
    }
    if (Input::IsMoveLeft()) {
        m_camera.position = Vector3Subtract(m_camera.position, Vector3Scale(right, moveSpeed * dt));
    }
    if (Input::IsMoveRight()) {
        m_camera.position = Vector3Add(m_camera.position, Vector3Scale(right, moveSpeed * dt));
    }
    if (Input::IsMoveUp()) {
        m_camera.position.y += moveSpeed * dt;
    }
    if (Input::IsMoveDown()) {
        m_camera.position.y -= moveSpeed * dt;
    }

    m_camera.target = Vector3Add(m_camera.position, forward);
}
}  // namespace fw
