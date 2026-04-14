#pragma once

#include <string>

#include "game/project/GameProjectDefinition.h"

namespace fw {

class GameProjectLoader {
public:
    static GameProjectDefinition LoadFromFile(const std::string& filePath);
};

} // namespace fw
