# V102 Builder Opening Fix

## Why the builder could look broken
The original runtime path only depended on `F10`. On many keyboards, especially laptops,
that means the user must press `Fn+F10`, which makes the builder seem dead even though
the code path exists.

## What V102 changes
- `F10` still works
- `F9` now also works
- `Ctrl+B` now also works
- the app logs whether the builder toggled open or closed

## Recommended local test
```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build
cmake --build build --config Release
```

Launch the executable and try:
- `F10`
- `F9`
- `Ctrl+B`

If the builder still does not appear, check the console/log output for:
- `Builder visibility toggled: open`
- `Builder visibility toggled: closed`
