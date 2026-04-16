# V111 Notes

## What V111 fixes
- repairs `src/content/ContentRegistry.cpp` by replacing the broken diagnostics
  block with valid escaped-newline output
- preserves the current real repo structure instead of introducing a scaffold

## What I verified here
I syntax-checked the repaired `src/content/ContentRegistry.cpp` in this
environment after patching it.

## What I could not verify here
I could not perform your full Windows/MSVC build in this environment because the
repo snapshot does not include the vendored `third_party/raylib` tree and the
fallback `FetchContent` path requires network access.

## Main changed files
- `src/content/ContentRegistry.cpp`
- `version_notes/VERSION.txt`
- `version_notes/V111_NOTES.md`
- `docs/V111_CONTENTREGISTRY_COMPILE_REPAIR.md`
