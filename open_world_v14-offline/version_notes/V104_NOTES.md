# V104 Notes

## What V104 fixes
This snapshot is built directly from the real uploaded repo and fixes the current
builder opening problem in the live code path you are actually compiling.

### Builder access
- added alternate builder shortcuts in `src/core/Application.cpp`
  - `F10`
  - `F9`
  - `Ctrl+B`
- added a log message when builder visibility toggles

### Compile repairs
- fixed malformed diagnostics string output in `src/content/ContentRegistry.cpp`
- fixed the `std::array` initializer mismatch in `src/editor/ui/GameBuilderPanel.cpp`

### UX update
- updated the on-screen builder hint text in `GameBuilderPanel.cpp`

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
5. Try `F10`, `F9`, and `Ctrl+B`
6. Check logs for builder toggle messages
