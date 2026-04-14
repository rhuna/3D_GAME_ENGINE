# V64 Notes

## Added
- VisualBuilderPanel editor UI.
- F8 toggle for builder UI.
- Click-to-place prefab/variant authoring.
- Primitive asset workshop for cube/sphere/cylinder/plane assets.
- FileSystem helpers for writing files and making directories.
- `primitiveShape` support in RenderComponent and prefab parsing.

## Changed
- Camera mouse-look now yields when the builder panel is under the mouse.
- Editor scene authoring save hotkey moved from F8 to F9.
- Renderer now supports more native raylib-drawn primitive types.

## Purpose
V64 starts shifting the engine from config-heavy authoring toward in-engine creation so future versions can focus on making asset creation and full game creation easier for non-coders.
