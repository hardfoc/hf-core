---
name: bump-driver
description: Move one chip driver (or interface-wrap, pincfg) to a new commit or tag in hf-core, through the hf-core-drivers submodule. Use when a driver change must reach hf-core.
---

Drivers are submodules of `hardfoc/hf-core-drivers` (`external/<repo>`, `internal/<repo>`), which is a submodule
of hf-core. A bump is two PRs, merged bottom-up; never edit code inside a submodule checkout.

1. The driver change is merged in the driver's repository (or the PR you pin is named, with its merge order).
2. **hf-core-drivers PR.** In a clone of `hardfoc/hf-core-drivers` on a branch `pin/<repo>-<what>`:
   `git submodule update --init external/<repo>`, check out the target commit inside it, `git add external/<repo>`,
   commit "Pin <repo> to <tag or what changed>". The PR says what changed in the driver and links it.
3. **hf-core PR.** On a branch here: `git -C hf-core-drivers fetch origin <branch or main>`, check out the
   hf-core-drivers commit from step 2, `git submodule update --init --recursive hf-core-drivers`, then build.
4. Adapt hf-core to the driver change in the same PR: the handler's adapter hooks, its `ToStatus()` switch (a new
   `Error` value is a `-Wswitch` error until mapped), its tests and docs. Keep the diff to what the bump needs.
5. If the driver's `cmake/hf_<chip>_build_settings.cmake` changed variable names, update its block in
   `cmake/hf_core_build_settings.cmake`.
6. Run the `verify` skill; `hardfoc-docs gen` if a handler header changed.
7. State the merge order in both PRs: driver, hf-core-drivers, hf-core. After the hf-core-drivers PR merges,
   re-point the hf-core PR's pin at its merge commit before merging.
