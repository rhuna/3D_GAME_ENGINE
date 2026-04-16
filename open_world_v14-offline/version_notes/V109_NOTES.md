# V109 Notes

## What V109 fixes
This snapshot is built from the latest uploaded real repo and stabilizes the two
remaining areas that were still causing confusing runtime behavior:

1. `src/core/Application.cpp`
   - one authoritative builder toggle path
   - `Esc` closes the builder first, then exits only if the builder is already closed
   - toggle logging now reports the explicit target visibility state

2. `src/content/ContentRegistry.cpp`
   - diagnostics output restored to proper line-separated text using escaped `\n`
   - avoids both malformed multiline string literals and the earlier overcorrection
     where all separators were removed

## Main changed files
- `src/core/Application.cpp`
- `src/content/ContentRegistry.cpp`
- `src/editor/ui/GameBuilderPanel.cpp`
- `version_notes/VERSION.txt`

## Local verification
1. Remove the `build` folder
2. Reconfigure
3. Rebuild Release
4. Launch the executable
5. Watch the builder visibility text in the top-left
6. Press `F10`, `F9`, and `Ctrl+B`
7. Confirm `Esc` closes the builder without quitting the app
