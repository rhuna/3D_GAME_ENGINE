#pragma once

#include <string>

#include "raylib.h"

namespace fw {
struct RenderComponent {
    Color tint {255, 255, 255, 255};
    bool visible = true;
    bool drawCube = true;
    bool drawSphere = false;
    float cubeSize = 1.0f;
    float sphereRadius = 0.5f;
    bool useModel = false;
    std::string modelPath;
};
}
