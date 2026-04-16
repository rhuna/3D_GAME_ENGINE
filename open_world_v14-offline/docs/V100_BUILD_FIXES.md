# V100 Build Fixes

## Fixed blocker 1: ContentRegistry multiline string corruption
`ContentRegistry::SaveDiagnosticsReport(...)` had literal line breaks inside quoted strings, which caused:
- `error C2001: newline in constant`
- repeated follow-on syntax errors on the next `out << ...` lines

V100 replaces those with normal escaped `\n` sequences.

## Fixed blocker 2: GameBuilderPanel initializer count mismatch
`GameBuilderPanel::WriteVersionMetadata()` declared:

```cpp
const std::array<std::pair<const char*, const char*>, 9> notes
```

but initialized 16 entries, causing:
- `error C2078: too many initializers`

V100 corrects the declared size to 16.

## Next local verification
After applying V100, rebuild from a clean `build/` folder and capture the first remaining compiler or linker error, if any.
