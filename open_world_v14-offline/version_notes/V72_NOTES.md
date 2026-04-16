# V72 — Game Data Authoring Suite

V72 is the first true low-code content-authoring step for the engine.

## Goal
Keep the engine coded, but move most game-content creation out of C++ and into authorable files and in-editor forms.

## Main upgrade
`GameBuilderPanel` now acts like a data authoring suite instead of a small starter form.

### New tabs
- **Create** — scene, prefab, starter NPC shell
- **Author** — dialogue, quests, items, merchants, loot tables, encounters, routines
- **Assets** — browse offline content on disk
- **Guide** — low-code workflow guidance

## What V72 writes
The panel now creates data directly into the engine's real content folders:
- `assets/dialogue/*.dialogue`
- `assets/encounters/*.encounter`
- `assets/items/*.item`
- `assets/loot/*.loot`
- `assets/npcs/*.npc`
- `assets/npcs/*.merchant`
- `assets/quests/*.quest`
- `assets/routines/*.routine`
- `assets/scenes/*.scene`
- `assets/prefabs/*.prefab`

## New authoring actions
- Create NPC
- Create Dialogue
- Create Quest
- Create Item
- Create Merchant
- Create Loot Table
- Create Encounter
- Create Routine
- Create Authoring Bundle
- Create Starter Slice

## Why this matters
This version is aimed at the rule:

> Code builds the engine.  
> Data builds the game.  
> The editor builds the content.

That means most content work should no longer require editing C++ source.

## Practical workflow
1. Open the builder panel.
2. Create a scene and prefab shell.
3. Create an authoring bundle.
4. Place prefabs and game hooks in the world.
5. Reload and test the runtime slice.

## Best first game with V72
- starter village
- forest edge
- quest giver NPC
- merchant NPC
- one loot table
- one encounter
- one short quest loop

## Recommended next version
**V73 — Region / Biome / Travel Authoring**
- region painter
- travel graph editor
- encounter stamping
- spawn and quest marker placement without code
