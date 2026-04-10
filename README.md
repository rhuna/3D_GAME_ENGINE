# 3D_GAME_ENGINE v8 Editor-Ready Content Pipeline

This version adds an editor-focused data pipeline on top of the v7 scene/prefab architecture.

## New in v8
- prefab inheritance chains using `baseVariant`
- scene export to `assets/editor/exported_scenes/runtime_export.scene`
- content validator for prefabs, variants, and scenes
- runtime inspector panel for entity selection and transform editing
- editor metadata stored per entity for export-friendly content authoring

## Hotkeys
- `F1` Toggle debug overlay
- `F2` Cycle entity selection
- `F3` Export current world as a scene file
- `F4` Run content validation
- `F5` Reload sandbox scene
- `F6` Save world runtime snapshot
- `F7` Load world runtime snapshot
- `Tab` Toggle inspector
- `Arrow Keys/PageUp/PageDown` Move selected entity
- `+` / `-` Scale selected entity
- `Delete` Remove selected entity

## Build
```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.\build\Release\3D_GAME_ENGINE.exe
```
# 3D_GAME_ENGINE v7 - Data-Driven Scenes + Prefab Variants

This version moves scene layout out of C++ and into asset files, and it upgrades prefabs with reusable variant definitions.

## What's new in v7

- data-driven scenes loaded from `assets/scenes`
- prefab variants loaded from `assets/prefab_variants`
- `SceneLibrary` and `SceneSpawner`
- `PrefabVariantDefinition` and prefab resolution through `PrefabLibrary`
- sandbox content now comes from scene data instead of hardcoded spawn loops
- cleaner separation for:
  - `src/scene/data`
  - `src/scene/spawning`
  - `src/gameplay/prefabs`

## Data files included

- `assets/scenes/sandbox.scene`
- `assets/prefabs/ground.prefab`
- `assets/prefabs/player.prefab`
- `assets/prefabs/dynamic_crate.prefab`
- `assets/prefabs/projectile.prefab`
- `assets/prefab_variants/crate_blue.variant`
- `assets/prefab_variants/crate_red.variant`
- `assets/prefab_variants/crate_tall.variant`

## Controls

- `Right Mouse Button` + mouse: look around
- `WASD` move camera horizontally
- `Q / E` move camera vertically
- `Left Shift` move faster
- `F1` toggle debug overlay
- `F5` reload sandbox scene
- `F6` save world
- `F7` load world
- `Left Mouse Button` spawn projectile
- `Escape` quit

## Build

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.\build\Release\3D_GAME_ENGINE.exe
```

## Note

`raylib` is fetched by CMake. If configure/build fails before compilation, make sure Git/network access is available for FetchContent.


## v9 editor scene authoring + gizmo tools

- mouse picking with left click
- gizmo modes: translate, rotate, scale
- direct scene save to `assets/scenes/sandbox_edited.scene`
- editor spawn flow for prefab variants
- runtime scene authoring controls for selected entities
