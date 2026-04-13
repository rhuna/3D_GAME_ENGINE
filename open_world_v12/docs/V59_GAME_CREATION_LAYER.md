# V59 Game Creation Layer

V59 begins turning the engine into a project authoring system instead of a code-first runtime.

## Main additions
- `assets/game/game.project` is now the high-level bootstrap file.
- scenes can declare:
  - `display_name=`
  - `gameplay_mode=`
  - `player_prefab=`
  - `auto_spawn_player=`
  - `preload=`
- `Application` now honors the project start scene instead of always forcing open-world runtime.
- `ContentScene` provides a generic scene runtime for content-driven game slices.

## Minimal workflow to make a new playable slice
1. Add or register assets in `assets/registry/assets.registry`
2. Create prefabs in `assets/prefabs/`
3. Create a scene in `assets/scenes/`
4. Point `assets/game/game.project` to that scene with `start_scene=`
5. Run the engine

## Scene example
```ini
scene=my_first_level
display_name=My First Level
gameplay_mode=standard
player_prefab=player
auto_spawn_player=true
preload=house_01,house_01_low,wood_wall_a
spawn=prefab=ground;position=0,-0.5,0;scale=40,1,40
spawn=prefab=starter_merchant;position=6,1,4
spawn=prefab=enemy_walker;position=-8,1,-6
```

## Gameplay modes
- `standard` -> uses the generic `ContentScene`
- `arena` -> uses `ArenaGameScene`
- `open_world` -> uses `OpenWorldRuntimeScene`

## Why this matters
You can now switch the project to a different playable slice mostly through content files rather than rewriting engine startup code.
