# V100 Notes

## What V100 fixes
This snapshot repairs the first two concrete compile blockers from the real uploaded repo:

1. `src/content/ContentRegistry.cpp`
   - fixed malformed multiline string literals in `SaveDiagnosticsReport`
   - restored escaped `\n` output so MSVC no longer reports `newline in constant`

2. `src/editor/ui/GameBuilderPanel.cpp`
   - fixed `std::array` initializer mismatch in `WriteVersionMetadata`
   - changed the declared array size from `9` to `16` to match the real initializer list

## Why this matters
These were real source-level compile blockers in the actual repo tree, not abstract milestone issues.

## Main changed files
- `src/content/ContentRegistry.cpp`
- `src/editor/ui/GameBuilderPanel.cpp`
- `version_notes/VERSION.txt`
- `version_notes/V100_NOTES.md`
- `docs/V100_BUILD_FIXES.md`

## Recommended local commands
```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build
cmake --build build --config Release
```
