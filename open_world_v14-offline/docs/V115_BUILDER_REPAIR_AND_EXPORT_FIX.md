# V115 Builder Repair and Export Fix

## What changed

- Moved builder update execution into the active draw frame so the tab body controls actually render.
- Blocked editor/world authoring hotkeys and gizmo updates while the builder is open.
- Hardened builder open/close status and cleared text focus on close.
- Fixed the Windows export pipeline compile collision by removing windows.h from ExportPipeline.cpp and forward-declaring GetModuleFileNameA instead.

## Why the builder looked broken before

The builder tab handlers used raylib draw calls inside `GameBuilderPanel::Update()`, but `Application::Run()` invoked that update before the frame draw began. That let the builder toggle on while much of its body failed to display reliably.
