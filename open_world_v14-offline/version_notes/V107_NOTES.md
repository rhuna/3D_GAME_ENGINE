# V107 Notes

## What V107 fixes
This snapshot fixes the builder close path in the real uploaded repo.

### Builder close behavior
- pressing `Esc` now closes the builder first instead of quitting the whole app
- the builder now has a visible **X Close** button in its header
- alternate builder shortcuts remain available:
  - `F10`
  - `F9`
  - `Ctrl+B`

### Compile safety
- carries forward the diagnostics string repair in `ContentRegistry.cpp`
- carries forward the `std::array` initializer fix in `GameBuilderPanel.cpp`

## Main changed files
- `src/core/Application.cpp`
- `src/editor/ui/GameBuilderPanel.h`
- `src/editor/ui/GameBuilderPanel.cpp`
- `src/content/ContentRegistry.cpp`
- `version_notes/VERSION.txt`

## Local verification
1. Remove `build`
2. Reconfigure
3. Rebuild Release
4. Launch the executable
5. Confirm the builder is visible
6. Press `Esc` and confirm the builder closes without exiting the app
7. Reopen with `F10`, `F9`, or `Ctrl+B`
8. Click the **X Close** button and confirm it closes
