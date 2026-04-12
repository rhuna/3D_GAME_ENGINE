# Add a Quest Chain

1. Create a chain definition in `assets/content/packs/<pack>/quests/`.
2. Add dialogue files for each important handoff.
3. Add objective bundle data in `assets/content/packs/<pack>/objectives/`.
4. Add reward data in `assets/content/packs/<pack>/rewards/`.
5. Add progression flags in `assets/content/packs/<pack>/progression/`.
6. Register the chain in your pack manifest if needed.

Keep each step small and data-driven so new chains do not require engine rewrites.
