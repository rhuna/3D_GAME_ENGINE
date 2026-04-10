3D_GAME_ENGINE v10 UI / window usability patch

Includes:
- src/core/Application.cpp
- src/render/DebugOverlay.cpp
- src/editor/runtime/InspectorPanel.cpp

What changed:
- Window is now resizable/maximizable.
- F11 toggles fullscreen.
- Debug overlay is split into separate readable panels.
- Inspector is docked on the right side instead of overlapping the overlay.
- Overlay and inspector scale with the current window size.
