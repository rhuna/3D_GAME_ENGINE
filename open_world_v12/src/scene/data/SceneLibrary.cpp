#include "scene/data/SceneLibrary.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <unordered_set>

#include "core/Logger.h"
#include "util/StringUtil.h"

namespace fw {

namespace {

float ParseFloat(const std::string& value) {
    try { return std::stof(value); } catch (...) { return 0.0f; }
}

Vector3 ParseVector3(const std::string& value) {
    const auto parts = StringUtil::Split(value, ',');
    if (parts.size() != 3) return Vector3{0.0f, 0.0f, 0.0f};
    return Vector3{
        ParseFloat(StringUtil::Trim(parts[0])),
        ParseFloat(StringUtil::Trim(parts[1])),
        ParseFloat(StringUtil::Trim(parts[2]))
    };
}

Color ParseColor(const std::string& value) {
    const auto parts = StringUtil::Split(value, ',');
    if (parts.size() != 4) return WHITE;
    return Color{
        static_cast<unsigned char>(ParseFloat(StringUtil::Trim(parts[0]))),
        static_cast<unsigned char>(ParseFloat(StringUtil::Trim(parts[1]))),
        static_cast<unsigned char>(ParseFloat(StringUtil::Trim(parts[2]))),
        static_cast<unsigned char>(ParseFloat(StringUtil::Trim(parts[3])))
    };
}

std::vector<std::string> ParseList(const std::string& value) {
    std::vector<std::string> out;
    for (const std::string& raw : StringUtil::Split(value, ',')) {
        const std::string item = StringUtil::Trim(raw);
        if (!item.empty()) out.push_back(item);
    }
    return out;
}

void AppendEntriesRecursive(const SceneLibrary& library,
                            const SceneDefinition& input,
                            SceneDefinition& output,
                            std::unordered_set<std::string>& stack) {
    if (stack.contains(input.name)) return;
    stack.insert(input.name);

    for (const std::string& includeName : input.includedScenes) {
        if (const SceneDefinition* included = library.Find(includeName)) {
            AppendEntriesRecursive(library, *included, output, stack);
        }
    }

    output.entries.insert(output.entries.end(), input.entries.begin(), input.entries.end());
    for (const std::string& asset : input.preloadAssets) {
        if (std::find(output.preloadAssets.begin(), output.preloadAssets.end(), asset) == output.preloadAssets.end()) {
            output.preloadAssets.push_back(asset);
        }
    }

    stack.erase(input.name);
}

} // namespace

void SceneLibrary::Clear() { m_scenes.clear(); }

bool SceneLibrary::LoadFromDirectory(const std::string& directoryPath) {
    Clear();
    const bool loadedAny = AppendFromDirectory(directoryPath);
    Logger::Info("Loaded scene count: " + std::to_string(m_scenes.size()));
    return loadedAny;
}

bool SceneLibrary::AppendFromDirectory(const std::string& directoryPath) {
    namespace fs = std::filesystem;
    if (!fs::exists(directoryPath)) {
        Logger::Warn("Scene directory not found: " + directoryPath);
        return false;
    }

    bool loadedAny = false;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".scene") continue;
        loadedAny = LoadSceneFile(entry.path().string()) || loadedAny;
    }
    return loadedAny;
}

void SceneLibrary::Register(SceneDefinition definition) {
    if (definition.name.empty()) return;
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
        if (line.empty() || line[0] == '#') continue;

        const std::size_t equals = line.find('=');
        if (equals == std::string::npos) continue;

        const std::string key = StringUtil::Trim(line.substr(0, equals));
        const std::string value = StringUtil::Trim(line.substr(equals + 1));

        if (key == "scene") { definition.name = value; continue; }
        if (key == "display_name") { definition.displayName = value; continue; }
        if (key == "player_prefab") { definition.playerPrefab = value; continue; }
        if (key == "auto_spawn_player") { definition.autoSpawnPlayer = (value == "true" || value == "1" || value == "yes"); continue; }
        if (key == "preload") { definition.preloadAssets = ParseList(value); continue; }
        if (key == "include") { definition.includedScenes = ParseList(value); continue; }
        if (key != "spawn") continue;

        SceneSpawnEntry entry;
        for (const std::string& tokenRaw : StringUtil::Split(value, ';')) {
            const std::string token = StringUtil::Trim(tokenRaw);
            if (token.empty()) continue;
            const std::size_t tokenEquals = token.find('=');
            if (tokenEquals == std::string::npos) continue;
            const std::string tokenKey = StringUtil::Trim(token.substr(0, tokenEquals));
            const std::string tokenValue = StringUtil::Trim(token.substr(tokenEquals + 1));

            if (tokenKey == "prefab") entry.prefabName = tokenValue;
            else if (tokenKey == "variant") entry.variantName = tokenValue;
            else if (tokenKey == "kit") entry.kitName = tokenValue;
            else if (tokenKey == "tag") entry.tagOverride = tokenValue;
            else if (tokenKey == "position") { entry.hasPosition = true; entry.transform.position = ParseVector3(tokenValue); }
            else if (tokenKey == "rotationEuler") { entry.hasRotation = true; entry.transform.rotationEuler = ParseVector3(tokenValue); }
            else if (tokenKey == "scale") { entry.hasScale = true; entry.transform.scale = ParseVector3(tokenValue); }
            else if (tokenKey == "color") { entry.hasTint = true; entry.tint = ParseColor(tokenValue); }
        }

        if (!entry.prefabName.empty() || !entry.variantName.empty() || !entry.kitName.empty()) {
            definition.entries.push_back(entry);
        }
    }

    if (definition.name.empty()) {
        namespace fs = std::filesystem;
        definition.name = fs::path(filePath).stem().string();
    }
    if (definition.displayName.empty()) definition.displayName = definition.name;

    Register(std::move(definition));
    Logger::Info("Loaded scene: " + filePath);
    return true;
}

SceneDefinition SceneLibrary::BuildResolvedScene(const std::string& name) const {
    SceneDefinition out;
    const SceneDefinition* base = Find(name);
    if (!base) return out;

    out = *base;
    out.entries.clear();
    out.preloadAssets.clear();

    std::unordered_set<std::string> stack;
    AppendEntriesRecursive(*this, *base, out, stack);
    return out;
}

bool SceneLibrary::BuildResolvedScene(const std::string& name, SceneDefinition& outDefinition) const {
    outDefinition = BuildResolvedScene(name);
    return !outDefinition.name.empty();
}

} // namespace fw
