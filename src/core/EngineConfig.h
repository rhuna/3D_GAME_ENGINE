#pragma once

#include <string>

namespace fw {

struct EngineConfig {
    int windowWidth = 1600;
    int windowHeight = 900;
    std::string windowTitle = "3D_GAME_ENGINE v4 Gameplay Framework";
    bool vsync = true;
    int targetFps = 144;
    float fixedTimestep = 1.0f / 60.0f;
    std::string startScene = "SandboxScene";
    float cameraMoveSpeed = 8.0f;
    float cameraFastSpeed = 18.0f;
    float cameraLookSensitivity = 0.003f;

    static EngineConfig LoadFromFile(const std::string& path);
};

} // namespace fw
