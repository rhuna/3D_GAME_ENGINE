#pragma once

namespace fw {

struct ContentWorkflowChecklist {
    static constexpr const char* Steps[] = {
        "Define region pack",
        "Define travel links",
        "Author encounters",
        "Author merchants and rewards",
        "Connect progression flags",
        "Verify save/load handoff"
    };
};

} // namespace fw
