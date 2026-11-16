# BSP dungeon map generator

![C11](https://img.shields.io/badge/C-11-blue)
![Build](https://img.shields.io/badge/build-Make-blue)
![License: MIT](https://img.shields.io/badge/license-MIT-green)
[![C build and tests](https://github.com/kleberandrade/bsp-tree-map-generator/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/kleberandrade/bsp-tree-map-generator/actions/workflows/ci.yml)

A dependency-free C dungeon generator using binary space partitioning (BSP).
Recursively divide a rectangle, carve a room in each leaf, then join sibling
components with inclusive L-shaped corridors. Every floor cell is reachable
from every other floor cell through four-direction movement.

## Requirements and quick start

Use a C11 compiler (Clang or GCC) and Make. No downloaded libraries are needed.
On Windows, use a compiler and Make through WSL or MSYS2; native MSVC is not tested.

```sh
make
./build/bsp-map
./build/bsp-map 40 20 5 3 42 > dungeon.txt
make test
make sanitize
```

Arguments are `width height depth min-room seed`, supplied together or omitted.
Defaults are `64 32 5 3 1`. Dimensions include a one-cell wall border; allowed
sizes are 3–4096 per axis. Depth is 0–32 and is an upper bound: subdivision
stops early when two minimum-sized rooms cannot fit. Minimum room size is
1 through the smaller interior dimension. Seeds are unsigned 32-bit decimal
integers, including zero. Identical parameters produce identical output across
platforms using the explicitly defined 32-bit generator. This PRNG is for games,
not cryptography. Different seeds need not produce distinct maps.

Output uses `#` for walls and `.` for floors, one row per line, on stdout.
Redirect stdout to save a map. Errors go to stderr; exits are 0 for success,
2 for malformed arguments, and 1 for invalid geometry, allocation or output failure.
There are no interactive controls.

## Library API

Include `include/bsp.h` and compile/link `src/bsp.c`:

```c
#include "bsp.h"
#include <stdlib.h>
int main(void) {
    char *map = bsp_generate(40, 20, 5, 3, 42);
    if (!map) return 1;
    /* Access map[y * 40 + x]; buffer has no row terminators. */
    free(map);
    return 0;
}
```

The caller owns the returned row-major buffer. NULL indicates invalid input or
allocation failure. Generation uses local state, no global RNG and no retained
heap tree. The map takes at most 16 MiB; recursive stack depth is at most 33.
Each split covers its parent exactly and leaves both children at least as large
as the minimum room dimension. Corridors may merge rooms; wall spacing between
rooms is not guaranteed. The outer border is never carved.

## Layout and migration

- `src/`: generation and geometry rules.
- `include/`: public library interface.
- `examples/`: CLI and argument/output handling.
- `tests/`: behavior and CLI checks.
- `legacy/`: original headers, Code::Blocks project and original sample map.

The original Code::Blocks project remains together with its original relative
paths in `legacy/`. It is historical, unsafe for arbitrary dimensions and is not
part of the supported build. Replace its header-defined functions with the
public API above when migrating. The new generator preserves the BSP approach,
but changes the split policy, RNG and corridor endpoints: historical maps and
seeds are not output compatible. Semicolon-delimited export is replaced by
plain ASCII redirection. The original sample remains in `legacy/map.txt`.

## Validation and limitations

`make test` checks deterministic generation, seed variation, boundary walls,
floor connectivity by flood fill, narrow/minimum/maximum dimensions, early
termination at depth 32, malformed CLI input and excessive input limits.
`make sanitize` runs the same suite with AddressSanitizer and UndefinedBehaviorSanitizer
(requires compiler support). CI runs these commands on Linux and macOS.
Allocation failure is handled but not fault-injected. No coverage percentage or
mutation score is claimed. The optional crapper, dryer and mutator tools do not
support C, so their metrics do not apply. There are no third-party runtime dependencies.

MIT licensed; see [LICENSE](LICENSE) for the original copyright and full terms.
