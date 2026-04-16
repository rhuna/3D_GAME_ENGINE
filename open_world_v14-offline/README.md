V122 Builder Sidebar Layout + Readability Pass

# 3D_GAME_ENGINE V114 — Export Pipeline Staging and Builder Toggle Hardening

This snapshot advances the uploaded V113 repo into **V114**.

## Focus
- real **export bundle staging** from the builder Release tab
- runtime scene snapshot copied into the staged package
- Windows launcher and packaging batch scripts generated automatically
- builder toggle hardening so the panel no longer fights mouse-look
- close button support for the builder panel
- version metadata refreshed to V114

## Builder controls
Use **F10**, **F9**, or **Ctrl+B** to toggle the builder.
Press **Esc** to close the builder before exiting the application.
Click the new **X** button to close the builder with the mouse.
While the builder is open, **RMB mouse-look is disabled** so the UI stays usable.

## Release/export workflow in V114
Open the builder and go to **Release**:
1. Fill in build id, title, version, startup ids, and output folder
2. Click **Write Release Bundle** to save the release metadata files
3. Click **Stage Export Bundle** to create a runnable export folder

The staged export now includes:
- copied project assets
- a runtime scene export snapshot
- a launch config file
- a Windows launcher batch file
- a packaging batch file for zipping the staged export

Recommended output folder example:
```text
exports/starter_build
```

Do **not** choose a folder inside `assets/` for the staged export.

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


## V115

- Builder tab body rendering repaired
- Builder now captures editor/world input while open
- Windows export pipeline compile fix for `ExportPipeline.cpp`


## V121 large-workspace polish

- Larger builder workspace with more horizontal room for pane-based tabs
- Wider field rows and a bigger second-column offset for clearer separation
- Larger default startup window for easier builder use on desktop displays
