# V73 — World Authoring Suite

V73 extends the low-code direction of V72 by making **world setup** itself part of the builder workflow.

## Main goal
Reduce the amount of C++ required to start a playable game loop by adding form-driven authoring for:
- region shells
- world scenes
- travel links
- starter world bundles

## What changed
### GameBuilderPanel
- added a new **World** tab
- widened the builder panel slightly to support the extra workflow
- added world-facing form fields for:
  - start region id
  - scene id
  - biome
  - travel link id
  - target region id
  - spawn tag
  - world bundle id

### New low-code world actions
The builder can now create:
- `assets/regions/<region>.region`
- `assets/scenes/<scene>.scene`
- `assets/world/<travel>.travel`
- `assets/world/<bundle>.world`

### Asset browser
- added a **World** category for travel/world manifests
- keeps models, prefabs, scenes, data, and quests browsing intact

## Why V73 matters
V72 made gameplay data easier to author.
V73 makes the **world structure** easier to author too.

This pushes the engine closer to:
- coded core
- data-driven game
- form-driven content workflow

## Recommended use flow
1. Open **World** tab.
2. Create a region shell.
3. Create a travel link.
4. Open **Author** tab and make gameplay content.
5. Create a world bundle.
6. Open the visual builder/runtime scene and place actual content.

## Outcome
V73 keeps the engine coder-capable, but removes more coding from the process of making an actual game.
