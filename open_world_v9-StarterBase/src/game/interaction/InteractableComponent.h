#pragma once

#include <string>

namespace fw {

enum class InteractionKind {
    Talk,
    Travel,
    Loot,
    SavePoint,
    Inspect
};

struct InteractableComponent {
    InteractionKind kind = InteractionKind::Inspect;
    std::string prompt = "Press E to interact";
    float radius = 2.5f;
    bool enabled = true;
};

} // namespace fw
