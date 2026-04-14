#pragma once

#include <string>   // Stores text ids, categories, owners, and status values.
#include <vector>   // Stores lists of production tasks.

namespace fw {      // Keeps the content-production types inside the engine namespace.

struct ContentProductionTask {
    std::string id;         // Unique task id used by tooling or backlogs.
    std::string category;   // Broad bucket such as quests, encounters, dialogue, or loot.
    std::string owner;      // Optional human or pipeline owner for the task.
    std::string status;     // Current state such as todo, in_progress, or done.
};

struct ContentProductionManifest {
    std::string packId;                     // Identifies which content pack this manifest belongs to.
    std::vector<ContentProductionTask> tasks; // Lists every tracked production task for the pack.
};

} // namespace fw  // End of the framework namespace.
