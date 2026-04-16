# V114 Export Pipeline and Builder Toggle Hardening

## What changed
- Added `ExportPipeline` to stage a real export folder from the current runtime state.
- The staged export now writes:
  - copied assets
  - runtime scene snapshot
  - launch config
  - launch batch file
  - package batch file
  - export readme
- Builder toggle now forces mouse-look off while the panel is open.
- Builder got a clickable close button.
- Output folders under `assets/` are rejected to prevent recursive staging mistakes.

## Main usage
1. Build and run the engine.
2. Open the builder with `F10`, `F9`, or `Ctrl+B`.
3. Open the **Release** tab.
4. Write the release bundle metadata.
5. Stage the export bundle.
6. Open the staged folder and run the launcher or package script.
