#ifndef BSP_H
#define BSP_H
#include <stdint.h>
/* Dimensions include a one-cell wall border. Output owns width*height bytes.
 * Returns NULL for invalid parameters or allocation failure. Free with free(). */
char *bsp_generate(int width, int height, unsigned depth, int min_room, uint32_t seed);
#endif
