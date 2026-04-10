# 3D_GAME_ENGINE v16 - World Simulation + Combat Encounters

# 3D_GAME_ENGINE v15 - Exploration Gameplay + Inventory + Quests

# 3D_GAME_ENGINE v14 - Open World Foundation

This version pivots the project from an arena-first prototype into a **persistent multi-region single-player exploration boilerplate**.

## Core goals

- reusable single-player exploration foundation
- persistent save profile
- connected world regions
- inventory and quest scaffolding
- NPC interaction and dialogue hooks
- travel triggers between regions
- structure that can be reused across multiple future games

## Included open-world foundation systems

- `OpenWorldFoundationScene`
- `RegionManager`
- `SaveGameProfile`
- `InteractionSystem`
- `InventorySystem`
- `OpenWorldHud`
- `DialogueState`
- quest state scaffolding
- interactable / NPC / travel trigger components

## Regions included

- `village_region.scene`
- `forest_region.scene`
- `ruins_region.scene`

## Controls

- `WASD` move
- `LMB` cast projectile
- `E` interact / advance dialogue / travel
- `I` inventory panel
- `J` quest log
- `F9` save open-world profile
- `F5` reload start scene
- `F11` fullscreen

## Save files

- `assets/saves/open_world_profile.txt`
- `assets/saves/open_world_snapshot.txt`

## Notes

This is intentionally a **boilerplate foundation**, not a finished Skyrim-like game. It is designed to give you a reusable base for building multiple single-player exploration games with shared technology and content workflows.


## v15 additions
- dialogue window scaffolding
- quest log window scaffolding
- inventory window
- equipment system hooks
- loot container system hooks
- interaction prompt system
- quest giver and chest prefab examples
- forest quest test scene


## v16 additions
- encounter manager scaffolding
- roaming encounter system hooks
- respawn profile scaffolding
- faction system hooks
- patrol system
- road region sample scene
- bandit, wolf, and campfire checkpoint prefabs
