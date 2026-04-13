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

    // Legacy direct-path support. V58 keeps this so older content still works.
    std::string modelPath;

    // V58 asset-id based rendering fields.
    std::string meshId;
    std::string textureId;
    std::string lod0MeshId;
    std::string lod1MeshId;
    std::string lod2MeshId;
    float lod1Distance = 25.0f;
    float lod2Distance = 60.0f;
};
}
