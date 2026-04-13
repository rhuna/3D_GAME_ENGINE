#pragma once

#include <string>

namespace fw {

struct AnimationRigComponent {
    std::string rigId;
    std::string skeletonAssetPath;
    bool usesRootMotion = false;
};

} // namespace fw
