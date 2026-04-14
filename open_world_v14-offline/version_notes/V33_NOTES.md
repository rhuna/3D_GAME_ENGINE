# V33 - Quest/runtime state alignment scaffold

This snapshot adds a unified runtime quest type header for the next quest-system cleanup.

Added:
- `src/game/quests/QuestRuntimeTypes.h`

Target migration:
- align quest gameplay systems, quest log UI, and save/load around one quest status model
- remove duplicate or mismatched quest structs
- make dialogue and rewards update the same runtime quest state

This is intentionally a scaffold snapshot so it can sit on top of the current patched repo without forcing a large compile-risky refactor in one step.
