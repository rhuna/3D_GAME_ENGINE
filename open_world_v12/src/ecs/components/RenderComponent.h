#pragma once

#include <string>

#include "raylib.h"

namespace fw {

struct PrimitiveShape {
    enum Value {
        None = 0,
        Cube,
        Sphere,
        Cylinder,
        Plane
    } value = Cube;

    PrimitiveShape() = default;
    PrimitiveShape(Value v) : value(v) {}

    PrimitiveShape& operator=(Value v) {
        value = v;
        return *this;
    }

    PrimitiveShape& operator=(const std::string& text) {
        if (text == "cube") value = Cube;
        else if (text == "sphere") value = Sphere;
        else if (text == "cylinder") value = Cylinder;
        else if (text == "plane") value = Plane;
        else value = None;
        return *this;
    }

    operator Value() const { return value; }
};

struct RenderComponent {
    Color tint {255, 255, 255, 255};
    bool visible = true;
    bool drawCube = true;
    bool drawSphere = false;
    float cubeSize = 1.0f;
    float sphereRadius = 0.5f;

    bool useModel = false;
    std::string modelPath;

    PrimitiveShape primitiveShape = PrimitiveShape::Cube;
    float primitiveSize = 1.0f;
    float primitiveRadius = 0.5f;
    float primitiveHeight = 1.0f;

    std::string meshId;
    std::string textureId;
};

} // namespace fw
