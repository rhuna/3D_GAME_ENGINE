#include "game/content/ContentPackLoader.h"

#include <filesystem>

#include "core/Logger.h"
#include "gameplay/prefabs/PrefabLibrary.h"
#include "scene/data/SceneLibrary.h"

namespace fw {
namespace fs = std::filesystem;

std::vector<std::string> ContentPackLoader::LoadEnabledPacks(const std::string& packsRoot,
                                                             const std::vector<std::string>& enabledPackIds,
                                                             PrefabLibrary& prefabs,
                                                             SceneLibrary& scenes) {
    std::vector<std::string> loaded;
    if (enabledPackIds.empty()) {
        return loaded;
    }

    for (const std::string& packId : enabledPackIds) {
        const fs::path packRoot = fs::path(packsRoot) / packId;
        if (!fs::exists(packRoot) || !fs::is_directory(packRoot)) {
            Logger::Warn("Content pack directory not found: " + packRoot.string());
            continue;
        }

        prefabs.AppendFromDirectory((packRoot / "prefabs").string());
        prefabs.AppendVariantsFromDirectory((packRoot / "prefab_variants").string());
        scenes.AppendFromDirectory((packRoot / "scenes").string());

        loaded.push_back(packId);
        Logger::Info("Loaded content pack: " + packId);
    }

    return loaded;
}

} // namespace fw
