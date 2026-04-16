# V107 Builder Close Fix

## Problem
The builder could be shown, but closing depended too heavily on function-key
behavior, and `Esc` quit the whole app instead of acting like a normal panel close.

## Fix
- `Esc` closes the builder first
- the header now draws an explicit **X Close** button
- alternate builder toggle shortcuts remain available

## Expected behavior
- app launches
- builder is visible
- `Esc` hides builder
- `F10`, `F9`, or `Ctrl+B` reopens builder
- clicking **X Close** hides builder
