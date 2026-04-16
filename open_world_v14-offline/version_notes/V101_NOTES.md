# V101 Notes

## What V101 is
V101 is the clean compile and link baseline after V100.

## What V101 does
- carries forward the real-repo V100 fixes
- keeps the full repo structure intact
- updates the version line and metadata cleanly
- establishes the next milestone focus: compile and link verification on the real project tree

## Main focus
The point of V101 is not to invent a new subsystem. It is to keep the repaired repo stable while you continue the local Windows compile cycle.

## Main files carried forward
- `src/content/ContentRegistry.cpp`
- `src/editor/ui/GameBuilderPanel.cpp`
- `CMakeLists.txt`
- `version_notes/VERSION.txt`

## What to verify locally
1. Clean configure
2. Clean Release build
3. No compile errors
4. No linker errors

## Important note
This is a full professional repo snapshot built from the repaired V100 baseline.
