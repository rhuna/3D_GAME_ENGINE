# V65 Notes

## Focus
V65 begins the shift from scene authoring toward **asset creation inside the engine**.

## Added
- `BuildAssetWorkshop` runtime/editor tool
- `F9` workshop panel
- primitive stamping on ground plane
- snap size controls
- duplicate selected piece
- mirror selected piece
- save selected build piece as prefab
- new primitive prefab fields:
  - `primitiveShape`
  - `primitiveSize`
  - `primitiveRadius`
  - `primitiveHeight`

## Changed
- `Application` now integrates the workshop and blocks mouse-look when over the workshop UI
- `EditorSceneAuthoring` respects workshop UI hit-testing
- `Renderer` now supports cube, sphere, cylinder, and plane primitives
- `PrefabLibrary` parses the new primitive fields
- `FileSystem` can now write text files and create directories

## Next logical direction
V66 should focus on:
- multi-piece grouped asset saving
- axis gizmo handles for build pieces
- better prefab palette browsing
- simple material presets
- convert selected group into reusable asset pack
