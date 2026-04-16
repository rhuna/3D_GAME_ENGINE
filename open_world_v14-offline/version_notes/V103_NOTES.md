# V103 Notes

## What V103 fixes
This snapshot is built directly from the real uploaded repo and fixes the current
builder opening problem in the live code path you are actually compiling.

### Builder opening
- added alternate builder shortcuts in `src/core/Application.cpp`
  - `F10`
  - `F9`
  - `Ctrl+B`
- added a log message when builder visibility toggles

### Compile stability
- fixed malformed diagnostics string output in `src/content/ContentRegistry.cpp`
- fixed the `std::array` initializer mismatch in `src/editor/ui/GameBuilderPanel.cpp`

### UX update
- updated the on-screen builder hint text in `GameBuilderPanel.cpp`
