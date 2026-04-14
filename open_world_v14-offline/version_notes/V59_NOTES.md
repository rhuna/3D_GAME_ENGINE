V59 - Game Creation Layer

Goals:
- make the whole game easier to author through content files
- allow the project to boot into different playable slices without code edits
- lay the foundation for future editor-side project generation and region/scene packs

What changed:
- added `assets/game/game.project`
- added `GameProjectDefinition` and `GameProjectLoader`
- scenes now support metadata fields:
  - display_name
  - gameplay_mode
  - player_prefab
  - auto_spawn_player
  - preload
- added `ContentScene` for generic content-driven play slices
- `Application` now builds the starting runtime from scene metadata and project config
- validation now checks scene preload entries and scene player prefab references

Recommended next version:
- V60 should add scene includes, region packs, project templates, and one-click content scaffolding for prefabs/items/quests/npcs.
