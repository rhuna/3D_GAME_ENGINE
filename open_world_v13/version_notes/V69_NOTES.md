# V69 - Kit Palette + One-Click Kit Placement

## Goal
Make game creation much easier by turning saved build groups into reusable, browseable kits that can be stamped back into the world.

## What changed
- Added `KitLibrary` to scan `assets/scenes/generated` for reusable kits.
- Added `.kit` metadata sidecar support next to generated `.scene` kit files.
- Added `KitPlacement` to place a full kit into the world from a root position.
- Upgraded `BuildAssetWorkshop` so saving a grouped build now writes:
  - `assets/scenes/generated/<kit>.scene`
  - `assets/scenes/generated/<kit>.kit`
- Upgraded `VisualBuilderPanel` with a new **Kits** tab.
- Added direct placement actions:
  - place at current selection
  - place at origin
  - left-click kit stamping on the ground
- Wired `VisualBuilderPanel` into `Application` and assigned **F10** to toggle it.

## Main workflow
1. Use the Build Asset Workshop to group pieces.
2. Save the group as a kit.
3. Press `F10` to open the Visual Builder.
4. Open the **Kits** tab.
5. Pick a saved kit.
6. Place it at selection, at origin, or stamp it repeatedly with left click.

## New files
- `src/editor/runtime/KitLibrary.h`
- `src/editor/runtime/KitLibrary.cpp`
- `src/editor/runtime/KitPlacement.h`
- `src/editor/runtime/KitPlacement.cpp`

## Modified files
- `src/editor/runtime/BuildAssetWorkshop.cpp`
- `src/editor/ui/VisualBuilderPanel.h`
- `src/editor/ui/VisualBuilderPanel.cpp`
- `src/core/Application.h`
- `src/core/Application.cpp`

## Notes
- Category and favorite filtering are supported from `.kit` metadata.
- Kits are discovered from `assets/scenes/generated`.
- If a `.kit` file does not exist, the system can still infer a generated kit from a `.scene` file.
