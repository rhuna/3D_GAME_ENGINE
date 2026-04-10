# 3D_GAME_ENGINE v10 - First Real Game Layer

This version turns the editor-first engine scaffold into a playable prototype layer.

## What changed

- Added a real `src/game` layer
- Added `ArenaGameScene` as the startup scene
- Added game-specific components:
  - `PlayerComponent`
  - `HealthComponent`
  - `EnemyComponent`
  - `ProjectileComponent`
  - `TeamComponent`
  - `SpawnerComponent`
- Added game systems:
  - `PlayerControllerSystem`
  - `EnemyAISystem`
  - `DamageSystem`
- Added `WaveDirector` and `ArenaGameState`
- Added `GameHud`
- Added `arena_game.scene`
- Added `enemy_walker.prefab` and arena support prefabs

## Play loop

- Move with `WASD`
- Hold `RMB` to look the camera around
- Cast with `LMB`
- Clear all waves to win
- Press `F5` to restart the arena
- Press `F1` for the debug overlay

## Build

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.\build\Release\3D_GAME_ENGINE.exe
```

## Notes

This is the first playable game layer, not a finished commercial game.
The editor/data-driven pipeline is still present underneath, but the startup flow now behaves like a real game prototype instead of only an engine sandbox.
