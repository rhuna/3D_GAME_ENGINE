# V104 Real Builder Access Fix

## Why the builder still looked broken
The actual repo you uploaded still only listened for `F10` in the live runtime
path. On many keyboards that means `Fn+F10`, so the builder can look dead even
when the code path exists.

## What V104 changes
- the real `Application.cpp` now supports:
  - `F10`
  - `F9`
  - `Ctrl+B`
- a log message is written when the builder opens or closes

## Also fixed
- malformed diagnostics string output in `ContentRegistry.cpp`
- `std::array` initializer mismatch in `GameBuilderPanel.cpp`
