# V91 Runtime Integration + Hardening Plan

V91 is the milestone where the project stops behaving like a chain of separate upgrade notes
and starts behaving like a professional baseline that can be maintained.

## Main V91 goals
1. Keep the low-code builder workflow integrated into the runtime application.
2. Keep the content registry aligned with the current asset folder layout.
3. Preserve an offline-first CMake option for vendored dependencies.
4. Improve readability and maintainability with strong source comments.
5. Emit clear runtime-facing reports that make local verification easier.

## What still needs local verification after this snapshot
- clean configure and clean build on the Windows toolchain
- runtime load verification for authored file types created across V83-V90
- playable slice smoke test
- export bundle smoke test
- true fully-vendored offline dependency tree under `third_party/`

## Philosophy
- Code builds the engine.
- Data builds the game.
- The editor builds the content.
- V91 hardens the glue between those layers.
