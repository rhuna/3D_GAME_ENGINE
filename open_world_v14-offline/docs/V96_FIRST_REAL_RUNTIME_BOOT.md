# V96 First Real Runtime Boot

## Goal
Use V96 as the baseline for the first real executable launch milestone.

## Local verification sequence
1. Configure from a clean build directory
2. Build Release with MSVC
3. Launch the executable
4. Confirm the application window opens
5. Toggle the builder
6. Export diagnostics
7. Confirm the runtime boot report exists
8. Confirm the builder runtime flag can be marked verified

## Exit criteria
- clean Release build
- successful executable launch
- builder toggle confirmed
- diagnostics exported
- runtime boot report written
