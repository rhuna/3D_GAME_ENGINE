#pragma once
#include <string>   // Used for checklist descriptions.
#include <vector>   // Used for ordered checklist entries.

namespace fw {

struct WorkflowChecklistEntry {
    std::string id;           // Stable checklist entry id.
    std::string description;  // What the creator should verify.
};

struct ContentWorkflowChecklist {
    std::string workflowId;                        // Names the workflow being checked.
    std::vector<WorkflowChecklistEntry> entries;   // Ordered checklist entries for the workflow.
};

} // namespace fw
