/* Compile with make examples; run ./build/library-example. */
#include "bsp.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const int width = 24, height = 12;
    char *map = bsp_generate(width, height, 4, 2, 42);
    if (!map) {
        fputs("Could not generate the dungeon.\n", stderr);
        return 1;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            /* Row-major access also works when copying cells into a game. */
            if (putchar(map[(size_t)y * (size_t)width + (size_t)x]) == EOF) {
                free(map);
                return 1;
            }
        }
        if (putchar('\n') == EOF) {
            free(map);
            return 1;
        }
    }
    free(map);
    return fflush(stdout) == EOF ? 1 : 0;
}
