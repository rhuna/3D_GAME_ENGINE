#pragma once

#include <string>
#include <vector>

#include "game/project/GameProjectDefinition.h"

namespace fw {

struct StarterScaffoldResult {
    bool wroteAnyFiles = false;
    std::vector<std::string> createdFiles;
};

class GameStarterScaffold {
public:
    static StarterScaffoldResult EnsureStarterProject(const std::string& assetsRoot,
                                                      const GameProjectDefinition& project);
};

} // namespace fw
