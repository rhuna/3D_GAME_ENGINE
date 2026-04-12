#include "scene/data/SceneLibrary.h"

#include <filesystem>
#include <fstream>

#include "core/Logger.h"
#include "util/StringUtil.h"

namespace fw {

namespace {

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

} // namespace

void SceneLibrary::Clear() {
    m_scenes.clear();
}

bool SceneLibrary::LoadFromDirectory(const std::string& directoryPath) {
    Clear();

    namespace fs = std::filesystem;
    if (!fs::exists(directoryPath)) {
        Logger::Warn("Scene directory not found: " + directoryPath);
        return false;
    }

    bool loadedAny = false;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".scene") {
            continue;
        }
        loadedAny = LoadSceneFile(entry.path().string()) || loadedAny;
    }

    Logger::Info("Loaded scene count: " + std::to_string(m_scenes.size()));
    return loadedAny;
}

void SceneLibrary::Register(SceneDefinition definition) {
    if (definition.name.empty()) {
        return;
    }
    m_scenes[definition.name] = std::move(definition);
}

const SceneDefinition* SceneLibrary::Find(const std::string& name) const {
    const auto it = m_scenes.find(name);
    return it != m_scenes.end() ? &it->second : nullptr;
}

bool SceneLibrary::LoadSceneFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        Logger::Warn("Failed to open scene file: " + filePath);
        return false;
    }

    SceneDefinition definition;
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

        if (key == "scene") {
            definition.name = value;
            continue;
        }

        if (key != "spawn") {
            continue;
        }

        SceneSpawnEntry entry;
        for (const std::string& tokenRaw : StringUtil::Split(value, ';')) {
            const std::string token = StringUtil::Trim(tokenRaw);
            if (token.empty()) {
                continue;
            }

            const std::size_t tokenEquals = token.find('=');
            if (tokenEquals == std::string::npos) {
                continue;
            }

            const std::string tokenKey = StringUtil::Trim(token.substr(0, tokenEquals));
            const std::string tokenValue = StringUtil::Trim(token.substr(tokenEquals + 1));

            if (tokenKey == "prefab") entry.prefabName = tokenValue;
            else if (tokenKey == "variant") entry.variantName = tokenValue;
            else if (tokenKey == "tag") entry.tagOverride = tokenValue;
            else if (tokenKey == "position") { entry.hasPosition = true; entry.transform.position = ParseVector3(tokenValue); }
            else if (tokenKey == "rotationEuler") { entry.hasRotation = true; entry.transform.rotationEuler = ParseVector3(tokenValue); }
            else if (tokenKey == "scale") { entry.hasScale = true; entry.transform.scale = ParseVector3(tokenValue); }
            else if (tokenKey == "color") { entry.hasTint = true; entry.tint = ParseColor(tokenValue); }
        }

        if (!entry.prefabName.empty() || !entry.variantName.empty()) {
            definition.entries.push_back(entry);
        }
    }

    if (definition.name.empty()) {
        namespace fs = std::filesystem;
        definition.name = fs::path(filePath).stem().string();
    }

    Register(std::move(definition));
    Logger::Info("Loaded scene: " + filePath);
    return true;
}

} // namespace fw
