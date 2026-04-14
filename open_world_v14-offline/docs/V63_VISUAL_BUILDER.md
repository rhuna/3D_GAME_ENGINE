# V63 Visual Builder

V63 is the first editor-facing upgrade that starts to feel like a level builder instead of a file generator.

## Goals
- make placement easier for non-coders
- give the engine a visual asset browser
- let the editor understand when the mouse is over UI versus the 3D world
- make scene layout faster with click-to-place authoring

## What was added
- `VisualBuilderPanel`
- click-to-place in `EditorSceneAuthoring`
- asset browser scan support in `AssetManager`
- recursive file helpers in `FileSystem`
- application-level editor UI hit testing so the camera and placement controls do not fight the panel

## Hotkeys
- `F8` toggle Visual Builder panel
- `P` toggle click placement
- `F9` quick save current authored scene
- `F10` rescan asset browser
- `F5` reload start scene

## Current workflow
1. press `F8`
2. choose `Place`
3. select a prefab or variant
4. enable click placement
5. left click on the ground plane to place entities
6. press `F9` to export a quick scene

## V63 scope
This version focuses on simple cards and lists instead of a full thumbnail renderer. The important engine seam is now in place, so V64 can move into better previews, drag placement, and scene-side prefab pickers.
