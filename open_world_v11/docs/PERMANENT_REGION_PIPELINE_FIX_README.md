# Permanent Region Pipeline Fix

## Goal
Eliminate the split between:
- legacy regions in `assets/regions/*.region`
- content-pack regions in `assets/content/packs/.../regions/*.json`
- hardcoded defaults in `RegionManager` and `RegionLayoutDatabase`

After this patch, the intended source of truth is:

```text
assets/content/packs/.../regions/*.json
    ↓
RegionDatabase
    ↓
RegionManager
    ↓
OpenWorldRuntimeScene / RegionEcsLoader
```

## Files included
- `src/game/content/models/RegionDefinition.h`
- `src/game/content/RegionDatabase.h`
- `src/game/content/RegionDatabase.cpp`
- `src/game/content/ContentPipeline.cpp`
- `src/game/world/RegionManager.h`
- `src/game/world/RegionManager.cpp`
- notes for `OpenWorldRuntimeScene.cpp`
- notes for `RegionEcsLoader.cpp`

## Why notes instead of full replacements for two files
`OpenWorldRuntimeScene.cpp` and `RegionEcsLoader.cpp` contain a lot of gameplay logic that has drifted across versions.
The included notes show the exact permanent-fix edits to make without blindly overwriting unrelated gameplay changes.

## What this patch changes now
1. `RegionDatabase` now recursively scans the assets root.
2. It loads both legacy `.region` files and JSON content-pack region files under `.../regions/` folders.
3. `ContentPipeline` now lets `RegionDatabase` scan the full assets tree.
4. `RegionManager` gains `InitializeFromDatabase()` so regions come from content instead of hardcoded registration.
5. `RegisterDefaults()` becomes fallback-only.

## What you still need to apply manually
### In `OpenWorldRuntimeScene.cpp`
- use RegionDatabase to choose the initial region
- remove hardcoded safe-zone logic
- stop relying on hotkeys for only `village/forest/ruins`

### In `RegionEcsLoader.cpp`
- stop returning early when no fallback `RegionLayout` exists
- allow authored scene content to drive a region even without a hardcoded layout
- use `RegionDefinition.links` for travel instead of hardcoded destinations

## Recommended content format
Example `assets/content/packs/starter_vale/regions/western_trail.json`:

```json
{
  "id": "western_trail",
  "displayName": "Western Trail",
  "scene": "assets/scenes/western_trail.scene",
  "description": "A rough road west of Starter Vale.",
  "safeZone": false,
  "links": [
    { "targetRegion": "starter_vale_core", "anchorId": "to_vale" },
    { "targetRegion": "river_crossing", "anchorId": "to_river" }
  ]
}
```

## After applying
Rebuild and verify logs show:
- region loaded from JSON
- region registered in RegionManager
- scene loaded from region definition
- scene content spawned even without a fallback layout
