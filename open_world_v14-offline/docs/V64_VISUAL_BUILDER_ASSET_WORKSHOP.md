# V64 Visual Builder + Primitive Asset Workshop

V64 is the first upgrade that starts moving the engine toward non-coder asset creation using native raylib drawing.

## Main goals
- Add a simple in-engine builder panel.
- Let the user stamp prefabs and variants into the scene with click placement.
- Let the user create new primitive-based assets without leaving the engine.
- Keep everything compatible with the existing prefab and scene text pipeline.

## New workflow
- Press `F8` to open the V64 builder.
- Use the **Place** tab to switch between prefab and variant placement.
- Turn on placement and left-click the ground to spawn content.
- Use the **Create** tab to build a primitive prop using cube, sphere, cylinder, or plane.
- Save it as a `.prefab` or `.variant` file.
- Reload/start placing it immediately.

## Primitive asset creation
The new primitive workshop writes standard asset files into:
- `assets/prefabs/`
- `assets/prefab_variants/`

This makes it possible to block out buildings, props, collision volumes, markers, and simple environment pieces entirely inside the engine using raylib drawing primitives.

## Rendering additions
`RenderComponent` now supports a `primitiveShape` field with:
- `cube`
- `sphere`
- `cylinder`
- `plane`

These shapes are rendered directly by `Renderer` and can be authored in prefab files.

## Why this matters
This is the first concrete step toward future versions where a non-coder can:
- create gameplay props
- lay out a level visually
- save the content
- keep building the game without touching code
