# V103 Real Builder Toggle Fix

## Why the previous attempt did not help
The actual repo you uploaded still had only this live toggle path:

```cpp
if (m_input.IsKeyPressed(KEY_F10)) m_gameBuilderPanel.ToggleVisible();
```

So if you were still compiling that repo, the added alternate shortcuts never
reached the runtime.

## What V103 changes
- the real `Application.cpp` now supports:
  - `F10`
  - `F9`
  - `Ctrl+B`
- a log message is written when the builder opens or closes
