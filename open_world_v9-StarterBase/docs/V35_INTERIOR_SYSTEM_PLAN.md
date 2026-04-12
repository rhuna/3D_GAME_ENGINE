# v35 Interior / Enterable Building Upgrade

This version introduces the first scaffold for enterable buildings.

## Goal
- Let the player move from an exterior region into an interior scene and back out again.

## Added scaffolding
- `BuildingInteriorRuntime.h`
- `BuildingInteriorSystem.h/.cpp`
- `building_enter_trigger.prefab`
- `building_exit_trigger.prefab`
- `herbalist_hut_interior.scene`

## Intended runtime flow
1. Player overlaps an exterior enter trigger.
2. Trigger resolves a destination scene and anchor.
3. Runtime stores the current exterior scene / anchor in `BuildingInteriorState`.
4. Scene manager loads the target interior scene.
5. Player is spawned or repositioned at the requested interior anchor.
6. Exit trigger sends the player back to the saved exterior anchor.

## Next code tasks
- Hook `BuildingInteriorSystem` into the open-world update loop.
- Add scene anchor lookup.
- Persist open/closed and visited interior state.
- Add building-specific lighting, music, and NPC schedules.
