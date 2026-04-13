#include "serialization/WorldSerializer.h"

#include <fstream>
#include <vector>

#include "core/Logger.h"
#include "util/StringUtil.h"

namespace fw {

namespace {
std::string BoolToText(bool value) {
    return value ? "1" : "0";
}

bool TextToBool(const std::string& text) {
    return text == "1" || text == "true" || text == "TRUE";
}
}

bool WorldSerializer::SaveToFile(const World& world, const std::string& path) {
    std::ofstream out(path);
    if (!out.is_open()) {
        Logger::Error("Failed to save world: " + path);
        return false;
    }

    for (const Entity entity : world.Entities()) {
        const auto* transform = world.GetComponent<TransformComponent>(entity);
        if (!transform) {
            continue;
        }

        out << "ENTITY|" << entity
            << "|" << (world.GetComponent<TagComponent>(entity) ? world.GetComponent<TagComponent>(entity)->value : "")
            << "|" << transform->position.x << ',' << transform->position.y << ',' << transform->position.z
            << "|" << transform->scale.x << ',' << transform->scale.y << ',' << transform->scale.z;

        if (const auto* render = world.GetComponent<RenderComponent>(entity)) {
            out << "|RENDER|" << BoolToText(render->visible)
                << '|' << BoolToText(render->drawCube)
                << '|' << BoolToText(render->drawSphere)
                << '|' << render->cubeSize
                << '|' << render->sphereRadius
                << '|' << static_cast<int>(render->tint.r)
                << ',' << static_cast<int>(render->tint.g)
                << ',' << static_cast<int>(render->tint.b)
                << ',' << static_cast<int>(render->tint.a);
        }

        if (const auto* rb = world.GetComponent<RigidbodyComponent>(entity)) {
            out << "|RIGIDBODY|" << rb->velocity.x << ',' << rb->velocity.y << ',' << rb->velocity.z
                << '|' << rb->drag
                << '|' << BoolToText(rb->useGravity)
                << '|' << BoolToText(rb->kinematic);
        }

        if (const auto* collider = world.GetComponent<BoxColliderComponent>(entity)) {
            out << "|COLLIDER|" << collider->halfExtents.x << ',' << collider->halfExtents.y << ',' << collider->halfExtents.z
                << '|' << BoolToText(collider->isTrigger)
                << '|' << BoolToText(collider->isStatic);
        }

        out << '\n';
    }

    Logger::Info("Saved world to: " + path);
    return true;
}

bool WorldSerializer::LoadFromFile(World& world, const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        Logger::Warn("World save not found: " + path);
        return false;
    }

    world.Clear();

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;
        }

        std::vector<std::string> parts = StringUtil::Split(line, '|');
        if (parts.size() < 5 || parts[0] != "ENTITY") {
            continue;
        }

        const Entity entity = world.CreateEntity();
        if (!parts[2].empty()) {
            world.AddComponent<TagComponent>(entity, TagComponent{parts[2]});
        }

        TransformComponent transform;
        auto pos = StringUtil::Split(parts[3], ',');
        auto scale = StringUtil::Split(parts[4], ',');
        if (pos.size() == 3) {
            transform.position = Vector3{std::stof(pos[0]), std::stof(pos[1]), std::stof(pos[2])};
        }
        if (scale.size() == 3) {
            transform.scale = Vector3{std::stof(scale[0]), std::stof(scale[1]), std::stof(scale[2])};
        }
        world.AddComponent<TransformComponent>(entity, transform);

        std::size_t index = 5;
        while (index < parts.size()) {
            const std::string& section = parts[index++];
            if (section == "RENDER" && index + 5 < parts.size()) {
                RenderComponent render;
                render.visible = TextToBool(parts[index++]);
                render.drawCube = TextToBool(parts[index++]);
                render.drawSphere = TextToBool(parts[index++]);
                render.cubeSize = std::stof(parts[index++]);
                render.sphereRadius = std::stof(parts[index++]);
                auto color = StringUtil::Split(parts[index++], ',');
                if (color.size() == 4) {
                    render.tint = Color{
                        static_cast<unsigned char>(std::stoi(color[0])),
                        static_cast<unsigned char>(std::stoi(color[1])),
                        static_cast<unsigned char>(std::stoi(color[2])),
                        static_cast<unsigned char>(std::stoi(color[3]))
                    };
                }
                world.AddComponent<RenderComponent>(entity, render);
            } else if (section == "RIGIDBODY" && index + 3 < parts.size()) {
                RigidbodyComponent rb;
                auto vel = StringUtil::Split(parts[index++], ',');
                if (vel.size() == 3) {
                    rb.velocity = Vector3{std::stof(vel[0]), std::stof(vel[1]), std::stof(vel[2])};
                }
                rb.drag = std::stof(parts[index++]);
                rb.useGravity = TextToBool(parts[index++]);
                rb.kinematic = TextToBool(parts[index++]);
                world.AddComponent<RigidbodyComponent>(entity, rb);
            } else if (section == "COLLIDER" && index + 2 < parts.size()) {
                BoxColliderComponent collider;
                auto half = StringUtil::Split(parts[index++], ',');
                if (half.size() == 3) {
                    collider.halfExtents = Vector3{std::stof(half[0]), std::stof(half[1]), std::stof(half[2])};
                }
                collider.isTrigger = TextToBool(parts[index++]);
                collider.isStatic = TextToBool(parts[index++]);
                world.AddComponent<BoxColliderComponent>(entity, collider);
            } else {
                break;
            }
        }
    }

    Logger::Info("Loaded world from: " + path);
    return true;
}

} // namespace fw
