# V62 Notes

## Headline
V62 introduces the first real non-coder authoring interface.

## Added
- builder panel inside the engine
- `F8` toggle
- create forms for common content shells
- starter playable slice generator
- asset browser for common asset folders
- file writing helpers in `FileSystem`

## Key design choice
This version uses native raylib drawing instead of adding a larger external UI dependency. That keeps the upgrade lighter and easier to merge into the existing project.

## Next logical step
V63 should add:
- asset thumbnails
- click-to-place scene authoring
- scene hierarchy panel
- prefab picker drop-downs
- project-level templates and presets
