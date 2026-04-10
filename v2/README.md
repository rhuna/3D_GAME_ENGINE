# 3D_GAME_ENGINE v11 - Game Expansion Layer

This version builds on the playable v10 arena and adds a real game-expansion pass:

- health pickups
- second enemy type: runner
- start / pause / restart flow
- HUD expansion with pickups and status text
- wave progression expanded to 4 waves

## Controls

- Enter: start run / replay after win or loss
- WASD: move
- LMB: cast projectile
- P: pause
- F5: reload arena
- F11: fullscreen
- F1: debug overlay
- Tab: inspector

## Build

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.uild\ReleaseD_GAME_ENGINE.exe
```
