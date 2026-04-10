#include "gameplay/WorldSerializer.h"

#include <fstream>
#include <sstream>
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

    for (const auto& entity : world.Entities()) {
        if (!entity.active || !entity.transform) {
            continue;
        }

        const auto& t = *entity.transform;
        out << "ENTITY|" << entity.id
            << "|" << (entity.tag ? entity.tag->value : "")
            << "|" << t.position.x << ',' << t.position.y << ',' << t.position.z
            << "|" << t.scale.x << ',' << t.scale.y << ',' << t.scale.z;

        if (entity.render) {
            const auto& r = *entity.render;
            out << "|RENDER|" << BoolToText(r.visible)
                << '|' << BoolToText(r.drawCube)
                << '|' << BoolToText(r.drawSphere)
                << '|' << r.cubeSize
                << '|' << r.sphereRadius
                << '|' << static_cast<int>(r.tint.r)
                << ',' << static_cast<int>(r.tint.g)
                << ',' << static_cast<int>(r.tint.b)
                << ',' << static_cast<int>(r.tint.a);
        }

        if (entity.rigidbody) {
            const auto& rb = *entity.rigidbody;
            out << "|RIGIDBODY|" << rb.velocity.x << ',' << rb.velocity.y << ',' << rb.velocity.z
                << '|' << rb.drag
                << '|' << BoolToText(rb.useGravity)
                << '|' << BoolToText(rb.kinematic);
        }

        if (entity.collider) {
            const auto& c = *entity.collider;
            out << "|COLLIDER|" << c.halfExtents.x << ',' << c.halfExtents.y << ',' << c.halfExtents.z
                << '|' << BoolToText(c.isTrigger)
                << '|' << BoolToText(c.isStatic);
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

        const Entity id = world.CreateEntity();
        EntityRecord* entity = world.FindEntity(id);
        if (!entity || !entity->transform) {
            continue;
        }

        if (!parts[2].empty()) {
            entity->tag = TagComponent{parts[2]};
        }

        auto pos = StringUtil::Split(parts[3], ',');
        auto scale = StringUtil::Split(parts[4], ',');
        if (pos.size() == 3) {
            entity->transform->position = Vector3{std::stof(pos[0]), std::stof(pos[1]), std::stof(pos[2])};
        }
        if (scale.size() == 3) {
            entity->transform->scale = Vector3{std::stof(scale[0]), std::stof(scale[1]), std::stof(scale[2])};
        }

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
                entity->render = render;
            } else if (section == "RIGIDBODY" && index + 3 < parts.size()) {
                RigidbodyComponent rb;
                auto vel = StringUtil::Split(parts[index++], ',');
                if (vel.size() == 3) {
                    rb.velocity = Vector3{std::stof(vel[0]), std::stof(vel[1]), std::stof(vel[2])};
                }
                rb.drag = std::stof(parts[index++]);
                rb.useGravity = TextToBool(parts[index++]);
                rb.kinematic = TextToBool(parts[index++]);
                entity->rigidbody = rb;
            } else if (section == "COLLIDER" && index + 2 < parts.size()) {
                BoxColliderComponent collider;
                auto half = StringUtil::Split(parts[index++], ',');
                if (half.size() == 3) {
                    collider.halfExtents = Vector3{std::stof(half[0]), std::stof(half[1]), std::stof(half[2])};
                }
                collider.isTrigger = TextToBool(parts[index++]);
                collider.isStatic = TextToBool(parts[index++]);
                entity->collider = collider;
            } else {
                break;
            }
        }
    }

    Logger::Info("Loaded world from: " + path);
    return true;
}

} // namespace fw
