#ifndef BSP_H
#define BSP_H

#include <stdint.h>

#define BSP_MAX_DIMENSION 4096
#define BSP_MAX_DEPTH 32
#define BSP_WALL '#'
#define BSP_FLOOR '.'

/* Generate a connected dungeon with a one-cell wall border.
 * width/height: 3..BSP_MAX_DIMENSION, including the border.
 * depth: 0..BSP_MAX_DEPTH, maximum number of recursive splits along a path.
 * min_room: minimum room width and height; must fit the interior.
 * seed: any uint32_t value, including zero.
 * Returns width*height row-major bytes (no terminators), owned by the caller.
 * Free the result with free(). Returns NULL for invalid input or allocation
 * failure. Does not use global mutable state or perform I/O. */
char *bsp_generate(int width, int height, unsigned depth, int min_room, uint32_t seed);

#endif
