# V76 - Guided Workflow Suite

## Goal
Reduce the number of scattered steps required to get from an empty project to a playable starter slice.

## Main upgrade
- Added a new **Workflow / Flow** tab to `GameBuilderPanel`
- Added one-click guided slice creation
- Added workflow checklist export
- Kept the V75 export pipeline, but tied it to a clearer creation sequence

## Outputs
- `assets/world/<slice>.slice`
- `assets/registry/<slice>_workflow.txt`
- existing V75 build/export outputs

## Why V76 matters
V72 reduced coding for gameplay data.
V73 reduced coding for world structure.
V74 reduced coding for wiring and validation.
V75 reduced coding for packaging.
V76 reduces friction between all of those steps by making the builder feel guided instead of scattered.

## Important note
This is a repo snapshot upgrade for the normal local Windows compile/test workflow.
