# V70 - Starter World Wizard

This upgrade is focused on making game creation easier by letting the editor generate a playable starter world from saved scene kits.

## Main changes
- Added V70 Game Builder integration to the runtime application
- Added a new **Template** tab to `GameBuilderPanel`
- Added starter world generation that writes:
  - `assets/scenes/generated/<scene>.scene`
  - `assets/template/<template>.json`
  - `assets/game/<template>.project` or `assets/game.project`
- Added generated-kit scanning from `assets/scenes/generated`
- Added runtime support for `spawn=kit=<scene_name>` in scene files
- Added `F10` toggle for the V70 Game Builder panel

## Why this matters
The engine can now move closer to a real game-maker workflow:
1. Build kits in-editor
2. Save kits as generated scenes
3. Open the Template tab
4. Pick village/forest kits
5. Generate a starter world scene and project scaffold

## Key files changed
- `src/core/Application.h`
- `src/core/Application.cpp`
- `src/editor/ui/GameBuilderPanel.h`
- `src/editor/ui/GameBuilderPanel.cpp`
- `src/scene/spawning/SceneSpawner.h`
- `src/scene/spawning/SceneSpawner.cpp`
- `src/scene/scenes/ContentScene.cpp`
- `src/scene/scenes/SandboxScene.cpp`
- `src/game/scenes/ArenaGameScene.cpp`
- `src/game/world/RegionEcsLoader.cpp`

## Usage
- Press `F10`
- Open **Template**
- Refresh kits
- Pick generated scene kits for village/forest
- Click **Create World** or **Create + Set Start**

## Notes
- I did not do a full native build inside the container because the project fetches external dependencies during CMake.
- The runtime kit support is included so generated template scenes can actually resolve `spawn=kit=...` entries.
