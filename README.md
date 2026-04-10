# FireWizard3DEngine v2 Core

A more complete **core-engine starter repo** in **C++20 + raylib + CMake**.

This version is focused on the **engine foundation**, not content. It gives you a cleaner structure you can keep upgrading into a real 3D project.

## What is in v2

- Application bootstrap with config loading
- Fixed timestep main loop
- Input manager with pressed/down/released queries
- Time system
- Logger
- Simple file/path utilities
- Asset manager with cached texture/model/shader loading
- Minimal ECS-style world
- Scene interface + scene manager
- Sandbox scene for testing
- Renderer abstraction
- Debug overlay
- Expandable assets folder

## Controls

- `W A S D` move camera
- `Q / E` move down/up
- Hold `Right Mouse` to freelook
- `Shift` move faster
- `F1` toggle debug overlay
- `F5` reload sandbox scene
- `Esc` exit

## Build

### Visual Studio 2022

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.\build\Release\FireWizard3DEngine.exe
```

### MinGW Makefiles

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
.\build\FireWizard3DEngine.exe
```

## Folder layout

```text
firewizard3d_engine_v2_core/
  assets/
    engine.cfg
    models/
    textures/
    shaders/
  src/
    assets/
    core/
    ecs/
    render/
    scene/
    util/
```

## Next upgrades after v2 core

1. Real component stores and system scheduling
2. Model import pipeline + manifests
3. Material system
4. Collision and physics hooks
5. Scene serialization
6. Animation layer
7. Audio system
8. Editor mode and gizmos
9. Save/load snapshots
10. Headless simulation mode

## Notes

- `raylib` is downloaded automatically by CMake using `FetchContent`.
- If you add your own models/textures, put them in `assets/` and load through `AssetManager`.
