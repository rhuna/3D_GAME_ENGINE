# 3D_GAME_ENGINE v4 Gameplay Framework

This version upgrades the engine from a rendering-focused core into a gameplay-ready foundation.

## Added in v4

- richer ECS-style entity records using optional gameplay components
- tag, transform, render, rigidbody, collider, and lifetime components
- spawn helper functions for player, crates, static geometry, and projectiles
- fixed-step movement system
- simple collision system with static-vs-dynamic grounding behavior
- trigger-style projectile collision hooks
- world save/load scaffolding
- updated sandbox scene that demonstrates gameplay flow

## Controls

- `W A S D` move camera
- `Q / E` down/up
- hold `Right Mouse Button` to look around
- `Shift` move faster
- `F1` toggle debug overlay
- `F5` reload sandbox scene
- `F6` save world to `assets/saves/sandbox_world.txt`
- `F7` load world from `assets/saves/sandbox_world.txt`
- `Left Mouse Button` spawn a projectile orb
- `Esc` quit

## Build

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.uild\ReleaseD_GAME_ENGINE.exe
```

## Notes

This is still intentionally lightweight. The collision and save systems are scaffolding meant to be expanded into a fuller engine architecture later.

## Good next upgrade

- dedicated component stores instead of optional fields on one record
- broadphase collision and contact resolution
- prefab/asset manifests
- player controller + state machine
- world partitions / chunk streaming
- editor tooling
