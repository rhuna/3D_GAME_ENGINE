# 3D_GAME_ENGINE V113 — Offline Build Bootstrap and Builder Session Persistence

This snapshot advances the uploaded V112 repo into **V113**.

## Focus
- Offline-friendly raylib bootstrap logic in CMake
- Clear failure messaging when raylib is missing locally
- Builder panel session persistence across launches
- Version metadata refreshed to V113

## Builder controls
Use **F10**, **F9**, or **Ctrl+B** to toggle the builder.
Press **Esc** to close the builder before exiting the application.

## Offline build behavior
V113 now searches for raylib in this order:
1. installed raylib package
2. `third_party/raylib`
3. FetchContent when `-DFW_OFFLINE_ONLY=OFF`

## Recommended local Windows build
```powershell
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

If raylib is not installed and you do not have `third_party/raylib`, configure with:
```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -DFW_OFFLINE_ONLY=OFF
```
