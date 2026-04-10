# 3D_GAME_ENGINE v13

v13 upgrades the working v12 game slice into a more campaign-like prototype.

## Main additions
- arena rotation on the start screen
- difficulty selection on the start screen
- campaign profile scaffolding
- best score and arenas cleared tracking
- combo / multiplier HUD hooks
- third arena scene: `arena_game_03`
- tank enemy prefab scaffold: `enemy_tank.prefab`

## Controls
- Enter: start / replay
- [ / ]: change arena from the start screen
- - / =: change difficulty from the start screen
- WASD: move
- LMB: cast
- P: pause
- F11: fullscreen

## Build
```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.\build\Release\3D_GAME_ENGINE.exe
```

## Notes
This version is intended as the next full repo step after v12. It focuses on progression and replay structure instead of raw engine plumbing.
