# V75 - Export + Packaging Suite

## Goal
Reduce the amount of coding required to turn authored content into a runnable offline game package.

## Main upgrade
- Added an **Export** tab to `GameBuilderPanel`
- Added low-code build profile generation
- Added content manifest export
- Added staged offline bundle export to `exports/<folder>`

## Outputs
- `assets/builds/<build>.build`
- `assets/builds/<build>.manifest.txt`
- `exports/<folder>/...`

## Why V75 matters
V72 reduced coding for gameplay data.
V73 reduced coding for world structure.
V74 reduced coding for content wiring and validation.
V75 reduces coding for packaging the game into a deliverable offline bundle.

## Important note
This is a repo snapshot upgrade. It is meant to be compiled and tested locally in the normal Windows toolchain workflow.
