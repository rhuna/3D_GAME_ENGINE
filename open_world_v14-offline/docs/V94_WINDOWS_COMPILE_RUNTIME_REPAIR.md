# V94 Windows Compile + Runtime Repair

## Purpose
Use V94 as the local Windows verification baseline after V93. The emphasis is on
getting through the next compile blockers cleanly and confirming the runtime
integration surface behaves as expected.

## Local checklist
1. Configure from a clean build directory
2. Build Release with MSVC
3. Fix the first compiler error only
4. Rebuild
5. Launch the executable
6. Toggle the builder
7. Rebuild the registry
8. Export diagnostics
9. Confirm runtime verification output files exist

## Priority repair areas
- parser-safe source formatting
- include correctness
- namespace/type consistency
- registry path alignment
- builder/application integration
