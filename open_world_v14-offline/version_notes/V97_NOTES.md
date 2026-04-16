# V97 Notes

## What V97 is
V97 is the first full snapshot built from the uploaded real repo tree instead of a simplified milestone scaffold.

## What V97 fixes
- restores a real integrated `GameBuilderPanel`
- aligns `Application.h` and `Application.cpp` with the builder/runtime path
- aligns `ContentRegistry` with the actual asset folders present in the repo
- keeps the root `CMakeLists.txt` as the real build entry point
- defaults the build to the vendored offline dependency path when available

## Main changed files
- `CMakeLists.txt`
- `src/core/Application.h`
- `src/core/Application.cpp`
- `src/editor/ui/GameBuilderPanel.h`
- `src/editor/ui/GameBuilderPanel.cpp`
- `src/content/ContentRegistry.h`
- `src/content/ContentRegistry.cpp`
- `README.md`
- `version_notes/VERSION.txt`

## New outputs
- `assets/registry/v97_registry_diagnostics.txt`

## Important note
This is a full repo snapshot built from the uploaded project structure.
