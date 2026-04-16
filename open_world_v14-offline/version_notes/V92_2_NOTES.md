# V92.2 Notes

## Purpose
V92.2 is the first compile-fix snapshot after the automated line-by-line comment pass broke `ContentRegistry.cpp`.

## What changed
- replaced the broken `src/content/ContentRegistry.cpp` with a clean, compile-safe implementation
- restored correct escaped newline usage in all registry text output paths
- added diagnostics helpers back into `ContentRegistry`
- expanded registry scanning for `assets/templates`, `assets/projects`, `assets/ambience`, and `assets/music`
- updated `CMakeLists.txt` so the repo can prefer a vendored offline `third_party/raylib` checkout
- updated version metadata to report V92.2 consistently

## What this fixes from your build log
Your local build failed first in `ContentRegistry.cpp` with `newline in constant`. That parser failure then caused the rest of the file to misparse and explode into many fake namespace/type errors.
