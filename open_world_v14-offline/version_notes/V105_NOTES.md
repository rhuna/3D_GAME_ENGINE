# V105 Notes

## What V105 fixes
This snapshot fixes the remaining live compile blocker in `ContentRegistry.cpp`
and makes the builder open automatically on startup so you can verify it visually.

### Builder behavior
- builder now auto-opens on startup
- builder can still be toggled with:
  - `F10`
  - `F9`
  - `Ctrl+B`
- a log message is written when the builder auto-opens or toggles

### Compile repairs
- fixed malformed diagnostics string output in `src/content/ContentRegistry.cpp`
- fixed the `std::array` initializer mismatch in `src/editor/ui/GameBuilderPanel.cpp`

## Main changed files
- `src/core/Application.cpp`
- `src/content/ContentRegistry.cpp`
- `src/editor/ui/GameBuilderPanel.cpp`
- `version_notes/VERSION.txt`

## Local verification
1. Remove the `build` folder
2. Reconfigure
3. Rebuild Release
4. Launch the executable
5. Confirm the builder is visible immediately on startup
6. Try `F10`, `F9`, and `Ctrl+B`
