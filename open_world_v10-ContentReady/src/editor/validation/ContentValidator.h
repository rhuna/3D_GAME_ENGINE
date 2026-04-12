#pragma once

#include <string>
#include <vector>

namespace fw {
class PrefabLibrary;
class SceneLibrary;

struct ValidationMessage {
    std::string severity;
    std::string text;
};

class ContentValidator {
public:
    static std::vector<ValidationMessage> ValidateAll(const PrefabLibrary& prefabs, const SceneLibrary& scenes);
};

} // namespace fw
