# V65 Build Asset Workshop

V65 shifts the editor from simple scene authoring toward **in-engine asset creation using raylib primitives**.

## Main goals
- create primitive-based assets in the engine
- place them with ground stamping
- keep placement clean with snapping
- duplicate and mirror quickly
- save the selected build piece as a reusable prefab

## Controls
- `F9` toggle workshop panel
- `B` toggle stamp mode
- `1` cube
- `2` sphere
- `3` cylinder
- `4` plane
- `,` decrease snap size
- `.` increase snap size
- `Ctrl+D` duplicate selected piece
- `M` mirror selected piece across X
- `G` snap selected piece to grid
- `Ctrl+P` save selected piece as prefab

## Workflow
1. Press `F9` to open the V65 workshop.
2. Pick a primitive shape.
3. Turn on stamp mode.
4. Left click the ground to place pieces.
5. Select a piece and duplicate or mirror it.
6. Enter a prefab id in the workshop.
7. Press `Ctrl+P` or click **Save Selected**.
8. The prefab is written to `assets/prefabs/<id>.prefab`.

## New prefab fields
You can now author primitive prefabs directly in text files too:

```ini
primitiveShape=cube
primitiveSize=1,1,1
primitiveRadius=0.5
primitiveHeight=1.0
```

Supported primitive shapes:
- `cube`
- `sphere`
- `cylinder`
- `plane`

## Why V65 matters
This is the first version that treats the engine as an **asset blockout tool**, not just a runtime/editor.
It is aimed at making early game creation much easier for a non-coder by letting them build simple props and markers directly in the engine.
