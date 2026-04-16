# V110 Notes

## What V110 fixes
This snapshot repairs the current real compile blocker and cleans up the builder
toggle path in the live repo you uploaded.

### Compile blocker
- `src/content/ContentRegistry.cpp`
  - replaced the broken diagnostics output block with a clean escaped-newline
    implementation
  - avoids the repeated MSVC `newline in constant` parser errors

### Builder toggle
- `src/core/Application.cpp`
  - uses one authoritative `SetVisible(...)` path for the builder
  - logs the explicit target visibility state
  - keeps `Esc` closing the builder first

### Builder panel
- `src/editor/ui/GameBuilderPanel.cpp`
  - keeps the array-size repair
  - updates the on-screen hint text

## Main changed files
- `src/content/ContentRegistry.cpp`
- `src/core/Application.cpp`
- `src/editor/ui/GameBuilderPanel.cpp`
- `version_notes/VERSION.txt`

## Local verification
1. Remove `build`
2. Reconfigure
3. Rebuild Release
4. Launch the executable
5. Watch the top-left builder visibility text
6. Press `F10`, `F9`, and `Ctrl+B`
7. Press `Esc` to close the builder
