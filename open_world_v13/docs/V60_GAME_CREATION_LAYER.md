# V60 Game Creation Layer

V60 shifts the engine toward whole-game assembly through content files.

## Main ideas
- `assets/game/game.project` chooses the game bootstrap scene and enabled content packs.
- Content packs can layer additional prefabs, variants, and scenes on top of the base engine content.
- Scenes support `include=` so you can build a level out of reusable chunks.
- `ContentScene` provides a generic playable runtime for content-authored slices.

## New scene keys
- `display_name=`
- `gameplay_mode=content|arena|open_world`
- `player_prefab=`
- `auto_spawn_player=true|false`
- `include=scene_a,scene_b`
- `preload=path_a,path_b`
- `content_pack=pack_id`

## New project file
Example:

```ini
project=my_game

display_name=My Game
start_scene=my_first_level
content_pack=core_adventure
content_pack=town_pack
preload=assets/models/props/campfire.glb
```

## Recommended workflow
1. Duplicate `assets/content/packs/starter_template`.
2. Rename it to your new pack name.
3. Edit the scenes inside that pack.
4. Add `content_pack=your_pack_name` to `assets/game/game.project`.
5. Set `start_scene=` to your new scene.
6. Press F5 in-game to reload the start scene.

## Why this helps
You can now make a new playable slice mostly by editing content files instead of touching startup C++ code.
