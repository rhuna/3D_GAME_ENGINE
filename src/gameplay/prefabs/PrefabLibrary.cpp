#include "gameplay/prefabs/PrefabLibrary.h"

#include <filesystem>
#include <fstream>

#include "core/Logger.h"
#include "util/StringUtil.h"

namespace fw {

namespace {

bool ParseBool(const std::string& value) {
    return value == "1" || value == "true" || value == "TRUE" || value == "True";
}

float ParseFloat(const std::string& value) {
    try {
        return std::stof(value);
    } catch (...) {
        return 0.0f;
    }
}

Vector3 ParseVector3(const std::string& value) {
    const auto parts = StringUtil::Split(value, ',');
    if (parts.size() != 3) {
        return Vector3{0.0f, 0.0f, 0.0f};
    }
    return Vector3{
        ParseFloat(StringUtil::Trim(parts[0])),
        ParseFloat(StringUtil::Trim(parts[1])),
        ParseFloat(StringUtil::Trim(parts[2]))
    };
}

Color ParseColor(const std::string& value) {
    const auto parts = StringUtil::Split(value, ',');
    if (parts.size() != 4) {
        return WHITE;
    }
    return Color{
        static_cast<unsigned char>(ParseFloat(StringUtil::Trim(parts[0]))),
        static_cast<unsigned char>(ParseFloat(StringUtil::Trim(parts[1]))),
        static_cast<unsigned char>(ParseFloat(StringUtil::Trim(parts[2]))),
        static_cast<unsigned char>(ParseFloat(StringUtil::Trim(parts[3])))
    };
}

}

void PrefabLibrary::Clear() {
    m_prefabs.clear();
}

bool PrefabLibrary::LoadFromDirectory(const std::string& directoryPath) {
    Clear();

    namespace fs = std::filesystem;
    if (!fs::exists(directoryPath)) {
        Logger::Warn("Prefab directory not found: " + directoryPath);
        return false;
    }

    bool loadedAny = false;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        if (entry.path().extension() != ".prefab") {
            continue;
        }
        loadedAny = LoadFromFile(entry.path().string()) || loadedAny;
    }

    Logger::Info("Loaded prefab count: " + std::to_string(m_prefabs.size()));
    return loadedAny;
}

void PrefabLibrary::Register(PrefabDefinition definition) {
    if (definition.name.empty()) {
        return;
    }
    m_prefabs[definition.name] = std::move(definition);
}

const PrefabDefinition* PrefabLibrary::Find(const std::string& name) const {
    const auto it = m_prefabs.find(name);
    return it != m_prefabs.end() ? &it->second : nullptr;
}

bool PrefabLibrary::LoadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        Logger::Warn("Failed to open prefab file: " + filePath);
        return false;
    }

    PrefabDefinition definition;
    std::string line;

    while (std::getline(file, line)) {
        line = StringUtil::Trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }

        const std::size_t equals = line.find('=');
        if (equals == std::string::npos) {
            continue;
        }

        const std::string key = StringUtil::Trim(line.substr(0, equals));
        const std::string value = StringUtil::Trim(line.substr(equals + 1));

        if (key == "name") definition.name = value;
        else if (key == "tag") definition.tag.value = value;
        else if (key == "position") definition.transform.position = ParseVector3(value);
        else if (key == "rotationEuler") definition.transform.rotationEuler = ParseVector3(value);
        else if (key == "scale") definition.transform.scale = ParseVector3(value);
        else if (key == "color") definition.render.tint = ParseColor(value);
        else if (key == "visible") definition.render.visible = ParseBool(value);
        else if (key == "drawCube") definition.render.drawCube = ParseBool(value);
        else if (key == "drawSphere") definition.render.drawSphere = ParseBool(value);
        else if (key == "cubeSize") definition.render.cubeSize = ParseFloat(value);
        else if (key == "sphereRadius") definition.render.sphereRadius = ParseFloat(value);
        else if (key == "useModel") definition.render.useModel = ParseBool(value);
        else if (key == "modelPath") definition.render.modelPath = value;
        else if (key == "colliderHalfExtents") { definition.hasCollider = true; definition.collider.halfExtents = ParseVector3(value); }
        else if (key == "colliderIsTrigger") { definition.hasCollider = true; definition.collider.isTrigger = ParseBool(value); }
        else if (key == "colliderIsStatic") { definition.hasCollider = true; definition.collider.isStatic = ParseBool(value); }
        else if (key == "rigidbodyVelocity") { definition.hasRigidbody = true; definition.rigidbody.velocity = ParseVector3(value); }
        else if (key == "rigidbodyAcceleration") { definition.hasRigidbody = true; definition.rigidbody.acceleration = ParseVector3(value); }
        else if (key == "rigidbodyDrag") { definition.hasRigidbody = true; definition.rigidbody.drag = ParseFloat(value); }
        else if (key == "rigidbodyUseGravity") { definition.hasRigidbody = true; definition.rigidbody.useGravity = ParseBool(value); }
        else if (key == "rigidbodyKinematic") { definition.hasRigidbody = true; definition.rigidbody.kinematic = ParseBool(value); }
        else if (key == "lifetime") { definition.hasLifetime = true; definition.lifetime.secondsRemaining = ParseFloat(value); }
    }

    if (definition.name.empty()) {
        Logger::Warn("Prefab missing name: " + filePath);
        return false;
    }

    if (definition.tag.value.empty()) {
        definition.tag.value = definition.name;
    }

    Register(std::move(definition));
    Logger::Info("Loaded prefab: " + filePath);
    return true;
}

} // namespace fw
