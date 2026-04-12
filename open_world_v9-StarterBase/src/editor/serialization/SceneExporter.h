#pragma once

#include <string>

namespace fw {
class World;
class PrefabLibrary;

class SceneExporter {
public:
    static bool ExportWorldAsScene(const World& world, const PrefabLibrary& prefabs, const std::string& sceneName, const std::string& outputPath);
};

} // namespace fw
