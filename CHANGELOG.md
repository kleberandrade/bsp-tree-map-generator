# Maintenance log

## 2026-11-16

- Cloned the clean `master` branch from the requested origin (baseline `ce30af9`).
  No repository or ancestor AGENTS.md was present. Reviewed every C header,
  the entry point, Code::Blocks configuration and MIT license.
- Preserved the complete original implementation and project under `legacy/`:
  historical paths remain valid together, but unsafe APIs are excluded from the build.
- Added public C11 API and behavior tests before implementation. The initial
  test build failed to link because `bsp_generate` did not yet exist; this was
  a missing-implementation check, not an observed original regression test.
- Replaced global rand and heap tree with a local fixed-width RNG and bounded
  recursive partitions. Validated dimensions before allocation; each split can
  fit two children, corridors include both endpoints, and caller ownership is explicit.
- Added Make build, CLI example, strict compiler warnings and CI. No external
  dependency or toolchain installation was necessary.
- Ran `make test` and `make sanitize` successfully using Apple Clang 21.0.0.
  603 geometry/seed cases are generated twice, plus seed variation; flood fill
  checks connectivity and walls. CLI checks deterministic output and errors.
  ASan/UBSan reported no failures. Generated the committed 24x12 sample through CLI.
- Read crapper/dryer/mutator help: none supports C, so no metric or mutation run
  is claimed. Allocation failure injection and exhaustive partition introspection
  remain outside the tests. CI execution on GitHub awaits a future authorized push.
- Rewrote English documentation, added version/license/build and dynamic CI badges.
  Preserved original MIT attribution. Updated and read back GitHub About description:
  “Portable C11 BSP dungeon generator with deterministic seeds, connected rooms,
  an ASCII CLI, and sanitizer-tested behavior.”
- Verified six repository topics: ascii, binary-space-partitioning, bsp-tree, c,
  dungeon-generator, procedural-generation. No push, release, deploy or visibility change.

## Organization and educational examples follow-up

- Moved the production CLI from examples to `cli/`; separated local PRNG state
  into private `src/random.h` and `src/random.c`. Named generation steps and
  expanded formatting to make geometry, carving and orchestration readable.
- Kept the bounded algorithm and public function signature. Exposed shared
  dimension/depth/tile constants, documented ownership and input contracts.
- Compared the 24x12 seed-42 output before and after refactoring byte for byte.
- Added a runnable, error-checked library example and made its expected ASCII
  output part of both normal and sanitized test targets.
- Expanded the English README with BSP definitions, historical milestones,
  linked research references, practical uses, generation steps, connectivity
  reasoning, complexity, trade-offs and executable parameter examples.
- Verified `make test`, `make sanitize` and `git diff --check` successfully.
  No push or remote workflow execution was performed.
