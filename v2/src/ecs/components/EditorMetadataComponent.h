#pragma once

#include <string>

namespace fw {
struct EditorMetadataComponent {
    std::string sourcePrefab;
    std::string sourceVariant;
    bool locked = false;
    bool hiddenInEditor = false;
};
}
