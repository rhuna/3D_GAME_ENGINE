#pragma once

#include <string>
#include <vector>

#include "core/Time.h"
#include "ecs/Entity.h"
#include "ecs/World.h"
#include "editor/validation/ContentValidator.h"
#include "raylib.h"

namespace fw {

class DebugOverlay {
public:
    void Draw(const Time& time, const World& world, const Camera3D& camera, const std::string& sceneName, Entity selectedEntity, const std::vector<ValidationMessage>& validationMessages, const std::string& lastExportPath, bool inspectorVisible) const;
};

} // namespace fw
