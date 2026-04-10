# 3D_GAME_ENGINE v12

v12 pushes the project from a functional prototype into a more complete game slice.

## Highlights
- combat polish pass
- third enemy archetype: ranged arcanist
- health and damage-boost pickups
- title/start screen and end screen overlays
- score tracking
- boost timer HUD
- team-aware projectile damage
- richer arena scene: `arena_game_02.scene`

## Controls
- `Enter` start or replay
- `WASD` move
- `LMB` cast
- `P` pause
- `F5` reload arena
- `F11` fullscreen
- `F1` debug overlay
- `Tab` inspector

## Build
```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.\build\Release\3D_GAME_ENGINE.exe
```

## Notes
Audio hooks are stubbed through `AudioBus` so the gameplay code is ready for real sound effects later without forcing an audio asset pipeline first.
