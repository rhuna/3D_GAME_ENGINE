# V45 Playable Vertical Slice Plan

This version defines the first compact gameplay loop for the starter vale slice.

## Intended loop
1. Enter Starter Vale
2. Speak with Mara the herbalist
3. Gather herbs from nearby nodes
4. Craft field medicine
5. Return to Mara
6. Receive reward and faction standing change

## Systems touched
- merchant/shop runtime
- gather node runtime
- crafting window path
- quest runtime path
- save point / persistence path
- faction reaction hook

## Exit criteria
- one repeatable authored route exists
- all required data lives in the starter vale content pack
- adding a second similar slice should be mostly data work
