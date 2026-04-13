#include "core/EngineConfig.h"

#include "core/FileSystem.h"
#include "core/Logger.h"
#include "util/StringUtil.h"

namespace fw {

EngineConfig EngineConfig::LoadFromFile(const std::string& path) {
    EngineConfig config;

    if (!FileSystem::Exists(path)) {
        Logger::Warn("Config file not found, using defaults: " + path);
        return config;
    }

    const std::string content = FileSystem::ReadTextFile(path);
    for (const std::string& lineRaw : StringUtil::Split(content, '\n')) {
        const std::string line = StringUtil::Trim(lineRaw);
        if (line.empty() || line[0] == '#') {
            continue;
        }

        const auto parts = StringUtil::Split(line, '=');
        if (parts.size() != 2) {
            continue;
        }

        const std::string key = StringUtil::Trim(parts[0]);
        const std::string value = StringUtil::Trim(parts[1]);

        if (key == "window_width") config.windowWidth = std::stoi(value);
        else if (key == "window_height") config.windowHeight = std::stoi(value);
        else if (key == "window_title") config.windowTitle = value;
        else if (key == "vsync") config.vsync = (value == "true" || value == "1");
        else if (key == "target_fps") config.targetFps = std::stoi(value);
        else if (key == "fixed_timestep") config.fixedTimestep = std::stof(value);
        else if (key == "start_scene") config.startScene = value;
        else if (key == "camera_move_speed") config.cameraMoveSpeed = std::stof(value);
        else if (key == "camera_fast_speed") config.cameraFastSpeed = std::stof(value);
        else if (key == "camera_look_sensitivity") config.cameraLookSensitivity = std::stof(value);
    }

    Logger::Info("Loaded engine config: " + path);
    return config;
}

} // namespace fw
