# Logging Patch README

This patch adds structured content-debug logging to the runtime.

## Updated files
- `src/core/Logger.h`
- `src/core/Logger.cpp`
- `src/game/world/RegionManager.cpp`
- `src/scene/spawning/SceneSpawner.cpp`
- `src/game/world/RegionEcsLoader.cpp`

## What it logs
- region registration and switching
- scene loading and scene entry counts
- prefab resolution and entity spawning
- gather/NPC/travel/save marker attachment
- scene-key lookup from region content

## What to look for in the console

### Region load
`[INFO][REGION] Rebuilding region ECS for region: western_trail`

### Scene load
`[INFO][SCENE] Loading scene: western_trail`

### Marker scan
`[DEBUG][SCENE] Entity tag=gather_marker entity=...`

### Spawn confirmation
`[INFO][SPAWN] Spawned gather node id=...`

## Best first use
Add one piece of content, run the game, and verify the console shows:
1. region load
2. scene load
3. scene entries found
4. spawn performed
5. persistent id attached
