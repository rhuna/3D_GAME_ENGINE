#include "game/project/GameProjectLoader.h"

#include <fstream>

#include "core/Logger.h"
#include "util/StringUtil.h"

namespace fw {
namespace {
std::vector<std::string> ParseList(const std::string& value) {
    std::vector<std::string> result;
    for (const std::string& raw : StringUtil::Split(value, ',')) {
        const std::string item = StringUtil::Trim(raw);
        if (!item.empty()) {
            result.push_back(item);
        }
    }
    return result;
}
} // namespace

GameProjectDefinition GameProjectLoader::LoadFromFile(const std::string& filePath) {
    GameProjectDefinition definition;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        Logger::Warn("Game project file not found, using defaults: " + filePath);
        return definition;
    }

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

        if (key == "project") definition.id = value;
        else if (key == "display_name") definition.displayName = value;
        else if (key == "start_scene") definition.startScene = value;
        else if (key == "start_region") definition.startRegion = value;
        else if (key == "starter_template") definition.starterTemplate = value;
        else if (key == "create_starter_content") definition.createStarterContentIfMissing = (value == "true" || value == "1" || value == "yes");
        else if (key == "content_pack") definition.enabledContentPacks.push_back(value);
        else if (key == "content_packs") {
            const auto list = ParseList(value);
            definition.enabledContentPacks.insert(definition.enabledContentPacks.end(), list.begin(), list.end());
        }
        else if (key == "preload") {
            const auto list = ParseList(value);
            definition.startupPreloadAssets.insert(definition.startupPreloadAssets.end(), list.begin(), list.end());
        }
    }

    Logger::Info("Loaded game project: " + definition.displayName + " startScene=" + definition.startScene + " startRegion=" + definition.startRegion);
    return definition;
}

} // namespace fw
