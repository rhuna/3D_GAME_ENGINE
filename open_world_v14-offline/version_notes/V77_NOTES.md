# V77 - Dialogue Graph Suite

## Goal
Reduce the amount of coding required to build NPC conversations by adding a low-code dialogue graph layer on top of the existing authoring workflow.

## Main upgrade
- Added a new **Talk** tab to `GameBuilderPanel`
- Added form-driven fields for:
  - opening text
  - two default player choices
  - follow-up node ids
  - follow-up node text
- Added generation for:
  - readable dialogue files
  - graph-style dialogue assets
  - NPC conversation bundles

## Outputs
- `assets/dialogue/<dialogue>.dialogue`
- `assets/dialogue/<dialogue>.dialoguegraph`
- `assets/links/<dialogue>_conversation.links`

## Why V77 matters
V72 reduced coding for gameplay data.
V73 reduced coding for world structure.
V74 reduced coding for wiring and validation.
V75 reduced coding for packaging.
V76 reduced workflow friction.
V77 reduces coding for the first major authored gameplay surface players actually read and click through: NPC dialogue.

## Practical use
1. Open **Talk**.
2. Fill in the opening line and the two main player choices.
3. Set the quest path and merchant path follow-up nodes.
4. Create the readable dialogue file.
5. Create the dialogue graph asset.
6. Create the full conversation bundle.

## Important note
This is a repo snapshot upgrade for the normal local Windows compile/test workflow.
