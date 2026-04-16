# V102 Notes

## What V102 fixes
This snapshot fixes the current real-repo builder access and compile blockers together.

### Builder access
- added builder toggle support for:
  - `F10`
  - `F9`
  - `Ctrl+B`
- added a log message when the builder visibility toggles so you can verify the shortcut fired

### Compile repairs
- fixed malformed multiline string output in `src/content/ContentRegistry.cpp`
- fixed the `std::array` initializer count mismatch in `src/editor/ui/GameBuilderPanel.cpp`

## Main changed files
- `src/core/Application.cpp`
- `src/content/ContentRegistry.cpp`
- `src/editor/ui/GameBuilderPanel.cpp`
- `README.md`
- `version_notes/VERSION.txt`

## Local verification
1. Remove the `build` folder
2. Reconfigure
3. Rebuild Release
4. Launch the executable
5. Press `F10`, `F9`, and `Ctrl+B` to confirm the builder opens
