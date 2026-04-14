#pragma once

#include <string>
#include <vector>

namespace fw {

class PrefabLibrary;
class SceneLibrary;

class ContentPackLoader {
public:
    static std::vector<std::string> LoadEnabledPacks(const std::string& packsRoot,
                                                     const std::vector<std::string>& enabledPackIds,
                                                     PrefabLibrary& prefabs,
                                                     SceneLibrary& scenes);
};

} // namespace fw
