# V92.2 Local Build Focus

This snapshot specifically targets the first blocking compile error in `ContentRegistry.cpp`.

## Suggested commands
```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build
cmake --build build --config Release
```

## What to send back next
- the next first compile error after `ContentRegistry.cpp`
- whether the build is using `third_party/raylib` or falling back to FetchContent
