# V101 Clean Compile and Link Baseline

## Purpose
Use V101 as the next full snapshot after the V100 blocker repairs.

## Local commands
```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build
cmake --build build --config Release
```

## Success criteria
- the project configures from the repo root
- the project compiles cleanly
- the project links cleanly
- the first remaining issue, if any, is now a smaller downstream problem instead of the earlier source corruption

## Notes
V101 is intentionally conservative. It is a stabilization milestone, not a new feature milestone.
