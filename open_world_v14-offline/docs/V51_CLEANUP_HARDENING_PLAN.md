# V51 Cleanup and Hardening Plan

Goal: tighten the framework so the current starter-base line is easier to build on without carrying temporary compatibility debt.

## Main targets
- unify recommended runtime path
- document deprecated compatibility layers
- standardize spawn/content entry points
- reduce duplicate data models
- harden save/load and region transition expectations
- define a stable “starter-base” contract for future game content

## Cleanup checklist
1. Confirm one recommended application -> scene -> runtime flow.
2. Mark legacy/compatibility code paths for removal or quarantine.
3. Standardize prefab, region, and content-pack naming conventions.
4. Freeze starter template folder structure.
5. Align docs with the actual shipped scaffold paths.
6. Define the minimum stable systems required for the base.
