# V62 UI System

V62 is the first in-engine, non-coder authoring layer for the engine.

## Goals
- stop forcing the user to hand-create every content file
- provide a visible builder panel inside the running game
- make scene, prefab, NPC, quest, dialogue, and item creation one-click
- provide a lightweight asset browser without adding an external GUI dependency yet

## What was added
- `GameBuilderPanel` under `src/editor/ui/`
- file writing helpers in `FileSystem`
- `F8` toggle for the builder panel
- create forms for:
  - scene
  - prefab
  - NPC
  - dialogue
  - quest
  - item
- starter-slice generator that creates a small playable bundle
- asset browser tab for models, textures, prefabs, and scenes

## Why this version matters
The engine is now starting to behave like a game builder instead of only a programmer-facing engine. The UI still writes the same text content files your runtime already understands, which keeps the architecture simple while making authoring much easier.

## User workflow
1. Launch the engine.
2. Press `F8` if the panel is hidden.
3. Open the Create tab.
4. Fill in a few fields.
5. Click a create button.
6. The panel writes files into `assets/` and reloads prefabs/scenes.

## Good starter flow
1. Create a prefab.
2. Use the Assets tab to pick a model path.
3. Create dialogue, item, quest, and NPC shells.
4. Click `Starter Slice`.
5. Press `F5` to reload the scene if you want to immediately test it.

## Intentional limits in V62
- no thumbnails yet
- no drag gizmos from the UI yet
- no scene graph tree yet
- no dialogue tree editor yet
- no visual quest graph yet

Those are better next steps for V63+ once this shell is stable.
