# V91 Notes

## Theme
V91 is the first **post-V90 hardening milestone**.  
Instead of adding another disconnected feature, this version focuses on turning the V90 line into a more consistent,
more documented, and more maintainable baseline.

## What V91 improves
- professional-level source comments in the core integration files
- clearer application startup flow and builder wiring
- more explicit content-registry responsibilities and diagnostics
- optional offline build path documented and preserved in CMake
- runtime integration report output for local verification
- updated project metadata to identify the repo as V91 consistently

## Main source files improved
- `CMakeLists.txt`
- `src/core/Application.h`
- `src/core/Application.cpp`
- `src/content/ContentRegistry.h`
- `src/content/ContentRegistry.cpp`
- `src/editor/ui/GameBuilderPanel.h`
- `src/editor/ui/GameBuilderPanel.cpp`

## New runtime-facing outputs
- `assets/registry/v91_runtime_registry_report.txt`
- `assets/reports/v91_integration_checklist.txt`
- `docs/V91_RUNTIME_INTEGRATION_HARDENING_PLAN.md`

## Why V91 matters
V90 was the **creator-workflow completion baseline**.  
V91 is the **integration and hardening baseline** that prepares the project for a real compile-clean pass,
runtime verification, and future professional engine work.

## Important note
This snapshot is designed as a **complete repo snapshot** and a stronger engineering baseline.
A full local Windows compile/test pass is still required on your machine.
