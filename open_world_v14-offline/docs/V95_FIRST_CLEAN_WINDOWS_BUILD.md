# V95 First Clean Windows Build

## Goal
Use V95 as the baseline for the first fully clean MSVC Release build milestone.

## Local verification sequence
1. Configure from a clean build directory
2. Build Release with MSVC
3. Confirm there are no compile errors
4. Confirm there are no linker errors
5. Launch the executable
6. Toggle the builder
7. Export diagnostics
8. Confirm all V95 report files exist

## Exit criteria
- clean Release build
- successful executable launch
- builder toggle confirmed
- diagnostics exported
