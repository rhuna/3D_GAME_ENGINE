# V109 Toggle and Diagnostics Stabilization

## Why this snapshot matters
At this point the repo already had the underlying builder methods implemented.
The remaining confusion came from:
- mixed toggle logic in the live app loop
- diagnostics output that had been repaired once, then overcorrected into a
  format that lost its line separators

## What V109 does
- keeps one authoritative toggle path in `Application.cpp`
- uses `SetVisible(...)` consistently
- preserves clear line-separated diagnostics output in `ContentRegistry.cpp`

## What to test
- builder visible state text changes in the top-left
- `F10`, `F9`, and `Ctrl+B` all toggle the builder
- `Esc` closes the builder first
- the app stays open when closing the builder
