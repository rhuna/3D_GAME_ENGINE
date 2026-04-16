# V110 Compile and Toggle Repair

## Purpose
Use the latest uploaded repo as the source of truth, fix the current
`ContentRegistry.cpp` compile blocker, and keep the builder toggle logic simple
and deterministic.

## Expected results
- `ContentRegistry.cpp` compiles without `newline in constant`
- the app builds from a clean `build` folder
- the builder visibility debug text changes when you press the toggle keys
- `Esc` closes the builder before the app exits
