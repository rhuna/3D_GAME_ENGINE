#pragma once

#include <string>

#include "game/dialogue/DialogueState.h"
#include "game/save/SaveGameProfile.h"

namespace fw {

struct OpenWorldGameState {
    SaveGameProfile saveProfile {};
    DialogueState dialogue {};
    std::string hoveredPrompt;
    std::string regionBanner = "Rivermoor Village";
    std::string lastInteraction;
    bool inventoryOpen = false;
    bool questLogOpen = false;
    bool equipmentOpen = false;
    std::string activeQuestId;
    float regionBannerTimer = 4.0f;
    int comboCount = 0;
    float comboTimer = 0.0f;
    std::string activeEncounterId;
};

} // namespace fw
