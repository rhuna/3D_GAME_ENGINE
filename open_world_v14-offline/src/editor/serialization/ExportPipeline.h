#pragma once

#include <string>
#include <vector>

namespace fw {

class World;
class PrefabLibrary;

struct ExportBundleSettings {
    std::string buildId;
    std::string gameTitle;
    std::string version;
    std::string startScene;
    std::string startRegion;
    std::string profileId;
    std::string menuId;
    std::string hudId;
    std::string outputFolder;
    bool includeSaves = false;
    bool compressExport = false;
    bool zipOutput = false;
};

struct ExportBundleResult {
    bool success = false;
    std::string outputFolder;
    std::string exportedScenePath;
    std::string manifestPath;
    std::string launcherPath;
    std::string packageScriptPath;
    std::string executablePath;
    std::vector<std::string> copiedRoots;
    std::string message;
};

class ExportPipeline {
public:
    static ExportBundleResult StageBundle(const ExportBundleSettings& settings, const World& world, const PrefabLibrary& prefabs);
};

} // namespace fw
