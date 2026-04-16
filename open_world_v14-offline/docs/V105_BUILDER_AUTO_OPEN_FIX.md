# V105 Builder Auto-Open Fix

## Why this change helps
If the builder is still not appearing, the fastest way to separate input problems
from draw/layout problems is to force the panel visible on startup.

## What V105 changes
- the builder auto-opens during `Application::Initialize()`
- alternate builder shortcuts remain available:
  - `F10`
  - `F9`
  - `Ctrl+B`
- the diagnostics string corruption in `ContentRegistry.cpp` is repaired so a
  fresh executable can actually be produced

## What to test
1. Clean rebuild from the repo root
2. Launch the executable
3. Confirm the builder is already visible
4. Toggle it closed and open again with the shortcuts
